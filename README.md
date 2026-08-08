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

## Future Roadmap to Full Qt Replacement
To reach parity with a professional framework like Qt, the following milestones are planned:

### 1. Advanced Input & Interaction
- [ ] **Keyboard Focus Management**: Tab-navigation and focus rings.
- [ ] **Complex Event Handling**: Drag-and-drop, right-click context menus.
- [ ] **Input Bridge Refinement**: Full mapping of OS events to Widget methods.

### 2. High-Fidelity Rendering
- [ ] **Text Engine Integration**: Integrate FreeType/HarfBuzz for professional typography and RTL support.
- [ ] **Image Pipeline**: Implement a full texture loader (stb_image) and sprite batching.
- [ ] **Shader System**: Move from fixed-function OpenGL to GLSL shaders for rounded corners, gradients, and shadows.

### 3. Complex UI Components
- [ ] **Composite Widgets**: Dropdowns (ComboBox), Tabbed Views, TreeViews, and Data Tables.
- [ ] **Scrollable Containers**: Full implementation of `ScrollArea` with scrollbars and virtualized lists.
- [ ] **Modal System**: Dialog windows and overlay layers.

### 4. Framework Tooling & DX
- [ ] **Declarative UI**: A DSL or JSON/XML loader to define UIs without writing C++ code for every widget.
- [ ] **Binding System**: Property binding (similar to Qt's QProperty) for reactive data updates.
- [ ] **Packaging**: Full CPack configuration for binary distribution.

## Build Requirements
- CMake 3.22+
- C++20 compatible compiler (MSVC / GCC / Clang)
- GLFW & OpenGL development libraries
