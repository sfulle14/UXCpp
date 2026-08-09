/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>

namespace uxcpp::core {

/**
 * @brief Abstract base class for table data models.
 * 
 * Similar to QAbstractItemModel, this decouples the data from the view.
 */
class TableModel {
public:
    virtual ~TableModel() = default;

    virtual int rowCount() const = 0;
    virtual int columnCount() const = 0;
    virtual std::string data(int row, int col) const = 0;
    virtual std::string headerData(int section, bool isRow) const = 0;
};

/**
 * @brief A simple implementation of TableModel for basic string data.
 */
class SimpleTableModel : public TableModel {
public:
    SimpleTableModel(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& data)
        : m_headers(headers), m_data(data) {}

    int rowCount() const override { return static_cast<int>(m_data.size()); }
    int columnCount() const override { return static_cast<int>(m_headers.size()); }
    std::string data(int row, int col) const override {
        if (row >= 0 && row < m_data.size() && col >= 0 && col < m_data[row].size()) {
            return m_data[row][col];
        }
        return "";
    }
    std::string headerData(int section, bool isRow) const override {
        if (!isRow && section >= 0 && section < m_headers.size()) {
            return m_headers[section];
        }
        return "";
    }

private:
    std::vector<std::string> m_headers;
    std::vector<std::vector<std::string>> m_data;
};

} // namespace uxcpp::core
