import 'package:flutter/material.dart';

import '../widgets/home_nav_bar.dart';
import '../widgets/hero_section.dart';
import '../widgets/how_it_works_section.dart';
import '../widgets/home_footer.dart';

class HomePage extends StatelessWidget {
  const HomePage({super.key});

  // Helper method to show the contact details dialog
  void _showContactDialog(BuildContext context) {
    final theme = Theme.of(context);
    final primaryColor = theme.colorScheme.primary;

    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Row(
          children: [
            Icon(Icons.contact_mail, color: primaryColor),
            const SizedBox(width: 8),
            const Text('Contact EventFlow Team'),
          ],
        ),
        content: Column(
          mainAxisSize: MainAxisSize.min,
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text(
              'Have questions about EventFlow or need support setting up your event pipeline?',
              style: TextStyle(fontSize: 14),
            ),
            const SizedBox(height: 16),
            Row(
              children: [
                Icon(Icons.email, size: 20, color: primaryColor),
                const SizedBox(width: 8),
                const Text(
                  'support@eventflow.app',
                  style: TextStyle(fontWeight: FontWeight.bold),
                ),
              ],
            ),
            const SizedBox(height: 8),
            Row(
              children: [
                Icon(Icons.school, size: 20, color: primaryColor),
                const SizedBox(width: 8),
                const Text('School of Computer Sciences, USM'),
              ],
            ),
          ],
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context),
            child: Text('Close', style: TextStyle(color: primaryColor)),
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    // Pulling the scaffold background color directly from the active theme
    return Scaffold(
      backgroundColor: Theme.of(context).scaffoldBackgroundColor,
      body: SingleChildScrollView(
        child: Column(
          children: [
            // 1. Navigation Header
            HomeNavBar(onContactPressed: () => _showContactDialog(context)),

            // 2. Main Value Proposition (Hero Section)
            const HeroSection(),

            // 3. Educational Step-by-Step Guide
            const HowItWorksSection(),

            // 4. Compact Footer
            const HomeFooter(),
          ],
        ),
      ),
    );
  }
}
