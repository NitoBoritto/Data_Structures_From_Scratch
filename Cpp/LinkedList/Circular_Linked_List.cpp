#include <iostream>
using namespace std;
struct Node {
    int data;       // Value stored in this node
    Node* next;     // Pointer to the next node (the last one points to the first)

    Node(int value)
    {
        data = value;
        next = nullptr;
    }
};


class Circular_Linked_List_Number {
private:

    Node* tail;     // Last node, tail->next is the first node (nullptr when empty)
    int length;     // Number of nodes currently stored

private:

    // The first node (same idea in all list types)
    Node* first_node()
    {
        if (tail == nullptr)
        {
            return nullptr;
        }

        return tail->next;
    }


    // Return the node at the given index
    // The index must be valid
    Node* get_node(int index)
    {
        Node* current = tail->next;

        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }

        return current;
    }


    // Copy all nodes of another list into this (empty) list
    void copy_from(const Circular_Linked_List_Number& other)
    {
        if (other.tail == nullptr)
        {
            return;
        }

        Node* current = other.tail->next;

        // Count with length: there is no NULL to stop us
        for (int i = 0; i < other.length; i++)
        {
            append(current->data);

            current = current->next;
        }
    }

public:

    // Constructor
    // Creates an empty list
    Circular_Linked_List_Number()
    {
        tail = nullptr;
        length = 0;
    }


    // Destructor
    // Every node was created with new, so every node must be deleted
    ~Circular_Linked_List_Number()
    {
        clear();
    }


    // Copy Constructor
    // Creates a deep copy of another list
    Circular_Linked_List_Number(const Circular_Linked_List_Number& other)
    {
        tail = nullptr;
        length = 0;

        copy_from(other);
    }


    // Assignment Operator
    // Performs deep copy between objects
    Circular_Linked_List_Number& operator=(const Circular_Linked_List_Number& other)
    {
        // Avoid self-assignment
        if (this == &other)
        {
            return *this;
        }

        // Build the copy first, so nothing is lost if something fails
        Circular_Linked_List_Number temp(other);

        // Swap contents, temp will delete our old nodes
        Node* oldTail = tail;
        tail = temp.tail;
        temp.tail = oldTail;

        int oldLength = length;
        length = temp.length;
        temp.length = oldLength;

        return *this;
    }


    // Delete every node
    int clear()
    {
        if (tail != nullptr)
        {
            Node* current = tail->next;

            // Count with length: there is no NULL to stop us
            for (int i = 0; i < length; i++)
            {
                Node* nextNode = current->next;

                delete current;

                current = nextNode;
            }
        }

        tail = nullptr;
        length = 0;

        return 1;
    }


    // Add value at the end
    // O(1) because the tail is known
    int append(int value)
    {
        Node* newNode = new Node(value);

        if (tail == nullptr)
        {
            // Single node points to itself
            newNode->next = newNode;
            tail = newNode;
        }
        else
        {
            // New node points to the head, old tail points to new node
            newNode->next = tail->next;
            tail->next = newNode;

            // New node becomes the tail
            tail = newNode;
        }

        length++;

        return 1;
    }


    // Add value at the start
    // Same links as append, but the tail does not move
    int prepend(int value)
    {
        Node* newNode = new Node(value);

        if (tail == nullptr)
        {
            // Single node points to itself
            newNode->next = newNode;
            tail = newNode;
        }
        else
        {
            newNode->next = tail->next;
            tail->next = newNode;
        }

        length++;

        return 1;
    }


    // Insert value at a specific index
    int insert(int index, int value)
    {
        // Valid index is from 0 to length
        if (index < 0 || index > length)
        {
            return 0;
        }

        if (index == 0)
        {
            return prepend(value);
        }

        if (index == length)
        {
            return append(value);
        }

        // Find the node before the insertion point
        Node* previous = get_node(index - 1);

        Node* newNode = new Node(value);

        newNode->next = previous->next;
        previous->next = newNode;

        length++;

        return 1;
    }


    // Insert value while keeping list sorted
    // The list must already be sorted
    int insert_sorted(int value)
    {
        if (tail == nullptr || value < tail->next->data)
        {
            return prepend(value);
        }

        if (value >= tail->data)
        {
            return append(value);
        }

        Node* current = tail->next;

        // Move forward while the next value is not larger
        // This stops before the tail, because value < tail->data
        while (current->next->data <= value)
        {
            current = current->next;
        }

        Node* newNode = new Node(value);

        newNode->next = current->next;
        current->next = newNode;

        length++;

        return 1;
    }


    // Delete element by index
    // Returns deleted value
    int delete_index(int index)
    {
        if (index >= 0 && index < length)
        {
            // Only one node: the list becomes empty
            if (length == 1)
            {
                int x = tail->data;

                delete tail;

                tail = nullptr;
                length = 0;

                return x;
            }

            // For index 0 the node before the head is the tail
            Node* previous;

            if (index == 0)
            {
                previous = tail;
            }
            else
            {
                previous = get_node(index - 1);
            }

            Node* toDelete = previous->next;

            // Skip over the node we want to remove
            previous->next = toDelete->next;

            // If we removed the tail, the previous node is the new tail
            if (toDelete == tail)
            {
                tail = previous;
            }

            // Save deleted value
            int x = toDelete->data;

            delete toDelete;

            length--;

            return x;
        }

        return -1;
    }


    // Linear Search
    // Returns index if found, otherwise -1
    int linear_search(int key)
    {
        Node* current = first_node();

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


    // Get value at index
    // Returns true if index is valid
    bool get(int index, int& value)
    {
        if (index >= 0 && index < length)
        {
            value = get_node(index)->data;

            return true;
        }

        return false;
    }


    // Set value at index
    int set(int index, int value)
    {
        if (index >= 0 && index < length)
        {
            get_node(index)->data = value;

            return 1;
        }

        return 0;
    }


    // Return maximum value
    // Returns -1 if list is empty
    int max()
    {
        if (length == 0)
        {
            return -1;
        }

        Node* current = first_node();
        int max_value = current->data;

        for (int i = 1; i < length; i++)
        {
            current = current->next;

            if (current->data > max_value)
            {
                max_value = current->data;
            }
        }

        return max_value;
    }


    // Return minimum value
    // Returns -1 if list is empty
    int min()
    {
        if (length == 0)
        {
            return -1;
        }

        Node* current = first_node();
        int min_value = current->data;

        for (int i = 1; i < length; i++)
        {
            current = current->next;

            if (current->data < min_value)
            {
                min_value = current->data;
            }
        }

        return min_value;
    }


    // Return sum of all values
    int sum()
    {
        int total = 0;

        Node* current = first_node();

        for (int i = 0; i < length; i++)
        {
            total += current->data;
            current = current->next;
        }

        return total;
    }


    // Return average of all values
    double average()
    {
        if (length == 0)
        {
            return 0.0;
        }

        return static_cast<double>(sum()) / length;
    }


    // Check if list is sorted in ascending order
    int is_sorted()
    {
        if (length <= 1)
        {
            return 1;
        }

        Node* current = first_node();

        for (int i = 0; i < length - 1; i++)
        {
            if (current->data > current->next->data)
            {
                return 0;
            }

            current = current->next;
        }

        return 1;
    }


    // Return number of stored elements
    int get_length()
    {
        return length;
    }


    // Reverse the list
    // Only the links change, no value is moved
    int reverse()
    {
        if (length <= 1)
        {
            return 1;
        }

        Node* oldHead = tail->next;

        Node* previous = tail;
        Node* current = oldHead;

        // Count with length: there is no NULL to stop us
        for (int i = 0; i < length; i++)
        {
            Node* nextNode = current->next;

            // Point this node backward
            current->next = previous;

            // Move one step forward
            previous = current;
            current = nextNode;
        }

        // The old head is now the last node
        tail = oldHead;

        return 1;
    }


    // Rotate the list to the left by k positions
    // [1, 2, 3, 4] rotated by 1 becomes [2, 3, 4, 1]
    // No value is moved: only the tail pointer steps forward
    int rotate(int k)
    {
        if (k < 0)
        {
            return 0;
        }

        if (length > 1)
        {
            // Rotating by length gives the same list
            k = k % length;

            for (int i = 0; i < k; i++)
            {
                tail = tail->next;
            }
        }

        return 1;
    }


    // Display all stored elements
    int display()
    {
        if (tail == nullptr)
        {
            cout << "NULL" << endl;

            return 1;
        }

        Node* current = tail->next;

        // Count with length: there is no NULL to stop us
        for (int i = 0; i < length; i++)
        {
            cout << current->data << " -> ";

            current = current->next;
        }

        cout << "(back to " << tail->next->data << ")" << endl;

        return 1;
    }
};


