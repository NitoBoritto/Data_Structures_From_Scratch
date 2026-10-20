#include <iostream>
#include <cstdlib>
using namespace std;

struct B_Plus_Tree_Node {
    int* keys;
    B_Plus_Tree_Node** children;
    B_Plus_Tree_Node* next;
    int keyCount;
    bool isLeaf;

    B_Plus_Tree_Node(int minimumDegree, bool Leaf)
    {
        keys = new int[(2 * minimumDegree) - 1];
        children = new B_Plus_Tree_Node*[2 * minimumDegree];
        next = nullptr;
        keyCount = 0;
        isLeaf = Leaf;

        for (int i = 0; i < 2 * minimumDegree; i++)
        {
            children[i] = nullptr;
        }
    }

    ~B_Plus_Tree_Node()
    {
        delete[] keys;
        delete[] children;
    }
};


class B_Plus_Tree_Number {
private:

    B_Plus_Tree_Node* root;
    int minimumDegree;
    int length;

private:

    B_Plus_Tree_Node* copy_node(const B_Plus_Tree_Node* node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        B_Plus_Tree_Node* copy = new B_Plus_Tree_Node(minimumDegree, node->isLeaf);
        copy->keyCount = node->keyCount;

        for (int i = 0; i < node->keyCount; i++)
        {
            copy->keys[i] = node->keys[i];
        }

        if (node->isLeaf)
        {
            copy->next = nullptr;
        }
        else
        {
            for (int i = 0; i <= node->keyCount; i++)
            {
                copy->children[i] = copy_node(node->children[i]);
            }
        }

        return copy;
    }


    void link_leaves(B_Plus_Tree_Node* node, B_Plus_Tree_Node*& previousLeaf)
    {
        if (node == nullptr)
        {
            return;
        }

        if (node->isLeaf)
        {
            if (previousLeaf != nullptr)
            {
                previousLeaf->next = node;
            }
            previousLeaf = node;
            return;
        }

        for (int i = 0; i <= node->keyCount; i++)
        {
            link_leaves(node->children[i], previousLeaf);
        }
    }


    void delete_tree(B_Plus_Tree_Node* node)
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


    void split_child(B_Plus_Tree_Node* parent, int childIndex)
    {
        B_Plus_Tree_Node* fullChild = parent->children[childIndex];
        B_Plus_Tree_Node* rightChild = new B_Plus_Tree_Node(
            minimumDegree,
            fullChild->isLeaf
        );
        int separator;

        if (fullChild->isLeaf)
        {
            rightChild->keyCount = minimumDegree - 1;
            fullChild->keyCount = minimumDegree;

            for (int i = 0; i < rightChild->keyCount; i++)
            {
                rightChild->keys[i] = fullChild->keys[i + minimumDegree];
            }

            rightChild->next = fullChild->next;
            fullChild->next = rightChild;
            separator = rightChild->keys[0];
        }
        else
        {
            separator = fullChild->keys[minimumDegree - 1];
            rightChild->keyCount = minimumDegree - 1;

            for (int i = 0; i < minimumDegree - 1; i++)
            {
                rightChild->keys[i] = fullChild->keys[i + minimumDegree];
            }
            for (int i = 0; i < minimumDegree; i++)
            {
                rightChild->children[i] = fullChild->children[i + minimumDegree];
                fullChild->children[i + minimumDegree] = nullptr;
            }

            fullChild->keyCount = minimumDegree - 1;
        }

        for (int i = parent->keyCount; i >= childIndex + 1; i--)
        {
            parent->children[i + 1] = parent->children[i];
        }
        parent->children[childIndex + 1] = rightChild;

        for (int i = parent->keyCount - 1; i >= childIndex; i--)
        {
            parent->keys[i + 1] = parent->keys[i];
        }
        parent->keys[childIndex] = separator;
        parent->keyCount++;
    }


