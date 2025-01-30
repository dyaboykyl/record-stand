// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'connection_store.dart';

// **************************************************************************
// StoreGenerator
// **************************************************************************

// ignore_for_file: non_constant_identifier_names, unnecessary_brace_in_string_interps, unnecessary_lambdas, prefer_expression_function_bodies, lines_longer_than_80_chars, avoid_as, avoid_annotating_with_dynamic, no_leading_underscores_for_local_identifiers

mixin _$ConnectionStore on _ConnectionStoreBase, Store {
  Computed<bool>? _$readyComputed;

  @override
  bool get ready => (_$readyComputed ??=
          Computed<bool>(() => super.ready, name: '_ConnectionStoreBase.ready'))
      .value;

  late final _$deviceAtom =
      Atom(name: '_ConnectionStoreBase.device', context: context);

  @override
  BluetoothDevice? get device {
    _$deviceAtom.reportRead();
    return super.device;
  }

  @override
  set device(BluetoothDevice? value) {
    _$deviceAtom.reportWrite(value, super.device, () {
      super.device = value;
    });
  }

  late final _$characteristicAtom =
      Atom(name: '_ConnectionStoreBase.characteristic', context: context);

  @override
  BluetoothCharacteristic? get characteristic {
    _$characteristicAtom.reportRead();
    return super.characteristic;
  }

  @override
  set characteristic(BluetoothCharacteristic? value) {
    _$characteristicAtom.reportWrite(value, super.characteristic, () {
      super.characteristic = value;
    });
  }

  late final _$isConnectingAtom =
      Atom(name: '_ConnectionStoreBase.isConnecting', context: context);

  @override
  bool get isConnecting {
    _$isConnectingAtom.reportRead();
    return super.isConnecting;
  }

  @override
  set isConnecting(bool value) {
    _$isConnectingAtom.reportWrite(value, super.isConnecting, () {
      super.isConnecting = value;
    });
  }

  late final _$connectedAtom =
      Atom(name: '_ConnectionStoreBase.connected', context: context);

  @override
  bool get connected {
    _$connectedAtom.reportRead();
    return super.connected;
  }

  @override
  set connected(bool value) {
    _$connectedAtom.reportWrite(value, super.connected, () {
      super.connected = value;
    });
  }

  late final _$isScanningAtom =
      Atom(name: '_ConnectionStoreBase.isScanning', context: context);

  @override
  bool get isScanning {
    _$isScanningAtom.reportRead();
    return super.isScanning;
  }

  @override
  set isScanning(bool value) {
    _$isScanningAtom.reportWrite(value, super.isScanning, () {
      super.isScanning = value;
    });
  }

  late final _$_ConnectionStoreBaseActionController =
      ActionController(name: '_ConnectionStoreBase', context: context);

  @override
  dynamic _onScanningStateChange(bool state) {
    final _$actionInfo = _$_ConnectionStoreBaseActionController.startAction(
        name: '_ConnectionStoreBase._onScanningStateChange');
    try {
      return super._onScanningStateChange(state);
    } finally {
      _$_ConnectionStoreBaseActionController.endAction(_$actionInfo);
    }
  }

  @override
  dynamic _onScanResults(List<ScanResult> results) {
    final _$actionInfo = _$_ConnectionStoreBaseActionController.startAction(
        name: '_ConnectionStoreBase._onScanResults');
    try {
      return super._onScanResults(results);
    } finally {
      _$_ConnectionStoreBaseActionController.endAction(_$actionInfo);
    }
  }

  @override
  dynamic _onConnectingChange(bool state) {
    final _$actionInfo = _$_ConnectionStoreBaseActionController.startAction(
        name: '_ConnectionStoreBase._onConnectingChange');
    try {
      return super._onConnectingChange(state);
    } finally {
      _$_ConnectionStoreBaseActionController.endAction(_$actionInfo);
    }
  }

  @override
  dynamic _onConnectionStateChange(BluetoothConnectionState state) {
    final _$actionInfo = _$_ConnectionStoreBaseActionController.startAction(
        name: '_ConnectionStoreBase._onConnectionStateChange');
    try {
      return super._onConnectionStateChange(state);
    } finally {
      _$_ConnectionStoreBaseActionController.endAction(_$actionInfo);
    }
  }

  @override
  dynamic _onServicesDiscovered(List<BluetoothService> services) {
    final _$actionInfo = _$_ConnectionStoreBaseActionController.startAction(
        name: '_ConnectionStoreBase._onServicesDiscovered');
    try {
      return super._onServicesDiscovered(services);
    } finally {
      _$_ConnectionStoreBaseActionController.endAction(_$actionInfo);
    }
  }

  @override
  String toString() {
    return '''
device: ${device},
characteristic: ${characteristic},
isConnecting: ${isConnecting},
connected: ${connected},
isScanning: ${isScanning},
ready: ${ready}
    ''';
  }
}
