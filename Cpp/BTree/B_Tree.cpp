#include <iostream>
#include <cstdlib>
using namespace std;

struct B_Tree_Node {
    int* keys;
    B_Tree_Node** children;
    int keyCount;
    bool isLeaf;

    B_Tree_Node(int minimumDegree, bool Leaf)
    {
        keys = new int[(2 * minimumDegree) - 1];
        children = new B_Tree_Node*[2 * minimumDegree];
        keyCount = 0;
        isLeaf = Leaf;

        for (int i = 0; i < 2 * minimumDegree; i++)
        {
            children[i] = nullptr;
        }
    }

    ~B_Tree_Node()
    {
        delete[] keys;
        delete[] children;
    }
};


class B_Tree_Number {
private:

    B_Tree_Node* root;
    int minimumDegree;
    int length;

private:

    B_Tree_Node* copy_node(const B_Tree_Node* node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        B_Tree_Node* copy = new B_Tree_Node(minimumDegree, node->isLeaf);
        copy->keyCount = node->keyCount;

        for (int i = 0; i < node->keyCount; i++)
        {
            copy->keys[i] = node->keys[i];
        }

        if (!node->isLeaf)
        {
            for (int i = 0; i <= node->keyCount; i++)
            {
                copy->children[i] = copy_node(node->children[i]);
            }
        }

        return copy;
    }


    void delete_tree(B_Tree_Node* node)
    {
        if (node == nullptr)
        {
            return;
        }

        if (!node->isLeaf)
        {
            for (int i = 0; i <= node->keyCount; i++)
            {
                delete_tree(node->children[i]);
            }
        }

        delete node;
    }


    void split_child(B_Tree_Node* parent, int childIndex)
    {
        B_Tree_Node* fullChild = parent->children[childIndex];
        B_Tree_Node* rightChild = new B_Tree_Node(minimumDegree, fullChild->isLeaf);
        rightChild->keyCount = minimumDegree - 1;

        for (int i = 0; i < minimumDegree - 1; i++)
        {
            rightChild->keys[i] = fullChild->keys[i + minimumDegree];
        }

        if (!fullChild->isLeaf)
        {
            for (int i = 0; i < minimumDegree; i++)
            {
                rightChild->children[i] = fullChild->children[i + minimumDegree];
                fullChild->children[i + minimumDegree] = nullptr;
            }
        }

        int median = fullChild->keys[minimumDegree - 1];
        fullChild->keyCount = minimumDegree - 1;

        for (int i = parent->keyCount; i >= childIndex + 1; i--)
        {
            parent->children[i + 1] = parent->children[i];
        }
        parent->children[childIndex + 1] = rightChild;

        for (int i = parent->keyCount - 1; i >= childIndex; i--)
        {
            parent->keys[i + 1] = parent->keys[i];
        }

        parent->keys[childIndex] = median;
        parent->keyCount++;
    }


    void insert_non_full(B_Tree_Node* node, int value)
    {
        int index = node->keyCount - 1;

        if (node->isLeaf)
        {
            while (index >= 0 && node->keys[index] > value)
            {
                node->keys[index + 1] = node->keys[index];
                index--;
            }

            node->keys[index + 1] = value;
            node->keyCount++;
            return;
        }

        while (index >= 0 && node->keys[index] > value)
        {
            index--;
        }
        index++;

        if (node->children[index]->keyCount == (2 * minimumDegree) - 1)
        {
            split_child(node, index);
            if (value > node->keys[index])
            {
                index++;
            }
        }

        insert_non_full(node->children[index], value);
    }


    int find_key_index(B_Tree_Node* node, int value)
    {
        int index = 0;
        while (index < node->keyCount && node->keys[index] < value)
        {
            index++;
        }

        return index;
    }


    int get_predecessor(B_Tree_Node* node)
    {
        while (!node->isLeaf)
        {
            node = node->children[node->keyCount];
        }

        return node->keys[node->keyCount - 1];
    }


    int get_successor(B_Tree_Node* node)
    {
        while (!node->isLeaf)
        {
            node = node->children[0];
        }

        return node->keys[0];
    }


