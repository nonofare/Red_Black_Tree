#pragma once
#include <string>
#include <vector>
#include <cmath>

namespace RBT {

	enum Color { RED, BLACK };

	template <typename T>
	class RedBlackTree {

		struct Node {
			T data;
			Color color;
			size_t index;
			Node* parent;
			Node* left;
			Node* right;

			Node(T in_data, size_t in_index) {
				data = in_data;
				color = RED;
				index = in_index;
				parent = nullptr;
				left = nullptr;
				right = nullptr;
			}

			~Node() {

			}
		};

		size_t size;
		size_t next_index;
		Node* root;

		int HeightOf(Node* node) const {
			if (!node) {
				return 0;
			}

			int rightCount = 0, leftCount = 0;
			if (node->right) {
				rightCount = HeightOf(node->right);
			}
			if (node->left) {
				leftCount = HeightOf(node->left);
			}

			return ((leftCount > rightCount) ? leftCount : rightCount) + 1;
		}

		bool Insert(Node* node, Node* current, bool (*cmp)(T, T)) {
			if (!node) {
				return false;
			}

			if (!current) {
				if (!root) {
					root = node;
					root->color = BLACK;
					size++;
					return true;
				}
				return false;
			}

			if (cmp) {
				if (cmp(node->data, current->data)) {
					if (!current->right) {
						current->right = node;
						node->parent = current;
					}
					else {
						return Insert(node, current->right, cmp);
					}
				}
				else {
					if (!current->left) {
						current->left = node;
						node->parent = current;
					}
					else {
						return Insert(node, current->left, cmp);
					}
				}
			}
			else if constexpr (std::is_arithmetic_v<T>) {
				if (node->data > current->data) {
					if (!current->right) {
						current->right = node;
						node->parent = current;
					}
					else {
						return Insert(node, current->right, cmp);
					}
				}
				else {
					if (!current->left) {
						current->left = node;
						node->parent = current;
					}
					else {
						return Insert(node, current->left, cmp);
					}
				}
			}
			else {
				return false;
			}

			size++;
			return InsertFix(node);
		}

		bool InsertFix(Node* node) {
			if (!node) {
				return false;
			}

			if (node->parent && node->parent->color != BLACK) {
				Node* node_p = node->parent;
				Node* node_gp = node->parent->parent;
				Node* node_u = (node_p == node_gp->left) ? node_gp->right : node_gp->left;

				if (node_u && node_u->color == RED) {
					node_p->color = BLACK;
					node_gp->color = RED;
					node_u->color = BLACK;

					return InsertFix(node_gp);
				}

				if (node_p == node_gp->left) {
					if (node == node_p->right) {
						node = node_p;
						RotateLeft(node);
					}

					node->parent->color = BLACK;
					node_gp->color = RED;
					RotateRight(node_gp);
				}
				else {
					if (node == node_p->left) {
						node = node_p;
						RotateRight(node);
					}

					node->parent->color = BLACK;
					node_gp->color = RED;
					RotateLeft(node_gp);
				}
			}

			root->color = BLACK;
			return true;
		}

		bool RotateLeft(Node* node) {
			Node* temp = node->right;
			if (!temp) {
				return false;
			}

			node->right = temp->left;
			if (temp->left) {
				temp->left->parent = node;
			}

			temp->parent = node->parent;
			if (!node->parent) {
				root = temp;
			}
			else if (node == node->parent->left) {
				node->parent->left = temp;
			}
			else {
				node->parent->right = temp;
			}

			temp->left = node;
			node->parent = temp;

			return true;
		}

		bool RotateRight(Node* node) {
			Node* temp = node->left;
			if (!temp) {
				return false;
			}

			node->left = temp->right;
			if (temp->right) {
				temp->right->parent = node;
			}

			temp->parent = node->parent;
			if (!node->parent) {
				root = temp;
			}
			else if (node == node->parent->left) {
				node->parent->left = temp;
			}
			else {
				node->parent->right = temp;
			}

			temp->right = node;
			node->parent = temp;

			return true;
		}

		bool RemoveAllUnder(Node* node) {
			if (!node) {
				return false;
			}

			if (node->left) {
				RemoveAllUnder(node->left);
			}
			if (node->right) {
				RemoveAllUnder(node->right);
			}

			if (node->parent) {
				if (node->parent->left == node) {
					node->parent->left = nullptr;
				}
				else {
					node->parent->right = nullptr;
				}
			}
			else {
				root = nullptr;
			}

			delete node;
			size--;

			return true;
		}

