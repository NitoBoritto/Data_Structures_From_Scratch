#include <iostream>
#include <limits>
#include <utility>

using namespace std;

enum NodeColor {
    RED,
    BLACK
};

struct RedBlackNode {
    int data;
    NodeColor color;
    RedBlackNode* left;
    RedBlackNode* right;
    RedBlackNode* parent;

    RedBlackNode()
        : data(0), color(BLACK), left(nullptr), right(nullptr), parent(nullptr)
    {
    }

    RedBlackNode(int value, RedBlackNode* sentinel)
        : data(value), color(RED), left(sentinel), right(sentinel), parent(sentinel)
    {
    }
};

class Red_Black_Tree {
private:
    RedBlackNode* root;
    RedBlackNode* nil;
    size_t length;

    void destroy_tree(RedBlackNode* node)
    {
        if (node == nil)
        {
            return;
        }

        destroy_tree(node->left);
        destroy_tree(node->right);
        delete node;
    }

    RedBlackNode* clone_tree(const RedBlackNode* node, const RedBlackNode* sourceNil,
                             RedBlackNode* parent)
    {
        if (node == sourceNil)
        {
            return nil;
        }

        RedBlackNode* copy = new RedBlackNode(node->data, nil);
        copy->color = node->color;
        copy->parent = parent;
        try
        {
            copy->left = clone_tree(node->left, sourceNil, copy);
            copy->right = clone_tree(node->right, sourceNil, copy);
        }
        catch (...)
        {
            destroy_partial(copy);
            throw;
        }
        return copy;
    }

    void destroy_partial(RedBlackNode* node)
    {
        if (node == nil)
        {
            return;
        }
        destroy_partial(node->left);
        destroy_partial(node->right);
        delete node;
    }

    RedBlackNode* search_node(int value) const
    {
        RedBlackNode* current = root;
        while (current != nil && current->data != value)
        {
            current = value < current->data ? current->left : current->right;
        }
        return current;
    }

    void rotate_left(RedBlackNode* node)
    {
        RedBlackNode* child = node->right;
        node->right = child->left;
        if (child->left != nil)
        {
            child->left->parent = node;
        }

        child->parent = node->parent;
        if (node->parent == nil)
        {
            root = child;
        }
        else if (node == node->parent->left)
        {
            node->parent->left = child;
        }
        else
        {
            node->parent->right = child;
        }
        child->left = node;
        node->parent = child;
    }

    void rotate_right(RedBlackNode* node)
    {
        RedBlackNode* child = node->left;
        node->left = child->right;
        if (child->right != nil)
        {
            child->right->parent = node;
        }

        child->parent = node->parent;
        if (node->parent == nil)
        {
            root = child;
        }
        else if (node == node->parent->right)
        {
            node->parent->right = child;
        }
        else
        {
            node->parent->left = child;
        }
        child->right = node;
        node->parent = child;
    }

