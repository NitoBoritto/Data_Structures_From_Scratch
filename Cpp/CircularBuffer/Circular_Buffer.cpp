#include <iostream>
#include <cstdlib>
using namespace std;

class Circular_Buffer_Number {
private:

    int* Array;
    int capacity;
    int front;
    int length;

public:

    // Constructor
    // Creates a circular buffer with fixed capacity
    Circular_Buffer_Number(int Capacity)
    {
        capacity = (Capacity > 0) ? Capacity : 0;
        front = 0;
        length = 0;
        Array = (capacity > 0) ? new int[capacity] : nullptr;
    }


    // Destructor
    // Releases dynamically allocated memory
    ~Circular_Buffer_Number()
    {
        delete[] Array;
    }


    // Copy Constructor
    // Copies values in logical front-to-back order
    Circular_Buffer_Number(const Circular_Buffer_Number& other)
    {
        capacity = other.capacity;
        front = 0;
        length = other.length;
        Array = (capacity > 0) ? new int[capacity] : nullptr;

        for (int i = 0; i < length; i++)
        {
            Array[i] = other.Array[(other.front + i) % other.capacity];
        }
    }


    // Assignment Operator
    // Performs deep copy in logical front-to-back order
    Circular_Buffer_Number& operator=(const Circular_Buffer_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        int* newArray = (other.capacity > 0) ? new int[other.capacity] : nullptr;
        for (int i = 0; i < other.length; i++)
        {
            newArray[i] = other.Array[(other.front + i) % other.capacity];
        }

        delete[] Array;
        Array = newArray;
        capacity = other.capacity;
        front = 0;
        length = other.length;
        return *this;
    }


    // Add a value at the rear
    // Returns false when full; existing values are preserved
    bool enqueue(int value)
    {
        if (length == capacity)
        {
            return false;
        }

        int rear = (front + length) % capacity;
        Array[rear] = value;
        length++;
        return true;
    }


    // Add a value at the rear, overwriting the oldest value when full
    bool enqueue_overwrite(int value)
    {
        if (capacity == 0)
        {
            return false;
        }

        if (length < capacity)
        {
            return enqueue(value);
        }

        Array[front] = value;
        front = (front + 1) % capacity;
        return true;
    }


    // Remove the oldest value and return it through value
    bool dequeue(int& value)
    {
        if (length == 0)
        {
            return false;
        }

        value = Array[front];
        front = (front + 1) % capacity;
        length--;

        if (length == 0)
        {
            front = 0;
        }

        return true;
    }


    // Read the oldest value without removing it
    bool peek_front(int& value)
    {
        if (length == 0)
        {
            return false;
        }

        value = Array[front];
        return true;
    }


    // Read the newest value without removing it
    bool peek_rear(int& value)
    {
        if (length == 0)
        {
            return false;
        }

        int rear = (front + length - 1) % capacity;
        value = Array[rear];
        return true;
    }


    // Return value at a logical index from the front
    bool get(int index, int& value)
    {
        if (index < 0 || index >= length)
        {
            return false;
        }

        value = Array[(front + index) % capacity];
        return true;
    }


    // Print values from oldest to newest
    int display()
    {
        if (length == 0)
        {
            cout << "Buffer is empty." << endl;
            return 0;
        }

        for (int i = 0; i < length; i++)
        {
            cout << Array[(front + i) % capacity] << " ";
        }
        cout << endl;
        return 1;
    }


    // Remove every value; capacity stays the same
    int clear()
    {
        front = 0;
        length = 0;
        return 1;
    }


    // Check if the buffer has no values
    bool is_empty()
    {
        return length == 0;
    }


    // Check if the buffer has reached its capacity
    bool is_full()
    {
        return length == capacity;
    }


    // Return number of stored values
    int get_length()
    {
        return length;
    }


    // Return total capacity
    int get_capacity()
    {
        return capacity;
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
    cout << "    Welcome To Circular Buffer Application" << endl;
    cout << "============================================" << endl;
    cout << "The buffer supports both reject-full and overwrite-oldest enqueue." << endl;

    int capacity;
    do {
        cout << "\nEnter buffer capacity: ";
        read_input(capacity);
        if (capacity < 0) {
            cout << "Capacity cannot be negative!" << endl;
        }
    } while (capacity < 0);

    Circular_Buffer_Number buffer(capacity);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Buffer Operations" << endl;
        cout << "3. Buffer Information" << endl;
        cout << "4. Exit" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        read_input(mainChoice);

        if (mainChoice == 1) {
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "              BASIC OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Enqueue (Reject If Full)" << endl;
                cout << "2. Enqueue (Overwrite Oldest)" << endl;
                cout << "3. Dequeue Oldest" << endl;
                cout << "4. Peek Front" << endl;
                cout << "5. Peek Rear" << endl;
                cout << "6. Clear Buffer" << endl;
                cout << "7. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1 || choice == 2) {
                    int value;
                    cout << "\nEnter number: ";
                    read_input(value);
                    bool success = (choice == 1)
                        ? buffer.enqueue(value)
                        : buffer.enqueue_overwrite(value);

                    if (success) {
                        cout << "Number added successfully." << endl;
                    }
                    else if (buffer.get_capacity() == 0) {
                        cout << "Buffer capacity is zero." << endl;
                    }
                    else {
                        cout << "Buffer is full; number was not added." << endl;
                    }
                }
                else if (choice == 3) {
                    int value;
                    if (buffer.dequeue(value)) {
                        cout << "Removed number: " << value << endl;
                    }
                    else {
                        cout << "Buffer is empty." << endl;
                    }
                }
                else if (choice == 4 || choice == 5) {
                    int value;
                    bool success = (choice == 4)
                        ? buffer.peek_front(value)
                        : buffer.peek_rear(value);

                    if (success) {
                        cout << (choice == 4 ? "Front number: " : "Rear number: ") << value << endl;
                    }
                    else {
                        cout << "Buffer is empty." << endl;
                    }
                }
                else if (choice == 6) {
                    buffer.clear();
                    cout << "Buffer cleared successfully." << endl;
                }
                else if (choice != 7) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 7);
        }
        else if (mainChoice == 2) {
            cout << "\nBuffer (oldest to newest): ";
            buffer.display();
        }
        else if (mainChoice == 3) {
            cout << "\nValues: " << buffer.get_length() << endl;
            cout << "Capacity: " << buffer.get_capacity() << endl;
            cout << "Empty: " << (buffer.is_empty() ? "Yes" : "No") << endl;
            cout << "Full: " << (buffer.is_full() ? "Yes" : "No") << endl;
        }
        else if (mainChoice != 4) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 4);

    cout << "\nThank you for using Circular Buffer Application." << endl;
    return 0;
}
