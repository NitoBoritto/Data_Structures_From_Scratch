#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

struct Rope_Node {
    string text;
    unsigned int priority;
    int subtreeLength;
    Rope_Node* left;
    Rope_Node* right;

    Rope_Node(const string& Text, unsigned int Priority)
    {
        text = Text;
        priority = Priority;
        subtreeLength = static_cast<int>(Text.length());
        left = nullptr;
        right = nullptr;
    }
};


class Rope_String {
private:

    static const int CHUNK_SIZE = 32;
    Rope_Node* root;
    unsigned int randomState;

private:

    int subtree_length(Rope_Node* node)
    {
        return (node == nullptr) ? 0 : node->subtreeLength;
    }


    void update(Rope_Node* node)
    {
        if (node != nullptr)
        {
            node->subtreeLength = subtree_length(node->left)
                + static_cast<int>(node->text.length())
                + subtree_length(node->right);
        }
    }


    unsigned int next_priority()
    {
        randomState = (randomState * 1664525U) + 1013904223U;
        return randomState;
    }


    Rope_Node* make_node(const string& text)
    {
        if (text.empty())
        {
            return nullptr;
        }

        return new Rope_Node(text, next_priority());
    }


    Rope_Node* merge(Rope_Node* left, Rope_Node* right)
    {
        if (left == nullptr)
        {
            return right;
        }
        if (right == nullptr)
        {
            return left;
        }

        if (left->priority >= right->priority)
        {
            left->right = merge(left->right, right);
            update(left);
            return left;
        }

        right->left = merge(left, right->left);
        update(right);
        return right;
    }


    void split(Rope_Node* node, int position, Rope_Node*& left, Rope_Node*& right)
    {
        if (node == nullptr)
        {
            left = nullptr;
            right = nullptr;
            return;
        }

        int leftLength = subtree_length(node->left);
        int chunkEnd = leftLength + static_cast<int>(node->text.length());

        if (position < leftLength)
        {
            split(node->left, position, left, node->left);
            update(node);
            right = node;
            return;
        }

        if (position > chunkEnd)
        {
            split(node->right, position - chunkEnd, node->right, right);
            update(node);
            left = node;
            return;
        }

        if (position == leftLength)
        {
            left = node->left;
            node->left = nullptr;
            update(node);
            right = node;
            return;
        }

        if (position == chunkEnd)
        {
            right = node->right;
            node->right = nullptr;
            update(node);
            left = node;
            return;
        }

        int chunkOffset = position - leftLength;
        string leftText = node->text.substr(0, chunkOffset);
        string rightText = node->text.substr(chunkOffset);
        Rope_Node* oldLeft = node->left;
        Rope_Node* oldRight = node->right;
        delete node;

        left = merge(oldLeft, make_node(leftText));
        right = merge(make_node(rightText), oldRight);
    }


    Rope_Node* build(const string& text)
    {
        Rope_Node* result = nullptr;

        for (size_t i = 0; i < text.length(); i += CHUNK_SIZE)
        {
            result = merge(result, make_node(text.substr(i, CHUNK_SIZE)));
        }

        return result;
    }


    void delete_tree(Rope_Node* node)
    {
        if (node == nullptr)
        {
            return;
        }

        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
    }


    Rope_Node* copy_tree(const Rope_Node* node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        Rope_Node* copy = new Rope_Node(node->text, node->priority);
        copy->left = copy_tree(node->left);
        copy->right = copy_tree(node->right);
        update(copy);
        return copy;
    }


    void append_text(Rope_Node* node, string& result)
    {
        if (node == nullptr)
        {
            return;
        }

        append_text(node->left, result);
        result += node->text;
        append_text(node->right, result);
    }


    bool get_character(Rope_Node* node, int index, char& value)
    {
        if (node == nullptr)
        {
            return false;
        }

        int leftLength = subtree_length(node->left);
        int chunkLength = static_cast<int>(node->text.length());

        if (index < leftLength)
        {
            return get_character(node->left, index, value);
        }
        if (index < leftLength + chunkLength)
        {
            value = node->text[index - leftLength];
            return true;
        }

        return get_character(node->right, index - leftLength - chunkLength, value);
    }

public:

    // Constructor
    // Creates an empty rope
    Rope_String(unsigned int Seed = 1)
    {
        root = nullptr;
        randomState = (Seed != 0) ? Seed : 1;
    }


    // Constructor
    // Creates a rope from an initial string
    Rope_String(const string& Text, unsigned int Seed = 1)
    {
        root = nullptr;
        randomState = (Seed != 0) ? Seed : 1;
        root = build(Text);
    }


    // Destructor
    // Releases all rope nodes
    ~Rope_String()
    {
        delete_tree(root);
    }


    // Copy Constructor
    // Creates a deep copy of another rope
    Rope_String(const Rope_String& other)
    {
        root = copy_tree(other.root);
        randomState = other.randomState;
    }


    // Assignment Operator
    // Performs deep copy between ropes
    Rope_String& operator=(const Rope_String& other)
    {
        if (this == &other)
        {
            return *this;
        }

        Rope_Node* newRoot = copy_tree(other.root);
        delete_tree(root);
        root = newRoot;
        randomState = other.randomState;
        return *this;
    }


    // Insert text before position
    bool insert(int position, const string& text)
    {
        if (position < 0 || position > subtree_length(root))
        {
            return false;
        }
        if (text.empty())
        {
            return true;
        }

        Rope_Node* left;
        Rope_Node* right;
        split(root, position, left, right);
        root = merge(merge(left, build(text)), right);
        return true;
    }


