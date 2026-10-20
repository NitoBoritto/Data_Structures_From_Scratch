#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

class Static_Queue_Number {
private:

    int* Array;     // Pointer to dynamically allocated array
    int size;       // Total capacity of the queue
    int front;      // Index of the front element
    int length;     // Number of elements currently stored
                    // The rear is at (front + length - 1) % size

public:

    // Constructor
    // Creates an empty queue with the given capacity
    Static_Queue_Number(int Size)
    {
        size = (Size >= 0) ? Size : 0;
        front = 0;
        length = 0;

        // Allocate memory dynamically
        Array = new int[size];
    }


    // Destructor
    // Releases dynamically allocated memory
    ~Static_Queue_Number()
    {
        delete[] Array;
    }


    // Copy Constructor
    // Creates a deep copy of another object
    Static_Queue_Number(const Static_Queue_Number& other)
    {
        size = other.size;
        front = 0;
        length = other.length;

        // Allocate new memory
        Array = new int[size];

        // Copy elements in queue order
        for (int i = 0; i < length; i++)
        {
            Array[i] = other.Array[(other.front + i) % other.size];
        }
    }


    // Assignment Operator
    // Performs deep copy between objects
    Static_Queue_Number& operator=(const Static_Queue_Number& other)
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


    // Add value at the rear of the queue
    // Returns 0 if the queue is full
    int enqueue(int value)
    {
        if (length < size)
        {
            // The rear wraps around to the start of the array
            Array[(front + length) % size] = value;
            length++;

            return 1;
        }

        return 0;
    }


    // Remove the front value and return it through value
    // Returns false if the queue is empty (Queue Underflow)
    bool dequeue(int& value)
    {
        if (length > 0)
        {
            value = Array[front];

            // The front moves forward and wraps around at the end of the array
            front = (front + 1) % size;
            length--;

            return true;
        }

        return false;
    }


    // Read the front value without removing it
    // Returns false if the queue is empty
    bool peek(int& value)
    {
        if (length > 0)
        {
            value = Array[front];

            return true;
        }

        return false;
    }


    // Check if the queue has no elements
    bool is_empty()
    {
        return length == 0;
    }

    // Check if the queue has no free space
    bool is_full()
    {
        return length == size;
    }


    // Remove all elements
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


    // Reverse the queue
    // The front becomes the rear
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


