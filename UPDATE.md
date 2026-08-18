# UXCpp Remaining Roadmap

This document tracks the remaining work needed to make UXCpp a practical replacement for Qt in real desktop applications.

## Goals

- Reach feature depth that is sufficient for business applications, dashboards, editors, and internal tools.
- Keep the codebase clean-room and permissive.
- Preserve the current C++20 design and avoid introducing a meta-object compiler.

## Pending Work

### Phase 6: Advanced Graphics & Charts (Remaining)
Priority: medium to high

Work items:
- SVG support.
- Rounded rectangles, gradients, shadows, and clipping improvements.
- Better antialiasing.
- Chart types beyond line plots: scatter, bar, area, pie, candlestick.
- Zoom and pan for plots.
- Legend, axis, and tick customization.

Deliverables:
- Vector asset pipeline (SVG).
- Expanded charting subsystem.

### Phase 7: Application Services Implementation
Priority: medium

Work items:
- Full implementation of Networking helpers and HTTP client.
- SQL database access layer implementation.
- XML and JSON helpers.
- File system watcher.
- Process launch and control.
- Settings and configuration storage.
- Serial port support.

### Phase 9: Tooling and Developer Experience
Priority: medium

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

### Phase 10: Plugins and Extensibility
Priority: medium

Work items:
- Plugin loading interface.
- Module registration API.
- Backend discovery.
- Optional component packages.

Deliverables:
- A stable plugin ABI or API contract.

## Definition of “Full Replacement”
UXCpp should be considered a full replacement for Qt only when it can cover these categories without major external scaffolding:
- complex multilingual text,
- accessibility,
- OS integration,
- robust models and views,
- rich controls and editors,
- charts and graphics,
- resource packaging,
- and developer tooling.

## Near-Term Next Steps

The most valuable near-term additions are:
- SVG support and advanced charting,
- Full implementation of application service modules (HTTP/SQL),
- Visual UI designer and debugging tools.
