// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'home_screen.dart';

// **************************************************************************
// StoreGenerator
// **************************************************************************

// ignore_for_file: non_constant_identifier_names, unnecessary_brace_in_string_interps, unnecessary_lambdas, prefer_expression_function_bodies, lines_longer_than_80_chars, avoid_as, avoid_annotating_with_dynamic, no_leading_underscores_for_local_identifiers

mixin _$HomeScreenStore on _HomeScreenStoreBase, Store {
  Computed<String>? _$connectionStatusComputed;

  @override
  String get connectionStatus => (_$connectionStatusComputed ??=
          Computed<String>(() => super.connectionStatus,
              name: '_HomeScreenStoreBase.connectionStatus'))
      .value;
  Computed<String>? _$detailsComputed;

  @override
  String get details =>
      (_$detailsComputed ??= Computed<String>(() => super.details,
              name: '_HomeScreenStoreBase.details'))
          .value;
  Computed<bool>? _$connectButtonEnabledComputed;

  @override
  bool get connectButtonEnabled => (_$connectButtonEnabledComputed ??=
          Computed<bool>(() => super.connectButtonEnabled,
              name: '_HomeScreenStoreBase.connectButtonEnabled'))
      .value;
  Computed<String?>? _$titleComputed;

  @override
  String? get title => (_$titleComputed ??= Computed<String?>(() => super.title,
          name: '_HomeScreenStoreBase.title'))
      .value;
  Computed<String?>? _$artistComputed;

  @override
  String? get artist =>
      (_$artistComputed ??= Computed<String?>(() => super.artist,
              name: '_HomeScreenStoreBase.artist'))
          .value;
  Computed<String?>? _$albumComputed;

  @override
  String? get album => (_$albumComputed ??= Computed<String?>(() => super.album,
          name: '_HomeScreenStoreBase.album'))
      .value;
  Computed<String>? _$identifyStatusComputed;

  @override
  String get identifyStatus =>
      (_$identifyStatusComputed ??= Computed<String>(() => super.identifyStatus,
              name: '_HomeScreenStoreBase.identifyStatus'))
          .value;
  Computed<VoidCallback?>? _$connectButtonActionComputed;

  @override
  VoidCallback? get connectButtonAction => (_$connectButtonActionComputed ??=
          Computed<VoidCallback?>(() => super.connectButtonAction,
              name: '_HomeScreenStoreBase.connectButtonAction'))
      .value;
  Computed<VoidCallback?>? _$identifyButtonActionComputed;

  @override
  VoidCallback? get identifyButtonAction => (_$identifyButtonActionComputed ??=
          Computed<VoidCallback?>(() => super.identifyButtonAction,
              name: '_HomeScreenStoreBase.identifyButtonAction'))
      .value;

  @override
  String toString() {
    return '''
connectionStatus: ${connectionStatus},
details: ${details},
connectButtonEnabled: ${connectButtonEnabled},
title: ${title},
artist: ${artist},
album: ${album},
identifyStatus: ${identifyStatus},
connectButtonAction: ${connectButtonAction},
identifyButtonAction: ${identifyButtonAction}
    ''';
  }
}
