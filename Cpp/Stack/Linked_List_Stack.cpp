#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

struct Node {
    int data;       // Value stored in this node
    Node* next;     // Pointer to the node below this one (nullptr = bottom of the stack)

    Node(int value)
    {
        data = value;
        next = nullptr;
    }
};


class Linked_List_Stack_Number {
private:

    Node* top;      // Top node of the stack (nullptr when empty)
    int length;     // Number of nodes currently stored

private:

    // Copy all nodes of another stack into this (empty) stack
    // Same order: the top stays on top
    void copy_from(const Linked_List_Stack_Number& other)
    {
        Node* current = other.top;
        Node* last = nullptr;

        while (current != nullptr)
        {
            Node* newNode = new Node(current->data);

            if (last == nullptr)
            {
                top = newNode;
            }
            else
            {
                last->next = newNode;
            }

            last = newNode;
            length++;

            current = current->next;
        }
    }

public:

    // Constructor
    // Creates an empty stack
    Linked_List_Stack_Number()
    {
        top = nullptr;
        length = 0;
    }


    // Destructor
    // Every node was created with new, so every node must be deleted
    ~Linked_List_Stack_Number()
    {
        clear();
    }


    // Copy Constructor
    // Creates a deep copy of another stack
    Linked_List_Stack_Number(const Linked_List_Stack_Number& other)
    {
        top = nullptr;
        length = 0;

        copy_from(other);
    }


    // Assignment Operator
    // Performs deep copy between objects
    Linked_List_Stack_Number& operator=(const Linked_List_Stack_Number& other)
    {
        // Avoid self-assignment
        if (this == &other)
        {
            return *this;
        }

        // Build the copy first, so nothing is lost if something fails
        Linked_List_Stack_Number temp(other);

        // Swap contents, temp will delete our old nodes
        Node* oldTop = top;
        top = temp.top;
        temp.top = oldTop;

        int oldLength = length;
        length = temp.length;
        temp.length = oldLength;

        return *this;
    }


    // Push value on top of the stack
    // The new node points to the old top, then becomes the top
    int push(int value)
    {
        Node* newNode = new Node(value);

        newNode->next = top;
        top = newNode;

        length++;

        return 1;
    }


    // Remove the top value and return it through value
    // Returns false if the stack is empty (Stack Underflow)
    bool pop(int& value)
    {
        if (top != nullptr)
        {
            Node* toDelete = top;

            // Save value, then move top down
            value = toDelete->data;
            top = top->next;

            delete toDelete;

            length--;

            return true;
        }

        return false;
    }


    // Read the top value without removing it
    // Returns false if the stack is empty
    bool peek(int& value)
    {
        if (top != nullptr)
        {
            value = top->data;

            return true;
        }

        return false;
    }


    // Check if the stack has no elements
    bool is_empty()
    {
        return top == nullptr;
    }


    // Delete every node
    int clear()
    {
        Node* current = top;

        while (current != nullptr)
        {
            // Save next before deleting, or we lose the rest of the stack
            Node* nextNode = current->next;

            delete current;

            current = nextNode;
        }

        top = nullptr;
        length = 0;

        return 1;
    }


    // Search from the top
    // Returns distance from the top (0 = top), otherwise -1
    int search(int key)
    {
        Node* current = top;

        for (int i = 0; i < length; i++)
        {
            if (current->data == key)
            {
                return i;
            }

            current = current->next;
        }

        return -1;
    }


    // Reverse the stack
    // Only the links change, no value is moved
    int reverse()
    {
        Node* previous = nullptr;
        Node* current = top;

        while (current != nullptr)
        {
            // Remember the rest of the stack
            Node* nextNode = current->next;

            // Point this node backward
            current->next = previous;

            // Move one step forward
            previous = current;
            current = nextNode;
        }

        top = previous;

        return 1;
    }


