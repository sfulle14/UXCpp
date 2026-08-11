/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>

namespace uxcpp::core {

/**
 * @brief Represents a node in a hierarchical data structure.
 */
struct TreeNode {
    std::string text;
    std::vector<std::shared_ptr<TreeNode>> children;
    bool expanded = false;

    TreeNode(std::string t) : text(std::move(t)) {}
};

/**
 * @brief Abstract base class for tree-based data models.
 */
class TreeModel {
public:
    virtual ~TreeModel() = default;
    virtual std::shared_ptr<TreeNode> getRoot() const = 0;
};

/**
 * @brief A simple implementation of a TreeModel.
 */
class SimpleTreeModel : public TreeModel {
public:
    explicit SimpleTreeModel(std::shared_ptr<TreeNode> root) : m_root(std::move(root)) {}
    std::shared_ptr<TreeNode> getRoot() const override { return m_root; }

private:
    std::shared_ptr<TreeNode> m_root;
};

} // namespace uxcpp::core