    void remove_from_leaf(B_Tree_Node* node, int index)
    {
        for (int i = index + 1; i < node->keyCount; i++)
        {
            node->keys[i - 1] = node->keys[i];
        }

        node->keyCount--;
    }


    void borrow_from_previous(B_Tree_Node* parent, int childIndex)
    {
        B_Tree_Node* child = parent->children[childIndex];
        B_Tree_Node* sibling = parent->children[childIndex - 1];

        for (int i = child->keyCount - 1; i >= 0; i--)
        {
            child->keys[i + 1] = child->keys[i];
        }

        if (!child->isLeaf)
        {
            for (int i = child->keyCount; i >= 0; i--)
            {
                child->children[i + 1] = child->children[i];
            }
            child->children[0] = sibling->children[sibling->keyCount];
            sibling->children[sibling->keyCount] = nullptr;
        }

        child->keys[0] = parent->keys[childIndex - 1];
        parent->keys[childIndex - 1] = sibling->keys[sibling->keyCount - 1];
        child->keyCount++;
        sibling->keyCount--;
    }


    void borrow_from_next(B_Tree_Node* parent, int childIndex)
    {
        B_Tree_Node* child = parent->children[childIndex];
        B_Tree_Node* sibling = parent->children[childIndex + 1];

        child->keys[child->keyCount] = parent->keys[childIndex];

        if (!child->isLeaf)
        {
            child->children[child->keyCount + 1] = sibling->children[0];

            for (int i = 1; i <= sibling->keyCount; i++)
            {
                sibling->children[i - 1] = sibling->children[i];
            }
            sibling->children[sibling->keyCount] = nullptr;
        }

        parent->keys[childIndex] = sibling->keys[0];
        for (int i = 1; i < sibling->keyCount; i++)
        {
            sibling->keys[i - 1] = sibling->keys[i];
        }

        child->keyCount++;
        sibling->keyCount--;
    }


    void merge_children(B_Tree_Node* parent, int childIndex)
    {
        B_Tree_Node* left = parent->children[childIndex];
        B_Tree_Node* right = parent->children[childIndex + 1];

        left->keys[minimumDegree - 1] = parent->keys[childIndex];
        for (int i = 0; i < right->keyCount; i++)
        {
            left->keys[i + minimumDegree] = right->keys[i];
        }

        if (!left->isLeaf)
        {
            for (int i = 0; i <= right->keyCount; i++)
            {
                left->children[i + minimumDegree] = right->children[i];
                right->children[i] = nullptr;
            }
        }

        left->keyCount += right->keyCount + 1;

        for (int i = childIndex + 1; i < parent->keyCount; i++)
        {
            parent->keys[i - 1] = parent->keys[i];
        }
        for (int i = childIndex + 2; i <= parent->keyCount; i++)
        {
            parent->children[i - 1] = parent->children[i];
        }

        parent->children[parent->keyCount] = nullptr;
        parent->keyCount--;
        delete right;
    }


    void fill_child(B_Tree_Node* parent, int childIndex)
    {
        if (childIndex > 0
            && parent->children[childIndex - 1]->keyCount >= minimumDegree)
        {
            borrow_from_previous(parent, childIndex);
        }
        else if (childIndex < parent->keyCount
            && parent->children[childIndex + 1]->keyCount >= minimumDegree)
        {
            borrow_from_next(parent, childIndex);
        }
        else if (childIndex < parent->keyCount)
        {
            merge_children(parent, childIndex);
        }
        else
        {
            merge_children(parent, childIndex - 1);
        }
    }


    bool remove_from_internal(B_Tree_Node* node, int index)
    {
        int value = node->keys[index];

        if (node->children[index]->keyCount >= minimumDegree)
        {
            int predecessor = get_predecessor(node->children[index]);
            node->keys[index] = predecessor;
            return remove_value(node->children[index], predecessor);
        }

        if (node->children[index + 1]->keyCount >= minimumDegree)
        {
            int successor = get_successor(node->children[index + 1]);
            node->keys[index] = successor;
            return remove_value(node->children[index + 1], successor);
        }

        merge_children(node, index);
        return remove_value(node->children[index], value);
    }