    void fix_insert(RedBlackNode* node)
    {
        while (node->parent->color == RED)
        {
            if (node->parent == node->parent->parent->left)
            {
                RedBlackNode* uncle = node->parent->parent->right;
                if (uncle->color == RED)
                {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else
                {
                    if (node == node->parent->right)
                    {
                        node = node->parent;
                        rotate_left(node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotate_right(node->parent->parent);
                }
            }
            else
            {
                RedBlackNode* uncle = node->parent->parent->left;
                if (uncle->color == RED)
                {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else
                {
                    if (node == node->parent->left)
                    {
                        node = node->parent;
                        rotate_right(node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotate_left(node->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    RedBlackNode* minimum_node(RedBlackNode* node) const
    {
        while (node->left != nil)
        {
            node = node->left;
        }
        return node;
    }

    RedBlackNode* maximum_node(RedBlackNode* node) const
    {
        while (node->right != nil)
        {
            node = node->right;
        }
        return node;
    }

    void transplant(RedBlackNode* oldNode, RedBlackNode* newNode)
    {
        if (oldNode->parent == nil)
        {
            root = newNode;
        }
        else if (oldNode == oldNode->parent->left)
        {
            oldNode->parent->left = newNode;
        }
        else
        {
            oldNode->parent->right = newNode;
        }
        newNode->parent = oldNode->parent;
    }

    void fix_erase(RedBlackNode* node)
    {
        while (node != root && node->color == BLACK)
        {
            if (node == node->parent->left)
            {
                RedBlackNode* sibling = node->parent->right;
                if (sibling->color == RED)
                {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotate_left(node->parent);
                    sibling = node->parent->right;
                }
                if (sibling->left->color == BLACK && sibling->right->color == BLACK)
                {
                    sibling->color = RED;
                    node = node->parent;
                }
                else
                {
                    if (sibling->right->color == BLACK)
                    {
                        sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotate_right(sibling);
                        sibling = node->parent->right;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    sibling->right->color = BLACK;
                    rotate_left(node->parent);
                    node = root;
                }
            }
            else
            {
                RedBlackNode* sibling = node->parent->left;
                if (sibling->color == RED)
                {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotate_right(node->parent);
                    sibling = node->parent->left;
                }
                if (sibling->right->color == BLACK && sibling->left->color == BLACK)
                {
                    sibling->color = RED;
                    node = node->parent;
                }
                else
                {
                    if (sibling->left->color == BLACK)
                    {
                        sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotate_left(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    sibling->left->color = BLACK;
                    rotate_right(node->parent);
                    node = root;
                }
            }
        }
        node->color = BLACK;
    }

    void display_node(const RedBlackNode* node) const
    {
        if (node == nil)
        {
            return;
        }
        display_node(node->left);
        cout << node->data << ' ';
        display_node(node->right);
    }

public:
    Red_Black_Tree()
        : root(nullptr), nil(new RedBlackNode()), length(0)
    {
        nil->left = nil;
        nil->right = nil;
        nil->parent = nil;
        root = nil;
    }

    Red_Black_Tree(const Red_Black_Tree& other)
        : root(nullptr), nil(new RedBlackNode()), length(other.length)
    {
        nil->left = nil;
        nil->right = nil;
        nil->parent = nil;
        try
        {
            root = clone_tree(other.root, other.nil, nil);
            if (root != nil)
            {
                root->parent = nil;
            }
        }
        catch (...)
        {
            delete nil;
            throw;
        }
    }

    Red_Black_Tree& operator=(const Red_Black_Tree& other)
    {
        if (this != &other)
        {
            Red_Black_Tree copy(other);
            swap(copy);
        }
        return *this;
    }

    ~Red_Black_Tree()
    {
        destroy_tree(root);
        delete nil;
    }

    void swap(Red_Black_Tree& other)
    {
        std::swap(root, other.root);
        std::swap(nil, other.nil);
        std::swap(length, other.length);
    }

    bool insert(int value)
    {
        RedBlackNode* parent = nil;
        RedBlackNode* current = root;
        while (current != nil)
        {
            parent = current;
            if (value == current->data)
            {
                return false;
            }
            current = value < current->data ? current->left : current->right;
        }

        RedBlackNode* node = new RedBlackNode(value, nil);
        node->parent = parent;
        if (parent == nil)
        {
            root = node;
        }
        else if (value < parent->data)
        {
            parent->left = node;
        }
        else
        {
            parent->right = node;
        }
        ++length;
        fix_insert(node);
        return true;
    }

    bool contains(int value) const
    {
        return search_node(value) != nil;
    }

    bool erase(int value)
    {
        RedBlackNode* target = search_node(value);
        if (target == nil)
        {
            return false;
        }

        RedBlackNode* moved = target;
        NodeColor originalColor = moved->color;
        RedBlackNode* fixNode;
        if (target->left == nil)
        {
            fixNode = target->right;
            transplant(target, target->right);
        }
        else if (target->right == nil)
        {
            fixNode = target->left;
            transplant(target, target->left);
        }
        else
        {
            moved = minimum_node(target->right);
            originalColor = moved->color;
            fixNode = moved->right;
            if (moved->parent == target)
            {
                fixNode->parent = moved;
            }
            else
            {
                transplant(moved, moved->right);
                moved->right = target->right;
                moved->right->parent = moved;
            }
            transplant(target, moved);
            moved->left = target->left;
            moved->left->parent = moved;
            moved->color = target->color;
        }

        delete target;
        --length;
        if (originalColor == BLACK)
        {
            fix_erase(fixNode);
        }
        nil->parent = nil;
        return true;
    }

    bool minimum(int& value) const
    {
        if (root == nil)
        {
            return false;
        }
        value = minimum_node(root)->data;
        return true;
    }

    bool maximum(int& value) const
    {
        if (root == nil)
        {
            return false;
        }
        value = maximum_node(root)->data;
        return true;
    }

    void display() const
    {
        if (root == nil)
        {
            cout << "The tree is empty.\n";
            return;
        }
        display_node(root);
        cout << '\n';
    }

    void clear()
    {
        destroy_tree(root);
        root = nil;
        nil->parent = nil;
        length = 0;
    }

    size_t size() const
    {
        return length;
    }
};

#ifndef RED_BLACK_TREE_TEST
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
    Red_Black_Tree tree;
    int choice;
    while (true)
    {
        cout << "\n--- Red-Black Tree ---\n"
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
                cout << (tree.insert(value) ? "Value inserted.\n" : "Duplicate value; no change.\n");
                break;
            case 2:
                cout << (tree.contains(value) ? "Value found.\n" : "Value not found.\n");
                break;
            case 3:
                cout << (tree.erase(value) ? "Value erased.\n" : "Value not found.\n");
                break;
            case 4:
                tree.display();
                break;
            case 5:
                if (tree.minimum(value))
                    cout << "Minimum: " << value << '\n';
                else
                    cout << "The tree is empty.\n";
                break;
            case 6:
                if (tree.maximum(value))
                    cout << "Maximum: " << value << '\n';
                else
                    cout << "The tree is empty.\n";
                break;
            case 7:
                tree.clear();
                cout << "Tree cleared.\n";
                break;
            case 8:
                cout << "Size: " << tree.size() << '\n';
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