    // Display all stored elements from bottom to top
    int display()
    {
        // Nodes only know the node below them, so the top comes first
        // Collect values into a temporary array to print from the bottom
        int* values = new int[length > 0 ? length : 1];

        Node* current = top;

        for (int i = length - 1; i >= 0; i--)
        {
            values[i] = current->data;
            current = current->next;
        }

        cout << "Bottom -> ";

        for (int i = 0; i < length; i++)
        {
            cout << values[i] << " ";
        }

        cout << "<- Top" << endl;

        delete[] values;

        return 1;
    }


    // Return number of stored elements
    int get_length()
    {
        return length;
    }
};


// ============================================
// Applications
// ============================================

// Check if (), [] and {} are balanced
// Every opening bracket waits on the stack for its closing bracket
bool is_balanced(const string& text)
{
    Linked_List_Stack_Number brackets;

    for (size_t i = 0; i < text.length(); i++)
    {
        char c = text[i];

        if (c == '(' || c == '[' || c == '{')
        {
            brackets.push(c);
        }
        else if (c == ')' || c == ']' || c == '}')
        {
            int open;

            // A closing bracket with nothing open
            if (!brackets.pop(open))
            {
                return false;
            }

            // The closing bracket must match the last opened one
            if ((c == ')' && open != '(') ||
                (c == ']' && open != '[') ||
                (c == '}' && open != '{'))
            {
                return false;
            }
        }
    }

    // Balanced only if nothing is still waiting
    return brackets.is_empty();
}


// Reverse a text
// Push every character, then pop them: last in, first out
string reverse_text(const string& text)
{
    Linked_List_Stack_Number letters;

    for (size_t i = 0; i < text.length(); i++)
    {
        letters.push(text[i]);
    }

    string result = "";
    int c;

    while (letters.pop(c))
    {
        result += static_cast<char>(c);
    }

    return result;
}


// Convert a decimal number to binary
// Remainders come out last digit first, the stack puts them in order
string decimal_to_binary(int number)
{
    if (number == 0)
    {
        return "0";
    }

    Linked_List_Stack_Number digits;

    while (number > 0)
    {
        digits.push(number % 2);
        number = number / 2;
    }

    string result = "";
    int bit;

    while (digits.pop(bit))
    {
        result += static_cast<char>('0' + bit);
    }

    return result;
}


void read_input(int& value)
{
    if (!(cin >> value))
    {
        cerr << "Input error: expected an integer." << endl;
        std::exit(EXIT_FAILURE);
    }
}


void read_text(string& value)
{
    if (!(cin >> value))
    {
        cerr << "Input error: expected text." << endl;
        std::exit(EXIT_FAILURE);
    }
}