int main() {

    // ============================================
    // Welcome
    // ============================================

    cout << "============================================" << endl;
    cout << "    Welcome To Circular Linked List Application" << endl;
    cout << "============================================" << endl;


    // ============================================
    // Create List
    // ============================================

    Circular_Linked_List_Number list;


    // ============================================
    // Main Menu
    // ============================================

    int mainChoice;

    do {

        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;

        cout << "1. Basic Operations" << endl;
        cout << "2. Search Operations" << endl;
        cout << "3. Mathematical Operations" << endl;
        cout << "4. List Operations" << endl;
        cout << "5. List Information" << endl;
        cout << "6. Exit" << endl;

        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        cin >> mainChoice;


        // ==================================================
        // 1. BASIC OPERATIONS
        // ==================================================

        if (mainChoice == 1) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "              BASIC OPERATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Append Number" << endl;
                cout << "2. Insert At Start" << endl;
                cout << "3. Insert At Index" << endl;
                cout << "4. Insert Sorted" << endl;
                cout << "5. Delete Number" << endl;
                cout << "6. Get Number" << endl;
                cout << "7. Set Number" << endl;
                cout << "8. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                cin >> choice;


                // Append Number
                if (choice == 1) {

                    int value;

                    cout << "\nEnter number: ";
                    cin >> value;

                    list.append(value);

                    cout << "Number added successfully." << endl;
                    cout << "List: ";
                    list.display();
                }


                // Insert At Start
                else if (choice == 2) {

                    int value;

                    cout << "\nEnter number: ";
                    cin >> value;

                    list.prepend(value);

                    cout << "Number added at the start successfully." << endl;
                    cout << "List: ";
                    list.display();
                }


                // Insert At Index
                else if (choice == 3) {

                    int index;
                    int value;

                    cout << "\nEnter index: ";
                    cin >> index;

                    cout << "Enter number: ";
                    cin >> value;

                    if (list.insert(index, value)) {
                        cout << "Number inserted successfully." << endl;
                    }
                    else {
                        cout << "Invalid index!" << endl;
                    }
                    cout << "List: ";
                    list.display();
                }


                // Insert Sorted
                else if (choice == 4) {

                    int value;

                    cout << "\nEnter number: ";
                    cin >> value;

                    if (list.is_sorted()) {
                        list.insert_sorted(value);

                        cout << "Number inserted successfully." << endl;
                    }
                    else {
                        cout << "List must be sorted first!" << endl;
                    }
                    cout << "List: ";
                    list.display();
                }


                // Delete Number
                else if (choice == 5) {

                    int index;

                    cout << "\nEnter index: ";
                    cin >> index;

                    if (index >= 0 && index < list.get_length()) {
                        cout << "Deleted number: " << list.delete_index(index) << endl;
                    }
                    else {
                        cout << "Invalid index!" << endl;
                    }
                    cout << "List: ";
                    list.display();
                }


                // Get Number
                else if (choice == 6) {

                    int index;
                    int value;

                    cout << "\nEnter index: ";
                    cin >> index;

                    if (list.get(index, value)) {
                        cout << "Number at index " << index << " = " << value << endl;
                    }
                    else {
                        cout << "Invalid index!" << endl;
                    }
                }


                // Set Number
                else if (choice == 7) {

                    int index;
                    int value;

                    cout << "\nEnter index: ";
                    cin >> index;

                    cout << "Enter new value: ";
                    cin >> value;

                    if (list.set(index, value)) {
                        cout << "Value updated successfully." << endl;
                    }
                    else {
                        cout << "Invalid index!" << endl;
                    }
                    cout << "List: ";
                    list.display();
                }


                else if (choice != 8) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 8);
        }


        // ==================================================
        // 2. SEARCH OPERATIONS
        // ==================================================

        else if (mainChoice == 2) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "             SEARCH OPERATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Linear Search" << endl;
                cout << "2. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                cin >> choice;


                // Linear Search
                if (choice == 1) {

                    int key;

                    cout << "\nEnter number to search: ";
                    cin >> key;

                    int index = list.linear_search(key);

                    if (index != -1) {
                        cout << "Number found at index " << index << endl;
                    }
                    else {
                        cout << "Number not found." << endl;
                    }
                }


                else if (choice != 2) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 2);
        }


        // ==================================================
        // 3. MATHEMATICAL OPERATIONS
        // ==================================================

        else if (mainChoice == 3) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "          MATHEMATICAL OPERATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Maximum" << endl;
                cout << "2. Minimum" << endl;
                cout << "3. Sum" << endl;
                cout << "4. Average" << endl;
                cout << "5. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                cin >> choice;


                // Maximum
                if (choice == 1) {

                    if (list.get_length() == 0) {
                        cout << "\nList is empty!" << endl;
                    }
                    else {
                        cout << "\nMaximum = " << list.max() << endl;
                    }
                }


                // Minimum
                else if (choice == 2) {

                    if (list.get_length() == 0) {
                        cout << "\nList is empty!" << endl;
                    }
                    else {
                        cout << "\nMinimum = " << list.min() << endl;
                    }
                }


                // Sum
                else if (choice == 3) {

                    cout << "\nSum = " << list.sum() << endl;
                }


                // Average
                else if (choice == 4) {

                    if (list.get_length() == 0) {
                        cout << "\nList is empty!" << endl;
                    }
                    else {
                        cout << "\nAverage = " << list.average() << endl;
                    }
                }


                else if (choice != 5) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 5);
        }


        // ==================================================
        // 4. LIST OPERATIONS
        // ==================================================

        else if (mainChoice == 4) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "              LIST OPERATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Reverse List" << endl;
                cout << "2. Check If Sorted" << endl;
                cout << "3. Rotate List" << endl;
                cout << "4. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                cin >> choice;


                // Reverse List
                if (choice == 1) {

                    list.reverse();

                    cout << "\nList reversed successfully." << endl;
                    cout << "List: ";
                    list.display();
                }


                // Check If Sorted
                else if (choice == 2) {

                    if (list.is_sorted()) {
                        cout << "\nList is sorted." << endl;
                    }
                    else {
                        cout << "\nList is NOT sorted." << endl;
                    }
                }


                // Rotate List
                else if (choice == 3) {

                    int k;

                    cout << "\nEnter number of positions: ";
                    cin >> k;

                    if (list.rotate(k)) {
                        cout << "List rotated successfully." << endl;
                    }
                    else {
                        cout << "Positions cannot be negative!" << endl;
                    }
                    cout << "List: ";
                    list.display();
                }


                else if (choice != 4) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 4);
        }


        // ==================================================
        // 5. LIST INFORMATION
        // ==================================================

        else if (mainChoice == 5) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "             LIST INFORMATION" << endl;
                cout << "============================================" << endl;

                cout << "1. Display List" << endl;
                cout << "2. Get Length" << endl;
                cout << "3. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                cin >> choice;


                // Display List
                if (choice == 1) {

                    if (list.get_length() == 0) {
                        cout << "\nList is empty!" << endl;
                    }
                    else {
                        cout << "\n";
                        list.display();
                    }
                }


                // Get Length
                else if (choice == 2) {

                    cout << "\nList Length = " << list.get_length() << endl;
                }


                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 3);
        }


        // ==================================================
        // 6. EXIT
        // ==================================================

        else if (mainChoice == 6) {
            cout << "\nThank you for using the List Application!" << endl;
        }

        else {
            cout << "\nInvalid choice!" << endl;
        }

    } while (mainChoice != 6);

    return 0;
}
