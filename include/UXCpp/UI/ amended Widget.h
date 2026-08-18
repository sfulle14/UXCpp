#include "widget.h"

namespace ui {

class Widget : public BaseWidget {
 public:
  Widget();
  ~Widget();

  void setAccessibleRole(ui::AccessibleRole role) { m_accessibleRole = role; }
  ui::AccessibleRole getAccessibleRole() const { return m_accessibleRole; }

  std::string m_id = "widget_" + std::to_string(reinterpret_cast<uintptr_t>(this));

  LayoutParams& layoutParams() { return m_layoutParams; }
};

}  // namespace ui