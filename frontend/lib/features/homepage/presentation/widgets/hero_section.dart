import 'package:flutter/material.dart';

class HeroSection extends StatelessWidget {
  const HeroSection({super.key});

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);
    final accentColor = theme.colorScheme.secondary;
    final double screenWidth = MediaQuery.of(context).size.width;
    final bool isDesktop = screenWidth > 800;

    return Container(
      width: double.infinity,
      color: const Color(
        0xFF0F172A,
      ), // Intentional dark landing banner background
      padding: EdgeInsets.symmetric(
        horizontal: isDesktop ? screenWidth * 0.1 : 24,
        vertical: 64,
      ),
      child: Column(
        crossAxisAlignment: isDesktop
            ? CrossAxisAlignment.center
            : CrossAxisAlignment.start,
        children: [
          // Brand Chip
          Container(
            padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 6),
            decoration: BoxDecoration(
              color: accentColor.withValues(alpha: 0.15),
              borderRadius: BorderRadius.circular(20),
              border: Border.all(color: accentColor, width: 1),
            ),
            child: Text(
              'AI-POWERED MEDIA DISTRIBUTION',
              style: TextStyle(
                color: accentColor,
                fontSize: 11,
                fontWeight: FontWeight.w900,
                letterSpacing: 1,
              ),
            ),
          ),
          const SizedBox(height: 24),

          // Main Header Text
          Text(
            'The Ultimate Event Media Pipeline',
            textAlign: isDesktop ? TextAlign.center : TextAlign.left,
            style: TextStyle(
              fontSize: isDesktop ? 44 : 32,
              fontWeight: FontWeight.w900,
              color: Colors.white,
              letterSpacing: -1,
              height: 1.2,
            ),
          ),
          const SizedBox(height: 16),

          // Subtitle
          const Text(
            'EventFlow automates the logistical nightmare of post-event photo delivery. '
            'Upload massive media files directly, execute dynamic watermarking, and let attendees '
            'instantly retrieve their personal memories using face-matching AI.',
            textAlign: TextAlign.left,
            style: TextStyle(
              fontSize: 16,
              color: Color(0xFF94A3B8),
              height: 1.5,
            ),
          ),
          const SizedBox(height: 32),

          // Action Button
          Row(
            mainAxisAlignment: isDesktop
                ? MainAxisAlignment.center
                : MainAxisAlignment.start,
            children: [
              ElevatedButton.icon(
                onPressed: () => Navigator.pushNamed(context, '/dashboard'),
                icon: const Icon(Icons.arrow_forward),
                label: const Text('Get Started as Organizer'),
                style: ElevatedButton.styleFrom(
                  backgroundColor: accentColor,
                  foregroundColor: Colors.white,
                  padding: const EdgeInsets.symmetric(
                    horizontal: 24,
                    vertical: 16,
                  ),
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
              ),
              const SizedBox(width: 12),
              OutlinedButton.icon(
                onPressed: () => Navigator.pushNamed(context, '/gallery'),
                icon: const Icon(Icons.face),
                label: const Text('Find My Photos'),
                style: OutlinedButton.styleFrom(
                  foregroundColor: Colors.white,
                  side: const BorderSide(color: Color(0xFF475569)),
                  padding: const EdgeInsets.symmetric(
                    horizontal: 24,
                    vertical: 16,
                  ),
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
              ),
            ],
          ),
        ],
      ),
    );
  }
}
