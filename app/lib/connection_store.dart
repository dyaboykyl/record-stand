import 'dart:async';
import 'dart:developer';

import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:mobx/mobx.dart';
import 'package:record_stand_app/example/utils/extra.dart';
import 'package:record_stand_app/extensions.dart';

part 'connection_store.g.dart';

final serviceUuid = Guid('2de7d2f2-0000-1000-8000-00805f9b34fb');
final identifyCharacteristicUuid = Guid('0000c870-0000-1000-8000-00805f9b34fb');

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

  void init() {
    _scanResultsSubscription =
        FlutterBluePlus.scanResults.listen(_onScanResults, onError: _onError);
    _isScanningSubscription =
        FlutterBluePlus.isScanning.listen(_onScanningStateChange, onError: _onError);
  }

  void destroy() {
    _scanResultsSubscription.cancel();
    _isScanningSubscription.cancel();
    disconnect();
  }

  void scan() {
    FlutterBluePlus.startScan(timeout: const Duration(seconds: 4));
  }

  void connect() {
    if (isConnecting || connected) {
      return;
    }

    device?.connect(autoConnect: true, mtu: null);
  }

  void disconnect() {
    device?.disconnect();
  }

  Future<void> send(String message) async {
    try {
      log('[send] $message');
      await characteristic?.write(message.codeUnits, withoutResponse: false, allowLongWrite: true);
      log('Sent');
    } catch (e) {
      log('Error sending', error: e);
    }
  }

  @action
  void _onScanningStateChange(bool state) {
    isScanning = state;
  }

  @action
  void _onScanResults(List<ScanResult> results) {
    if (device != null) {
      return;
    }

    log('Looking for device: ${results.map((result) => result.device.platformName)}');
    device =
        results.firstWhereOrNull((result) => result.device.platformName == 'Record Stand')?.device;

    if (device != null) {
      log('Device found');
      FlutterBluePlus.stopScan();
      device?.connectionState.listen(_onConnectionStateChange, onError: _onError);
      device?.isConnecting.listen(_onConnectingChange);
      connect();
    } else {
      log('Device not found');
    }
  }

  @action
  void _onConnectingChange(bool state) {
    log('Connecting: $state');
    isConnecting = state;
  }

  @action
  void _onConnectionStateChange(BluetoothConnectionState state) {
    log('Connection state changed to $state');
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
  void _onServicesDiscovered(List<BluetoothService> services) {
    log('Services discovered');
    final service = services.firstWhereOrNull((service) => service.serviceUuid == serviceUuid);
    if (service == null) {
      log('Service not found');
      return;
    }
    log('Service found');

    characteristic = service.characteristics
        .firstWhereOrNull((characteristic) => characteristic.uuid == identifyCharacteristicUuid);
    if (characteristic == null) {
      log('Characteristic not found');
      return;
    }
    log('Characteristic found');
  }

  void _onError(e, [stackTrace]) {
    log('Error', error: e, stackTrace: stackTrace);
  }
}
