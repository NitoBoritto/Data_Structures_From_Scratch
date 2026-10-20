#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

class Deque_Number {
private:

    int* Array;     // Pointer to dynamically allocated array
    int size;       // Total capacity of the deque
    int front;      // Index of the front element
    int length;     // Number of elements currently stored
                    // The back is at (front + length - 1) % size

private:

    // Allocate a bigger array and move the old elements into it
    // The elements are copied in order, so the front goes back to index 0
    void resize(int newSize)
    {
        // Never shrink
        if (newSize <= size)
        {
            return;
        }

        int* newArray = new int[newSize];

        // Copy old elements from the front to the back
        for (int i = 0; i < length; i++)
        {
            newArray[i] = Array[(front + i) % size];
        }

        // Delete old array
        delete[] Array;

        // Use the new array
        Array = newArray;
        size = newSize;
        front = 0;
    }


    // Make sure there is enough space for the requested number of elements
    // The capacity is doubled until it is big enough
    void ensure_capacity(int requiredCapacity)
    {
        if (requiredCapacity <= size)
        {
            return;
        }

        int newSize = size;

        while (newSize < requiredCapacity)
        {
            newSize *= 2;
        }

        resize(newSize);
    }

public:

    // Constructor
    // Creates an empty deque with the given starting capacity
    Deque_Number(int Size)
    {
        // A deque needs at least one slot so it can grow
        size = (Size > 0) ? Size : 1;
        front = 0;
        length = 0;

        // Allocate memory dynamically
        Array = new int[size];
    }


    // Destructor
    // Releases dynamically allocated memory
    ~Deque_Number()
    {
        delete[] Array;
    }


    // Copy Constructor
    // Creates a deep copy of another object
    Deque_Number(const Deque_Number& other)
    {
        size = other.size;
        front = 0;
        length = other.length;

        // Allocate new memory
        Array = new int[size];

        // Copy elements in order
        for (int i = 0; i < length; i++)
        {
            Array[i] = other.Array[(other.front + i) % other.size];
        }
    }


