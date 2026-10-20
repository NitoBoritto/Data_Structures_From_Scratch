#include <iostream>
#include <cstdlib>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value)
    {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};


class Binary_Search_Tree_Number {
private:

    Node* root;
    int length;

private:

    Node* copy_tree(Node* node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        Node* newNode = new Node(node->data);
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

        return node;
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

            if (node->left == nullptr)
            {
                Node* rightChild = node->right;
                delete node;
                return rightChild;
            }

            if (node->right == nullptr)
            {
                Node* leftChild = node->left;
                delete node;
                return leftChild;
            }

            // Replace the node with the smallest value from its right subtree
            Node* successor = find_minimum(node->right);
            node->data = successor->data;
            bool successorDeleted = false;
            node->right = delete_node(node->right, successor->data, successorDeleted);
        }

        return node;
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


    void display_postorder(Node* node)
    {
        if (node == nullptr)
        {
            return;
        }

        display_postorder(node->left);
        display_postorder(node->right);
        cout << node->data << " ";
    }


    void display_range(Node* node, int low, int high)
    {
        if (node == nullptr)
        {
            return;
        }

        if (node->data > low)
        {
            display_range(node->left, low, high);
        }

        if (node->data >= low && node->data <= high)
        {
            cout << node->data << " ";
        }

        if (node->data < high)
        {
            display_range(node->right, low, high);
        }
    }


