# UXCpp
A lightweight, cross-platform GUI framework for modern C++ (C++20/23).

## Goal
Designed as a clean-room, permissive replacement for Qt, allowing for closed-source and proprietary commercial use without licensing conflicts.

## Architectural Principles
- **Zero Copyleft:** No LGPL/GPL code. Strictly MIT/Apache 2.0/BSD.
- **Modern C++:** Leveraging C++20 concepts, RAII, and type-safety.
- **MOC-less:** Type-safe Signal/Slot system without a custom meta-object compiler.
- **Hardware Accelerated:** Designed for Vulkan/Metal/D3D12 backends.

## Progress Roadmap
- [x] **Phase 0: Project Setup & CMake Skeleton**
  - Directory structure established.
  - MIT License integrated.
  - Strict compiler hygiene configured.
- [x] **Phase 1: Event System & Core Signals**
  - Type-safe `Signal<Args...>` implementation.
  - Unit tests for connection/disconnection and emission.
- [ ] **Phase 2: Base App & Window Abstraction**
- [ ] **Phase 3: 2D Graphics Canvas & Rendering Pipeline**
- [ ] **Phase 4: Widget Base Class & Tree Management**
- [ ] **Phase 5: Layout Engine & Essential UI Controls**
- [ ] **Phase 6: Example App & Documentation**

## Build Requirements
- CMake 3.22+
- C++20 compatible compiler (MSVC / GCC / Clang)