		Node* SearchFor(T data, Node* node, bool (*cmp1)(T, T), bool (*cmp2)(T, T)) const {
			if (!node) {
				return nullptr;
			}

			if (cmp2) {
				if (cmp2(data, node->data)) {
					return node;
				}

				if (cmp1(data, node->data)) {
					return SearchFor(data, node->right, cmp1, cmp2);
				}
				else {
					return SearchFor(data, node->left, cmp1, cmp2);
				}
			}
			else if constexpr (std::is_arithmetic_v<T>) {
				if (data == node->data) {
					return node;
				}

				if (data > node->data) {
					return SearchFor(data, node->right, cmp1, cmp2);
				}
				else {
					return SearchFor(data, node->left, cmp1, cmp2);
				}
			}
			else {
				return nullptr;
			}
		}

		Node* MaxRight(Node* node) const {
			if (!node || !node->right) {
				return node;
			}
			else {
				return MaxRight(node->right);
			}
		}

		Node* MaxLeft(Node* node) const {
			if (!node || !node->left) {
				return node;
			}
			else {
				return MaxLeft(node->left);
			}
		}

		Node* MinRight(Node* node) const {
			if (!node || !node->left) {
				return node;
			}
			else {
				return MinRight(node->left);
			}
		}

		Node* MinLeft(Node* node) const {
			if (!node || !node->right) {
				return node;
			}
			else {
				return MinLeft(node->right);
			}
		}

		void PreorderTraversal(Node* node, std::vector<Node*>& result) const {
			if (!node) {
				return;
			}

			result.push_back(node);
			if (node->left) {
				PreorderTraversal(node->left, result);
			}
			if (node->right) {
				PreorderTraversal(node->right, result);
			}
		}

		void InorderTraversal(Node* node, std::vector<Node*>& result) const {
			if (!node) {
				return;
			}

			if (node->left) {
				InorderTraversal(node->left, result);
			}
			result.push_back(node);
			if (node->right) {
				InorderTraversal(node->right, result);
			}
		}

		std::string CollectStrings(Node* node, unsigned int limit, std::string(*str)(T)) const {
			if (!node || limit == 0) {
				return "";
			}

			std::string text = "[ Index: ";
			text += std::to_string(int(node->index));

			text += " | Color: ";
			if (node->color == RED) {
				text += "Red";
			}
			else {
				text += "Black";
			}

			text += " | Data: ";
			if (str) {
				text += str(node->data);
			}
			else if constexpr (std::is_arithmetic_v<T>) {
				text += std::to_string(node->data);
			}
			else {
				text = "Data type is not supported and no method was provided\n";
			}

			text += " | Parent: *";
			if (node->parent) {
				text += std::to_string(int(node->parent->index));
			}
			else {
				text += "NL";
			}

			text += " | Left: *";
			if (node->left) {
				text += std::to_string(int(node->left->index));
			}
			else {
				text += "NL";
			}

			text += " | Right: *";
			if (node->right) {
				text += std::to_string(int(node->right->index));
			}
			else {
				text += "NL";
			}
			text += " ]\n";

			if (node->left) {
				text += CollectStrings(node->left, limit - 1, str);
			}
			if (node->right) {
				text += CollectStrings(node->right, limit - 1, str);
			}

			return text;
		}

	public:
		RedBlackTree() {
			size = 0;
			next_index = 0;
			root = nullptr;
		}

		~RedBlackTree() {
			RemoveAllUnder(root);
		}

		size_t Size() const {
			return size;
		}

		size_t Height() const {
			return HeightOf(root);
		}

		bool IsEmpty() const {
			return size == 0;
		}

		bool Push(T data, bool (*cmp)(T, T) = nullptr) {
			Node* node = new Node(data, next_index++);

			if (Insert(node, root, cmp)) {
				return true;
			}

			delete node;
			return false;
		}

		bool Erase() {
			if (RemoveAllUnder(root)) {
				next_index = 0;
				return true;
			}

			return false;
		}

		Node* Find(T data, bool (*cmp1)(T, T) = nullptr, bool (*cmp2)(T, T) = nullptr) const {
			return SearchFor(data, root, cmp1, cmp2);
		}

		std::vector<Node*> PreorderVector() const {
			std::vector<Node*> nodes;

			PreorderTraversal(root, nodes);

			return nodes;
		}

		std::vector<Node*> InorderVector() const {
			std::vector<Node*> nodes;

			InorderTraversal(root, nodes);

			return nodes;
		}

		std::string ToString(unsigned int limit = 0, std::string(*str)(T) = nullptr) const {
			std::string text = "Red Black Tree:\n";
			text += "size: " + std::to_string(int(size)) + "\n";
			text += "height: " + std::to_string(int(HeightOf(root))) + "\n";
			text += "log2size: " + std::to_string(log2(size)) + "\n";
			text += "size to height ratio: " + std::to_string(double(size) / HeightOf(root)) + "\n";
			text += "height to log2size ratio: " + std::to_string(double(HeightOf(root)) / log2(size)) + "\n";
			text += "{\n";

			if (limit <= 0) {
				limit = size;
			}

			text += CollectStrings(root, limit, str);

			if (limit < size) {
				text += "[...]\n";
			}

			text += "}\n";

			return text;
		}
	};
}