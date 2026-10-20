#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

struct Node {
    int data;       // Value stored in this node
    Node* next;     // Pointer to the node behind this one (nullptr = rear of the queue)

    Node(int value)
    {
        data = value;
        next = nullptr;
    }
};


class Linked_List_Queue_Number {
private:

    Node* front;    // First node of the queue (nullptr when empty)
    Node* rear;     // Last node of the queue (nullptr when empty)
    int length;     // Number of nodes currently stored

public:

    // Constructor
    // Creates an empty queue
    Linked_List_Queue_Number()
    {
        front = nullptr;
        rear = nullptr;
        length = 0;
    }


    // Destructor
    // Every node was created with new, so every node must be deleted
    ~Linked_List_Queue_Number()
    {
        clear();
    }


    // Copy Constructor
    // Creates a deep copy of another queue
    Linked_List_Queue_Number(const Linked_List_Queue_Number& other)
    {
        front = nullptr;
        rear = nullptr;
        length = 0;

        Node* current = other.front;

        while (current != nullptr)
        {
            enqueue(current->data);

            current = current->next;
        }
    }


    // Assignment Operator
    // Performs deep copy between objects
    Linked_List_Queue_Number& operator=(const Linked_List_Queue_Number& other)
    {
        // Avoid self-assignment
        if (this == &other)
        {
            return *this;
        }

        // Build the copy first, so nothing is lost if something fails
        Linked_List_Queue_Number temp(other);

        // Swap contents, temp will delete our old nodes
        Node* oldFront = front;
        front = temp.front;
        temp.front = oldFront;

        Node* oldRear = rear;
        rear = temp.rear;
        temp.rear = oldRear;

        int oldLength = length;
        length = temp.length;
        temp.length = oldLength;

        return *this;
    }


    // Add value at the rear of the queue
    // O(1) because we keep a rear pointer
    int enqueue(int value)
    {
        Node* newNode = new Node(value);

        if (rear == nullptr)
        {
            // Empty queue: the new node is both front and rear
            front = newNode;
        }
        else
        {
            rear->next = newNode;
        }

        rear = newNode;

        length++;

        return 1;
    }


    // Remove the front value and return it through value
    // Returns false if the queue is empty (Queue Underflow)
    bool dequeue(int& value)
    {
        if (front != nullptr)
        {
            Node* toDelete = front;

            // Save value, then move front forward
            value = toDelete->data;
            front = front->next;

            // If the queue became empty, the rear must be cleared too
            if (front == nullptr)
            {
                rear = nullptr;
            }

            delete toDelete;

            length--;

            return true;
        }

        return false;
    }


    // Read the front value without removing it
    // Returns false if the queue is empty
    bool peek(int& value)
    {
        if (front != nullptr)
        {
            value = front->data;

            return true;
        }

        return false;
    }


    // Check if the queue has no elements
    bool is_empty()
    {
        return front == nullptr;
    }


    // Delete every node
    int clear()
    {
        Node* current = front;

        while (current != nullptr)
        {
            // Save next before deleting, or we lose the rest of the queue
            Node* nextNode = current->next;

            delete current;

            current = nextNode;
        }

        front = nullptr;
        rear = nullptr;
        length = 0;

        return 1;
    }


    // Search from the front
    // Returns distance from the front (0 = front), otherwise -1
    int search(int key)
    {
        Node* current = front;

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


    // Reverse the queue
    // Only the links change, no value is moved
    int reverse()
    {
        Node* previous = nullptr;
        Node* current = front;

        while (current != nullptr)
        {
            // Remember the rest of the queue
            Node* nextNode = current->next;

            // Point this node backward
            current->next = previous;

            // Move one step forward
            previous = current;
            current = nextNode;
        }

        // The old front is now the rear, and the old rear is now the front
        Node* oldFront = front;
        front = rear;
        rear = oldFront;

        return 1;
    }


    // Display all stored elements from front to rear
    int display()
    {
        Node* current = front;

        cout << "Front -> ";

        while (current != nullptr)
        {
            cout << current->data << " ";

            current = current->next;
        }

        cout << "<- Rear" << endl;

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

// Josephus game: n people stand in a circle, every k-th person is removed
// Returns the number of the last person standing
// The queue rotates: the front person goes to the rear, until the k-th one leaves
int last_person_standing(int n, int k)
{
    Linked_List_Queue_Number people;

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
    Linked_List_Queue_Number numbers;

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
    cout << "   Welcome To Linked List Queue Application" << endl;
    cout << "============================================" << endl;


    // ============================================
    // Create Queue
    // ============================================

    Linked_List_Queue_Number que;


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

                    que.enqueue(value);

                    cout << "Number added successfully." << endl;

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
                cout << "3. Get Length" << endl;
                cout << "4. Back To Main Menu" << endl;

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


                // Get Length
                else if (choice == 3) {

                    cout << "\nQueue Length = " << que.get_length() << endl;
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
