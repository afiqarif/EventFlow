import 'package:flutter/material.dart';

class HomeNavBar extends StatelessWidget {
  final VoidCallback onContactPressed;

  const HomeNavBar({required this.onContactPressed});

  @override
  Widget build(BuildContext context) {
    final bool isDesktop = MediaQuery.of(context).size.width > 800;

    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 16),
      color: Colors.white,
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          // Logo & Title
          Row(
            children: [
              Container(
                padding: const EdgeInsets.all(8),
                decoration: BoxDecoration(
                  color: const Color(0xFF0F172A),
                  borderRadius: BorderRadius.circular(8),
                ),
                child: const Icon(
                  Icons.blur_circular,
                  color: Colors.white,
                  size: 24,
                ),
              ),
              const SizedBox(width: 12),
              const Text(
                'EventFlow',
                style: TextStyle(
                  fontSize: 20,
                  fontWeight: FontWeight.bold,
                  letterSpacing: -0.5,
                  color: Color(0xFF0F172A),
                ),
              ),
            ],
          ),

          // Menu items (Adapts beautifully for desktop/mobile views)
          if (isDesktop)
            Row(
              children: [
                TextButton(
                  onPressed: onContactPressed,
                  child: const Text(
                    'Contacts',
                    style: TextStyle(color: Color(0xFF475569)),
                  ),
                ),
                const SizedBox(width: 16),
                ElevatedButton(
                  onPressed: () => Navigator.pushNamed(context, '/dashboard'),
                  style: ElevatedButton.styleFrom(
                    backgroundColor: const Color(0xFF0F172A),
                    foregroundColor: Colors.white,
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(8),
                    ),
                  ),
                  child: const Text('Login / Signup'),
                ),
              ],
            )
          else
            // Compact mobile controls
            IconButton(
              icon: const Icon(Icons.login, color: Color(0xFF0F172A)),
              tooltip: 'Enter App',
              onPressed: () => Navigator.pushNamed(context, '/dashboard'),
            ),
        ],
      ),
    );
  }
}
