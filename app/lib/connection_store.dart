import 'dart:async';
import 'dart:developer';

import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:mobx/mobx.dart';
import 'package:record_stand_app/example/utils/extra.dart';
import 'package:record_stand_app/extensions.dart';

part 'connection_store.g.dart';

final SERVICE_UUID = Guid("2de7d2f2-0000-1000-8000-00805f9b34fb");
final CHARACTERISTIC_UUID = Guid("0000c870-0000-1000-8000-00805f9b34fb");

class ConnectionStore = _ConnectionStoreBase with _$ConnectionStore;

abstract class _ConnectionStoreBase with Store {
  late StreamSubscription<List<ScanResult>> _scanResultsSubscription;
  late StreamSubscription<bool> _isScanningSubscription;

  @observable
  BluetoothDevice? device;
  @observable
  BluetoothCharacteristic? characteristic;

  @observable
  bool isConnecting = false;

  @observable
  bool connected = false;

  @computed
  bool get ready => characteristic != null;

  @observable
  bool isScanning = false;

  init() {
    _scanResultsSubscription =
        FlutterBluePlus.scanResults.listen(_onScanResults, onError: _onError);
    _isScanningSubscription =
        FlutterBluePlus.isScanning.listen(_onScanningStateChange, onError: _onError);
  }

  scan() {
    FlutterBluePlus.startScan(timeout: const Duration(seconds: 4));
  }

  connect() {
    if (isConnecting || connected) {
      return;
    }

    device?.connect(autoConnect: true, mtu: null);
  }

  disconnect() {
    device?.disconnect();
  }

  @action
  _onScanningStateChange(bool state) {
    isScanning = state;
  }

  @action
  _onScanResults(List<ScanResult> results) {
    if (device != null) {
      return;
    }

    log("Looking for device: ${results.map((result) => result.device.platformName)}");
    device =
        results.firstWhereOrNull((result) => result.device.platformName == "Record Stand")?.device;

    if (device != null) {
      log("Device found");
      FlutterBluePlus.stopScan();
      device?.connectionState.listen(_onConnectionStateChange, onError: _onError);
      device?.isConnecting.listen(_onConnectingChange);
      connect();
    } else {
      log("Device not found");
    }
  }

  @action
  _onConnectingChange(bool state) {
    log("Connecting: $state");
    isConnecting = state;
  }

  @action
  _onConnectionStateChange(BluetoothConnectionState state) {
    log("Connection state changed to $state");
    connected = state == BluetoothConnectionState.connected;
    if (state == BluetoothConnectionState.connected) {
      device?.discoverServices().then(_onServicesDiscovered);
    } else {
      connected = false;
      isConnecting = false;
      characteristic = null;
    }
  }

  @action
  _onServicesDiscovered(List<BluetoothService> services) {
    log("Services discovered");
    final service = services.firstWhereOrNull((service) => service.serviceUuid == SERVICE_UUID);
    if (service == null) {
      log("Service not found");
      return;
    }
    log("Service found");

    characteristic = service.characteristics
        .firstWhereOrNull((characteristic) => characteristic.uuid == CHARACTERISTIC_UUID);
    if (characteristic == null) {
      log("Characteristic not found");
      return;
    }
    log("Characteristic found");
  }

  _onError(e, [stackTrace]) {
    log("Error", error: e, stackTrace: stackTrace);
  }
}
