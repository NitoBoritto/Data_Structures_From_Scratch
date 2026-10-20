#include <iostream>
#include <cstdlib>
using namespace std;

struct Priority_Queue_Entry {
    int value;
    int priority;
    unsigned long long order;

    Priority_Queue_Entry()
    {
        value = 0;
        priority = 0;
        order = 0;
    }

    Priority_Queue_Entry(int Value, int Priority, unsigned long long Order)
    {
        value = Value;
        priority = Priority;
        order = Order;
    }
};


class Priority_Queue_Number {
private:

    Priority_Queue_Entry* Array;
    int size;
    int length;
    unsigned long long nextOrder;

private:

    int parent_index(int index)
    {
        return (index - 1) / 2;
    }


    int left_child_index(int index)
    {
        return (2 * index) + 1;
    }


    int right_child_index(int index)
    {
        return (2 * index) + 2;
    }


    // Higher priority comes first; equal priorities keep insertion order
    bool comes_before(const Priority_Queue_Entry& first, const Priority_Queue_Entry& second)
    {
        if (first.priority != second.priority)
        {
            return first.priority > second.priority;
        }

        return first.order < second.order;
    }


    void swap_entries(int first, int second)
    {
        Priority_Queue_Entry temp = Array[first];
        Array[first] = Array[second];
        Array[second] = temp;
    }


    // Move an entry up until priority order is restored
    void sift_up(int index)
    {
        while (index > 0 && comes_before(Array[index], Array[parent_index(index)]))
        {
            swap_entries(index, parent_index(index));
            index = parent_index(index);
        }
    }


    // Move an entry down until priority order is restored
    void sift_down(int index)
    {
        while (true)
        {
            int first = index;
            int left = left_child_index(index);
            int right = right_child_index(index);

            if (left < length && comes_before(Array[left], Array[first]))
            {
                first = left;
            }
            if (right < length && comes_before(Array[right], Array[first]))
            {
                first = right;
            }

            if (first == index)
            {
                return;
            }

            swap_entries(index, first);
            index = first;
        }
    }


    // Restore heap order after an entry changes or is replaced
    void restore_heap(int index)
    {
        if (index > 0 && comes_before(Array[index], Array[parent_index(index)]))
        {
            sift_up(index);
        }
        else
        {
            sift_down(index);
        }
    }


    // Allocate a bigger array and move the old entries into it
    void resize(int newSize)
    {
        if (newSize <= size)
        {
            return;
        }

        Priority_Queue_Entry* newArray = new Priority_Queue_Entry[newSize];
        for (int i = 0; i < length; i++)
        {
            newArray[i] = Array[i];
        }

        delete[] Array;
        Array = newArray;
        size = newSize;
    }


    // Make sure there is enough space for the requested number of entries
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


    // Remove an entry at a valid array index
    void remove_index(int index)
    {
        length--;
        if (index == length)
        {
            return;
        }

        Array[index] = Array[length];
        restore_heap(index);
    }

public:

    // Constructor
    // Creates an empty priority queue with the given starting capacity
    Priority_Queue_Number(int Size = 10)
    {
        size = (Size > 0) ? Size : 1;
        length = 0;
        nextOrder = 0;
        Array = new Priority_Queue_Entry[size];
    }


    // Destructor
    // Releases dynamically allocated memory
    ~Priority_Queue_Number()
    {
        delete[] Array;
    }


    // Copy Constructor
    // Creates a deep copy of another priority queue
    Priority_Queue_Number(const Priority_Queue_Number& other)
    {
        size = other.size;
        length = other.length;
        nextOrder = other.nextOrder;
        Array = new Priority_Queue_Entry[size];

        for (int i = 0; i < length; i++)
        {
            Array[i] = other.Array[i];
        }
    }


    // Assignment Operator
    // Performs deep copy between priority queues
    Priority_Queue_Number& operator=(const Priority_Queue_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        Priority_Queue_Entry* newArray = new Priority_Queue_Entry[other.size];
        for (int i = 0; i < other.length; i++)
        {
            newArray[i] = other.Array[i];
        }

        delete[] Array;
        Array = newArray;
        size = other.size;
        length = other.length;
        nextOrder = other.nextOrder;
        return *this;
    }


    // Add an item with its priority
    // Larger priority numbers are served first
    int enqueue(int value, int priority)
    {
        ensure_capacity(length + 1);
        Array[length] = Priority_Queue_Entry(value, priority, nextOrder);
        nextOrder++;
        sift_up(length);
        length++;
        return 1;
    }


    // Remove the highest-priority item and return its value and priority
    // Returns false if the queue is empty
    bool dequeue(int& value, int& priority)
    {
        if (length == 0)
        {
            return false;
        }

        value = Array[0].value;
        priority = Array[0].priority;
        remove_index(0);
        return true;
    }


    // Read the next item without removing it
    // Returns false if the queue is empty
    bool peek(int& value, int& priority)
    {
        if (length == 0)
        {
            return false;
        }

        value = Array[0].value;
        priority = Array[0].priority;
        return true;
    }


    // Search for an item by value; return its heap index, or -1 if missing
    int search(int value)
    {
        for (int i = 0; i < length; i++)
        {
            if (Array[i].value == value)
            {
                return i;
            }
        }

        return -1;
    }


