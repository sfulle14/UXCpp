# UXCpp Qt Parity Roadmap

This document tracks the remaining work needed to make UXCpp a practical replacement for Qt in real desktop applications.

UXCpp already covers the core UI foundation: retained-mode widgets, signals, layouts, theming, dialogs, tables, trees, plotting, and declarative loading. The remaining work is mostly about platform integration, text quality, accessibility, richer widgets, tooling, and ecosystem modules.

## Goals

- Reach feature depth that is sufficient for business applications, dashboards, editors, and internal tools.
- Keep the codebase clean-room and permissive.
- Preserve the current C++20 design and avoid introducing a meta-object compiler.
- Add functionality in phases so each step remains testable and shippable.

## Current State

Already implemented:
- Application lifecycle
- GLFW windowing backend
- OpenGL renderer
- Retained-mode widget tree
- Signals and reactive properties
- Box and Grid layouts
- Styling and theming
- Dialog overlays
- Focus traversal basics
- Accessibility metadata on widgets
- Keyboard shortcut registration
- Drag-and-drop framework hooks
- Native message boxes
- Menu bar and popup menu widgets
- Table and tree models/views
- Tab widgets, combo boxes, check boxes, text input
- Basic plotting support
- Declarative UI loading
- Animation primitives

Not yet at Qt parity:
- Text shaping and rich text
- Accessibility
- Clipboard, drag and drop, menus, file dialogs, system tray
- Clipboard, drag and drop, menus, file dialogs, message boxes, system tray (implemented)
- High-DPI and multi-window polish
- Full model/view virtualization and delegates
- Advanced charting
- Resource system and image pipeline
- Internationalization infrastructure
- Undo/redo framework
- Networking, SQL, XML, and other utility modules
- Plugin system
- Designer and debugging tooling

## Roadmap Phases

### Phase 1: Text and Typography
Priority: very high

Why this matters:
Text is one of the biggest gaps between a lightweight UI library and Qt. Without strong typography, the framework cannot support editors, forms, multilingual apps, or polished UI.

Work items:
- Add real font loading and caching.
- Integrate shaping for ligatures, complex scripts, and RTL text.
- Add bidirectional text support.
- Add font fallback and emoji support.
- Add rich text rendering with spans, links, and inline styling.
- Add text selection, caret rendering, and clipboard hooks for text widgets.

Deliverables:
- Text renderer backend abstraction.
- Rich text document model.
- Improved Label, TextInput, and future editor widgets.

Acceptance criteria:
- English, Arabic, Hebrew, and mixed-direction strings render correctly.
- Font fallback works when glyphs are missing.
- Multi-line and styled text render without layout corruption.

### Phase 2: Accessibility and Input Fidelity
Priority: very high

Why this matters:
Qt is strong on accessibility and input method support. UXCpp needs this to be usable in production and compliant with desktop expectations.

Work items:
- Add accessibility roles, labels, and focus semantics.
- Add screen-reader friendly metadata.
- Add keyboard navigation rules and focus traversal order.
- Add IME support for complex text entry.
- Add shortcut handling and accelerator registration.
- Add high-contrast theme support.

Deliverables:
- Accessibility tree API.
- Focus manager.
- Shortcut manager.
- Input method bridge.

Acceptance criteria:
- Widgets expose semantic roles and names.
- Keyboard-only navigation can reach all interactive controls.
- Text input works with non-Latin IMEs.

Status note:
- The framework now has focus traversal and widget accessibility metadata, so this phase is partially implemented.

### Phase 3: Native Platform Integration
Priority: very high

Why this matters:
Qt is widely used because it integrates deeply with the operating system. UXCpp needs the same practical features.

Work items:
- Clipboard API.
- Drag and drop.
- File dialogs.
- Standard menus and context menus.
- System tray integration.
- Native message boxes and alerts.
- Multi-window lifecycle management.
- Full high-DPI / scaling support.
- Multi-monitor positioning and screen querying.

Deliverables:
- Platform abstraction layer for clipboard and dialogs.
- Drag-and-drop payload API.
- Window manager helpers.

Acceptance criteria:
- Copy/paste works across applications.
- Dragging files or text into widgets works.
- Dialogs match the platform where possible.

Status note:
- Clipboard and file dialogs are implemented, and drag-and-drop framework hooks are now in place.
- Native message boxes are now implemented as part of the same service layer.
- System tray support is implemented for Windows via Shell_NotifyIcon.
- Menu bar and context menu widgets are now implemented as framework overlays.

### Phase 4: Model/View and Large Data Handling
Priority: high

Why this matters:
Qt’s model/view framework is one of its most powerful features. UXCpp needs better virtualization and delegate support to handle large datasets.

Work items:
- Sorting and filtering proxy models.
- Item delegates for custom cell rendering.
- In-place editors for rows and cells.
- Virtualized list and table rendering.
- Selection models.
- Incremental data updates and change notifications.
- Drag-and-drop reordering in views.

Deliverables:
- Proxy model layer.
- Delegate API.
- Selection and editing infrastructure.

Acceptance criteria:
- Tables with thousands of rows remain responsive.
- Sorting and filtering are handled without rebuilding the view manually.

