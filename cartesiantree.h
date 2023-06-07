#ifndef CARTESIANTREE_H
#define CARTESIANTREE_H

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "random.h"
#include "observerpattern.h"

namespace GraphicProject {

class CartesianTree : public Observable {
public:
    class Node {
    public:
        using PairNode = std::pair<std::unique_ptr<Node>, std::unique_ptr<Node>>;

        Node();
        explicit Node(int key);
        explicit Node(std::pair<int, int> data);
        Node(int key, int priority);
        Node(const Node &other) = delete;
        Node(Node &&other) noexcept;

        Node &operator=(const Node &other) = delete;
        Node &operator=(Node &&other) noexcept;

        ~Node() = default;

        static PairNode Split(std::unique_ptr<Node> &root, int key);

        static std::unique_ptr<Node> &Merge(std::unique_ptr<Node> &left,
                                           std::unique_ptr<Node> &right);

        static int GetMin(const std::unique_ptr<Node> &root);
        static int GetMax(const std::unique_ptr<Node> &root);

        const std::unique_ptr<Node> &GetLeft() const { return left_; }
        const std::unique_ptr<Node> &GetRight() const { return right_; }
        int GetKey() const { return key_; }
        int GetPriority() const { return priority_; }

        bool operator<(const Node &rhs);

    protected:
        static void Update_min_max(const std::unique_ptr<Node> &root);

    protected:
        int key_;
        int priority_;
        int min_key_;
        int max_key_;
        std::unique_ptr<Node> left_;
        std::unique_ptr<Node> right_;
    };

public:
    CartesianTree();
    explicit CartesianTree(int key);
    CartesianTree(int key, int priority);
    CartesianTree(const CartesianTree &other) = delete;
    CartesianTree(CartesianTree &&other) noexcept { roots_.swap(other.roots_); }

    CartesianTree &operator=(const CartesianTree &other) = delete;
    CartesianTree &operator=(CartesianTree &&other) noexcept;

    ~CartesianTree() = default;

    void Add(int key);
    void Add(std::pair<int, int> data);
    void Add(int key, int priority);

    bool Merge(size_t lhs, size_t rhs);

    void Split(size_t index, int key);

    void Insert(size_t index, int key);
    void Insert(size_t index, int key, int priority);

    void Erase(size_t index, int key);

    void Clear();

    size_t GetSize() const;
    const std::unique_ptr<Node> &operator[](size_t index);

protected:
    bool IsMergeable(size_t lhs, size_t rhs);
    void Sort();

protected:
    std::vector<std::unique_ptr<Node>> roots_;
};

} // namespace GraphicProject

#endif // CARTESIANTREE_H
