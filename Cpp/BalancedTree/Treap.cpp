#include <cstdint>
#include <iostream>
#include <limits>
#include <utility>

using namespace std;

struct TreapNode {
    int data;
    uint32_t priority;
    TreapNode* left;
    TreapNode* right;

    TreapNode(int value, uint32_t nodePriority)
        : data(value), priority(nodePriority), left(nullptr), right(nullptr)
    {
    }
};

class Treap {
private:
    TreapNode* root;
    size_t length;
    uint32_t randomState;

    static const uint32_t initialSeed = 0x6D2B79F5u;

    uint32_t next_priority()
    {
        // A small deterministic generator keeps runs reproducible without global rand state.
        randomState = randomState * 1664525u + 1013904223u;
        return randomState;
    }

    TreapNode* copy_tree(const TreapNode* node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        TreapNode* copy = new TreapNode(node->data, node->priority);
        try
        {
            copy->left = copy_tree(node->left);
            copy->right = copy_tree(node->right);
        }
        catch (...)
        {
            destroy_tree(copy);
            throw;
        }
        return copy;
    }

    void destroy_tree(TreapNode* node)
    {
        if (node == nullptr)
        {
            return;
        }
        destroy_tree(node->left);
        destroy_tree(node->right);
        delete node;
    }

    static TreapNode* rotate_right(TreapNode* node)
    {
        TreapNode* child = node->left;
        node->left = child->right;
        child->right = node;
        return child;
    }

    static TreapNode* rotate_left(TreapNode* node)
    {
        TreapNode* child = node->right;
        node->right = child->left;
        child->left = node;
        return child;
    }

    TreapNode* insert_node(TreapNode* node, int value, bool& inserted)
    {
        if (node == nullptr)
        {
            inserted = true;
            return new TreapNode(value, next_priority());
        }
        if (value < node->data)
        {
            node->left = insert_node(node->left, value, inserted);
            if (node->left->priority > node->priority)
            {
                node = rotate_right(node);
            }
        }
        else if (value > node->data)
        {
            node->right = insert_node(node->right, value, inserted);
            if (node->right->priority > node->priority)
            {
                node = rotate_left(node);
            }
        }
        return node;
    }

    static TreapNode* merge(TreapNode* left, TreapNode* right)
    {
        if (left == nullptr)
        {
            return right;
        }
        if (right == nullptr)
        {
            return left;
        }
        if (left->priority > right->priority)
        {
            left->right = merge(left->right, right);
            return left;
        }
        right->left = merge(left, right->left);
        return right;
    }

    TreapNode* erase_node(TreapNode* node, int value, bool& erased)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        if (value < node->data)
        {
            node->left = erase_node(node->left, value, erased);
        }
        else if (value > node->data)
        {
            node->right = erase_node(node->right, value, erased);
        }
        else
        {
            TreapNode* replacement = merge(node->left, node->right);
            delete node;
            erased = true;
            return replacement;
        }
        return node;
    }

    const TreapNode* search_node(int value) const
    {
        const TreapNode* current = root;
        while (current != nullptr && current->data != value)
        {
            current = value < current->data ? current->left : current->right;
        }
        return current;
    }

    static const TreapNode* minimum_node(const TreapNode* node)
    {
        while (node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }

    static const TreapNode* maximum_node(const TreapNode* node)
    {
        while (node->right != nullptr)
        {
            node = node->right;
        }
        return node;
    }

    static void display_node(const TreapNode* node)
    {
        if (node == nullptr)
        {
            return;
        }
        display_node(node->left);
        cout << node->data << ' ';
        display_node(node->right);
    }

public:
    Treap()
        : root(nullptr), length(0), randomState(initialSeed)
    {
    }

    Treap(const Treap& other)
        : root(nullptr), length(other.length), randomState(other.randomState)
    {
        root = copy_tree(other.root);
    }

    Treap& operator=(const Treap& other)
    {
        if (this != &other)
        {
            Treap copy(other);
            swap(copy);
        }
        return *this;
    }

    ~Treap()
    {
        destroy_tree(root);
    }

    void swap(Treap& other)
    {
        std::swap(root, other.root);
        std::swap(length, other.length);
        std::swap(randomState, other.randomState);
    }

    bool insert(int value)
    {
        bool inserted = false;
        root = insert_node(root, value, inserted);
        if (inserted)
        {
            ++length;
        }
        return inserted;
    }

    bool contains(int value) const
    {
        return search_node(value) != nullptr;
    }

    bool erase(int value)
    {
        bool erased = false;
        root = erase_node(root, value, erased);
        if (erased)
        {
            --length;
        }
        return erased;
    }

    bool minimum(int& value) const
    {
        if (root == nullptr)
        {
            return false;
        }
        value = minimum_node(root)->data;
        return true;
    }

    bool maximum(int& value) const
    {
        if (root == nullptr)
        {
            return false;
        }
        value = maximum_node(root)->data;
        return true;
    }

    void display() const
    {
        if (root == nullptr)
        {
            cout << "The treap is empty.\n";
            return;
        }
        display_node(root);
        cout << '\n';
    }

    void clear()
    {
        destroy_tree(root);
        root = nullptr;
        length = 0;
        randomState = initialSeed;
    }

    size_t size() const
    {
        return length;
    }
};

#ifndef TREAP_TEST
static bool read_integer(const char* prompt, int& value)
{
    cout << prompt;
    if (cin >> value)
    {
        return true;
    }
    if (cin.eof())
        return false;
    cout << "Invalid input. Please enter an integer.\n";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return false;
}

int main()
{
    Treap treap;
    int choice;
    while (true)
    {
        cout << "\n--- Treap ---\n"
             << "1. Insert\n2. Search\n3. Erase\n4. Display sorted values\n"
             << "5. Minimum\n6. Maximum\n7. Clear\n8. Size\n0. Exit\n"
             << "Choose an option: ";
        if (!(cin >> choice))
        {
            if (cin.eof())
            {
                break;
            }
            cout << "Invalid input. Please enter a menu number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        int value;
        if (choice >= 1 && choice <= 3)
        {
            if (!read_integer("Enter an integer: ", value))
            {
                if (cin.eof())
                    break;
                continue;
            }
        }

        switch (choice)
        {
            case 1:
                cout << (treap.insert(value) ? "Value inserted.\n" : "Duplicate value; no change.\n");
                break;
            case 2:
                cout << (treap.contains(value) ? "Value found.\n" : "Value not found.\n");
                break;
            case 3:
                cout << (treap.erase(value) ? "Value erased.\n" : "Value not found.\n");
                break;
            case 4:
                treap.display();
                break;
            case 5:
                if (treap.minimum(value))
                    cout << "Minimum: " << value << '\n';
                else
                    cout << "The treap is empty.\n";
                break;
            case 6:
                if (treap.maximum(value))
                    cout << "Maximum: " << value << '\n';
                else
                    cout << "The treap is empty.\n";
                break;
            case 7:
                treap.clear();
                cout << "Treap cleared.\n";
                break;
            case 8:
                cout << "Size: " << treap.size() << '\n';
                break;
            case 0:
                return 0;
            default:
                cout << "Invalid menu option.\n";
        }
    }
    return 0;
}
#endif
