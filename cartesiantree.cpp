#include "cartesiantree.h"

GraphicProject::CartesianTree::Node::Node() : left_(nullptr), right_(nullptr) {
    key_ = Random::GetInstance().GetInt();
    priority_ = Random::GetInstance().GetInt();
    min_key_ = max_key_ = key_;
}

GraphicProject::CartesianTree::Node::Node(int key)
    : key_(key), min_key_(key), max_key_(key), left_(nullptr), right_(nullptr) {
    priority_ = Random::GetInstance().GetInt();
}

GraphicProject::CartesianTree::Node::Node(std::pair<int, int> data)
    : key_(data.first), priority_(data.second), min_key_(data.first),
    max_key_(data.first), left_(nullptr), right_(nullptr) {
}

GraphicProject::CartesianTree::Node::Node(int key, int priority)
    : key_(key), priority_(priority), min_key_(key), max_key_(key),
    left_(nullptr), right_(nullptr) {}

GraphicProject::CartesianTree::Node::Node(CartesianTree::Node &&other) noexcept
    : key_(other.key_), priority_(other.priority_), min_key_(other.min_key_),
    max_key_(other.max_key_) {
    other.key_ = 0;
    other.priority_ = 0;
    other.min_key_ = 0;
    other.max_key_ = 0;
    left_.swap(other.left_);
    right_.swap(other.right_);
}

GraphicProject::CartesianTree::Node &
GraphicProject::CartesianTree::Node::operator=(
    CartesianTree::Node &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    std::swap(key_, other.key_);
    std::swap(priority_, other.priority_);
    std::swap(min_key_, other.min_key_);
    std::swap(max_key_, other.max_key_);
    left_.swap(other.left_);
    right_.swap(other.right_);
    return *this;
}

GraphicProject::CartesianTree::Node::PairNode
GraphicProject::CartesianTree::Node::Split(std::unique_ptr<Node> &root,
                                           int key) {
    if (!root) {
        return {nullptr, nullptr};
    }
    if (root->key_ < key) {
        PairNode q = Split(root->right_, key);
        root->right_ = std::move(q.first);
        Update_min_max(root);
        return {std::move(root), std::move(q.second)};
    } else {
        PairNode q = Split(root->left_, key);
        root->left_ = std::move(q.second);
        Update_min_max(root);
        return {std::move(q.first), std::move(root)};
    }
}

std::unique_ptr<GraphicProject::CartesianTree::Node> &
GraphicProject::CartesianTree::Node::Merge(std::unique_ptr<Node> &left,
                                           std::unique_ptr<Node> &right) {
    if (!left) {
        return right;
    }
    if (!right) {
        return left;
    }
    if (left->priority_ > right->priority_) {
        left->right_ = std::move(Merge(left->right_, right));
        Update_min_max(left);
        return left;
    } else {
        right->left_ = std::move(Merge(left, right->left_));
        Update_min_max(right);
        return right;
    }
}

int GraphicProject::CartesianTree::Node::GetMin(
    const std::unique_ptr<Node> &root) {
    return root ? root->min_key_ : INT_MAX;
}

int GraphicProject::CartesianTree::Node::GetMax(
    const std::unique_ptr<Node> &root) {
    return root ? root->max_key_ : INT_MIN;
}

bool GraphicProject::CartesianTree::Node::operator<(const Node &rhs) {
    return max_key_ < rhs.min_key_;
}

void GraphicProject::CartesianTree::Node::Update_min_max(
    const std::unique_ptr<Node> &root) {
    root->min_key_ =
        std::min(root->key_, std::min(GetMin(root->left_), GetMin(root->right_)));
    root->max_key_ =
        std::max(root->key_, std::max(GetMax(root->left_), GetMax(root->right_)));
}

GraphicProject::CartesianTree::CartesianTree() {
    roots_.reserve(10);
    roots_.push_back(nullptr);
}

GraphicProject::CartesianTree::CartesianTree(int key) {
    roots_.reserve(10);
    roots_.push_back(std::make_unique<Node>(key));
}