    void insert_non_full(B_Plus_Tree_Node* node, int value)
    {
        if (node->isLeaf)
        {
            int index = node->keyCount - 1;
            while (index >= 0 && node->keys[index] > value)
            {
                node->keys[index + 1] = node->keys[index];
                index--;
            }

            node->keys[index + 1] = value;
            node->keyCount++;
            return;
        }

        int childIndex = 0;
        while (childIndex < node->keyCount && value >= node->keys[childIndex])
        {
            childIndex++;
        }

        if (node->children[childIndex]->keyCount == (2 * minimumDegree) - 1)
        {
            split_child(node, childIndex);
            if (value >= node->keys[childIndex])
            {
                childIndex++;
            }
        }

        insert_non_full(node->children[childIndex], value);
    }


    int find_key_index(B_Plus_Tree_Node* node, int value)
    {
        int index = 0;
        while (index < node->keyCount && node->keys[index] < value)
        {
            index++;
        }

        return index;
    }


    int get_subtree_minimum(B_Plus_Tree_Node* node)
    {
        while (!node->isLeaf)
        {
            node = node->children[0];
        }

        return node->keys[0];
    }


    void rebuild_separators(B_Plus_Tree_Node* node)
    {
        if (node == nullptr || node->isLeaf)
        {
            return;
        }

        for (int i = 0; i < node->keyCount; i++)
        {
            node->keys[i] = get_subtree_minimum(node->children[i + 1]);
        }
    }


    void borrow_from_previous(B_Plus_Tree_Node* parent, int childIndex)
    {
        B_Plus_Tree_Node* child = parent->children[childIndex];
        B_Plus_Tree_Node* sibling = parent->children[childIndex - 1];

        if (child->isLeaf)
        {
            for (int i = child->keyCount - 1; i >= 0; i--)
            {
                child->keys[i + 1] = child->keys[i];
            }

            child->keys[0] = sibling->keys[sibling->keyCount - 1];
            child->keyCount++;
            sibling->keyCount--;
        }
        else
        {
            for (int i = child->keyCount; i >= 0; i--)
            {
                child->children[i + 1] = child->children[i];
            }

            child->children[0] = sibling->children[sibling->keyCount];
            sibling->children[sibling->keyCount] = nullptr;
            child->keyCount++;
            sibling->keyCount--;
            rebuild_separators(sibling);
            rebuild_separators(child);
        }

        rebuild_separators(parent);
    }


    void borrow_from_next(B_Plus_Tree_Node* parent, int childIndex)
    {
        B_Plus_Tree_Node* child = parent->children[childIndex];
        B_Plus_Tree_Node* sibling = parent->children[childIndex + 1];

        if (child->isLeaf)
        {
            child->keys[child->keyCount] = sibling->keys[0];
            child->keyCount++;

            for (int i = 1; i < sibling->keyCount; i++)
            {
                sibling->keys[i - 1] = sibling->keys[i];
            }
            sibling->keyCount--;
        }
        else
        {
            child->children[child->keyCount + 1] = sibling->children[0];
            sibling->children[0] = nullptr;

            for (int i = 1; i <= sibling->keyCount; i++)
            {
                sibling->children[i - 1] = sibling->children[i];
            }
            sibling->children[sibling->keyCount] = nullptr;
            child->keyCount++;
            sibling->keyCount--;
            rebuild_separators(child);
            rebuild_separators(sibling);
        }

        rebuild_separators(parent);
    }


    void merge_children(B_Plus_Tree_Node* parent, int childIndex)
    {
        B_Plus_Tree_Node* left = parent->children[childIndex];
        B_Plus_Tree_Node* right = parent->children[childIndex + 1];

        if (left->isLeaf)
        {
            for (int i = 0; i < right->keyCount; i++)
            {
                left->keys[left->keyCount + i] = right->keys[i];
            }

            left->keyCount += right->keyCount;
            left->next = right->next;
        }
        else
        {
            for (int i = 0; i <= right->keyCount; i++)
            {
                left->children[left->keyCount + 1 + i] = right->children[i];
                right->children[i] = nullptr;
            }

            left->keyCount += right->keyCount + 1;
            rebuild_separators(left);
        }

        for (int i = childIndex + 2; i <= parent->keyCount; i++)
        {
            parent->children[i - 1] = parent->children[i];
        }
        parent->children[parent->keyCount] = nullptr;
        parent->keyCount--;
        rebuild_separators(parent);
        delete right;
    }


