import 'package:flutter/material.dart';
import 'package:frontend/features/landing/presentation/pages/home_page.dart';

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
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.blueGrey),
        useMaterial3: true,
      ),

      // 3. Simple, explicit routing table mapping paths to imported feature pages
      initialRoute: '/',
      routes: {
        '/': (context) => const HomePage(), // Gateway / marketing landing page
      },
    );
  }
}
