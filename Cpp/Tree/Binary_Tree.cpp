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


class Binary_Tree_Number {
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


    void mirror_tree(Node* node)
    {
        if (node == nullptr)
        {
            return;
        }

        Node* temp = node->left;
        node->left = node->right;
        node->right = temp;

        mirror_tree(node->left);
        mirror_tree(node->right);
    }


    bool are_identical(Node* first, Node* second)
    {
        if (first == nullptr || second == nullptr)
        {
            return first == second;
        }

        return first->data == second->data
            && are_identical(first->left, second->left)
            && are_identical(first->right, second->right);
    }

public:

    // Constructor
    // Creates an empty binary tree
    Binary_Tree_Number()
    {
        root = nullptr;
        length = 0;
    }


    // Destructor
    // Every node was created with new, so every node must be deleted
    ~Binary_Tree_Number()
    {
        clear();
    }


    // Copy Constructor
    // Creates a deep copy of another tree
    Binary_Tree_Number(const Binary_Tree_Number& other)
    {
        root = copy_tree(other.root);
        length = other.length;
    }


    // Assignment Operator
    // Performs deep copy between trees
    Binary_Tree_Number& operator=(const Binary_Tree_Number& other)
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


    // Insert value in the first available position, from left to right
    int insert(int value)
    {
        Node* newNode = new Node(value);

        if (root == nullptr)
        {
            root = newNode;
            length++;
            return 1;
        }

        Node** queue = new Node*[length];
        int front = 0;
        int rear = 0;
        queue[rear++] = root;

        while (front < rear)
        {
            Node* current = queue[front++];

            if (current->left == nullptr)
            {
                current->left = newNode;
                delete[] queue;
                length++;
                return 1;
            }
            queue[rear++] = current->left;

            if (current->right == nullptr)
            {
                current->right = newNode;
                delete[] queue;
                length++;
                return 1;
            }
            queue[rear++] = current->right;
        }

        delete[] queue;
        delete newNode;
        return 0;
    }


    // Search for a value and return its level-order index, or -1 if it is missing
    int search(int value)
    {
        if (root == nullptr)
        {
            return -1;
        }

        Node** queue = new Node*[length];
        int front = 0;
        int rear = 0;
        queue[rear++] = root;

        while (front < rear)
        {
            Node* current = queue[front];

            if (current->data == value)
            {
                delete[] queue;
                return front;
            }

            front++;

            if (current->left != nullptr)
            {
                queue[rear++] = current->left;
            }
            if (current->right != nullptr)
            {
                queue[rear++] = current->right;
            }
        }

        delete[] queue;
        return -1;
    }


    // Delete the first matching value in level order
    // The deepest, rightmost node takes its place
    bool delete_value(int value)
    {
        if (root == nullptr)
        {
            return false;
        }

        Node** queue = new Node*[length];
        Node** parents = new Node*[length];
        int front = 0;
        int rear = 0;
        int targetIndex = -1;
        int lastIndex = -1;

        queue[rear] = root;
        parents[rear++] = nullptr;

        while (front < rear)
        {
            int currentIndex = front++;
            Node* current = queue[currentIndex];

            if (targetIndex == -1 && current->data == value)
            {
                targetIndex = currentIndex;
            }

            lastIndex = currentIndex;

            if (current->left != nullptr)
            {
                queue[rear] = current->left;
                parents[rear++] = current;
            }
            if (current->right != nullptr)
            {
                queue[rear] = current->right;
                parents[rear++] = current;
            }
        }

        if (targetIndex == -1)
        {
            delete[] queue;
            delete[] parents;
            return false;
        }

        Node* target = queue[targetIndex];
        Node* last = queue[lastIndex];
        Node* lastParent = parents[lastIndex];

        if (target != last)
        {
            target->data = last->data;
        }

        if (lastParent == nullptr)
        {
            root = nullptr;
        }
        else if (lastParent->left == last)
        {
            lastParent->left = nullptr;
        }
        else
        {
            lastParent->right = nullptr;
        }

        delete last;
        length--;
        delete[] queue;
        delete[] parents;

        return true;
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


    // Print inorder traversal
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


    // Exchange the left and right child of every node
    int mirror()
    {
        mirror_tree(root);
        return 1;
    }


    // Compare this tree with another tree
    bool is_identical(const Binary_Tree_Number& other)
    {
        return are_identical(root, other.root);
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
    cout << "       Welcome To Binary Tree Application" << endl;
    cout << "============================================" << endl;

    Binary_Tree_Number tree;
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Traversal Operations" << endl;
        cout << "3. Tree Operations" << endl;
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
                cout << "3. Search Number" << endl;
                cout << "4. Clear Tree" << endl;
                cout << "5. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    int value;
                    cout << "\nEnter number: ";
                    read_input(value);
                    tree.insert(value);
                    cout << "Number added successfully." << endl;
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
                    int value;
                    cout << "\nEnter number to search: ";
                    read_input(value);
                    int index = tree.search(value);
                    if (index != -1) {
                        cout << "Number found at level-order index " << index << endl;
                    }
                    else {
                        cout << "Number not found." << endl;
                    }
                }
                else if (choice == 4) {
                    tree.clear();
                    cout << "Tree cleared successfully." << endl;
                }
                else if (choice != 5) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 5);
        }
        else if (mainChoice == 2) {
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "             TRAVERSAL OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Level-Order Traversal" << endl;
                cout << "2. Preorder Traversal" << endl;
                cout << "3. Inorder Traversal" << endl;
                cout << "4. Postorder Traversal" << endl;
                cout << "5. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    tree.display_level_order();
                }
                else if (choice == 2) {
                    tree.display_preorder();
                }
                else if (choice == 3) {
                    tree.display_inorder();
                }
                else if (choice == 4) {
                    tree.display_postorder();
                }
                else if (choice != 5) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 5);
        }
        else if (mainChoice == 3) {
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "                TREE OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Mirror Tree" << endl;
                cout << "2. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    tree.mirror();
                    cout << "Tree mirrored successfully." << endl;
                }
                else if (choice != 2) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 2);
        }
        else if (mainChoice == 4) {
            cout << "\nTree: " << endl;
            tree.display_level_order();
            cout << "Nodes: " << tree.get_length() << endl;
            cout << "Height: " << tree.get_height() << endl;
            cout << "Leaves: " << tree.get_leaf_count() << endl;
            cout << "Sum: " << tree.get_sum() << endl;
            cout << "Empty: " << (tree.is_empty() ? "Yes" : "No") << endl;
        }
        else if (mainChoice != 5) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 5);

    cout << "\nThank you for using Binary Tree Application." << endl;
    return 0;
}
