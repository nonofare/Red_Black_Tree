namespace RBT {

	template <typename T>
	class RedBlackTree {

		struct Node {
			T data;
			size_t index;
			bool is_red;
			Node* parent;
			Node* left;
			Node* right;

			Node(T inData, size_t inIndex) {
				data = inData;
				index = inIndex;
				is_red = false;
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

	public:

		RedBlackTree() {
			size = 0;
			next_index = 0;
			root = nullptr;
		}

		~RedBlackTree() {
			//RemoveAllUnder(root);
		}
	};
}