int main() {

    // ============================================
    // Welcome
    // ============================================

    cout << "============================================" << endl;
    cout << "   Welcome To Linked List Stack Application" << endl;
    cout << "============================================" << endl;


    // ============================================
    // Create Stack
    // ============================================

    Linked_List_Stack_Number stk;


    // ============================================
    // Main Menu
    // ============================================

    int mainChoice;

    do {

        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;

        cout << "1. Basic Operations" << endl;
        cout << "2. Stack Operations" << endl;
        cout << "3. Stack Information" << endl;
        cout << "4. Applications" << endl;
        cout << "5. Exit" << endl;

        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        read_input(mainChoice);


        // ==================================================
        // 1. BASIC OPERATIONS
        // ==================================================

        if (mainChoice == 1) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "              BASIC OPERATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Push Number" << endl;
                cout << "2. Pop Number" << endl;
                cout << "3. Peek Top Number" << endl;
                cout << "4. Clear Stack" << endl;
                cout << "5. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Push Number
                if (choice == 1) {

                    int value;

                    cout << "\nEnter number: ";
                    read_input(value);

                    stk.push(value);

                    cout << "Number pushed successfully." << endl;
                    cout << "Stack: ";
                    stk.display();
                }


                // Pop Number
                else if (choice == 2) {

                    int value;

                    if (stk.pop(value)) {
                        cout << "\nPopped number: " << value << endl;
                    }
                    else {
                        cout << "\nStack Underflow! Stack is empty." << endl;
                    }
                    cout << "Stack: ";
                    stk.display();
                }


                // Peek Top Number
                else if (choice == 3) {

                    int value;

                    if (stk.peek(value)) {
                        cout << "\nTop number: " << value << endl;
                    }
                    else {
                        cout << "\nStack is empty!" << endl;
                    }
                }


                // Clear Stack
                else if (choice == 4) {

                    stk.clear();

                    cout << "\nStack cleared successfully." << endl;
                    cout << "Stack: ";
                    stk.display();
                }


                else if (choice != 5) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 5);
        }


        // ==================================================
        // 2. STACK OPERATIONS
        // ==================================================

        else if (mainChoice == 2) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "              STACK OPERATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Search Number" << endl;
                cout << "2. Reverse Stack" << endl;
                cout << "3. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Search Number
                if (choice == 1) {

                    int key;

                    cout << "\nEnter number to search: ";
                    read_input(key);

                    int position = stk.search(key);

                    if (position != -1) {
                        cout << "Number found " << position << " place(s) below the top." << endl;
                    }
                    else {
                        cout << "Number not found." << endl;
                    }
                }


                // Reverse Stack
                else if (choice == 2) {

                    stk.reverse();

                    cout << "\nStack reversed successfully." << endl;
                    cout << "Stack: ";
                    stk.display();
                }


                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 3);
        }


        // ==================================================
        // 3. STACK INFORMATION
        // ==================================================

        else if (mainChoice == 3) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "             STACK INFORMATION" << endl;
                cout << "============================================" << endl;

                cout << "1. Display Stack" << endl;
                cout << "2. Check If Empty" << endl;
                cout << "3. Get Length" << endl;
                cout << "4. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Display Stack
                if (choice == 1) {

                    if (stk.is_empty()) {
                        cout << "\nStack is empty!" << endl;
                    }
                    else {
                        cout << "\n";
                        stk.display();
                    }
                }


                // Check If Empty
                else if (choice == 2) {

                    if (stk.is_empty()) {
                        cout << "\nStack is empty." << endl;
                    }
                    else {
                        cout << "\nStack is NOT empty." << endl;
                    }
                }


                // Get Length
                else if (choice == 3) {

                    cout << "\nStack Length = " << stk.get_length() << endl;
                }


                else if (choice != 4) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 4);
        }


        // ==================================================
        // 4. APPLICATIONS
        // ==================================================

        else if (mainChoice == 4) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "               APPLICATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Check Balanced Brackets" << endl;
                cout << "2. Reverse A Text" << endl;
                cout << "3. Decimal To Binary" << endl;
                cout << "4. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Check Balanced Brackets
                if (choice == 1) {

                    string text;

                    cout << "\nEnter text (no spaces): ";
                    read_text(text);

                    if (is_balanced(text)) {
                        cout << "Brackets are balanced." << endl;
                    }
                    else {
                        cout << "Brackets are NOT balanced." << endl;
                    }
                }


                // Reverse A Text
                else if (choice == 2) {

                    string text;

                    cout << "\nEnter text (no spaces): ";
                    read_text(text);

                    cout << "Reversed: " << reverse_text(text) << endl;
                }


                // Decimal To Binary
                else if (choice == 3) {

                    int number;

                    cout << "\nEnter a non-negative number: ";
                    read_input(number);

                    if (number < 0) {
                        cout << "Number cannot be negative!" << endl;
                    }
                    else {
                        cout << "Binary: " << decimal_to_binary(number) << endl;
                    }
                }


                else if (choice != 4) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 4);
        }


        // ==================================================
        // 5. EXIT
        // ==================================================

        else if (mainChoice == 5) {
            cout << "\nThank you for using the Stack Application!" << endl;
        }

        else {
            cout << "\nInvalid choice!" << endl;
        }

    } while (mainChoice != 5);

    return 0;
}