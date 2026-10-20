#include <iostream>
#include <limits>
#include <vector>

using namespace std;

struct Interval {
    int id;
    int low;
    int high;

    Interval(int intervalId = 0, int lower = 0, int upper = 0)
        : id(intervalId), low(lower), high(upper)
    {
    }
};

class Interval_Tree {
private:
    struct Node {
        Interval value;
        int height;
        int maxHigh;
        Node* left;
        Node* right;

        explicit Node(const Interval& interval)
            : value(interval), height(1), maxHigh(interval.high),
              left(nullptr), right(nullptr)
        {
        }
    };

    Node* root;
    size_t length;

    static int node_height(const Node* node)
    {
        return node == nullptr ? 0 : node->height;
    }

    static int node_max_high(const Node* node)
    {
        return node == nullptr ? numeric_limits<int>::min() : node->maxHigh;
    }

    static bool less_key(const Interval& first, const Interval& second)
    {
        return first.low < second.low ||
            (first.low == second.low && first.id < second.id);
    }

    static void update(Node* node)
    {
        if (node == nullptr)
        {
            return;
        }

        int leftHeight = node_height(node->left);
        int rightHeight = node_height(node->right);
        node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
        node->maxHigh = node->value.high;
        if (node->left != nullptr && node->left->maxHigh > node->maxHigh)
        {
            node->maxHigh = node->left->maxHigh;
        }
        if (node->right != nullptr && node->right->maxHigh > node->maxHigh)
        {
            node->maxHigh = node->right->maxHigh;
        }
    }

    static int balance_factor(const Node* node)
    {
        return node == nullptr ? 0 :
            node_height(node->left) - node_height(node->right);
    }

    static Node* rotate_right(Node* oldRoot)
    {
        Node* newRoot = oldRoot->left;
        Node* movedSubtree = newRoot->right;
        newRoot->right = oldRoot;
        oldRoot->left = movedSubtree;
        update(oldRoot);
        update(newRoot);
        return newRoot;
    }

    static Node* rotate_left(Node* oldRoot)
    {
        Node* newRoot = oldRoot->right;
        Node* movedSubtree = newRoot->left;
        newRoot->left = oldRoot;
        oldRoot->right = movedSubtree;
        update(oldRoot);
        update(newRoot);
        return newRoot;
    }

