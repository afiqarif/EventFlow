import 'package:flutter/material.dart';

class HomeFooter extends StatelessWidget {
  const HomeFooter({super.key});

  @override
  Widget build(BuildContext context) {
    return Container(
      color: const Color(0xFF0F172A),
      padding: const EdgeInsets.symmetric(vertical: 24),
      width: double.infinity,
      alignment: Alignment.center,
      child: const Text(
        '© 2026 EventFlow. Deployed on Google Cloud. Built with Flutter & C++.',
        style: TextStyle(color: Color(0xFF64748B), fontSize: 12),
      ),
    );
  }
}
