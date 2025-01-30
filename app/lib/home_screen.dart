import 'package:flutter/material.dart';
import 'package:mobx/mobx.dart';
import 'package:record_stand_app/main.dart';
import 'package:record_stand_app/utils.dart';

part 'home_screen.g.dart';

class HomeScreenStore = _HomeScreenStoreBase with _$HomeScreenStore;

abstract class _HomeScreenStoreBase with Store {
  @computed
  String get connectionStatus => connectionStore.isScanning
      ? "Scanning"
      : connectionStore.isConnecting
          ? "Connecting"
          : connectionStore.ready
              ? "Ready"
              : connectionStore.connected
                  ? "Connected"
                  : "Disconnected";

  @computed
  String get details => {
        "Device": connectionStore.device?.platformName ?? "None",
        "Characteristic": connectionStore.characteristic?.uuid.toString() ?? "None",
        "Scanning": connectionStore.isScanning,
        "Connecting": connectionStore.isConnecting,
        "Connected": connectionStore.connected,
        "Ready": connectionStore.ready,
      }.entries.map((entry) => "${entry.key}: ${entry.value}").join("\n");

  @computed
  bool get connectButtonEnabled =>
      !connectionStore.connected &&
      !connectionStore.ready &&
      !connectionStore.isConnecting &&
      !connectionStore.isScanning;

  @computed
  VoidCallback? get connectButtonAction => connectButtonEnabled ? onPressConnect : null;

  void onPressConnect() {
    if (connectionStore.device != null) {
      connectionStore.connect();
    } else {
      connectionStore.scan();
    }
  }

  void onPressDisconnect() {
    connectionStore.disconnect();
  }
}

class HomeScreen extends StatefulWidget {
  const HomeScreen({super.key});

  @override
  State<HomeScreen> createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  final store = HomeScreenStore();
  static final key = GlobalKey<ScaffoldMessengerState>();

  @override
  Widget build(BuildContext context) {
    return ScaffoldMessenger(
      key: _HomeScreenState.key,
      child: Scaffold(
        appBar: AppBar(
          title: Text("Record Stand Companion"),
        ),
        body: SingleChildScrollView(
            child: observer(() => Column(
                  children: [
                    Text(store.connectionStatus),
                    ElevatedButton(onPressed: store.connectButtonAction, child: Text("Connect")),
                    ElevatedButton(onPressed: store.onPressDisconnect, child: Text("Disconnect")),
                    Text(store.details),
                  ],
                ))),
      ),
    );
  }
}
