#pragma once
#include <string>
#include <vector>
#include <cmath>

namespace RBT {
    enum Color { RED, BLACK };

    template<typename T>
    class RedBlackTree {
        struct Node {
            T data;
            Color color;
            Node *parent;
            Node *left;
            Node *right;

            explicit Node(T data) : data(data) {
                color = RED;
                parent = nullptr;
                left = nullptr;
                right = nullptr;
            }

            ~Node() = default;
        };

        size_t size;
        Node *root;

        int height_of(Node *node) const {
            if (!node) { return 0; }

            int right_count = 0, left_count = 0;

            if (node->right) { right_count = height_of(node->right); }
            if (node->left) { left_count = height_of(node->left); }

            return (left_count > right_count ? left_count : right_count) + 1;
        }

        void insert(Node *node, Node *current, bool (*cmp_lgreater)(T, T)) {
            if (!root) {
                root = node;
                root->color = BLACK;
                size++;

                return;
            }

            if (cmp_lgreater) {
                if (cmp_lgreater(node->data, current->data)) {
                    if (!current->right) {
                        current->right = node;
                        node->parent = current;
                        size++;
                        insert_fix(node);
                    } else {
                        return insert(node, current->right, cmp_lgreater);
                    }
                } else {
                    if (!current->left) {
                        current->left = node;
                        node->parent = current;
                        size++;
                        insert_fix(node);
                    } else {
                        return insert(node, current->left, cmp_lgreater);
                    }
                }
            } else if constexpr (std::is_arithmetic_v<T>) {
                if (node->data > current->data) {
                    if (!current->right) {
                        current->right = node;
                        node->parent = current;
                        size++;
                        insert_fix(node);
                    } else {
                        return insert(node, current->right, cmp_lgreater);
                    }
                } else {
                    if (!current->left) {
                        current->left = node;
                        node->parent = current;
                        size++;
                        insert_fix(node);
                    } else {
                        return insert(node, current->left, cmp_lgreater);
                    }
                }
            } else {
                throw std::invalid_argument("T was not arithmetic and no comparator was provided");
            }
        }

        void insert_fix(Node *node) {
            while (node != root && node->parent->color == RED) {
                Node *p = node->parent;
                Node *gp = p->parent;

                if (!gp) break;

                if (p == gp->left) {
                    Node *u = gp->right;

                    if (u && u->color == RED) {
                        p->color = BLACK;
                        u->color = BLACK;
                        gp->color = RED;
                        node = gp;
                    } else {
                        if (node == p->right) {
                            node = p;
                            rotate_left(node);
                        }
                        p = node->parent;
                        gp = p->parent;
                        p->color = BLACK;
                        gp->color = RED;
                        rotate_right(gp);
                    }
                } else {
                    Node *u = gp->left;

                    if (u && u->color == RED) {
                        p->color = BLACK;
                        u->color = BLACK;
                        gp->color = RED;
                        node = gp;
                    } else {
                        if (node == p->left) {
                            node = p;
                            rotate_right(node);
                        }
                        p = node->parent;
                        gp = p->parent;
                        p->color = BLACK;
                        gp->color = RED;
                        rotate_left(gp);
                    }
                }
            }
            root->color = BLACK;
        }

        void rotate_left(Node *node) {
            Node *temp = node->right;
            if (!temp) { throw std::invalid_argument("Cannot rotate left on node with no right child"); }

            node->right = temp->left;
            if (temp->left) {
                temp->left->parent = node;
            }

            temp->parent = node->parent;
            if (!node->parent) {
                root = temp;
            } else if (node == node->parent->left) {
                node->parent->left = temp;
            } else {
                node->parent->right = temp;
            }

            temp->left = node;
            node->parent = temp;
        }

        void rotate_right(Node *node) {
            Node *temp = node->left;
            if (!temp) { throw std::invalid_argument("Cannot rotate right on node with no left child"); }

            node->left = temp->right;
            if (temp->right) {
                temp->right->parent = node;
            }

            temp->parent = node->parent;
            if (!node->parent) {
                root = temp;
            } else if (node == node->parent->left) {
                node->parent->left = temp;
            } else {
                node->parent->right = temp;
            }

            temp->right = node;
            node->parent = temp;
        }

        void remove_all_under(Node *node) {
            if (node->left) { remove_all_under(node->left); }
            if (node->right) { remove_all_under(node->right); }

            if (node->parent) {
                if (node->parent->left == node) {
                    node->parent->left = nullptr;
                } else {
                    node->parent->right = nullptr;
                }
            } else {
                root = nullptr;
            }

            delete node;
            size--;
        }