GraphicProject::CartesianTree::CartesianTree(int key, int priority) {
    roots_.reserve(10);
    roots_.push_back(std::make_unique<Node>(key, priority));
}

GraphicProject::CartesianTree &
GraphicProject::CartesianTree::operator=(CartesianTree &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    roots_.swap(other.roots_);
    return *this;
}

void GraphicProject::CartesianTree::Add(int key) {
    if (roots_.size() == 1 && !roots_[0]) {
        roots_[0] = std::make_unique<Node>(key);
    } else {
        roots_.push_back(std::make_unique<Node>(key));
    }
    Sort();
    Notify();
}

void GraphicProject::CartesianTree::Add(std::pair<int, int> data) {
    if (roots_.size() == 1 && !roots_[0]) {
        roots_[0] = std::make_unique<Node>(data);
    } else {
        roots_.push_back(std::make_unique<Node>(data));
    }
    Sort();
    Notify();
}

void GraphicProject::CartesianTree::Add(int key, int priority) {
    if (roots_.size() == 1 && !roots_[0]) {
        roots_[0] = std::make_unique<Node>(key, priority);
    } else {
        roots_.push_back(std::make_unique<Node>(key, priority));
    }
    Sort();
    Notify();
}

void GraphicProject::CartesianTree::Merge(size_t lhs, size_t rhs) {
    if (IsMergeable(lhs, rhs)) {
        roots_[lhs] = std::move(Node::Merge(roots_[lhs], roots_[rhs]));
        roots_[rhs].swap(roots_.back());
        roots_.pop_back();
        Sort();
        Notify();
    } else if (IsMergeable(rhs, lhs)) {
        roots_[rhs] = std::move(Node::Merge(roots_[rhs], roots_[lhs]));
        roots_[lhs].swap(roots_.back());
        roots_.pop_back();
        Sort();
        Notify();
    }
}

void GraphicProject::CartesianTree::Split(size_t index, int key) {
    Node::PairNode tmp = Node::Split(roots_[index], key);
    if (tmp.first) {
        roots_[index] = std::move(tmp.first);
        if (tmp.second) {
            roots_.emplace_back(std::move(tmp.second));
        }
    } else {
        roots_[index] = std::move(tmp.second);
    }
    Sort();
    Notify();
}

void GraphicProject::CartesianTree::Insert(size_t index, int key) {
    std::unique_ptr<Node> new_node(new Node(key));
    Node::PairNode tmp = Node::Split(roots_[index], key);
    tmp.first = std::move(Node::Merge(tmp.first, new_node));
    roots_[index] = std::move(Node::Merge(tmp.first, tmp.second));

    Sort();
    Notify();
}

void GraphicProject::CartesianTree::Insert(size_t index, int key,
                                           int priority) {
    std::unique_ptr<Node> new_node(new Node(key, priority));
    Node::PairNode tmp = Node::Split(roots_[index], key);
    tmp.first = std::move(Node::Merge(tmp.first, new_node));
    roots_[index] = std::move(Node::Merge(tmp.first, tmp.second));

    Sort();
    Notify();
}

void GraphicProject::CartesianTree::Erase(size_t index, int key) {
    Node::PairNode tmp = Node::Split(roots_[index], key + 1);
    Node::PairNode tmp2 = Node::Split(tmp.first, key);
    roots_[index] = std::move(Node::Merge(tmp2.first, tmp.second));

    if (!roots_[index]) {
        roots_[index].swap(roots_.back());
        roots_.pop_back();
    }
    Sort();
    Notify();
}

void GraphicProject::CartesianTree::Clear() {
    roots_.clear();
}

size_t GraphicProject::CartesianTree::GetSize() const {
    return roots_.size();
}

const std::unique_ptr<GraphicProject::CartesianTree::Node> &GraphicProject::CartesianTree::operator[](size_t index) {
    return roots_[index];
}

bool GraphicProject::CartesianTree::IsMergeable(size_t lhs, size_t rhs) {
    return Node::GetMax(roots_[lhs]) < Node::GetMin(roots_[rhs]);
}

void GraphicProject::CartesianTree::Sort() {
    std::sort(roots_.begin(), roots_.end());
}
