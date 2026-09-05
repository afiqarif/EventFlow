import 'package:flutter/material.dart';
import 'package:frontend/features/homepage/presentation/pages/home_page.dart';
import 'package:frontend/features/auth/presentation/pages/login_page.dart';
import 'package:frontend/features/auth/presentation/pages/register_page.dart';

import 'core/theme/theme.dart';

void main() {
  // 1. Core initializations (e.g., storage, dependencies, WebSockets) go here.
  WidgetsFlutterBinding.ensureInitialized();

  runApp(const EventFlowApp());
}

class EventFlowApp extends StatelessWidget {
  const EventFlowApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'EventFlow: AI Media Processing',
      debugShowCheckedModeBanner: false,

      // 2. Define global themes that apply across all features
      theme: AppTheme.lightTheme,
      darkTheme: AppTheme.darkTheme,
      themeMode: ThemeMode.system,

      // 3. Simple, explicit routing table mapping paths to imported feature pages
      initialRoute: '/',
      routes: {
        '/': (context) => const HomePage(), // Gateway / marketing landing page
        '/login': (context) => const LoginPage(),
        '/register': (context) => const RegisterPage(),
      },
    );
  }
}