    // Update the priority of the earliest enqueued matching item
    bool update_priority(int value, int newPriority)
    {
        int foundIndex = -1;

        for (int i = 0; i < length; i++)
        {
            if (Array[i].value == value
                && (foundIndex == -1 || Array[i].order < Array[foundIndex].order))
            {
                foundIndex = i;
            }
        }

        if (foundIndex == -1)
        {
            return false;
        }

        Array[foundIndex].priority = newPriority;
        restore_heap(foundIndex);
        return true;
    }


    // Remove the highest-priority matching item
    bool remove_value(int value)
    {
        int foundIndex = -1;

        for (int i = 0; i < length; i++)
        {
            if (Array[i].value == value
                && (foundIndex == -1 || comes_before(Array[i], Array[foundIndex])))
            {
                foundIndex = i;
            }
        }

        if (foundIndex == -1)
        {
            return false;
        }

        remove_index(foundIndex);
        return true;
    }


    // Print entries in heap array order (not dequeue order)
    int display()
    {
        if (length == 0)
        {
            cout << "Priority queue is empty." << endl;
            return 0;
        }

        for (int i = 0; i < length; i++)
        {
            cout << "(Value: " << Array[i].value
                 << ", Priority: " << Array[i].priority << ") ";
        }
        cout << endl;
        return 1;
    }


    // Remove all entries; the capacity stays the same
    int clear()
    {
        length = 0;
        nextOrder = 0;
        return 1;
    }


    // Check if the priority queue has no entries
    bool is_empty()
    {
        return length == 0;
    }


    // Return number of stored entries
    int get_length()
    {
        return length;
    }


    // Return total capacity
    int get_capacity()
    {
        return size;
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
    cout << "   Welcome To Priority Queue Application" << endl;
    cout << "============================================" << endl;

    int size;
    do {
        cout << "\nEnter Starting Capacity: ";
        read_input(size);
        if (size < 0) {
            cout << "Capacity cannot be negative!" << endl;
        }
    } while (size < 0);

    Priority_Queue_Number queue(size);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Search Operations" << endl;
        cout << "3. Queue Operations" << endl;
        cout << "4. Queue Information" << endl;
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
                cout << "1. Enqueue Item" << endl;
                cout << "2. Dequeue Highest Priority" << endl;
                cout << "3. Peek Next Item" << endl;
                cout << "4. Update Item Priority" << endl;
                cout << "5. Remove Item By Value" << endl;
                cout << "6. Clear Queue" << endl;
                cout << "7. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    int value;
                    int priority;
                    cout << "\nEnter item value: ";
                    read_input(value);
                    cout << "Enter priority (larger number = higher priority): ";
                    read_input(priority);
                    queue.enqueue(value, priority);
                    cout << "Item added successfully." << endl;
                }
                else if (choice == 2) {
                    int value;
                    int priority;
                    if (queue.dequeue(value, priority)) {
                        cout << "Item removed: " << value << " (Priority: " << priority << ")" << endl;
                    }
                    else {
                        cout << "Priority queue is empty." << endl;
                    }
                }
                else if (choice == 3) {
                    int value;
                    int priority;
                    if (queue.peek(value, priority)) {
                        cout << "Next item: " << value << " (Priority: " << priority << ")" << endl;
                    }
                    else {
                        cout << "Priority queue is empty." << endl;
                    }
                }
                else if (choice == 4) {
                    int value;
                    int priority;
                    cout << "\nEnter item value: ";
                    read_input(value);
                    cout << "Enter new priority: ";
                    read_input(priority);
                    if (queue.update_priority(value, priority)) {
                        cout << "Priority updated successfully." << endl;
                    }
                    else {
                        cout << "Item not found." << endl;
                    }
                }
                else if (choice == 5) {
                    int value;
                    cout << "\nEnter item value to remove: ";
                    read_input(value);
                    if (queue.remove_value(value)) {
                        cout << "Item removed successfully." << endl;
                    }
                    else {
                        cout << "Item not found." << endl;
                    }
                }
                else if (choice == 6) {
                    queue.clear();
                    cout << "Priority queue cleared successfully." << endl;
                }
                else if (choice != 7) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 7);
        }
        else if (mainChoice == 2) {
            int value;
            cout << "\nEnter item value to search: ";
            read_input(value);
            int index = queue.search(value);
            if (index != -1) {
                cout << "Item found at heap index " << index << endl;
            }
            else {
                cout << "Item not found." << endl;
            }
        }
        else if (mainChoice == 3) {
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "               QUEUE OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Display Heap Array" << endl;
                cout << "2. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    cout << "Priority queue (heap array): ";
                    queue.display();
                }
                else if (choice != 2) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 2);
        }
        else if (mainChoice == 4) {
            int value;
            int priority;
            cout << "\nItems: " << queue.get_length() << endl;
            cout << "Capacity: " << queue.get_capacity() << endl;
            cout << "Empty: " << (queue.is_empty() ? "Yes" : "No") << endl;
            if (queue.peek(value, priority)) {
                cout << "Next item: " << value << " (Priority: " << priority << ")" << endl;
            }
        }
        else if (mainChoice != 5) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 5);

    cout << "\nThank you for using Priority Queue Application." << endl;
    return 0;
}