    // Remove count characters starting at position
    bool erase(int position, int count)
    {
        int length = subtree_length(root);
        if (position < 0 || count < 0 || position > length
            || count > length - position)
        {
            return false;
        }
        if (count == 0)
        {
            return true;
        }

        Rope_Node* left;
        Rope_Node* remainder;
        Rope_Node* removed;
        Rope_Node* right;
        split(root, position, left, remainder);
        split(remainder, count, removed, right);
        delete_tree(removed);
        root = merge(left, right);
        return true;
    }


    // Replace one character
    bool set(int index, char value)
    {
        if (index < 0 || index >= subtree_length(root))
        {
            return false;
        }

        Rope_Node* left;
        Rope_Node* remainder;
        Rope_Node* selected;
        Rope_Node* right;
        split(root, index, left, remainder);
        split(remainder, 1, selected, right);

        string character(1, value);
        Rope_Node* replacement = make_node(character);
        delete_tree(selected);
        root = merge(merge(left, replacement), right);
        return true;
    }


    // Return one character through value
    bool get(int index, char& value)
    {
        if (index < 0 || index >= subtree_length(root))
        {
            return false;
        }

        return get_character(root, index, value);
    }


    // Return a substring; position may equal the rope length
    bool substring(int position, int count, string& result)
    {
        int length = subtree_length(root);
        if (position < 0 || count < 0 || position > length
            || count > length - position)
        {
            return false;
        }

        Rope_Node* left;
        Rope_Node* remainder;
        Rope_Node* selected;
        Rope_Node* right;
        split(root, position, left, remainder);
        split(remainder, count, selected, right);
        result.clear();
        append_text(selected, result);
        root = merge(left, merge(selected, right));
        return true;
    }


    // Concatenate text at the end
    int append(const string& text)
    {
        root = merge(root, build(text));
        return 1;
    }


    // Return all text in the rope
    string to_string()
    {
        string result;
        result.reserve(subtree_length(root));
        append_text(root, result);
        return result;
    }


    // Remove all text
    int clear()
    {
        delete_tree(root);
        root = nullptr;
        return 1;
    }


    // Check whether the rope is empty
    bool is_empty()
    {
        return root == nullptr;
    }


    // Return number of characters
    int get_length()
    {
        return subtree_length(root);
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


bool read_text(string& value)
{
    cout << "\nEnter text: ";
    if (!getline(cin >> ws, value))
    {
        cerr << "Input error: expected text." << endl;
        std::exit(EXIT_FAILURE);
    }
    return true;
}


int main()
{
    cout << "============================================" << endl;
    cout << "              Welcome To Rope Application" << endl;
    cout << "============================================" << endl;
    cout << "Indices and lengths count bytes; UTF-8 characters may use multiple bytes." << endl;

    string initialText;
    cout << "\nEnter initial text (use - for empty): ";
    if (!(cin >> initialText)) {
        cerr << "Input error: expected text." << endl;
        return EXIT_FAILURE;
    }
    if (initialText == "-") {
        initialText.clear();
    }

    Rope_String rope(initialText);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Insert Text" << endl;
        cout << "2. Delete Text" << endl;
        cout << "3. Replace Character" << endl;
        cout << "4. Get Character" << endl;
        cout << "5. Get Substring" << endl;
        cout << "6. Display Text" << endl;
        cout << "7. Rope Information" << endl;
        cout << "8. Clear Rope" << endl;
        cout << "9. Exit" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        read_input(mainChoice);

        if (mainChoice == 1) {
            int position;
            string text;
            cout << "\nEnter insertion position: ";
            read_input(position);
            read_text(text);
            cout << (rope.insert(position, text)
                ? "Text inserted successfully."
                : "Invalid position.") << endl;
        }
        else if (mainChoice == 2) {
            int position;
            int count;
            cout << "\nEnter start position: ";
            read_input(position);
            cout << "Enter number of bytes to delete: ";
            read_input(count);
            cout << (rope.erase(position, count)
                ? "Text deleted successfully."
                : "Invalid position or length.") << endl;
        }
        else if (mainChoice == 3) {
            int index;
            string text;
            cout << "\nEnter character index: ";
            read_input(index);
            read_text(text);
            if (text.length() != 1) {
                cout << "Enter exactly one byte." << endl;
            }
            else {
                cout << (rope.set(index, text[0])
                    ? "Character replaced successfully."
                    : "Invalid index.") << endl;
            }
        }
        else if (mainChoice == 4) {
            int index;
            char value;
            cout << "\nEnter character index: ";
            read_input(index);
            if (rope.get(index, value)) {
                cout << "Character: " << value << endl;
            }
            else {
                cout << "Invalid index." << endl;
            }
        }
        else if (mainChoice == 5) {
            int position;
            int count;
            string result;
            cout << "\nEnter start position: ";
            read_input(position);
            cout << "Enter number of bytes: ";
            read_input(count);
            if (rope.substring(position, count, result)) {
                cout << "Substring: " << result << endl;
            }
            else {
                cout << "Invalid position or length." << endl;
            }
        }
        else if (mainChoice == 6) {
            cout << "\nText: " << rope.to_string() << endl;
        }
        else if (mainChoice == 7) {
            cout << "\nLength in bytes: " << rope.get_length() << endl;
            cout << "Empty: " << (rope.is_empty() ? "Yes" : "No") << endl;
        }
        else if (mainChoice == 8) {
            rope.clear();
            cout << "Rope cleared successfully." << endl;
        }
        else if (mainChoice != 9) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 9);

    cout << "\nThank you for using Rope Application." << endl;
    return 0;
}
