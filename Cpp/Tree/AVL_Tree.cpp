#include <iostream>
#include <cstdlib>
using namespace std;

struct Node {
    int data;
    int height;
    Node* left;
    Node* right;

    Node(int value)
    {
        data = value;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};


class AVL_Tree_Number {
private:

    Node* root;
    int length;

private:

    int node_height(Node* node)
    {
        return (node == nullptr) ? 0 : node->height;
    }


    int maximum(int first, int second)
    {
        return (first > second) ? first : second;
    }


    int balance_factor(Node* node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        return node_height(node->left) - node_height(node->right);
    }


    void update_height(Node* node)
    {
        node->height = 1 + maximum(node_height(node->left), node_height(node->right));
    }


    // Right rotation fixes a left-heavy subtree
    Node* rotate_right(Node* oldRoot)
    {
        Node* newRoot = oldRoot->left;
        Node* movedSubtree = newRoot->right;

        newRoot->right = oldRoot;
        oldRoot->left = movedSubtree;

        update_height(oldRoot);
        update_height(newRoot);
        return newRoot;
    }


    // Left rotation fixes a right-heavy subtree
    Node* rotate_left(Node* oldRoot)
    {
        Node* newRoot = oldRoot->right;
        Node* movedSubtree = newRoot->left;

        newRoot->left = oldRoot;
        oldRoot->right = movedSubtree;

        update_height(oldRoot);
        update_height(newRoot);
        return newRoot;
    }


