/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

namespace uxcpp::core {

/**
 * @brief Manages selection state for a list of items.
 */
class ItemSelectionModel {
public:
    enum class SelectionMode { Single, Multiple };

    explicit ItemSelectionModel(SelectionMode mode = SelectionMode::Single) 
        : m_mode(mode) {}

    void select(int index, bool multi = false) {
        if (m_mode == SelectionMode::Single || !multi) {
            m_selectedIndices.clear();
        }
        
        auto it = std::find(m_selectedIndices.begin(), m_selectedIndices.end(), index);
        if (it == m_selectedIndices.end()) {
            m_selectedIndices.push_back(index);
            std::sort(m_selectedIndices.begin(), m_selectedIndices.end());
        }
    }

    void deselect(int index) {
        auto it = std::remove(m_selectedIndices.begin(), m_selectedIndices.end(), index);
        m_selectedIndices.erase(it, m_selectedIndices.end());
    }

    bool isSelected(int index) const {
        return std::find(m_selectedIndices.begin(), m_selectedIndices.end(), index) != m_selectedIndices.end();
    }

    const std::vector<int>& getSelectedIndices() const {
        return m_selectedIndices;
    }

    void clear() {
        m_selectedIndices.clear();
    }

private:
    SelectionMode m_mode;
    std::vector<int> m_selectedIndices;
};

} // namespace uxcpp::core
