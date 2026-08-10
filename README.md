# UXCpp
A lightweight, cross-platform GUI framework for modern C++ (C++20/23).

## Goal
Designed as a clean-room, permissive replacement for Qt, allowing for closed-source and proprietary commercial use without licensing conflicts.

## Architectural Principles
- **Zero Copyleft:** No LGPL/GPL code. Strictly MIT/Apache 2.0/BSD.
- **Modern C++:** Leveraging C++20 concepts, RAII, and type-safety.
- **MOC-less:** Type-safe Signal/Slot system without a custom meta-object compiler.
- **Hardware Accelerated:** Designed for Vulkan/Metal/D3D12 backends.

## Current Progress
- [x] **Phase 0: Project Setup & CMake Skeleton**
- [x] **Phase 1: Event System & Core Signals** (Type-safe `Signal<Args...>`)
- [x] **Phase 2: Base App & Window Abstraction** (GLFW Backend)
- [x] **Phase 3: Rendering Pipeline** (OpenGL Backend, Vector Primitives)
- [x] **Phase 4: Widget Base Class & Tree Management** (Retained Mode, Event Propagation)
- [x] **Phase 5: Layout Engine & Essential UI Controls** (Box/Grid Layouts, Styling System, Basic Widgets)
- [x] **Phase 6: Advanced Framework Features** (Property Binding, Modal Dialogs, TableViews, Declarative UI Loading)

## Comparison with Qt / Missing Features
While UXCpp provides the core architectural foundations of a professional GUI framework, it is a lightweight alternative. To reach full parity with the massive Qt ecosystem, the following areas would need further development:

### 1. Component Depth
- **Widget Library**: Qt offers hundreds of specialized widgets (e.g., `QTreeWidget`, `QCalendarWidget`). UXCpp provides the essential primitives and composite patterns to build these.
- **Rich Text**: UXCpp currently supports basic text rendering; it lacks a full HTML/CSS rich-text engine like `QTextDocument`.

### 2. System Integration & Modules
- **Network & Database**: Unlike Qt, UXCpp does not include built-in modules for HTTP/FTP (`QtNetwork`) or SQL databases (`QtSql`). It is designed to be paired with existing C++ libraries (e.g., `curl`, `sqlite3`).
- **OS Abstractions**: Qt provides deep abstractions for file systems and processes. UXCpp focuses strictly on the UI layer, relying on standard C++ (`std::filesystem`) or OS APIs.

### 3. Advanced Tooling & DX
- **Visual Designer**: UXCpp implements a declarative loader, but lacks a visual "Drag-and-Drop" editor like Qt Designer.
- **Animation Framework**: While it supports basic updates, it does not yet have a dedicated animation system (like `QPropertyAnimation`).

### 4. Internationalization (i18n)
- **Translation**: UXCpp does not currently include a translation framework for multi-language support (`.ts`/`.qm` files).

## Build Requirements
- CMake 3.22+
- C++20 compatible compiler (MSVC / GCC / Clang)
- GLFW & OpenGL development libraries
