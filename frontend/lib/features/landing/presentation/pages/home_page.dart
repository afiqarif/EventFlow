import 'package:flutter/material.dart';

import '../widgets/home_nav_bar.dart';
import '../widgets/hero_section.dart';
import '../widgets/how_it_works_section.dart';
import '../widgets/home_footer.dart';

class HomePage extends StatelessWidget {
  const HomePage({super.key});

  // Helper method to show the contact details dialog
  void _showContactDialog(BuildContext context) {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: const Row(
          children: [
            Icon(Icons.contact_mail, color: Colors.blueGrey),
            SizedBox(width: 8),
            Text('Contact EventFlow Team'),
          ],
        ),
        content: const Column(
          mainAxisSize: MainAxisSize.min,
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              'Have questions about EventFlow or need support setting up your event pipeline?',
              style: TextStyle(fontSize: 14),
            ),
            SizedBox(height: 16),
            Row(
              children: [
                Icon(Icons.email, size: 20, color: Colors.blueGrey),
                SizedBox(width: 8),
                Text(
                  'support@eventflow.app',
                  style: TextStyle(fontWeight: FontWeight.bold),
                ),
              ],
            ),
            SizedBox(height: 8),
            Row(
              children: [
                Icon(Icons.school, size: 20, color: Colors.blueGrey),
                SizedBox(width: 8),
                Text('School of Computer Sciences, USM'),
              ],
            ),
          ],
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context),
            child: const Text(
              'Close',
              style: TextStyle(color: Colors.blueGrey),
            ),
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    // We use a clean Scaffold with a SingleChildScrollView so the page wraps beautifully on any device.
    return Scaffold(
      backgroundColor: const Color(
        0xFFF8FAFC,
      ), // Ultra-clean light-grey background
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
