import 'package:flutter/material.dart';

class HomeNavBar extends StatelessWidget {
  final VoidCallback onContactPressed;

  const HomeNavBar({super.key, required this.onContactPressed});

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);
    final bool isDesktop = MediaQuery.of(context).size.width > 800;
    final primaryColor = theme.colorScheme.primary;
    final onSurfaceColor = theme.colorScheme.onSurface;

    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 16),
      color: theme.colorScheme.surface,
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          // Logo & Title
          Row(
            children: [
              Container(
                padding: const EdgeInsets.all(8),
                decoration: BoxDecoration(
                  color: primaryColor,
                  borderRadius: BorderRadius.circular(8),
                ),
                child: const Icon(
                  Icons.blur_circular,
                  color: Colors.white,
                  size: 24,
                ),
              ),
              const SizedBox(width: 12),
              Text(
                'EventFlow',
                style: TextStyle(
                  fontSize: 20,
                  fontWeight: FontWeight.bold,
                  letterSpacing: -0.5,
                  color: onSurfaceColor,
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
                  child: Text(
                    'Contacts',
                    style: TextStyle(color: theme.colorScheme.onSurfaceVariant),
                  ),
                ),
                const SizedBox(width: 16),
                ElevatedButton(
                  onPressed: () => Navigator.pushNamed(context, '/login'),
                  style: ElevatedButton.styleFrom(
                    backgroundColor: primaryColor,
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
              icon: Icon(Icons.login, color: onSurfaceColor),
              tooltip: 'Enter App',
              onPressed: () => Navigator.pushNamed(context, '/dashboard'),
            ),
        ],
      ),
    );
  }
}