    bool remove_value(B_Tree_Node* node, int value)
    {
        int index = find_key_index(node, value);

        if (index < node->keyCount && node->keys[index] == value)
        {
            if (node->isLeaf)
            {
                remove_from_leaf(node, index);
                return true;
            }

            return remove_from_internal(node, index);
        }

        if (node->isLeaf)
        {
            return false;
        }

        bool wasLastChild = (index == node->keyCount);
        if (node->children[index]->keyCount < minimumDegree)
        {
            fill_child(node, index);
        }

        if (wasLastChild && index > node->keyCount)
        {
            return remove_value(node->children[index - 1], value);
        }

        return remove_value(node->children[index], value);
    }


    void display_inorder(B_Tree_Node* node)
    {
        if (node == nullptr)
        {
            return;
        }

        for (int i = 0; i < node->keyCount; i++)
        {
            if (!node->isLeaf)
            {
                display_inorder(node->children[i]);
            }
            cout << node->keys[i] << " ";
        }

        if (!node->isLeaf)
        {
            display_inorder(node->children[node->keyCount]);
        }
    }


    void display_range(B_Tree_Node* node, int low, int high)
    {
        if (node == nullptr)
        {
            return;
        }

        for (int i = 0; i < node->keyCount; i++)
        {
            if (!node->isLeaf && low <= node->keys[i])
            {
                display_range(node->children[i], low, high);
            }

            if (node->keys[i] >= low && node->keys[i] <= high)
            {
                cout << node->keys[i] << " ";
            }
        }

        if (!node->isLeaf && high >= node->keys[node->keyCount - 1])
        {
            display_range(node->children[node->keyCount], low, high);
        }
    }


    int get_height(B_Tree_Node* node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        int height = 1;
        while (!node->isLeaf)
        {
            height++;
            node = node->children[0];
        }

        return height;
    }

public:

    // Constructor
    // Creates an empty B-tree with the requested minimum degree
    B_Tree_Number(int Minimum_Degree = 2)
    {
        minimumDegree = (Minimum_Degree >= 2) ? Minimum_Degree : 2;
        root = nullptr;
        length = 0;
    }


    // Destructor
    // Releases all B-tree nodes
    ~B_Tree_Number()
    {
        clear();
    }


    // Copy Constructor
    // Creates a deep copy of another B-tree
    B_Tree_Number(const B_Tree_Number& other)
    {
        minimumDegree = other.minimumDegree;
        length = other.length;
        root = copy_node(other.root);
    }


    // Assignment Operator
    // Performs deep copy between B-trees
    B_Tree_Number& operator=(const B_Tree_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        B_Tree_Number temp(other);
        B_Tree_Node* oldRoot = root;
        root = temp.root;
        temp.root = oldRoot;
        minimumDegree = temp.minimumDegree;

        int oldLength = length;
        length = temp.length;
        temp.length = oldLength;
        return *this;
    }


    // Insert a unique value while preserving B-tree balance
    bool insert(int value)
    {
        if (search(value))
        {
            return false;
        }

        if (root == nullptr)
        {
            root = new B_Tree_Node(minimumDegree, true);
            root->keys[0] = value;
            root->keyCount = 1;
            length++;
            return true;
        }

        if (root->keyCount == (2 * minimumDegree) - 1)
        {
            B_Tree_Node* newRoot = new B_Tree_Node(minimumDegree, false);
            newRoot->children[0] = root;
            split_child(newRoot, 0);

            int childIndex = (value > newRoot->keys[0]) ? 1 : 0;
            insert_non_full(newRoot->children[childIndex], value);
            root = newRoot;
        }
        else
        {
            insert_non_full(root, value);
        }

        length++;
        return true;
    }


    // Search for a value
    bool search(int value)
    {
        B_Tree_Node* current = root;

        while (current != nullptr)
        {
            int index = 0;
            while (index < current->keyCount && value > current->keys[index])
            {
                index++;
            }

            if (index < current->keyCount && current->keys[index] == value)
            {
                return true;
            }

            if (current->isLeaf)
            {
                return false;
            }

            current = current->children[index];
        }

        return false;
    }


