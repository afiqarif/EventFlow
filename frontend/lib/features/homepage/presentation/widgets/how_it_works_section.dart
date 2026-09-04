import 'package:flutter/material.dart';

class HowItWorksSection extends StatelessWidget {
  const HowItWorksSection({super.key});

  @override
  Widget build(BuildContext context) {
    final double screenWidth = MediaQuery.of(context).size.width;
    final bool isDesktop = screenWidth > 900;

    return Padding(
      padding: const EdgeInsets.symmetric(vertical: 80, horizontal: 24),
      child: Column(
        children: [
          const Text(
            'How It Works',
            style: TextStyle(
              fontSize: 28,
              fontWeight: FontWeight.w900,
              color: Color(0xFF0F172A),
            ),
          ),
          const SizedBox(height: 8),
          const Text(
            'Simple. Fast. Secured.',
            style: TextStyle(color: Color(0xFF64748B), fontSize: 16),
          ),
          const SizedBox(height: 48),

          // Steps layout switches from vertical list (mobile) to horizontal row (desktop)
          if (isDesktop)
            const Row(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Expanded(
                  child: _StepCard(
                    stepNumber: '1',
                    icon: Icons.unarchive,
                    title: 'Direct Bulk Upload',
                    description: 'Organizers upload a single massive .zip file directly to cloud storage, completely bypassing API bottlenecks.',
                  ),
                ),
                SizedBox(width: 24),
                Expanded(
                  child: _StepCard(
                    stepNumber: '2',
                    icon: Icons.settings_suggest,
                    title: 'C++ Parallel Processing',
                    description: 'Our ultra-fast C++ workers downsample, apply sponsor watermarks, and extract biometric search vectors instantly.',
                  ),
                ),
                SizedBox(width: 24),
                Expanded(
                  child: _StepCard(
                    stepNumber: '3',
                    icon: Icons.search,
                    title: 'Selfie / Bib Match',
                    description: 'Guests snap a selfie or type their runner bib number to pull their custom gallery in seconds.',
                  ),
                ),
              ],
            )
          else
            const Column(
              children: [
                _StepCard(
                  stepNumber: '1',
                  icon: Icons.unarchive,
                  title: 'Direct Bulk Upload',
                  description: 'Organizers upload a single massive .zip file directly to cloud storage, completely bypassing API bottlenecks.',
                ),
                SizedBox(height: 24),
                _StepCard(
                  stepNumber: '2',
                  icon: Icons.settings_suggest,
                  title: 'C++ Parallel Processing',
                  description: 'Our ultra-fast C++ workers downsample, apply sponsor watermarks, and extract biometric search vectors instantly.',
                ),
                SizedBox(height: 24),
                _StepCard(
                  stepNumber: '3',
                  icon: Icons.search,
                  title: 'Selfie / Bib Match',
                  description: 'Guests snap a selfie or type their runner bib number to pull their custom gallery in seconds.',
                ),
              ],
            ),
        ],
      ),
    );
  }
}

class _StepCard extends StatelessWidget {
  final String stepNumber;
  final IconData icon;
  final String title;
  final String description;

  const _StepCard({
    required this.stepNumber,
    required this.icon,
    required this.title,
    required this.description,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      elevation: 0,
      color: Colors.white,
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(12),
        side: const BorderSide(color: Color(0xFFE2E8F0)),
      ),
      child: Padding(
        padding: const EdgeInsets.all(24.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Icon(icon, color: const Color(0xFF3B82F6), size: 36),
                Text(
                  '0$stepNumber',
                  style: const TextStyle(
                    fontSize: 24,
                    fontWeight: FontWeight.bold,
                    color: Color(0xFFCBD5E1),
                  ),
                ),
              ],
            ),
            const SizedBox(height: 16),
            Text(
              title,
              style: const TextStyle(
                fontSize: 18,
                fontWeight: FontWeight.bold,
                color: Color(0xFF0F172A),
              ),
            ),
            const SizedBox(height: 8),
            Text(
              description,
              style: const TextStyle(color: Color(0xFF475569), height: 1.4),
            ),
          ],
        ),
      ),
    );
  }
}
