#include <iostream>
#include <cstdlib>
using namespace std;

struct Skip_List_Node {
    int value;
    int level;
    Skip_List_Node** forward;

    Skip_List_Node(int Value, int Level)
    {
        value = Value;
        level = Level;
        forward = new Skip_List_Node*[Level + 1];

        for (int i = 0; i <= Level; i++)
        {
            forward[i] = nullptr;
        }
    }

    ~Skip_List_Node()
    {
        delete[] forward;
    }
};


class Skip_List_Number {
private:

    Skip_List_Node* head;
    int maximumLevel;
    int currentLevel;
    int length;
    unsigned int randomState;

private:

    unsigned int next_random()
    {
        randomState = (randomState * 1664525U) + 1013904223U;
        return randomState;
    }


    int generate_level()
    {
        int level = 0;

        while (level < maximumLevel && (next_random() & 1U) == 0)
        {
            level++;
        }

        return level;
    }


    void delete_nodes()
    {
        Skip_List_Node* current = head->forward[0];

        while (current != nullptr)
        {
            Skip_List_Node* nextNode = current->forward[0];
            delete current;
            current = nextNode;
        }

        for (int i = 0; i <= maximumLevel; i++)
        {
            head->forward[i] = nullptr;
        }

        currentLevel = 0;
        length = 0;
    }


    void copy_nodes(const Skip_List_Number& other)
    {
        Skip_List_Node** tails = new Skip_List_Node*[maximumLevel + 1];
        for (int level = 0; level <= maximumLevel; level++)
        {
            tails[level] = head;
        }

        Skip_List_Node* source = other.head->forward[0];
        while (source != nullptr)
        {
            Skip_List_Node* copy = new Skip_List_Node(source->value, source->level);

            for (int level = 0; level <= source->level; level++)
            {
                tails[level]->forward[level] = copy;
                tails[level] = copy;
            }

            source = source->forward[0];
        }

        delete[] tails;
    }

public:

    // Constructor
    // Creates an empty skip list with a maximum number of levels
    Skip_List_Number(int Maximum_Level = 16, unsigned int Seed = 1)
    {
        maximumLevel = (Maximum_Level > 0) ? Maximum_Level : 1;
        currentLevel = 0;
        length = 0;
        randomState = (Seed != 0) ? Seed : 1;
        head = new Skip_List_Node(0, maximumLevel);
    }


    // Destructor
    // Releases every skip-list node
    ~Skip_List_Number()
    {
        delete_nodes();
        delete head;
    }


    // Copy Constructor
    // Copies values and preserves their level layout
    Skip_List_Number(const Skip_List_Number& other)
    {
        maximumLevel = other.maximumLevel;
        currentLevel = other.currentLevel;
        length = other.length;
        randomState = other.randomState;
        head = new Skip_List_Node(0, maximumLevel);
        copy_nodes(other);
    }


    // Assignment Operator
    // Performs deep copy between skip lists
    Skip_List_Number& operator=(const Skip_List_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        delete_nodes();
        delete head;

        maximumLevel = other.maximumLevel;
        currentLevel = other.currentLevel;
        length = other.length;
        randomState = other.randomState;
        head = new Skip_List_Node(0, maximumLevel);
        copy_nodes(other);

        return *this;
    }


    // Insert a value while keeping the list sorted
    // Duplicate values are not stored
    bool insert(int value)
    {
        Skip_List_Node** update = new Skip_List_Node*[maximumLevel + 1];
        Skip_List_Node* current = head;

        for (int level = currentLevel; level >= 0; level--)
        {
            while (current->forward[level] != nullptr
                && current->forward[level]->value < value)
            {
                current = current->forward[level];
            }

            update[level] = current;
        }

        current = current->forward[0];
        if (current != nullptr && current->value == value)
        {
            delete[] update;
            return false;
        }

        int newLevel = generate_level();
        if (newLevel > currentLevel)
        {
            for (int level = currentLevel + 1; level <= newLevel; level++)
            {
                update[level] = head;
            }
            currentLevel = newLevel;
        }

        Skip_List_Node* newNode = new Skip_List_Node(value, newLevel);
        for (int level = 0; level <= newLevel; level++)
        {
            newNode->forward[level] = update[level]->forward[level];
            update[level]->forward[level] = newNode;
        }

        length++;
        delete[] update;
        return true;
    }