    // Remove a value while preserving B-tree occupancy and balance
    bool remove(int value)
    {
        if (!search(value))
        {
            return false;
        }

        remove_value(root, value);
        length--;

        if (root->keyCount == 0)
        {
            B_Tree_Node* oldRoot = root;
            if (root->isLeaf)
            {
                root = nullptr;
            }
            else
            {
                root = root->children[0];
                oldRoot->children[0] = nullptr;
            }
            delete oldRoot;
        }

        return true;
    }


    // Print values in sorted order
    int display()
    {
        if (root == nullptr)
        {
            cout << "B-tree is empty." << endl;
            return 0;
        }

        display_inorder(root);
        cout << endl;
        return 1;
    }


    // Print values in the inclusive range [low, high]
    int display_range(int low, int high)
    {
        if (root == nullptr)
        {
            cout << "B-tree is empty." << endl;
            return 0;
        }
        if (low > high)
        {
            cout << "Invalid range." << endl;
            return 0;
        }

        display_range(root, low, high);
        cout << endl;
        return 1;
    }


    // Return the smallest value through value
    bool get_minimum(int& value)
    {
        if (root == nullptr)
        {
            return false;
        }

        B_Tree_Node* current = root;
        while (!current->isLeaf)
        {
            current = current->children[0];
        }

        value = current->keys[0];
        return true;
    }


    // Return the largest value through value
    bool get_maximum(int& value)
    {
        if (root == nullptr)
        {
            return false;
        }

        B_Tree_Node* current = root;
        while (!current->isLeaf)
        {
            current = current->children[current->keyCount];
        }

        value = current->keys[current->keyCount - 1];
        return true;
    }


    // Remove all values
    int clear()
    {
        delete_tree(root);
        root = nullptr;
        length = 0;
        return 1;
    }


    // Check if the tree has no values
    bool is_empty()
    {
        return root == nullptr;
    }


    // Return number of stored values
    int get_length()
    {
        return length;
    }


    // Return number of levels
    int get_height()
    {
        return get_height(root);
    }


    // Return the configured minimum degree
    int get_minimum_degree()
    {
        return minimumDegree;
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
    cout << "          Welcome To B-Tree Application" << endl;
    cout << "============================================" << endl;

    int minimumDegree;
    do {
        cout << "\nEnter minimum degree (at least 2): ";
        read_input(minimumDegree);
        if (minimumDegree < 2) {
            cout << "Minimum degree must be at least 2." << endl;
        }
    } while (minimumDegree < 2);

    B_Tree_Number tree(minimumDegree);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Search Operations" << endl;
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
                cout << "3. Clear Tree" << endl;
                cout << "4. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1 || choice == 2) {
                    int value;
                    cout << (choice == 1 ? "\nEnter number to insert: " : "\nEnter number to delete: ");
                    read_input(value);
                    if (choice == 1) {
                        cout << (tree.insert(value) ? "Number added successfully." : "Number already exists.") << endl;
                    }
                    else {
                        cout << (tree.remove(value) ? "Number deleted successfully." : "Number not found.") << endl;
                    }
                }
                else if (choice == 3) {
                    tree.clear();
                    cout << "B-tree cleared successfully." << endl;
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
                cout << "               TREE OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Display Sorted Values" << endl;
                cout << "2. Display Values In Range" << endl;
                cout << "3. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    tree.display();
                }
                else if (choice == 2) {
                    int low;
                    int high;
                    cout << "\nEnter minimum value: ";
                    read_input(low);
                    cout << "Enter maximum value: ";
                    read_input(high);
                    tree.display_range(low, high);
                }
                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 3);
        }
        else if (mainChoice == 4) {
            int minimum;
            int maximum;
            cout << "\nValues: " << tree.get_length() << endl;
            cout << "Height: " << tree.get_height() << endl;
            cout << "Minimum degree: " << tree.get_minimum_degree() << endl;
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

    cout << "\nThank you for using B-Tree Application." << endl;
    return 0;
}
