# UXCpp Framework User Guide

Welcome to the official documentation for **UXCpp**, a professional, clean-room C++20 UI framework designed as a permissive alternative to traditional GUI libraries. 

UXCpp provides a retained-mode widget hierarchy, high-performance OpenGL rendering, and a comprehensive suite of application services—all without requiring a meta-object compiler (MOC) or introducing copyleft dependencies.

## 📚 Table of Contents

This guide is divided into six technical sections:

1. [**Architecture & Core Concepts**](01_architecture_and_core.md)
   - Framework philosophy, C++20 design, and the "No MOC" approach.
   - Application lifecycle and the main event loop.
   - Retained-mode widget hierarchy and lifecycle hooks (`onInit`, `onUpdate`, `onDraw`).
   - Reactive programming: Signals, Slots, and Properties API.
   - Theme and styling engine.

2. [**Layouts, Controls & Dialogs**](02_layouts_controls_and_dialogs.md)
   - Layout managers: `BoxLayout`, `GridLayout`, `Splitter`, and `ScrollArea`.
   - The Standard Control Catalog: Buttons, Labels, TextInputs, etc.
   - Menus, Toolbars, and Status bars.
   - Modal overlays and native OS dialogs.

3. [**Advanced Data & Graphics**](03_data_graphics_and_editors.md)
   - Model/View architecture: `AbstractItemModel` and virtualized views.
   - Vector graphics, SVG rendering, and the 2D transform stack.
   - High-performance Charting subsystem (Line, Scatter, Bar, etc.).
   - Rich text documents and the syntax-highlighted Code Editor.

4. [**Application Services**](04_application_services.md)
   - Asynchronous networking with `HttpClient`.
   - Database access layer via `SqlDatabase`.
   - OS Integration: File system watchers, Process control, and Clipboard APIs.
   - Cross-platform Settings management and Serial communication.
   - Internationalization (I18n) and resource bundles.

5. [**Developer Tools & Plugins**](05_developer_tools_and_plugins.md)
   - Runtime Debug Overlay: Inspecting widget bounds and render profiling.
   - Live Reload engine for declarative UI updates.
   - Plugin architecture and the C++ dynamic module loading ABI.

6. [**Complete Tutorial**](06_complete_tutorial.md)
   - A step-by-step guide to building a "System Monitor & SQL Dashboard" application, combining all framework modules into a production-ready example.

---

## 🚀 Quick Start

To get started quickly, we recommend reading the [Architecture & Core Concepts](01_architecture_and_core.md) section first, followed by the [Complete Tutorial](06_complete_tutorial.md).

**Build Requirements:**
- C++20 compliant compiler (GCC 10+, Clang 10+, MSVC 19.28+)
- OpenGL 3.3+ compatible GPU
- CMake 3.15+