    int get_height(Node* node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        int leftHeight = get_height(node->left);
        int rightHeight = get_height(node->right);
        return 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
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


    long long get_sum(Node* node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        return node->data + get_sum(node->left) + get_sum(node->right);
    }


    bool find_kth_smallest(Node* node, int& k, int& value)
    {
        if (node == nullptr)
        {
            return false;
        }

        if (find_kth_smallest(node->left, k, value))
        {
            return true;
        }

        k--;
        if (k == 0)
        {
            value = node->data;
            return true;
        }

        return find_kth_smallest(node->right, k, value);
    }


    Node* find_node(int value)
    {
        Node* current = root;

        while (current != nullptr)
        {
            if (value == current->data)
            {
                return current;
            }

            current = (value < current->data) ? current->left : current->right;
        }

        return nullptr;
    }

public:

    // Constructor
    // Creates an empty binary search tree
    Binary_Search_Tree_Number()
    {
        root = nullptr;
        length = 0;
    }


    // Destructor
    // Every node was created with new, so every node must be deleted
    ~Binary_Search_Tree_Number()
    {
        clear();
    }


    // Copy Constructor
    // Creates a deep copy of another tree
    Binary_Search_Tree_Number(const Binary_Search_Tree_Number& other)
    {
        root = copy_tree(other.root);
        length = other.length;
    }


    // Assignment Operator
    // Performs deep copy between trees
    Binary_Search_Tree_Number& operator=(const Binary_Search_Tree_Number& other)
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


    // Insert value while keeping the binary search tree order
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


    // Remove a value and keep the binary search tree order
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
        return find_node(value) != nullptr;
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


    // Return the level of a value, starting at level 0
    int get_level(int value)
    {
        Node* current = root;
        int level = 0;

        while (current != nullptr)
        {
            if (value == current->data)
            {
                return level;
            }

            current = (value < current->data) ? current->left : current->right;
            level++;
        }

        return -1;
    }


    // Return the k-th smallest value (k starts at 1)
    bool get_kth_smallest(int k, int& value)
    {
        if (k < 1 || k > length)
        {
            return false;
        }

        return find_kth_smallest(root, k, value);
    }


    // Print values between low and high, inclusive
    int display_range(int low, int high)
    {
        if (root == nullptr)
        {
            cout << "Tree is empty." << endl;
            return 0;
        }

        if (low > high)
        {
            cout << "Invalid range: minimum cannot be greater than maximum." << endl;
            return 0;
        }

        display_range(root, low, high);
        cout << endl;
        return 1;
    }


    // Find the lowest common ancestor of two values
    // Returns false if either value is missing
    bool get_lowest_common_ancestor(int first, int second, int& ancestor)
    {
        if (!search(first) || !search(second))
        {
            return false;
        }

        Node* current = root;
        while (current != nullptr)
        {
            if (first < current->data && second < current->data)
            {
                current = current->left;
            }
            else if (first > current->data && second > current->data)
            {
                current = current->right;
            }
            else
            {
                ancestor = current->data;
                return true;
            }
        }

        return false;
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


    // Print postorder traversal
    int display_postorder()
    {
        if (root == nullptr)
        {
            cout << "Tree is empty." << endl;
            return 0;
        }

        display_postorder(root);
        cout << endl;
        return 1;
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
        return get_height(root);
    }


    // Return number of leaf nodes
    int get_leaf_count()
    {
        return count_leaves(root);
    }


    // Return the sum of all values
    long long get_sum()
    {
        return get_sum(root);
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
    cout << "  Welcome To Binary Search Tree Application" << endl;
    cout << "============================================" << endl;

    Binary_Search_Tree_Number tree;
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Search Operations" << endl;
        cout << "3. Traversal Operations" << endl;
        cout << "4. Tree Operations" << endl;
        cout << "5. Tree Information" << endl;
        cout << "6. Exit" << endl;
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
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "              SEARCH OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Search Number" << endl;
                cout << "2. Get Level" << endl;
                cout << "3. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1 || choice == 2) {
                    int value;
                    cout << "\nEnter number: ";
                    read_input(value);
                    if (choice == 1) {
                        cout << (tree.search(value) ? "Number found." : "Number not found.") << endl;
                    }
                    else {
                        int level = tree.get_level(value);
                        if (level != -1) {
                            cout << "Number is at level " << level << endl;
                        }
                        else {
                            cout << "Number not found." << endl;
                        }
                    }
                }
                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 3);
        }
        else if (mainChoice == 3) {
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "             TRAVERSAL OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Inorder Traversal (Sorted)" << endl;
                cout << "2. Preorder Traversal" << endl;
                cout << "3. Postorder Traversal" << endl;
                cout << "4. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    tree.display_inorder();
                }
                else if (choice == 2) {
                    tree.display_preorder();
                }
                else if (choice == 3) {
                    tree.display_postorder();
                }
                else if (choice != 4) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 4);
        }
        else if (mainChoice == 4) {
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "                TREE OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Display Values In Range" << endl;
                cout << "2. Find K-th Smallest Number" << endl;
                cout << "3. Find Lowest Common Ancestor" << endl;
                cout << "4. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    int low;
                    int high;
                    cout << "\nEnter minimum value: ";
                    read_input(low);
                    cout << "Enter maximum value: ";
                    read_input(high);
                    tree.display_range(low, high);
                }
                else if (choice == 2) {
                    int k;
                    int value;
                    cout << "\nEnter k (starting from 1): ";
                    read_input(k);
                    if (tree.get_kth_smallest(k, value)) {
                        cout << "K-th smallest number: " << value << endl;
                    }
                    else {
                        cout << "Invalid k." << endl;
                    }
                }
                else if (choice == 3) {
                    int first;
                    int second;
                    int ancestor;
                    cout << "\nEnter first number: ";
                    read_input(first);
                    cout << "Enter second number: ";
                    read_input(second);
                    if (tree.get_lowest_common_ancestor(first, second, ancestor)) {
                        cout << "Lowest common ancestor: " << ancestor << endl;
                    }
                    else {
                        cout << "One or both numbers were not found." << endl;
                    }
                }
                else if (choice != 4) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 4);
        }
        else if (mainChoice == 5) {
            int minimum;
            int maximum;
            cout << "\nTree (inorder): ";
            tree.display_inorder();
            cout << "Nodes: " << tree.get_length() << endl;
            cout << "Height: " << tree.get_height() << endl;
            cout << "Leaves: " << tree.get_leaf_count() << endl;
            cout << "Sum: " << tree.get_sum() << endl;
            if (tree.get_minimum(minimum)) {
                cout << "Minimum: " << minimum << endl;
                tree.get_maximum(maximum);
                cout << "Maximum: " << maximum << endl;
            }
            cout << "Empty: " << (tree.is_empty() ? "Yes" : "No") << endl;
        }
        else if (mainChoice != 6) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 6);

    cout << "\nThank you for using Binary Search Tree Application." << endl;
    return 0;
}
