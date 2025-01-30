// Copyright 2017-2023, Charles Weinberger & Paul DeMarco.
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:record_stand_app/connection_store.dart';
import 'package:record_stand_app/home_screen.dart';

import 'example/screens/bluetooth_off_screen.dart';

final connectionStore = ConnectionStore();

void main() {
  FlutterBluePlus.setLogLevel(LogLevel.verbose, color: true);
  connectionStore.init();
  runApp(const FlutterBlueApp());
}

//
// This widget shows BluetoothOffScreen or
// ScanScreen depending on the adapter state
//
class FlutterBlueApp extends StatefulWidget {
  const FlutterBlueApp({super.key});

  @override
  State<FlutterBlueApp> createState() => _FlutterBlueAppState();
}

class _FlutterBlueAppState extends State<FlutterBlueApp> {
  BluetoothAdapterState _adapterState = BluetoothAdapterState.unknown;

  late StreamSubscription<BluetoothAdapterState> _adapterStateStateSubscription;

  @override
  void initState() {
    super.initState();
    _adapterStateStateSubscription = FlutterBluePlus.adapterState.listen((state) {
      _adapterState = state;
      if (mounted) {
        setState(() {});
      }
    });
  }

  @override
  void dispose() {
    _adapterStateStateSubscription.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    Widget screen = _adapterState == BluetoothAdapterState.on
        ? HomeScreen()
        : BluetoothOffScreen(adapterState: _adapterState);

    return MaterialApp(
      color: Colors.lightBlue,
      home: screen,
      navigatorObservers: [BluetoothAdapterStateObserver()],
    );
  }
}

//
// This observer listens for Bluetooth Off and dismisses the DeviceScreen
//
class BluetoothAdapterStateObserver extends NavigatorObserver {
  StreamSubscription<BluetoothAdapterState>? _adapterStateSubscription;

  @override
  void didPush(Route route, Route? previousRoute) {
    super.didPush(route, previousRoute);
    if (route.settings.name == '/DeviceScreen') {
      // Start listening to Bluetooth state changes when a new route is pushed
      _adapterStateSubscription ??= FlutterBluePlus.adapterState.listen((state) {
        if (state != BluetoothAdapterState.on) {
          // Pop the current route if Bluetooth is off
          navigator?.pop();
        }
      });
    }
  }

  @override
  void didPop(Route route, Route? previousRoute) {
    super.didPop(route, previousRoute);
    // Cancel the subscription when the route is popped
    _adapterStateSubscription?.cancel();
    _adapterStateSubscription = null;
  }
}

// import 'package:flutter/material.dart';
// import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
// import 'package:provider/provider.dart';

// import 'src/ble/ble_device_connector.dart';
// import 'src/ble/ble_device_interactor.dart';
// import 'src/ble/ble_logger.dart';
// import 'src/ble/ble_scanner.dart';
// import 'src/ble/ble_status_monitor.dart';
// import 'src/ui/ble_status_screen.dart';
// import 'src/ui/device_list.dart';

// const _themeColor = Colors.lightGreen;

// void main() {
//   WidgetsFlutterBinding.ensureInitialized();

//   final ble = FlutterReactiveBle();
//   final bleLogger = BleLogger(ble: ble);
//   final scanner = BleScanner(ble: ble, logMessage: bleLogger.addToLog);
//   final monitor = BleStatusMonitor(ble);
//   final connector = BleDeviceConnector(
//     ble: ble,
//     logMessage: bleLogger.addToLog,
//   );
//   final serviceDiscoverer = BleDeviceInteractor(
//     bleDiscoverServices: (deviceId) async {
//       await ble.discoverAllServices(deviceId);
//       return ble.getDiscoveredServices(deviceId);
//     },
//     logMessage: bleLogger.addToLog,
//     readRssi: ble.readRssi,
//   );
//   runApp(
//     MultiProvider(
//       providers: [
//         Provider.value(value: scanner),
//         Provider.value(value: monitor),
//         Provider.value(value: connector),
//         Provider.value(value: serviceDiscoverer),
//         Provider.value(value: bleLogger),
//         StreamProvider<BleScannerState?>(
//           create: (_) => scanner.state,
//           initialData: const BleScannerState(
//             discoveredDevices: [],
//             scanIsInProgress: false,
//           ),
//         ),
//         StreamProvider<BleStatus?>(
//           create: (_) => monitor.state,
//           initialData: BleStatus.unknown,
//         ),
//         StreamProvider<ConnectionStateUpdate>(
//           create: (_) => connector.state,
//           initialData: const ConnectionStateUpdate(
//             deviceId: 'Unknown device',
//             connectionState: DeviceConnectionState.disconnected,
//             failure: null,
//           ),
//         ),
//       ],
//       child: MaterialApp(
//         title: 'Flutter Reactive BLE example',
//         color: _themeColor,
//         theme: ThemeData(primarySwatch: _themeColor),
//         home: const HomeScreen(),
//       ),
//     ),
//   );
// }

// class HomeScreen extends StatelessWidget {
//   const HomeScreen({
//     super.key,
//   });

//   @override
//   Widget build(BuildContext context) => Consumer<BleStatus?>(
//         builder: (_, status, __) {
//           if (status == BleStatus.ready) {
//             return const DeviceListScreen();
//           } else {
//             return BleStatusScreen(status: status ?? BleStatus.unknown);
//           }
//         },
//       );
// }
