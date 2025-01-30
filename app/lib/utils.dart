import 'package:flutter/material.dart';
import 'package:flutter_mobx/flutter_mobx.dart';

Observer observer(Widget Function() widgetBuilder) {
  return Observer(builder: (_) => widgetBuilder());
}