    // Search for a value
    bool search(int value)
    {
        Skip_List_Node* current = head;

        for (int level = currentLevel; level >= 0; level--)
        {
            while (current->forward[level] != nullptr
                && current->forward[level]->value < value)
            {
                current = current->forward[level];
            }
        }

        current = current->forward[0];
        return current != nullptr && current->value == value;
    }


    // Remove a value
    bool erase(int value)
    {
        Skip_List_Node** update = new Skip_List_Node*[maximumLevel + 1];
        Skip_List_Node* current = head;

        for (int level = currentLevel; level >= 0; level--)
        {
            while (current->forward[level] != nullptr
                && current->forward[level]->value < value)
            {
                current = current->forward[level];
            }

            update[level] = current;
        }

        current = current->forward[0];
        if (current == nullptr || current->value != value)
        {
            delete[] update;
            return false;
        }

        for (int level = 0; level <= currentLevel; level++)
        {
            if (update[level]->forward[level] == current)
            {
                update[level]->forward[level] = current->forward[level];
            }
        }

        delete current;
        length--;

        while (currentLevel > 0 && head->forward[currentLevel] == nullptr)
        {
            currentLevel--;
        }

        delete[] update;
        return true;
    }


    // Return the smallest value greater than or equal to value
    bool lower_bound(int value, int& result)
    {
        Skip_List_Node* current = head;

        for (int level = currentLevel; level >= 0; level--)
        {
            while (current->forward[level] != nullptr
                && current->forward[level]->value < value)
            {
                current = current->forward[level];
            }
        }

        current = current->forward[0];
        if (current == nullptr)
        {
            return false;
        }

        result = current->value;
        return true;
    }


    // Print values in sorted order
    int display()
    {
        if (length == 0)
        {
            cout << "Skip list is empty." << endl;
            return 0;
        }

        Skip_List_Node* current = head->forward[0];
        while (current != nullptr)
        {
            cout << current->value << " ";
            current = current->forward[0];
        }
        cout << endl;
        return 1;
    }


    // Remove all values
    int clear()
    {
        delete_nodes();
        return 1;
    }


    // Check if the skip list has no values
    bool is_empty()
    {
        return length == 0;
    }


    // Return number of stored values
    int get_length()
    {
        return length;
    }


    // Return the current highest populated level
    int get_current_level()
    {
        return currentLevel;
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
    cout << "        Welcome To Skip List Application" << endl;
    cout << "============================================" << endl;

    int maximumLevel;
    do {
        cout << "\nEnter maximum level: ";
        read_input(maximumLevel);
        if (maximumLevel < 1) {
            cout << "Maximum level must be at least 1." << endl;
        }
    } while (maximumLevel < 1);

    Skip_List_Number list(maximumLevel);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Search Operations" << endl;
        cout << "3. List Operations" << endl;
        cout << "4. List Information" << endl;
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
                cout << "3. Clear List" << endl;
                cout << "4. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1 || choice == 2) {
                    int value;
                    cout << "\nEnter number: ";
                    read_input(value);

                    bool success = (choice == 1)
                        ? list.insert(value)
                        : list.erase(value);

                    if (success) {
                        cout << (choice == 1 ? "Number added successfully." : "Number deleted successfully.") << endl;
                    }
                    else {
                        cout << (choice == 1 ? "Number already exists." : "Number not found.") << endl;
                    }
                }
                else if (choice == 3) {
                    list.clear();
                    cout << "Skip list cleared successfully." << endl;
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
                cout << "2. Find Lower Bound" << endl;
                cout << "3. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1 || choice == 2) {
                    int value;
                    cout << "\nEnter number: ";
                    read_input(value);

                    if (choice == 1) {
                        cout << (list.search(value) ? "Number found." : "Number not found.") << endl;
                    }
                    else {
                        int result;
                        if (list.lower_bound(value, result)) {
                            cout << "First number not less than the input: " << result << endl;
                        }
                        else {
                            cout << "No number is greater than or equal to the input." << endl;
                        }
                    }
                }
                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 3);
        }
        else if (mainChoice == 3) {
            cout << "\nSkip list in sorted order: ";
            list.display();
        }
        else if (mainChoice == 4) {
            cout << "\nValues: " << list.get_length() << endl;
            cout << "Current highest level: " << list.get_current_level() << endl;
            cout << "Empty: " << (list.is_empty() ? "Yes" : "No") << endl;
        }
        else if (mainChoice != 5) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 5);

    cout << "\nThank you for using Skip List Application." << endl;
    return 0;
}