### Phase 5: Rich Controls and Editor Widgets
Priority: high

Why this matters:
A Qt replacement needs more than buttons and panels. It needs the kinds of widgets used in real desktop tools.

Work items:
- Splitter widgets.
- Toolbar and status bar widgets.
- Menu bar and submenu support.
- Scroll bars with better behavior.
- List view and tree view virtualization.
- Numeric input controls.
- Slider, spin box, progress indicator polish.
- Text editor and code editor widgets.
- Docking and detachable panels.

Deliverables:
- Complete control catalog for business applications.
- Editor-grade text interaction.

Acceptance criteria:
- Common desktop layouts can be built without custom widgets.
- Editors support undo, selection, clipboard, and keyboard shortcuts.

### Phase 6: Graphics, Images, and Charts
Priority: medium to high

Why this matters:
Visual quality and data display are critical for dashboards, monitoring tools, and design apps.

Work items:
- Image loading and caching.
- SVG support.
- Transform stack for scale, rotate, and translate.
- Rounded rectangles, gradients, shadows, and clipping improvements.
- Better antialiasing.
- Chart types beyond line plots: scatter, bar, area, pie, candlestick.
- Zoom and pan for plots.
- Legend, axis, and tick customization.

Deliverables:
- Image and vector asset pipeline.
- Expanded charting subsystem.

Acceptance criteria:
- Image-heavy UIs load efficiently.
- Charts can be used in dashboard-style applications without custom rendering code.

### Phase 7: Application Services
Priority: medium

Why this matters:
Qt is not just a GUI library; it also includes a broad service layer. UXCpp should provide enough application utilities to reduce dependence on ad hoc third-party glue code.

Work items:
- Networking helpers and HTTP client abstraction.
- SQL database access layer.
- XML and JSON helpers.
- File system watcher.
- Process launch and control.
- Settings and configuration storage.
- Serial port support.

Deliverables:
- Small but coherent service modules.
- Async-friendly APIs.

Acceptance criteria:
- Applications can store settings, fetch remote data, and talk to local databases using consistent patterns.

### Phase 8: Internationalization and Resources
Priority: medium

Why this matters:
Qt is strong in multilingual applications and packaged assets.

Work items:
- Translation catalogs.
- Runtime language switching.
- Pluralization and locale-aware formatting.
- Resource packaging system for images, fonts, icons, and UI definitions.
- Embedded resource loading.

Deliverables:
- Translation manager.
- Resource bundle system.

Acceptance criteria:
- Language changes can be applied without restarting the app.
- Assets can ship in a single binary bundle or package.

### Phase 9: Undo/Redo and Application Editing
Priority: medium

Why this matters:
This is essential for document editors, design tools, and IDE-style apps.

Work items:
- Command pattern infrastructure.
- Undo stack and redo stack.
- Mergeable commands.
- Macro commands.
- History serialization if needed.

Deliverables:
- Reusable editing history engine.

Acceptance criteria:
- A text or scene editor can revert and replay actions reliably.

### Phase 10: Tooling and Developer Experience
Priority: medium

Why this matters:
Qt is successful partly because of its tooling. UXCpp needs a good developer experience to be attractive.

Work items:
- Visual UI designer.
- Property inspector.
- Live reload for UI definitions.
- Debug overlay for widget bounds and hit testing.
- Frame profiler and render diagnostics.
- Better error reporting for loaders and layouts.

Deliverables:
- Design-time tooling.
- Runtime inspection tools.

Acceptance criteria:
- Developers can inspect and debug widget trees without adding custom logging.

### Phase 11: Plugins and Extensibility
Priority: medium

Why this matters:
Qt has a large plugin story. UXCpp should support optional modules without making the core too large.

Work items:
- Plugin loading interface.
- Module registration API.
- Backend discovery.
- Optional component packages.

Deliverables:
- A stable plugin ABI or API contract.

Acceptance criteria:
- Third-party modules can register widgets, services, or render backends cleanly.

## Recommended Implementation Order

If the goal is a practical Qt replacement for business software, build in this order:
1. Text and typography.
2. Accessibility and input fidelity.
3. Clipboard, drag and drop, menus, and file dialogs.
4. Model/view virtualization.
5. Rich controls and editors.
6. Resource system and images.
7. Internationalization.
8. Undo/redo.
9. Tooling.
10. Service modules.
11. Plugins.

## Definition of “Full Replacement”
UXCpp should be considered a full replacement for Qt only when it can cover these categories without major external scaffolding:
- complex multilingual text,
- accessibility,
- OS integration,
- robust models and views,
- rich controls and editors,
- charts and graphics,
- resource packaging,
- undo/redo,
- and developer tooling.

## Near-Term Next Steps

The most valuable near-term additions are:
- font shaping and rich text,
- clipboard and file dialogs,
- accessibility metadata and focus traversal,
- model/view virtualization,
- and a stronger image/resource pipeline.

## Notes

- This roadmap intentionally keeps the core framework small and permissive.
- Features can be implemented as separate modules when possible.
- Each phase should end with documentation, examples, and a small test suite.
