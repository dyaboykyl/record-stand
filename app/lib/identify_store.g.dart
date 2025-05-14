// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'identify_store.dart';

// **************************************************************************
// StoreGenerator
// **************************************************************************

// ignore_for_file: non_constant_identifier_names, unnecessary_brace_in_string_interps, unnecessary_lambdas, prefer_expression_function_bodies, lines_longer_than_80_chars, avoid_as, avoid_annotating_with_dynamic, no_leading_underscores_for_local_identifiers

mixin _$IdentifyStore on _IdentifyStoreBase, Store {
  Computed<ACRCloudResponseMusicItem?>? _$identifiedMusicComputed;

  @override
  ACRCloudResponseMusicItem? get identifiedMusic =>
      (_$identifiedMusicComputed ??= Computed<ACRCloudResponseMusicItem?>(
              () => super.identifiedMusic,
              name: '_IdentifyStoreBase.identifiedMusic'))
          .value;
  Computed<bool>? _$isIdentifyingComputed;

  @override
  bool get isIdentifying =>
      (_$isIdentifyingComputed ??= Computed<bool>(() => super.isIdentifying,
              name: '_IdentifyStoreBase.isIdentifying'))
          .value;

  late final _$sessionAtom =
      Atom(name: '_IdentifyStoreBase.session', context: context);

  @override
  ACRCloudSession? get session {
    _$sessionAtom.reportRead();
    return super.session;
  }

  @override
  set session(ACRCloudSession? value) {
    _$sessionAtom.reportWrite(value, super.session, () {
      super.session = value;
    });
  }

  late final _$responseAtom =
      Atom(name: '_IdentifyStoreBase.response', context: context);

  @override
  ACRCloudResponse? get response {
    _$responseAtom.reportRead();
    return super.response;
  }

  @override
  set response(ACRCloudResponse? value) {
    _$responseAtom.reportWrite(value, super.response, () {
      super.response = value;
    });
  }

  late final _$isRecordingAtom =
      Atom(name: '_IdentifyStoreBase.isRecording', context: context);

  @override
  bool get isRecording {
    _$isRecordingAtom.reportRead();
    return super.isRecording;
  }

  @override
  set isRecording(bool value) {
    _$isRecordingAtom.reportWrite(value, super.isRecording, () {
      super.isRecording = value;
    });
  }

  late final _$isQueryingAtom =
      Atom(name: '_IdentifyStoreBase.isQuerying', context: context);

  @override
  bool get isQuerying {
    _$isQueryingAtom.reportRead();
    return super.isQuerying;
  }

  @override
  set isQuerying(bool value) {
    _$isQueryingAtom.reportWrite(value, super.isQuerying, () {
      super.isQuerying = value;
    });
  }

  late final _$identifyResultAtom =
      Atom(name: '_IdentifyStoreBase.identifyResult', context: context);

  @override
  IdentifyResult? get identifyResult {
    _$identifyResultAtom.reportRead();
    return super.identifyResult;
  }

  @override
  set identifyResult(IdentifyResult? value) {
    _$identifyResultAtom.reportWrite(value, super.identifyResult, () {
      super.identifyResult = value;
    });
  }

  late final _$identifyAsyncAction =
      AsyncAction('_IdentifyStoreBase.identify', context: context);

  @override
  Future<dynamic> identify() {
    return _$identifyAsyncAction.run(() => super.identify());
  }

  late final _$callAcrServiceAsyncAction =
      AsyncAction('_IdentifyStoreBase.callAcrService', context: context);

  @override
  Future<dynamic> callAcrService() {
    return _$callAcrServiceAsyncAction.run(() => super.callAcrService());
  }

  @override
  String toString() {
    return '''
session: ${session},
response: ${response},
isRecording: ${isRecording},
isQuerying: ${isQuerying},
identifyResult: ${identifyResult},
identifiedMusic: ${identifiedMusic},
isIdentifying: ${isIdentifying}
    ''';
  }
}