    // Display all stored elements from front to rear
    int display()
    {
        cout << "Front -> ";

        for (int i = 0; i < length; i++)
        {
            cout << Array[(front + i) % size] << " ";
        }

        cout << "<- Rear" << endl;

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

// Josephus game: n people stand in a circle, every k-th person is removed
// Returns the number of the last person standing
// The queue rotates: the front person goes to the rear, until the k-th one leaves
int last_person_standing(int n, int k)
{
    Static_Queue_Number people(n);

    for (int i = 1; i <= n; i++)
    {
        people.enqueue(i);
    }

    int person;

    while (people.get_length() > 1)
    {
        // The first k - 1 people are safe: they go back to the rear
        for (int i = 1; i < k; i++)
        {
            people.dequeue(person);
            people.enqueue(person);
        }

        // The k-th person is removed
        people.dequeue(person);
    }

    people.dequeue(person);

    return person;
}


// Print the binary numbers from 1 to n using a queue
// Every number creates its two children: x -> x0 and x1
// The numbers are stored as digits, so n must be small (up to 500)
void print_binary_numbers(int n)
{
    Static_Queue_Number numbers(n + 2);

    numbers.enqueue(1);

    for (int i = 0; i < n; i++)
    {
        int current;

        numbers.dequeue(current);

        cout << current << " ";

        numbers.enqueue(current * 10);
        numbers.enqueue(current * 10 + 1);
    }

    cout << endl;
}


void read_input(int& value)
{
    if (!(cin >> value))
    {
        cerr << "Input error: expected an integer." << endl;
        std::exit(EXIT_FAILURE);
    }
}


int main() {

    // ============================================
    // Welcome
    // ============================================

    cout << "============================================" << endl;
    cout << "     Welcome To Static Queue Application" << endl;
    cout << "============================================" << endl;


    // ============================================
    // Create Queue
    // ============================================

    int size;

    do {
        cout << "\nEnter Queue Capacity: ";
        read_input(size);

        if (size < 0) {
            cout << "Capacity cannot be negative!" << endl;
        }
    } while (size < 0);

    Static_Queue_Number que(size);


    // ============================================
    // Main Menu
    // ============================================

    int mainChoice;

    do {

        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;

        cout << "1. Basic Operations" << endl;
        cout << "2. Queue Operations" << endl;
        cout << "3. Queue Information" << endl;
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

                cout << "1. Enqueue Number" << endl;
                cout << "2. Dequeue Number" << endl;
                cout << "3. Peek Front Number" << endl;
                cout << "4. Clear Queue" << endl;
                cout << "5. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Enqueue Number
                if (choice == 1) {

                    int value;

                    cout << "\nEnter number: ";
                    read_input(value);

                    if (que.enqueue(value)) {
                        cout << "Number added successfully." << endl;
                    }
                    else {
                        cout << "Queue is full!" << endl;
                    }

                    cout << "Queue: ";
                    que.display();
                }


                // Dequeue Number
                else if (choice == 2) {

                    int value;

                    if (que.dequeue(value)) {
                        cout << "\nRemoved number: " << value << endl;
                    }
                    else {
                        cout << "\nQueue Underflow! Queue is empty." << endl;
                    }

                    cout << "Queue: ";
                    que.display();
                }


                // Peek Front Number
                else if (choice == 3) {

                    int value;

                    if (que.peek(value)) {
                        cout << "\nFront number: " << value << endl;
                    }
                    else {
                        cout << "\nQueue is empty!" << endl;
                    }
                }


                // Clear Queue
                else if (choice == 4) {

                    que.clear();

                    cout << "\nQueue cleared successfully." << endl;

                    cout << "Queue: ";
                    que.display();
                }


                else if (choice != 5) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 5);
        }


        // ==================================================
        // 2. QUEUE OPERATIONS
        // ==================================================

        else if (mainChoice == 2) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "              QUEUE OPERATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Search Number" << endl;
                cout << "2. Reverse Queue" << endl;
                cout << "3. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Search Number
                if (choice == 1) {

                    int key;

                    cout << "\nEnter number to search: ";
                    read_input(key);

                    int position = que.search(key);

                    if (position != -1) {
                        cout << "Number found " << position << " place(s) behind the front." << endl;
                    }
                    else {
                        cout << "Number not found." << endl;
                    }
                }


                // Reverse Queue
                else if (choice == 2) {

                    que.reverse();

                    cout << "\nQueue reversed successfully." << endl;

                    cout << "Queue: ";
                    que.display();
                }


                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 3);
        }


        // ==================================================
        // 3. QUEUE INFORMATION
        // ==================================================

        else if (mainChoice == 3) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "             QUEUE INFORMATION" << endl;
                cout << "============================================" << endl;

                cout << "1. Display Queue" << endl;
                cout << "2. Check If Empty" << endl;
                cout << "3. Check If Full" << endl;
                cout << "4. Get Length" << endl;
                cout << "5. Get Capacity" << endl;
                cout << "6. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Display Queue
                if (choice == 1) {

                    if (que.is_empty()) {
                        cout << "\nQueue is empty!" << endl;
                    }
                    else {
                        cout << "\n";
                        que.display();
                    }
                }


                // Check If Empty
                else if (choice == 2) {

                    if (que.is_empty()) {
                        cout << "\nQueue is empty." << endl;
                    }
                    else {
                        cout << "\nQueue is NOT empty." << endl;
                    }
                }


                // Check If Full
                else if (choice == 3) {

                    if (que.is_full()) {
                        cout << "\nQueue is full." << endl;
                    }
                    else {
                        cout << "\nQueue is NOT full." << endl;
                    }
                }


                // Get Length
                else if (choice == 4) {

                    cout << "\nQueue Length = " << que.get_length() << endl;
                }


                // Get Capacity
                else if (choice == 5) {

                    cout << "\nQueue Capacity = " << que.get_size() << endl;
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

                cout << "1. Josephus Game" << endl;
                cout << "2. Binary Numbers 1 To N" << endl;
                cout << "3. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Josephus Game
                if (choice == 1) {

                    int n;
                    int k;

                    cout << "\nEnter number of people: ";
                    read_input(n);

                    cout << "Enter step (every k-th person leaves): ";
                    read_input(k);

                    if (n < 1 || k < 1) {
                        cout << "Both numbers must be at least 1!" << endl;
                    }
                    else {
                        cout << "Last person standing: " << last_person_standing(n, k) << endl;
                    }
                }


                // Binary Numbers 1 To N
                else if (choice == 2) {

                    int n;

                    cout << "\nEnter how many numbers (1 to 500): ";
                    read_input(n);

                    if (n < 1 || n > 500) {
                        cout << "Number must be between 1 and 500!" << endl;
                    }
                    else {
                        print_binary_numbers(n);
                    }
                }


                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 3);
        }


        // ==================================================
        // 5. EXIT
        // ==================================================

        else if (mainChoice == 5) {
            cout << "\nThank you for using the Queue Application!" << endl;
        }

        else {
            cout << "\nInvalid choice!" << endl;
        }

    } while (mainChoice != 5);

    return 0;
}