    static Node* rebalance(Node* node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        update(node);
        int balance = balance_factor(node);
        if (balance > 1)
        {
            if (balance_factor(node->left) < 0)
            {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }
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

    static Node* insert_node(Node* node, const Interval& interval)
    {
        if (node == nullptr)
        {
            return new Node(interval);
        }

        if (less_key(interval, node->value))
        {
            node->left = insert_node(node->left, interval);
        }
        else
        {
            node->right = insert_node(node->right, interval);
        }
        return rebalance(node);
    }

    static Node* minimum_node(Node* node)
    {
        while (node != nullptr && node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }

    static Node* erase_key(Node* node, const Interval& interval)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (less_key(interval, node->value))
        {
            node->left = erase_key(node->left, interval);
        }
        else if (less_key(node->value, interval))
        {
            node->right = erase_key(node->right, interval);
        }
        else
        {
            if (node->left == nullptr || node->right == nullptr)
            {
                Node* child = node->left != nullptr ? node->left : node->right;
                delete node;
                return child;
            }

            Node* successor = minimum_node(node->right);
            node->value = successor->value;
            node->right = erase_key(node->right, successor->value);
        }
        return rebalance(node);
    }

    static bool find_id(const Node* node, int id, Interval& result)
    {
        if (node == nullptr)
        {
            return false;
        }
        if (node->value.id == id)
        {
            result = node->value;
            return true;
        }
        return find_id(node->left, id, result) ||
            find_id(node->right, id, result);
    }

    static bool overlaps(const Interval& stored, int low, int high)
    {
        return stored.low <= high && low <= stored.high;
    }

    static bool find_overlap(const Node* node, int low, int high,
                             Interval& result)
    {
        if (node == nullptr || node->maxHigh < low)
        {
            return false;
        }

        if (node->left != nullptr && node->left->maxHigh >= low &&
            find_overlap(node->left, low, high, result))
        {
            return true;
        }
        if (overlaps(node->value, low, high))
        {
            result = node->value;
            return true;
        }
        return node->value.low <= high &&
            find_overlap(node->right, low, high, result);
    }

    static void find_all_overlaps(const Node* node, int low, int high,
                                  vector<Interval>& result)
    {
        if (node == nullptr || node->maxHigh < low)
        {
            return;
        }

        find_all_overlaps(node->left, low, high, result);
        if (overlaps(node->value, low, high))
        {
            result.push_back(node->value);
        }
        if (node->value.low <= high)
        {
            find_all_overlaps(node->right, low, high, result);
        }
    }

    static void display_in_order(const Node* node)
    {
        if (node == nullptr)
        {
            return;
        }
        display_in_order(node->left);
        cout << "ID: " << node->value.id << ", [" << node->value.low
             << ", " << node->value.high << "], maxHigh: " << node->maxHigh
             << '\n';
        display_in_order(node->right);
    }

    static Node* clone(const Node* node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        Node* copy = new Node(node->value);
        copy->height = node->height;
        copy->maxHigh = node->maxHigh;
        try
        {
            copy->left = clone(node->left);
            copy->right = clone(node->right);
        }
        catch (...)
        {
            destroy(copy);
            throw;
        }
        return copy;
    }

    static void destroy(Node* node)
    {
        if (node == nullptr)
        {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    Interval_Tree() : root(nullptr), length(0)
    {
    }

    ~Interval_Tree()
    {
        clear();
    }

    Interval_Tree(const Interval_Tree& other)
        : root(clone(other.root)), length(other.length)
    {
    }

    Interval_Tree& operator=(const Interval_Tree& other)
    {
        if (this != &other)
        {
            Interval_Tree copy(other);
            swap(copy);
        }
        return *this;
    }

    void swap(Interval_Tree& other)
    {
        Node* savedRoot = root;
        root = other.root;
        other.root = savedRoot;

        size_t savedLength = length;
        length = other.length;
        other.length = savedLength;
    }

    bool insert(int id, int low, int high)
    {
        if (low > high)
        {
            return false;
        }
        Interval existing;
        if (find_id(root, id, existing))
        {
            return false;
        }

        root = insert_node(root, Interval(id, low, high));
        ++length;
        return true;
    }

    bool erase(int id)
    {
        Interval existing;
        if (!find_id(root, id, existing))
        {
            return false;
        }
        root = erase_key(root, existing);
        --length;
        return true;
    }

    bool find_any_overlap(int low, int high, Interval& result) const
    {
        return low <= high && find_overlap(root, low, high, result);
    }

    vector<Interval> find_all_overlaps(int low, int high) const
    {
        vector<Interval> result;
        if (low <= high)
        {
            find_all_overlaps(root, low, high, result);
        }
        return result;
    }

    void display() const
    {
        if (root == nullptr)
        {
            cout << "The interval tree is empty.\n";
            return;
        }
        cout << "Intervals in ascending (low, ID) order:\n";
        display_in_order(root);
    }

    void clear()
    {
        destroy(root);
        root = nullptr;
        length = 0;
    }

    size_t size() const
    {
        return length;
    }
};

#ifndef INTERVAL_TREE_NO_MAIN
int main()
{
    Interval_Tree tree;
    int choice = 0;

    do
    {
        cout << "\n--- Interval Tree (AVL) ---\n"
             << "1. Insert interval\n"
             << "2. Erase interval by ID\n"
             << "3. Find any overlap\n"
             << "4. List all overlaps\n"
             << "5. Display sorted intervals\n"
             << "6. Clear tree\n"
             << "7. Show size\n"
             << "0. Exit\n"
             << "Choice: ";
        if (!(cin >> choice))
        {
            return 0;
        }

        if (choice == 1)
        {
            int id, low, high;
            cout << "Enter ID, low, and high: ";
            if (!(cin >> id >> low >> high))
            {
                return 0;
            }
            cout << (tree.insert(id, low, high)
                ? "Interval inserted.\n"
                : "Invalid interval or duplicate ID.\n");
        }
        else if (choice == 2)
        {
            int id;
            cout << "Enter ID: ";
            if (!(cin >> id))
            {
                return 0;
            }
            cout << (tree.erase(id) ? "Interval erased.\n"
                                    : "No interval with that ID.\n");
        }
        else if (choice == 3 || choice == 4)
        {
            int low, high;
            cout << "Enter query low and high: ";
            if (!(cin >> low >> high))
            {
                return 0;
            }
            if (low > high)
            {
                cout << "Invalid query interval.\n";
            }
            else if (choice == 3)
            {
                Interval result;
                if (tree.find_any_overlap(low, high, result))
                {
                    cout << "Overlap: ID " << result.id << " ["
                         << result.low << ", " << result.high << "]\n";
                }
                else
                {
                    cout << "No overlapping interval found.\n";
                }
            }
            else
            {
                vector<Interval> results = tree.find_all_overlaps(low, high);
                if (results.empty())
                {
                    cout << "No overlapping intervals found.\n";
                }
                else
                {
                    cout << "Overlapping intervals:\n";
                    for (size_t i = 0; i < results.size(); ++i)
                    {
                        cout << "ID: " << results[i].id << ", ["
                             << results[i].low << ", " << results[i].high
                             << "]\n";
                    }
                }
            }
        }
        else if (choice == 5)
        {
            tree.display();
        }
        else if (choice == 6)
        {
            tree.clear();
            cout << "Tree cleared.\n";
        }
        else if (choice == 7)
        {
            cout << "Size: " << tree.size() << '\n';
        }
        else if (choice != 0)
        {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
#endif