        Node *search_for(T data, Node *node, bool (*cmp_equal)(T, T), bool (*cmp_lgreater)(T, T)) const {
            if (!node) { return nullptr; }

            if (cmp_equal) {
                if (cmp_equal(data, node->data)) { return node; }

                if (cmp_lgreater(data, node->data)) {
                    return search_for(data, node->right, cmp_equal, cmp_lgreater);
                } else {
                    return search_for(data, node->left, cmp_equal, cmp_lgreater);
                }
            } else if constexpr (std::is_arithmetic_v<T>) {
                if (data == node->data) { return node; }

                if (data > node->data) {
                    return search_for(data, node->right, cmp_equal, cmp_lgreater);
                } else {
                    return search_for(data, node->left, cmp_equal, cmp_lgreater);
                }
            } else {
                return nullptr;
            }
        }

        Node *max_right(Node *node) const {
            if (!node || !node->right) { return node; }

            return max_right(node->right);
        }

        Node *max_left(Node *node) const {
            if (!node || !node->left) { return node; }

            return max_left(node->left);
        }

        Node *min_right(Node *node) const {
            if (!node || !node->left) { return node; }

            return min_right(node->left);
        }

        Node *min_left(Node *node) const {
            if (!node || !node->right) { return node; }

            return min_left(node->right);
        }

        void preorder_traversal(Node *node, std::vector<Node *> &result) const {
            result.push_back(node);
            if (node->left) { PreorderTraversal(node->left, result); }
            if (node->right) { PreorderTraversal(node->right, result); }
        }

        void inorder_traversal(Node *node, std::vector<Node *> &result) const {
            if (node->left) { InorderTraversal(node->left, result); }
            result.push_back(node);
            if (node->right) { InorderTraversal(node->right, result); }
        }

        std::string str_collect(Node *node, const unsigned int limit, std::string (*fun_str)(T)) const {
            if (!node) throw std::invalid_argument("Node was null");
            if (limit == 0) return "";

            std::string text = "[ Color: ";
            if (node->color == RED) { text += "Red"; } else { text += "Black"; }

            text += " | This: ";
            if (fun_str) {
                text += fun_str(node->data);
            } else if constexpr (std::is_arithmetic_v<T>) {
                text += std::to_string(node->data);
            } else {
                text = "T was not arithmetic nad no function was provided\n";
            }

            text += " | Parent: *";
            if (node->parent) {
                if (fun_str) {
                    text += fun_str(node->parent->data);
                } else if constexpr (std::is_arithmetic_v<T>) {
                    text += std::to_string(node->parent->data);
                } else {
                    text = "T was not arithmetic nad no function was provided\n";
                }
            } else {
                text += "NULL";
            }

            text += " | Left: *";
            if (node->left) {
                if (fun_str) {
                    text += fun_str(node->left->data);
                } else if constexpr (std::is_arithmetic_v<T>) {
                    text += std::to_string(node->left->data);
                } else {
                    text = "T was not arithmetic nad no function was provided\n";
                }
            } else {
                text += "NULL";
            }

            text += " | Right: *";
            if (node->right) {
                if (fun_str) {
                    text += fun_str(node->right->data);
                } else if constexpr (std::is_arithmetic_v<T>) {
                    text += std::to_string(node->right->data);
                } else {
                    text = "T was not arithmetic nad no function was provided\n";
                }
            } else {
                text += "NULL";
            }
            text += " ]\n";

            if (node->left) { text += str_collect(node->left, limit - 1, fun_str); }
            if (node->right) { text += str_collect(node->right, limit - 1, fun_str); }

            return text;
        }

    public:
        RedBlackTree() : size(0), root(nullptr) {
        }

        ~RedBlackTree() {
            remove_all_under(root);
        }

        size_t get_ize() const {
            return size;
        }

        size_t get_height() const {
            return height_of(root);
        }

        bool is_empty() const {
            return size == 0;
        }

        void push(T data, bool (*cmp)(T, T) = nullptr) {
            insert(new Node(data), root, cmp);
        }

        void erase() {
            remove_all_under(root);
        }

        Node *find(T data, bool (*cmp_equal)(T, T) = nullptr, bool (*cmp_lgreater)(T, T) = nullptr) const {
            return search_for(data, root, cmp_equal, cmp_lgreater);
        }

        std::vector<Node *> get_preorder() const {
            std::vector<Node *> nodes;
            PreorderTraversal(root, nodes);
            return nodes;
        }

        std::vector<Node *> get_inorder() const {
            std::vector<Node *> nodes;
            InorderTraversal(root, nodes);
            return nodes;
        }

        std::string to_str(unsigned int limit = 0, std::string (*fun_str)(T) = nullptr) const {
            std::string text = "Red Black Tree:\n";
            text += "size: " + std::to_string(static_cast<int>(size)) + "\n";
            text += "height: " + std::to_string(height_of(root)) + "\n";
            text += "log2size: " + std::to_string(log2(size)) + "\n";
            text += "size to height ratio: " + std::to_string(static_cast<double>(size) / height_of(root)) + "\n";
            text += "height to log2size ratio: " + std::to_string(static_cast<double>(height_of(root)) / log2(size)) +
                    "\n";
            text += "{\n";

            if (limit <= 0) limit = size;
            text += str_collect(root, limit, fun_str);
            if (limit < size) text += "[...]\n";

            text += "}\n";

            return text;
        }
    };
}
