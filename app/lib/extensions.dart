import 'dart:math';

import 'package:flutter/material.dart';

extension ExtendedIterable<E> on Iterable<E> {
  static final random = Random();

  /// Like Iterable<T>.map but callback have index as second argument
  Iterable<T> mapIndex<T>(T Function(E e, int i) f) {
    var i = 0;
    return map((e) => f(e, i++));
  }

  void forEachIndex(void Function(E e, int i) f) {
    var i = 0;
    forEach((e) => f(e, i++));
  }

  E? firstWhereOrNull(bool Function(E) test) {
    try {
      return firstWhere(test);
    } on StateError {
      return null;
    }
  }

  Iterable<E> whereIndexed(bool Function(E e, int i) f) {
    var i = 0;
    return where((e) => f(e, i++));
  }

  E randomElement() {
    return toList()[random.nextInt(length)];
  }

  List<E> toSorted([int Function(E a, E b)? compator]) {
    return toList()..sort(compator);
  }
}

extension ExtendedColor on Color {
  Color darken([double amount = .1]) {
    final hsl = HSLColor.fromColor(this);
    final hslDark = hsl.withLightness((hsl.lightness - amount).clamp(0.0, 1.0).toDouble());

    return hslDark.toColor();
  }
}

extension StateExtension<T extends StatefulWidget> on State<T> {
  ThemeData get theme => Theme.of(context);
  Size get size => MediaQuery.of(context).size;
  ColorScheme get colorScheme => theme.colorScheme;
}

extension NumberExtension on num {
  int roundToNearest(int increment) {
    return (this / increment).round() * increment;
  }
}
