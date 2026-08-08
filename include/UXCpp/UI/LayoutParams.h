/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

namespace uxcpp::ui {

struct Spacing {
    float top = 0, bottom = 0, left = 0, right = 0;
};

enum class Alignment { Start, Center, End };

struct LayoutParams {
    Spacing margin;
    Spacing padding;
    Alignment horizontalAlign = Alignment::Start;
    Alignment verticalAlign = Alignment::Start;
    float preferredWidth = -1.0f; // -1 for auto
    float preferredHeight = -1.0f;
};

} // namespace uxcpp::ui