    void rebalance_child(B_Plus_Tree_Node* parent, int childIndex)
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
        else if (childIndex > 0)
        {
            merge_children(parent, childIndex - 1);
        }
        else
        {
            merge_children(parent, childIndex);
        }
    }


    bool remove_value(B_Plus_Tree_Node* node, int value)
    {
        if (node->isLeaf)
        {
            int index = find_key_index(node, value);
            if (index == node->keyCount || node->keys[index] != value)
            {
                return false;
            }

            for (int i = index + 1; i < node->keyCount; i++)
            {
                node->keys[i - 1] = node->keys[i];
            }
            node->keyCount--;
            return true;
        }

        int childIndex = 0;
        while (childIndex < node->keyCount && value >= node->keys[childIndex])
        {
            childIndex++;
        }

        if (!remove_value(node->children[childIndex], value))
        {
            return false;
        }

        if (!(node == root && node->keyCount == 0)
            && node->children[childIndex]->keyCount < minimumDegree - 1)
        {
            rebalance_child(node, childIndex);
        }

        rebuild_separators(node);
        return true;
    }


    B_Plus_Tree_Node* find_leaf(int value)
    {
        B_Plus_Tree_Node* current = root;

        while (current != nullptr && !current->isLeaf)
        {
            int childIndex = 0;
            while (childIndex < current->keyCount && value >= current->keys[childIndex])
            {
                childIndex++;
            }
            current = current->children[childIndex];
        }

        return current;
    }


    void display_range_from_leaf(B_Plus_Tree_Node* leaf, int low, int high)
    {
        while (leaf != nullptr)
        {
            for (int i = 0; i < leaf->keyCount; i++)
            {
                if (leaf->keys[i] >= low && leaf->keys[i] <= high)
                {
                    cout << leaf->keys[i] << " ";
                }
                else if (leaf->keys[i] > high)
                {
                    return;
                }
            }

            leaf = leaf->next;
        }
    }


