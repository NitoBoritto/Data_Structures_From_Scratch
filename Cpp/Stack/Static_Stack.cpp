#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

class Static_Stack_Number {
private:

    int* Array;     // Pointer to dynamically allocated array
    int size;       // Total capacity of the stack
    int length;     // Number of elements currently stored (the top is Array[length - 1])

public:

    // Constructor
    // Creates an empty stack with the given capacity
    Static_Stack_Number(int Size)
    {
        size = (Size >= 0) ? Size : 0;
        length = 0;

        // Allocate memory dynamically
        Array = new int[size];
    }


    // Destructor
    // Releases dynamically allocated memory
    ~Static_Stack_Number()
    {
        delete[] Array;
    }


    // Copy Constructor
    // Creates a deep copy of another object
    Static_Stack_Number(const Static_Stack_Number& other)
    {
        size = other.size;
        length = other.length;

        // Allocate new memory
        Array = new int[size];

        // Copy elements
        for (int i = 0; i < length; i++)
        {
            Array[i] = other.Array[i];
        }
    }


    // Assignment Operator
    // Performs deep copy between objects
    Static_Stack_Number& operator=(const Static_Stack_Number& other)
    {
        // Avoid self-assignment
        if (this == &other)
        {
            return *this;
        }

        // Allocate and copy first, so nothing is lost if allocation fails
        int* newArray = new int[other.size];

        for (int i = 0; i < other.length; i++)
        {
            newArray[i] = other.Array[i];
        }

        // Delete old memory
        delete[] Array;

        Array = newArray;
        size = other.size;
        length = other.length;

        return *this;
    }


    // Push value on top of the stack
    // Returns 0 if the stack is full (Stack Overflow)
    int push(int value)
    {
        if (length < size)
        {
            Array[length] = value;
            length++;

            return 1;
        }

        return 0;
    }


    // Remove the top value and return it through value
    // Returns false if the stack is empty (Stack Underflow)
    bool pop(int& value)
    {
        if (length > 0)
        {
            length--;
            value = Array[length];

            return true;
        }

        return false;
    }


    // Read the top value without removing it
    // Returns false if the stack is empty
    bool peek(int& value)
    {
        if (length > 0)
        {
            value = Array[length - 1];

            return true;
        }

        return false;
    }


    // Check if the stack has no elements
    bool is_empty()
    {
        return length == 0;
    }


    // Check if the stack has no free space
    bool is_full()
    {
        return length == size;
    }


    // Remove all elements
    int clear()
    {
        length = 0;

        return 1;
    }


    // Search from the top
    // Returns distance from the top (0 = top), otherwise -1
    int search(int key)
    {
        for (int i = length - 1; i >= 0; i--)
        {
            if (Array[i] == key)
            {
                return length - 1 - i;
            }
        }

        return -1;
    }


    // Reverse the stack
    // The top becomes the bottom
    int reverse()
    {
        int start = 0;
        int end = length - 1;

        while (start < end)
        {
            swap(Array[start], Array[end]);
            start++;
            end--;
        }

        return 1;
    }


    // Display all stored elements from bottom to top
    int display()
    {
        cout << "Bottom -> ";

        for (int i = 0; i < length; i++)
        {
            cout << Array[i] << " ";
        }

        cout << "<- Top" << endl;

        return 1;
    }


    // Return total capacity
    int get_size()
    {
        return size;
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
    Static_Stack_Number brackets(static_cast<int>(text.length()));

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
    Static_Stack_Number letters(static_cast<int>(text.length()));

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

    Static_Stack_Number digits(32);

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
    cout << "     Welcome To Static Stack Application" << endl;
    cout << "============================================" << endl;


    // ============================================
    // Create Stack
    // ============================================

    int size;

    do {
        cout << "\nEnter Stack Capacity: ";
        read_input(size);

        if (size < 0) {
            cout << "Capacity cannot be negative!" << endl;
        }
    } while (size < 0);

    Static_Stack_Number stk(size);


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

                    if (stk.push(value)) {
                        cout << "Number pushed successfully." << endl;
                    }
                    else {
                        cout << "Stack Overflow! Stack is full." << endl;
                    }
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
                cout << "3. Check If Full" << endl;
                cout << "4. Get Length" << endl;
                cout << "5. Get Capacity" << endl;
                cout << "6. Back To Main Menu" << endl;

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


                // Check If Full
                else if (choice == 3) {

                    if (stk.is_full()) {
                        cout << "\nStack is full." << endl;
                    }
                    else {
                        cout << "\nStack is NOT full." << endl;
                    }
                }


                // Get Length
                else if (choice == 4) {

                    cout << "\nStack Length = " << stk.get_length() << endl;
                }


                // Get Capacity
                else if (choice == 5) {

                    cout << "\nStack Capacity = " << stk.get_size() << endl;
                }


                else if (choice != 6) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 6);
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