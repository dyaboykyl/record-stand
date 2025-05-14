import 'dart:convert';
import 'dart:developer';
import 'dart:io';

import 'package:crypto/crypto.dart';
import 'package:flutter_acrcloud/flutter_acrcloud.dart';
import 'package:http/http.dart';
import 'package:mobx/mobx.dart';
import 'package:path_provider/path_provider.dart';
import 'package:record/record.dart';
import 'package:record_stand_app/main.dart';
import 'package:record_stand_app/secrets.dart';

part 'identify_store.g.dart';

const ACR_HOST = 'identify-eu-west-1.acrcloud.com';
const ACR_ENDPOINT = '/v1/identify';
final URI = Uri.parse('https://$ACR_HOST$ACR_ENDPOINT');
const DELIMETER = '+=+';

final options = {
  'access_key': ACR_ACCESS_KEY,
  'access_secret': ACR_ACCESS_SECRET,
  'signature_version': '1',
  'timestamp': '0',
  'secure': 'true',
  // 'signature': 'Ff2r6KobaCflFiQjFkG',
  'data_type': 'audio',
};

class IdentifyResult {
  final String? title;
  final String? artist;
  final String? album;
  final String? error;

  IdentifyResult(this.title, this.artist, this.album, [this.error]);

  Map<String, dynamic> toJson() {
    final map = <String, dynamic>{};
    if (error != null) {
      map['error'] = error;
    } else {
      map['title'] = title;
      map['artist'] = artist;
      map['album'] = album;
    }
    return map;
  }
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
  IdentifyResult? identifyResult;

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
      // isRecording = true;
      // await record.start(const RecordConfig(encoder: AudioEncoder.wav), path: filePath);
      // await Future.delayed(Duration(seconds: 5));
      // await record.stop();
      // isRecording = false;

      // await callAcrService();
      identifyResult = IdentifyResult('Song name', 'Artist name', '');
      log('Done identifying. Found result: ${identifyResult != null}');
      await sendToDevice();
    } finally {
      isRecording = false;
    }
  }

  @action
  Future callAcrService() async {
    log('Calling ACR service');

    try {
      isQuerying = true;
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
        identifyResult = IdentifyResult(
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

  Future sendToDevice() async {
    log('Sending to device');
    final data = [
      identifyResult?.title,
      identifyResult?.artist,
      identifyResult?.album,
      identifyResult?.error
    ].map((e) {
      if (e?.isEmpty ?? true) {
        return 'NO_RESULT';
      } else {
        return e;
      }
    }).join(DELIMETER);
    await connectionStore.send(data);
  }
}
