import 'dart:convert';
import 'dart:developer';
import 'dart:io';

import 'package:crypto/crypto.dart';
import 'package:flutter_acrcloud/flutter_acrcloud.dart';
import 'package:http/http.dart';
import 'package:mobx/mobx.dart';
import 'package:path_provider/path_provider.dart';
import 'package:record/record.dart';
import 'package:record_stand_app/secrets.dart';

part 'identify_store.g.dart';

const ACR_HOST = 'identify-eu-west-1.acrcloud.com';
const ACR_ENDPOINT = '/v1/identify';
final URI = Uri.parse('https://$ACR_HOST$ACR_ENDPOINT');

final options = {
  'access_key': ACR_ACCESS_KEY,
  'access_secret': ACR_ACCESS_SECRET,
  'signature_version': '1',
  'timestamp': '0',
  'secure': 'true',
  // 'signature': 'Ff2r6KobaCflFiQjFkG',
  'data_type': 'audio',
};

class SongInfo {
  final String? title;
  final String? artist;
  final String? album;

  SongInfo(this.title, this.artist, this.album);
}

class IdentifyStore = _IdentifyStoreBase with _$IdentifyStore;

abstract class _IdentifyStoreBase with Store {
  final record = AudioRecorder();

  late String filePath;

  @observable
  ACRCloudSession? session;
  @observable
  ACRCloudResponse? response;
  @computed
  ACRCloudResponseMusicItem? get identifiedMusic => response?.metadata?.music.firstOrNull;

  @observable
  bool isRecording = false;
  @observable
  bool isQuerying = false;
  @computed
  bool get isIdentifying => isRecording || isQuerying;

  @observable
  SongInfo? identifyResult;

  Future init() async {
    // ACRCloud.setUp(ACRCloudConfig(ACR_ACCESS_KEY, ACR_ACCESS_SECRET, ACR_HOST));
    await record.hasPermission();
    final directory = '${(await getApplicationDocumentsDirectory()).path}/recordings';
    await Directory(directory).create(recursive: true);
    filePath = '$directory/sample.wav';
  }

  @action
  Future identify() async {
    log('Identifying...');

    try {
      log('Recording');
      isRecording = true;
      await record.start(const RecordConfig(encoder: AudioEncoder.wav), path: filePath);
      await Future.delayed(Duration(seconds: 5));
      await record.stop();
      isQuerying = true;
      isRecording = false;

      await callAcrService();
      log('Done identifying. Found result: ${identifyResult != null}');
    } finally {
      isRecording = false;
    }

    // try {
    //   if (session != null) {
    //     session?.cancel();
    //   }
    //   session = ACRCloud.startSession();
    //   response = await session?.result;
    //   log('Result found: ${identifiedMusic != null}');
    // } finally {
    //   session?.dispose();
    //   session = null;
    // }
  }

  @action
  Future callAcrService() async {
    log('Calling ACR service');

    try {
      List<int> messageBytes = utf8.encode([
        'POST',
        ACR_ENDPOINT,
        ACR_ACCESS_KEY,
        options['data_type'],
        options['signature_version'],
        options['timestamp'],
      ].join('\n'));
      final digest = Hmac(sha1, ACR_ACCESS_SECRET.codeUnits).convert(messageBytes);
      final signature = base64Encode(digest.bytes);

      final size = await File(filePath).length();
      final request = MultipartRequest('POST', URI)
        ..fields.addAll(options)
        ..fields.addAll({'sample_bytes': size.toString(), 'signature': signature})
        ..files.add(await MultipartFile.fromPath('sample', filePath));
      final response = await request.send();
      final result = jsonDecode(await response.stream.bytesToString());
      log(result?['status']?['msg']);
      if (result?['status']?['code'] == 0) {
        final musicData = List.castFrom(result['metadata']?['music']).firstOrNull;
        identifyResult = SongInfo(
          musicData?['title'],
          List.castFrom(musicData?['artists'] ?? []).firstOrNull?['name'],
          Map.castFrom(musicData?['album'] ?? {})['name'],
        );
      } else {
        identifyResult = null;
      }
    } finally {
      isQuerying = false;
    }
  }
}
