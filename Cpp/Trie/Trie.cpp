#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

struct Trie_Node {
    bool is_word;
    int word_count;
    struct Trie_Edge* children;

    Trie_Node()
    {
        is_word = false;
        word_count = 0;
        children = nullptr;
    }
};


struct Trie_Edge {
    unsigned char character;
    Trie_Node* child;
    Trie_Edge* next;

    Trie_Edge(unsigned char Character, Trie_Node* Child)
    {
        character = Character;
        child = Child;
        next = nullptr;
    }
};


class Trie_String {
private:

    Trie_Node* root;
    int length;

private:

    Trie_Edge* find_edge(Trie_Node* node, unsigned char character)
    {
        Trie_Edge* current = node->children;

        while (current != nullptr)
        {
            if (current->character == character)
            {
                return current;
            }

            if (current->character > character)
            {
                return nullptr;
            }

            current = current->next;
        }

        return nullptr;
    }


    Trie_Node* find_node(const string& text)
    {
        Trie_Node* currentNode = root;

        for (size_t i = 0; i < text.length(); i++)
        {
            Trie_Edge* edge = find_edge(
                currentNode,
                static_cast<unsigned char>(text[i])
            );

            if (edge == nullptr)
            {
                return nullptr;
            }

            currentNode = edge->child;
        }

        return currentNode;
    }


    Trie_Node* copy_node(const Trie_Node* node)
    {
        Trie_Node* newNode = new Trie_Node();
        newNode->is_word = node->is_word;
        newNode->word_count = node->word_count;

        Trie_Edge* current = node->children;
        Trie_Edge* last = nullptr;

        while (current != nullptr)
        {
            Trie_Node* copiedChild = copy_node(current->child);
            Trie_Edge* newEdge = new Trie_Edge(current->character, copiedChild);

            if (last == nullptr)
            {
                newNode->children = newEdge;
            }
            else
            {
                last->next = newEdge;
            }

            last = newEdge;
            current = current->next;
        }

        return newNode;
    }


    void delete_node(Trie_Node* node)
    {
        Trie_Edge* current = node->children;

        while (current != nullptr)
        {
            Trie_Edge* nextEdge = current->next;
            delete_node(current->child);
            delete current;
            current = nextEdge;
        }

        delete node;
    }


