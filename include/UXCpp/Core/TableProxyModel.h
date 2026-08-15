/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Core/TableModel.h>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

namespace uxcpp::core {

/**
 * @brief A proxy model that allows sorting and filtering of a base TableModel.
 */
class TableProxyModel : public TableModel {
public:
    explicit TableProxyModel(std::shared_ptr<TableModel> source) 
        : m_source(std::move(source)) {
        updateMapping();
    }

    void setSortColumn(int column, bool ascending = true) {
        m_sortColumn = column;
        m_ascending = ascending;
        updateMapping();
    }

    void setFilter(const std::string& query) {
        m_filterQuery = query;
        updateMapping();
    }

    // TableModel overrides
    int rowCount() const override { return static_cast<int>(m_mapping.size()); }
    int columnCount() const override { return m_source ? m_source->columnCount() : 0; }
    std::string data(int row, int col) const override {
        if (row < 0 || row >= static_cast<int>(m_mapping.size())) return "";
        return m_source->data(m_mapping[row], col);
    }
    std::string headerData(int col, bool isSection) const override {
        return m_source ? m_source->headerData(col, isSection) : "";
    }

private:
    void updateMapping() {
        if (!m_source) return;

        // 1. Start with all indices
        std::vector<int> indices(m_source->rowCount());
        for (int i = 0; i < static_cast<int>(indices.size()); ++i) indices[i] = i;

        // 2. Filter
        if (!m_filterQuery.empty()) {
            auto it = std::remove_if(indices.begin(), indices.end(), [this](int row) {
                for (int col = 0; col < m_source->columnCount(); ++col) {
                    if (m_source->data(row, col).find(m_filterQuery) != std::string::npos) {
                        return false; // Keep it
                    }
                }
                return true; // Filter out
            });
            indices.erase(it, indices.end());
        }

        // 3. Sort
        if (m_sortColumn >= 0 && m_sortColumn < m_source->columnCount()) {
            std::sort(indices.begin(), indices.end(), [this](int a, int b) {
                const std::string& valA = m_source->data(a, m_sortColumn);
                const std::string& valB = m_source->data(b, m_sortColumn);
                return m_ascending ? valA < valB : valA > valB;
            });
        }

        m_mapping = std::move(indices);
    }

    std::shared_ptr<TableModel> m_source;
    std::vector<int> m_mapping;
    int m_sortColumn = -1;
    bool m_ascending = true;
    std::string m_filterQuery;
};

} // namespace uxcpp::core
