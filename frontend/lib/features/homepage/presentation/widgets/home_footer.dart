import 'package:flutter/material.dart';

class HomeFooter extends StatelessWidget {
  const HomeFooter({super.key});

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);

    return Container(
      color: theme.brightness == Brightness.dark
          ? const Color(0xFF0F172A)
          : theme.colorScheme.surface,
      padding: const EdgeInsets.symmetric(vertical: 24),
      width: double.infinity,
      alignment: Alignment.center,
      child: Text(
        '© 2026 EventFlow. Deployed on Google Cloud. Built with Flutter & C++.',
        style: TextStyle(
          color: theme.brightness == Brightness.dark
              ? const Color(0xFF64748B)
              : theme.colorScheme.onSurface.withValues(alpha: 0.6),
          fontSize: 12,
        ),
      ),
    );
  }
}