public:

    // Constructor
    // Creates an empty B+ tree with the requested minimum degree
    B_Plus_Tree_Number(int Minimum_Degree = 2)
    {
        minimumDegree = (Minimum_Degree >= 2) ? Minimum_Degree : 2;
        root = nullptr;
        length = 0;
    }


    // Destructor
    // Releases all tree nodes
    ~B_Plus_Tree_Number()
    {
        clear();
    }


    // Copy Constructor
    // Creates a deep copy and rebuilds leaf links
    B_Plus_Tree_Number(const B_Plus_Tree_Number& other)
    {
        minimumDegree = other.minimumDegree;
        length = other.length;
        root = copy_node(other.root);
        B_Plus_Tree_Node* previousLeaf = nullptr;
        link_leaves(root, previousLeaf);
    }


    // Assignment Operator
    // Performs deep copy between B+ trees
    B_Plus_Tree_Number& operator=(const B_Plus_Tree_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        B_Plus_Tree_Number temp(other);
        B_Plus_Tree_Node* oldRoot = root;
        root = temp.root;
        temp.root = oldRoot;
        minimumDegree = temp.minimumDegree;

        int oldLength = length;
        length = temp.length;
        temp.length = oldLength;
        return *this;
    }


    // Insert a unique value while preserving B+ tree balance
    bool insert(int value)
    {
        if (search(value))
        {
            return false;
        }

        if (root == nullptr)
        {
            root = new B_Plus_Tree_Node(minimumDegree, true);
            root->keys[0] = value;
            root->keyCount = 1;
            length++;
            return true;
        }

        if (root->keyCount == (2 * minimumDegree) - 1)
        {
            B_Plus_Tree_Node* newRoot = new B_Plus_Tree_Node(minimumDegree, false);
            newRoot->children[0] = root;
            split_child(newRoot, 0);
            root = newRoot;
        }

        insert_non_full(root, value);
        length++;
        return true;
    }


    // Search for a value in the leaf level
    bool search(int value)
    {
        B_Plus_Tree_Node* leaf = find_leaf(value);
        if (leaf == nullptr)
        {
            return false;
        }

        for (int i = 0; i < leaf->keyCount; i++)
        {
            if (leaf->keys[i] == value)
            {
                return true;
            }
        }

        return false;
    }


    // Remove a value while preserving B+ tree balance and leaf order
    bool erase(int value)
    {
        if (!search(value))
        {
            return false;
        }

        remove_value(root, value);
        length--;

        if (root->isLeaf && root->keyCount == 0)
        {
            delete root;
            root = nullptr;
        }
        else if (!root->isLeaf && root->keyCount == 0)
        {
            B_Plus_Tree_Node* oldRoot = root;
            root = root->children[0];
            oldRoot->children[0] = nullptr;
            delete oldRoot;
        }

        return true;
    }


    // Print all values in sorted order using linked leaves
    int display()
    {
        if (root == nullptr)
        {
            cout << "B+ tree is empty." << endl;
            return 0;
        }

        B_Plus_Tree_Node* leaf = root;
        while (!leaf->isLeaf)
        {
            leaf = leaf->children[0];
        }

        while (leaf != nullptr)
        {
            for (int i = 0; i < leaf->keyCount; i++)
            {
                cout << leaf->keys[i] << " ";
            }
            leaf = leaf->next;
        }
        cout << endl;
        return 1;
    }


    // Print values in the inclusive range [low, high]
    int display_range(int low, int high)
    {
        if (root == nullptr)
        {
            cout << "B+ tree is empty." << endl;
            return 0;
        }
        if (low > high)
        {
            cout << "Invalid range." << endl;
            return 0;
        }

        display_range_from_leaf(find_leaf(low), low, high);
        cout << endl;
        return 1;
    }


    // Return the number of values in the inclusive range [low, high]
    int count_range(int low, int high)
    {
        if (root == nullptr || low > high)
        {
            return 0;
        }

        int count = 0;
        B_Plus_Tree_Node* leaf = find_leaf(low);
        while (leaf != nullptr)
        {
            for (int i = 0; i < leaf->keyCount; i++)
            {
                if (leaf->keys[i] >= low && leaf->keys[i] <= high)
                {
                    count++;
                }
                else if (leaf->keys[i] > high)
                {
                    return count;
                }
            }
            leaf = leaf->next;
        }

        return count;
    }


    // Return the smallest value through value
    bool get_minimum(int& value)
    {
        if (root == nullptr)
        {
            return false;
        }

        B_Plus_Tree_Node* leaf = root;
        while (!leaf->isLeaf)
        {
            leaf = leaf->children[0];
        }

        value = leaf->keys[0];
        return true;
    }


    // Return the largest value through value
    bool get_maximum(int& value)
    {
        if (root == nullptr)
        {
            return false;
        }

        B_Plus_Tree_Node* leaf = root;
        while (!leaf->isLeaf)
        {
            leaf = leaf->children[leaf->keyCount];
        }

        while (leaf->next != nullptr)
        {
            leaf = leaf->next;
        }

        value = leaf->keys[leaf->keyCount - 1];
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
    cout << "         Welcome To B+ Tree Application" << endl;
    cout << "============================================" << endl;

    int minimumDegree;
    do {
        cout << "\nEnter minimum degree (at least 2): ";
        read_input(minimumDegree);
        if (minimumDegree < 2) {
            cout << "Minimum degree must be at least 2." << endl;
        }
    } while (minimumDegree < 2);

    B_Plus_Tree_Number tree(minimumDegree);
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
                        cout << (tree.erase(value) ? "Number deleted successfully." : "Number not found.") << endl;
                    }
                }
                else if (choice == 3) {
                    tree.clear();
                    cout << "B+ tree cleared successfully." << endl;
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
                cout << "3. Count Values In Range" << endl;
                cout << "4. Back To Main Menu" << endl;
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
                else if (choice == 3) {
                    int low;
                    int high;
                    cout << "\nEnter minimum value: ";
                    read_input(low);
                    cout << "Enter maximum value: ";
                    read_input(high);
                    cout << "Values in range: " << tree.count_range(low, high) << endl;
                }
                else if (choice != 4) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 4);
        }
        else if (mainChoice == 4) {
            int minimum;
            int maximum;
            cout << "\nValues: " << tree.get_length() << endl;
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

    cout << "\nThank you for using B+ Tree Application." << endl;
    return 0;
}