    // Remove a word and prune nodes that no longer lead to a word
    bool erase_word(Trie_Node* node, const string& word, size_t index, bool& erased)
    {
        if (index == word.length())
        {
            if (!node->is_word)
            {
                return false;
            }

            node->is_word = false;
            node->word_count--;
            erased = true;
            return node->word_count == 0;
        }

        unsigned char character = static_cast<unsigned char>(word[index]);
        Trie_Edge* current = node->children;
        Trie_Edge* previous = nullptr;

        while (current != nullptr && current->character < character)
        {
            previous = current;
            current = current->next;
        }

        if (current == nullptr || current->character != character)
        {
            return false;
        }

        bool removeChild = erase_word(current->child, word, index + 1, erased);
        if (!erased)
        {
            return false;
        }

        node->word_count--;

        if (removeChild)
        {
            if (previous == nullptr)
            {
                node->children = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            delete current->child;
            delete current;
        }

        return node->word_count == 0 && !node->is_word;
    }


    void display_words(Trie_Node* node, string& prefix, int& displayed, int limit)
    {
        if (node->is_word && displayed < limit)
        {
            cout << prefix << endl;
            displayed++;
        }

        Trie_Edge* current = node->children;
        while (current != nullptr && displayed < limit)
        {
            prefix.push_back(static_cast<char>(current->character));
            display_words(current->child, prefix, displayed, limit);
            prefix.pop_back();
            current = current->next;
        }
    }

public:

    // Constructor
    // Creates an empty trie
    Trie_String()
    {
        root = new Trie_Node();
        length = 0;
    }


    // Destructor
    // Every trie node and edge was created with new
    ~Trie_String()
    {
        delete_node(root);
    }


    // Copy Constructor
    // Creates a deep copy of another trie
    Trie_String(const Trie_String& other)
    {
        root = copy_node(other.root);
        length = other.length;
    }


    // Assignment Operator
    // Performs deep copy between tries
    Trie_String& operator=(const Trie_String& other)
    {
        if (this == &other)
        {
            return *this;
        }

        Trie_Node* newRoot = copy_node(other.root);
        delete_node(root);
        root = newRoot;
        length = other.length;

        return *this;
    }


    // Add a word
    // Returns 0 if the word already exists or is empty
    int insert(const string& word)
    {
        if (word.empty())
        {
            return 0;
        }

        Trie_Node* currentNode = root;

        for (size_t i = 0; i < word.length(); i++)
        {
            unsigned char character = static_cast<unsigned char>(word[i]);
            Trie_Edge* currentEdge = currentNode->children;
            Trie_Edge* previous = nullptr;

            while (currentEdge != nullptr && currentEdge->character < character)
            {
                previous = currentEdge;
                currentEdge = currentEdge->next;
            }

            if (currentEdge == nullptr || currentEdge->character != character)
            {
                Trie_Node* newNode = new Trie_Node();
                Trie_Edge* newEdge = new Trie_Edge(character, newNode);
                newEdge->next = currentEdge;

                if (previous == nullptr)
                {
                    currentNode->children = newEdge;
                }
                else
                {
                    previous->next = newEdge;
                }

                currentNode = newNode;
            }
            else
            {
                currentNode = currentEdge->child;
            }
        }

        if (currentNode->is_word)
        {
            return 0;
        }

        currentNode->is_word = true;
        currentNode = root;
        currentNode->word_count++;

        for (size_t i = 0; i < word.length(); i++)
        {
            Trie_Edge* edge = find_edge(
                currentNode,
                static_cast<unsigned char>(word[i])
            );
            currentNode = edge->child;
            currentNode->word_count++;
        }

        length++;
        return 1;
    }


    // Check whether a complete word exists
    bool search(const string& word)
    {
        if (word.empty())
        {
            return false;
        }

        Trie_Node* node = find_node(word);
        return node != nullptr && node->is_word;
    }


    // Check whether at least one word starts with the given prefix
    // An empty prefix matches every non-empty word
    bool starts_with(const string& prefix)
    {
        Trie_Node* node = find_node(prefix);
        return node != nullptr && node->word_count > 0;
    }


    // Return the number of words that start with the given prefix
    int count_prefix(const string& prefix)
    {
        Trie_Node* node = find_node(prefix);
        return (node == nullptr) ? 0 : node->word_count;
    }


    // Remove a word and nodes used only by that word
    bool erase(const string& word)
    {
        if (word.empty())
        {
            return false;
        }

        bool erased = false;
        erase_word(root, word, 0, erased);

        if (erased)
        {
            length--;
        }

        return erased;
    }


    // Print up to limit words matching prefix, in byte order
    int display_suggestions(const string& prefix, int limit)
    {
        Trie_Node* node = find_node(prefix);

        if (node == nullptr || node->word_count == 0)
        {
            cout << "No matching words." << endl;
            return 0;
        }

        if (limit <= 0)
        {
            cout << "Suggestion limit must be greater than zero." << endl;
            return 0;
        }

        string currentPrefix = prefix;
        int displayed = 0;
        display_words(node, currentPrefix, displayed, limit);
        return displayed;
    }


    // Print every stored word
    int display()
    {
        if (length == 0)
        {
            cout << "Trie is empty." << endl;
            return 0;
        }

        string prefix;
        int displayed = 0;
        display_words(root, prefix, displayed, length);
        return displayed;
    }


    // Remove all words
    int clear()
    {
        delete_node(root);
        root = new Trie_Node();
        length = 0;
        return 1;
    }


    // Check if the trie has no words
    bool is_empty()
    {
        return length == 0;
    }


    // Return number of stored words
    int get_length()
    {
        return length;
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

    return !value.empty();
}


int main()
{
    cout << "============================================" << endl;
    cout << "            Welcome To Trie Application" << endl;
    cout << "============================================" << endl;
    cout << "Trie matching is case-sensitive." << endl;

    Trie_String trie;
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Search Operations" << endl;
        cout << "3. Trie Operations" << endl;
        cout << "4. Trie Information" << endl;
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
                cout << "1. Insert Word" << endl;
                cout << "2. Delete Word" << endl;
                cout << "3. Clear Trie" << endl;
                cout << "4. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1 || choice == 2) {
                    string word;

                    if (!read_text(word)) {
                        cout << "Text cannot be empty." << endl;
                    }
                    else if (choice == 1) {
                        if (trie.insert(word)) {
                            cout << "Word added successfully." << endl;
                        }
                        else {
                            cout << "Word already exists." << endl;
                        }
                    }
                    else if (trie.erase(word)) {
                        cout << "Word deleted successfully." << endl;
                    }
                    else {
                        cout << "Word not found." << endl;
                    }
                }
                else if (choice == 3) {
                    trie.clear();
                    cout << "Trie cleared successfully." << endl;
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
                cout << "1. Search Complete Word" << endl;
                cout << "2. Search Prefix" << endl;
                cout << "3. Count Words With Prefix" << endl;
                cout << "4. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice >= 1 && choice <= 3) {
                    string text;

                    if (!read_text(text)) {
                        cout << "Text cannot be empty." << endl;
                    }
                    else if (choice == 1) {
                        cout << (trie.search(text) ? "Word found." : "Word not found.") << endl;
                    }
                    else if (choice == 2) {
                        cout << (trie.starts_with(text) ? "Matching words exist." : "No matching words.") << endl;
                    }
                    else {
                        cout << "Matching words: " << trie.count_prefix(text) << endl;
                    }
                }
                else if (choice != 4) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 4);
        }
        else if (mainChoice == 3) {
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "                TRIE OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Show Suggestions" << endl;
                cout << "2. Display All Words" << endl;
                cout << "3. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    string prefix;
                    int limit;
                    if (!read_text(prefix)) {
                        cout << "Prefix cannot be empty here." << endl;
                    }
                    else {
                        cout << "Enter maximum number of suggestions: ";
                        read_input(limit);
                        cout << "Suggestions:" << endl;
                        trie.display_suggestions(prefix, limit);
                    }
                }
                else if (choice == 2) {
                    cout << "\nWords in the trie:" << endl;
                    trie.display();
                }
                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 3);
        }
        else if (mainChoice == 4) {
            cout << "\nWords: " << trie.get_length() << endl;
            cout << "Empty: " << (trie.is_empty() ? "Yes" : "No") << endl;
            cout << "Matching every word: " << trie.count_prefix("") << endl;
        }
        else if (mainChoice != 5) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 5);

    cout << "\nThank you for using Trie Application." << endl;
    return 0;
}