    Node* rebalance(Node* node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        update_height(node);
        int balance = balance_factor(node);

        // Left-left or left-right imbalance
        if (balance > 1)
        {
            if (balance_factor(node->left) < 0)
            {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }

        // Right-right or right-left imbalance
        if (balance < -1)
        {
            if (balance_factor(node->right) > 0)
            {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        }

        return node;
    }


    Node* copy_tree(Node* node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        Node* newNode = new Node(node->data);
        newNode->height = node->height;
        newNode->left = copy_tree(node->left);
        newNode->right = copy_tree(node->right);
        return newNode;
    }


    void delete_tree(Node* node)
    {
        if (node == nullptr)
        {
            return;
        }

        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
    }


    Node* insert_node(Node* node, int value, bool& inserted)
    {
        if (node == nullptr)
        {
            inserted = true;
            return new Node(value);
        }

        if (value < node->data)
        {
            node->left = insert_node(node->left, value, inserted);
        }
        else if (value > node->data)
        {
            node->right = insert_node(node->right, value, inserted);
        }
        else
        {
            return node;
        }

        return rebalance(node);
    }


    Node* find_minimum(Node* node)
    {
        while (node != nullptr && node->left != nullptr)
        {
            node = node->left;
        }

        return node;
    }


    Node* delete_node(Node* node, int value, bool& deleted)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (value < node->data)
        {
            node->left = delete_node(node->left, value, deleted);
        }
        else if (value > node->data)
        {
            node->right = delete_node(node->right, value, deleted);
        }
        else
        {
            deleted = true;

            if (node->left == nullptr || node->right == nullptr)
            {
                Node* child = (node->left != nullptr) ? node->left : node->right;
                delete node;
                return child;
            }

            // Replace the node with the smallest value from its right subtree
            Node* successor = find_minimum(node->right);
            node->data = successor->data;
            bool successorDeleted = false;
            node->right = delete_node(node->right, successor->data, successorDeleted);
        }

        return rebalance(node);
    }


    void display_inorder(Node* node)
    {
        if (node == nullptr)
        {
            return;
        }

        display_inorder(node->left);
        cout << node->data << " ";
        display_inorder(node->right);
    }


    void display_preorder(Node* node)
    {
        if (node == nullptr)
        {
            return;
        }

        cout << node->data << " ";
        display_preorder(node->left);
        display_preorder(node->right);
    }


    int count_leaves(Node* node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        if (node->left == nullptr && node->right == nullptr)
        {
            return 1;
        }

        return count_leaves(node->left) + count_leaves(node->right);
    }


public:

    // Constructor
    // Creates an empty AVL tree
    AVL_Tree_Number()
    {
        root = nullptr;
        length = 0;
    }


    // Destructor
    // Every node was created with new, so every node must be deleted
    ~AVL_Tree_Number()
    {
        clear();
    }


    // Copy Constructor
    // Creates a deep copy of another tree
    AVL_Tree_Number(const AVL_Tree_Number& other)
    {
        root = copy_tree(other.root);
        length = other.length;
    }


    // Assignment Operator
    // Performs deep copy between trees
    AVL_Tree_Number& operator=(const AVL_Tree_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        Node* newRoot = copy_tree(other.root);
        clear();
        root = newRoot;
        length = other.length;
        return *this;
    }


    // Insert value and rebalance the tree when needed
    // Returns 0 if the value already exists
    int insert(int value)
    {
        bool inserted = false;
        root = insert_node(root, value, inserted);

        if (inserted)
        {
            length++;
            return 1;
        }

        return 0;
    }


    // Remove value and rebalance the tree when needed
    bool delete_value(int value)
    {
        bool deleted = false;
        root = delete_node(root, value, deleted);

        if (deleted)
        {
            length--;
        }

        return deleted;
    }


    // Search for a value
    bool search(int value)
    {
        Node* current = root;

        while (current != nullptr)
        {
            if (value == current->data)
            {
                return true;
            }
            current = (value < current->data) ? current->left : current->right;
        }

        return false;
    }


    // Print values breadth-first, one level per line
    int display_level_order()
    {
        if (root == nullptr)
        {
            cout << "Tree is empty." << endl;
            return 0;
        }

        Node** queue = new Node*[length];
        int front = 0;
        int rear = 0;
        queue[rear++] = root;

        while (front < rear)
        {
            int levelSize = rear - front;
            while (levelSize > 0)
            {
                Node* current = queue[front++];
                cout << current->data << " ";

                if (current->left != nullptr)
                {
                    queue[rear++] = current->left;
                }
                if (current->right != nullptr)
                {
                    queue[rear++] = current->right;
                }

                levelSize--;
            }
            cout << endl;
        }

        delete[] queue;
        return 1;
    }


    // Print inorder traversal (sorted order)
    int display_inorder()
    {
        if (root == nullptr)
        {
            cout << "Tree is empty." << endl;
            return 0;
        }

        display_inorder(root);
        cout << endl;
        return 1;
    }


    // Print preorder traversal
    int display_preorder()
    {
        if (root == nullptr)
        {
            cout << "Tree is empty." << endl;
            return 0;
        }

        display_preorder(root);
        cout << endl;
        return 1;
    }


    // Return the smallest value through value
    bool get_minimum(int& value)
    {
        Node* minimum = find_minimum(root);
        if (minimum == nullptr)
        {
            return false;
        }

        value = minimum->data;
        return true;
    }


    // Return the largest value through value
    bool get_maximum(int& value)
    {
        Node* current = root;
        while (current != nullptr && current->right != nullptr)
        {
            current = current->right;
        }

        if (current == nullptr)
        {
            return false;
        }

        value = current->data;
        return true;
    }


    // Remove every node
    int clear()
    {
        delete_tree(root);
        root = nullptr;
        length = 0;
        return 1;
    }


    // Check if the tree has no nodes
    bool is_empty()
    {
        return root == nullptr;
    }


    // Return number of nodes
    int get_length()
    {
        return length;
    }


    // Return number of levels (an empty tree has height 0)
    int get_height()
    {
        return node_height(root);
    }


    // Return root's balance factor
    int get_root_balance_factor()
    {
        return balance_factor(root);
    }


    // Return number of leaf nodes
    int get_leaf_count()
    {
        return count_leaves(root);
    }
};


void read_input(int& value)
{
    if (!(cin >> value))
    {
        cerr << "Input error: expected an integer." << endl;
        std::exit(EXIT_FAILURE);
    }
}


int main()
{
    cout << "============================================" << endl;
    cout << "         Welcome To AVL Tree Application" << endl;
    cout << "============================================" << endl;

    AVL_Tree_Number tree;
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Search Operations" << endl;
        cout << "3. Traversal Operations" << endl;
        cout << "4. Tree Information" << endl;
        cout << "5. Exit" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        read_input(mainChoice);

        if (mainChoice == 1) {
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "              BASIC OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Insert Number" << endl;
                cout << "2. Delete Number" << endl;
                cout << "3. Clear Tree" << endl;
                cout << "4. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    int value;
                    cout << "\nEnter number: ";
                    read_input(value);
                    if (tree.insert(value)) {
                        cout << "Number added successfully." << endl;
                    }
                    else {
                        cout << "Number already exists." << endl;
                    }
                }
                else if (choice == 2) {
                    int value;
                    cout << "\nEnter number to delete: ";
                    read_input(value);
                    if (tree.delete_value(value)) {
                        cout << "Number deleted successfully." << endl;
                    }
                    else {
                        cout << "Number not found." << endl;
                    }
                }
                else if (choice == 3) {
                    tree.clear();
                    cout << "Tree cleared successfully." << endl;
                }
                else if (choice != 4) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 4);
        }
        else if (mainChoice == 2) {
            int value;
            cout << "\nEnter number to search: ";
            read_input(value);
            cout << (tree.search(value) ? "Number found." : "Number not found.") << endl;
        }
        else if (mainChoice == 3) {
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "             TRAVERSAL OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Level-Order Traversal" << endl;
                cout << "2. Inorder Traversal (Sorted)" << endl;
                cout << "3. Preorder Traversal" << endl;
                cout << "4. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    tree.display_level_order();
                }
                else if (choice == 2) {
                    tree.display_inorder();
                }
                else if (choice == 3) {
                    tree.display_preorder();
                }
                else if (choice != 4) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 4);
        }
        else if (mainChoice == 4) {
            int minimum;
            int maximum;
            cout << "\nTree (level order): " << endl;
            tree.display_level_order();
            cout << "Nodes: " << tree.get_length() << endl;
            cout << "Height: " << tree.get_height() << endl;
            cout << "Leaves: " << tree.get_leaf_count() << endl;
            cout << "Root balance factor: " << tree.get_root_balance_factor() << endl;
            if (tree.get_minimum(minimum)) {
                cout << "Minimum: " << minimum << endl;
                tree.get_maximum(maximum);
                cout << "Maximum: " << maximum << endl;
            }
            cout << "Empty: " << (tree.is_empty() ? "Yes" : "No") << endl;
        }
        else if (mainChoice != 5) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 5);

    cout << "\nThank you for using AVL Tree Application." << endl;
    return 0;
}