    // Assignment Operator
    // Performs deep copy between objects
    Deque_Number& operator=(const Deque_Number& other)
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
            newArray[i] = other.Array[(other.front + i) % other.size];
        }

        // Delete old memory
        delete[] Array;

        Array = newArray;
        size = other.size;
        front = 0;
        length = other.length;

        return *this;
    }


    // Add value at the front
    // The front moves backward and wraps around to the end of the array
    int push_front(int value)
    {
        ensure_capacity(length + 1);

        front = (front - 1 + size) % size;
        Array[front] = value;
        length++;

        return 1;
    }


    // Add value at the back
    int push_back(int value)
    {
        ensure_capacity(length + 1);

        Array[(front + length) % size] = value;
        length++;

        return 1;
    }


    // Remove the front value and return it through value
    // Returns false if the deque is empty
    bool pop_front(int& value)
    {
        if (length > 0)
        {
            value = Array[front];

            front = (front + 1) % size;
            length--;

            return true;
        }

        return false;
    }


    // Remove the back value and return it through value
    // Returns false if the deque is empty
    bool pop_back(int& value)
    {
        if (length > 0)
        {
            value = Array[(front + length - 1) % size];

            length--;

            return true;
        }

        return false;
    }


    // Read the front value without removing it
    // Returns false if the deque is empty
    bool peek_front(int& value)
    {
        if (length > 0)
        {
            value = Array[front];

            return true;
        }

        return false;
    }


    // Read the back value without removing it
    // Returns false if the deque is empty
    bool peek_back(int& value)
    {
        if (length > 0)
        {
            value = Array[(front + length - 1) % size];

            return true;
        }

        return false;
    }


    // Check if the deque has no elements
    bool is_empty()
    {
        return length == 0;
    }


    // Remove all elements
    // The capacity stays the same
    int clear()
    {
        front = 0;
        length = 0;

        return 1;
    }


    // Search from the front
    // Returns distance from the front (0 = front), otherwise -1
    int search(int key)
    {
        for (int i = 0; i < length; i++)
        {
            if (Array[(front + i) % size] == key)
            {
                return i;
            }
        }

        return -1;
    }


    // Reverse the deque
    // The front becomes the back
    int reverse()
    {
        int start = 0;
        int end = length - 1;

        while (start < end)
        {
            int a = (front + start) % size;
            int b = (front + end) % size;

            swap(Array[a], Array[b]);
            start++;
            end--;
        }

        return 1;
    }


    // Display all stored elements from front to back
    int display()
    {
        cout << "Front <-> ";

        for (int i = 0; i < length; i++)
        {
            cout << Array[(front + i) % size] << " ";
        }

        cout << "<-> Back" << endl;

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

// Check if a text reads the same from both ends
// Compare the front and the back, then remove both: this needs a deque
bool is_palindrome(const string& text)
{
    Deque_Number letters(static_cast<int>(text.length()));

    for (size_t i = 0; i < text.length(); i++)
    {
        letters.push_back(text[i]);
    }

    int first;
    int last;

    while (letters.get_length() > 1)
    {
        letters.pop_front(first);
        letters.pop_back(last);

        if (first != last)
        {
            return false;
        }
    }

    return true;
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
    cout << "         Welcome To Deque Application" << endl;
    cout << "============================================" << endl;


    // ============================================
    // Create Deque
    // ============================================

    int size;

    do {
        cout << "\nEnter Starting Capacity: ";
        read_input(size);

        if (size < 0) {
            cout << "Capacity cannot be negative!" << endl;
        }
    } while (size < 0);

    Deque_Number dq(size);


    // ============================================
    // Main Menu
    // ============================================

    int mainChoice;

    do {

        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;

        cout << "1. Basic Operations" << endl;
        cout << "2. Deque Operations" << endl;
        cout << "3. Deque Information" << endl;
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

                cout << "1. Push Front" << endl;
                cout << "2. Push Back" << endl;
                cout << "3. Pop Front" << endl;
                cout << "4. Pop Back" << endl;
                cout << "5. Peek Front" << endl;
                cout << "6. Peek Back" << endl;
                cout << "7. Clear Deque" << endl;
                cout << "8. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Push Front
                if (choice == 1) {

                    int value;

                    cout << "\nEnter number: ";
                    read_input(value);

                    dq.push_front(value);

                    cout << "Number added at the front." << endl;

                    cout << "Deque: ";
                    dq.display();
                }


                // Push Back
                else if (choice == 2) {

                    int value;

                    cout << "\nEnter number: ";
                    read_input(value);

                    dq.push_back(value);

                    cout << "Number added at the back." << endl;

                    cout << "Deque: ";
                    dq.display();
                }


                // Pop Front
                else if (choice == 3) {

                    int value;

                    if (dq.pop_front(value)) {
                        cout << "\nRemoved from the front: " << value << endl;
                    }
                    else {
                        cout << "\nDeque is empty!" << endl;
                    }

                    cout << "Deque: ";
                    dq.display();
                }


                // Pop Back
                else if (choice == 4) {

                    int value;

                    if (dq.pop_back(value)) {
                        cout << "\nRemoved from the back: " << value << endl;
                    }
                    else {
                        cout << "\nDeque is empty!" << endl;
                    }

                    cout << "Deque: ";
                    dq.display();
                }


                // Peek Front
                else if (choice == 5) {

                    int value;

                    if (dq.peek_front(value)) {
                        cout << "\nFront number: " << value << endl;
                    }
                    else {
                        cout << "\nDeque is empty!" << endl;
                    }
                }


                // Peek Back
                else if (choice == 6) {

                    int value;

                    if (dq.peek_back(value)) {
                        cout << "\nBack number: " << value << endl;
                    }
                    else {
                        cout << "\nDeque is empty!" << endl;
                    }
                }


                // Clear Deque
                else if (choice == 7) {

                    dq.clear();

                    cout << "\nDeque cleared successfully." << endl;

                    cout << "Deque: ";
                    dq.display();
                }


                else if (choice != 8) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 8);
        }


        // ==================================================
        // 2. DEQUE OPERATIONS
        // ==================================================

        else if (mainChoice == 2) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "              DEQUE OPERATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Search Number" << endl;
                cout << "2. Reverse Deque" << endl;
                cout << "3. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Search Number
                if (choice == 1) {

                    int key;

                    cout << "\nEnter number to search: ";
                    read_input(key);

                    int position = dq.search(key);

                    if (position != -1) {
                        cout << "Number found " << position << " place(s) behind the front." << endl;
                    }
                    else {
                        cout << "Number not found." << endl;
                    }
                }


                // Reverse Deque
                else if (choice == 2) {

                    dq.reverse();

                    cout << "\nDeque reversed successfully." << endl;

                    cout << "Deque: ";
                    dq.display();
                }


                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 3);
        }


        // ==================================================
        // 3. DEQUE INFORMATION
        // ==================================================

        else if (mainChoice == 3) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "             DEQUE INFORMATION" << endl;
                cout << "============================================" << endl;

                cout << "1. Display Deque" << endl;
                cout << "2. Check If Empty" << endl;
                cout << "3. Get Length" << endl;
                cout << "4. Get Capacity" << endl;
                cout << "5. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Display Deque
                if (choice == 1) {

                    if (dq.is_empty()) {
                        cout << "\nDeque is empty!" << endl;
                    }
                    else {
                        cout << "\n";
                        dq.display();
                    }
                }


                // Check If Empty
                else if (choice == 2) {

                    if (dq.is_empty()) {
                        cout << "\nDeque is empty." << endl;
                    }
                    else {
                        cout << "\nDeque is NOT empty." << endl;
                    }
                }


                // Get Length
                else if (choice == 3) {

                    cout << "\nDeque Length = " << dq.get_length() << endl;
                }


                // Get Capacity
                else if (choice == 4) {

                    cout << "\nDeque Capacity = " << dq.get_size() << endl;
                }


                else if (choice != 5) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 5);
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

                cout << "1. Check Palindrome" << endl;
                cout << "2. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Check Palindrome
                if (choice == 1) {

                    string text;

                    cout << "\nEnter text (no spaces): ";
                    read_text(text);

                    if (is_palindrome(text)) {
                        cout << "The text is a palindrome." << endl;
                    }
                    else {
                        cout << "The text is NOT a palindrome." << endl;
                    }
                }


                else if (choice != 2) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 2);
        }


        // ==================================================
        // 5. EXIT
        // ==================================================

        else if (mainChoice == 5) {
            cout << "\nThank you for using the Deque Application!" << endl;
        }

        else {
            cout << "\nInvalid choice!" << endl;
        }

    } while (mainChoice != 5);

    return 0;
}
