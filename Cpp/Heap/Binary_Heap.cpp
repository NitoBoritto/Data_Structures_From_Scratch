#include <iostream>
#include <cstdlib>
using namespace std;

class Binary_Heap_Number {
private:

    int* Array;
    int size;
    int length;

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


    void swap_values(int first, int second)
    {
        int temp = Array[first];
        Array[first] = Array[second];
        Array[second] = temp;
    }


    // Move a value up until the max-heap order is restored
    void sift_up(int index)
    {
        while (index > 0 && Array[parent_index(index)] < Array[index])
        {
            swap_values(index, parent_index(index));
            index = parent_index(index);
        }
    }


    // Move a value down until the max-heap order is restored
    void sift_down(int index)
    {
        while (true)
        {
            int largest = index;
            int left = left_child_index(index);
            int right = right_child_index(index);

            if (left < length && Array[left] > Array[largest])
            {
                largest = left;
            }
            if (right < length && Array[right] > Array[largest])
            {
                largest = right;
            }

            if (largest == index)
            {
                return;
            }

            swap_values(index, largest);
            index = largest;
        }
    }


    // Allocate a bigger array and move the old elements into it
    void resize(int newSize)
    {
        if (newSize <= size)
        {
            return;
        }

        int* newArray = new int[newSize];

        for (int i = 0; i < length; i++)
        {
            newArray[i] = Array[i];
        }

        delete[] Array;
        Array = newArray;
        size = newSize;
    }


    // Make sure there is enough space for the requested number of elements
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


    void heapify_down(int* array, int arrayLength, int index)
    {
        while (true)
        {
            int largest = index;
            int left = (2 * index) + 1;
            int right = (2 * index) + 2;

            if (left < arrayLength && array[left] > array[largest])
            {
                largest = left;
            }
            if (right < arrayLength && array[right] > array[largest])
            {
                largest = right;
            }

            if (largest == index)
            {
                return;
            }

            int temp = array[index];
            array[index] = array[largest];
            array[largest] = temp;
            index = largest;
        }
    }

public:

    // Constructor
    // Creates an empty max heap with the given starting capacity
    Binary_Heap_Number(int Size = 10)
    {
        size = (Size > 0) ? Size : 1;
        length = 0;
        Array = new int[size];
    }


    // Destructor
    // Releases dynamically allocated memory
    ~Binary_Heap_Number()
    {
        delete[] Array;
    }


    // Copy Constructor
    // Creates a deep copy of another heap
    Binary_Heap_Number(const Binary_Heap_Number& other)
    {
        size = other.size;
        length = other.length;
        Array = new int[size];

        for (int i = 0; i < length; i++)
        {
            Array[i] = other.Array[i];
        }
    }


    // Assignment Operator
    // Performs deep copy between heaps
    Binary_Heap_Number& operator=(const Binary_Heap_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        int* newArray = new int[other.size];

        for (int i = 0; i < other.length; i++)
        {
            newArray[i] = other.Array[i];
        }

        delete[] Array;
        Array = newArray;
        size = other.size;
        length = other.length;

        return *this;
    }


    // Add value and restore max-heap order
    int insert(int value)
    {
        ensure_capacity(length + 1);
        Array[length] = value;
        sift_up(length);
        length++;
        return 1;
    }


    // Remove the maximum value and return it through value
    // Returns false if the heap is empty
    bool extract_max(int& value)
    {
        if (length == 0)
        {
            return false;
        }

        value = Array[0];
        length--;

        if (length > 0)
        {
            Array[0] = Array[length];
            sift_down(0);
        }

        return true;
    }


    // Read the maximum value without removing it
    // Returns false if the heap is empty
    bool peek(int& value)
    {
        if (length == 0)
        {
            return false;
        }

        value = Array[0];
        return true;
    }


    // Search for a value and return its array index, or -1 if missing
    int search(int key)
    {
        for (int i = 0; i < length; i++)
        {
            if (Array[i] == key)
            {
                return i;
            }
        }

        return -1;
    }


    // Delete a value at a valid array index
    bool delete_index(int index)
    {
        if (index < 0 || index >= length)
        {
            return false;
        }

        length--;

        if (index == length)
        {
            return true;
        }

        Array[index] = Array[length];

        if (index > 0 && Array[index] > Array[parent_index(index)])
        {
            sift_up(index);
        }
        else
        {
            sift_down(index);
        }

        return true;
    }


    // Build a max heap from the given array
    // Existing values are replaced; duplicate values are allowed
    bool build_heap(int* values, int count)
    {
        if (count < 0 || (count > 0 && values == nullptr))
        {
            return false;
        }

        ensure_capacity((count > 0) ? count : 1);

        for (int i = 0; i < count; i++)
        {
            Array[i] = values[i];
        }
        length = count;

        for (int i = (length / 2) - 1; i >= 0; i--)
        {
            sift_down(i);
        }

        return true;
    }


    // Return values in ascending order without changing the heap
    int heap_sort(int* sortedArray, int arraySize)
    {
        if (arraySize < length || (length > 0 && sortedArray == nullptr))
        {
            return 0;
        }

        int* copy = new int[length];
        for (int i = 0; i < length; i++)
        {
            copy[i] = Array[i];
        }

        int copyLength = length;
        for (int i = length - 1; i >= 0; i--)
        {
            sortedArray[i] = copy[0];
            copyLength--;

            if (copyLength > 0)
            {
                copy[0] = copy[copyLength];
                heapify_down(copy, copyLength, 0);
            }
        }

        delete[] copy;
        return 1;
    }


    // Print heap array representation
    int display()
    {
        if (length == 0)
        {
            cout << "Heap is empty." << endl;
            return 0;
        }

        for (int i = 0; i < length; i++)
        {
            cout << Array[i] << " ";
        }
        cout << endl;
        return 1;
    }


    // Remove all values; the capacity stays the same
    int clear()
    {
        length = 0;
        return 1;
    }


    // Check if the heap has no values
    bool is_empty()
    {
        return length == 0;
    }


    // Return number of stored values
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
    cout << "      Welcome To Binary Heap Application" << endl;
    cout << "============================================" << endl;

    int size;
    do {
        cout << "\nEnter Starting Capacity: ";
        read_input(size);
        if (size < 0) {
            cout << "Capacity cannot be negative!" << endl;
        }
    } while (size < 0);

    Binary_Heap_Number heap(size);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Search Operations" << endl;
        cout << "3. Heap Operations" << endl;
        cout << "4. Heap Information" << endl;
        cout << "5. Applications" << endl;
        cout << "6. Exit" << endl;
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
                cout << "2. Extract Maximum" << endl;
                cout << "3. Peek Maximum" << endl;
                cout << "4. Delete By Index" << endl;
                cout << "5. Clear Heap" << endl;
                cout << "6. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    int value;
                    cout << "\nEnter number: ";
                    read_input(value);
                    heap.insert(value);
                    cout << "Number added successfully." << endl;
                    cout << "Heap: ";
                    heap.display();
                }
                else if (choice == 2) {
                    int value;
                    if (heap.extract_max(value)) {
                        cout << "Maximum number removed: " << value << endl;
                        cout << "Heap: ";
                        heap.display();
                    }
                    else {
                        cout << "Heap is empty." << endl;
                    }
                }
                else if (choice == 3) {
                    int value;
                    if (heap.peek(value)) {
                        cout << "Maximum number: " << value << endl;
                    }
                    else {
                        cout << "Heap is empty." << endl;
                    }
                }
                else if (choice == 4) {
                    int index;
                    cout << "\nEnter array index: ";
                    read_input(index);
                    if (heap.delete_index(index)) {
                        cout << "Value deleted successfully." << endl;
                        cout << "Heap: ";
                        heap.display();
                    }
                    else {
                        cout << "Invalid index!" << endl;
                    }
                }
                else if (choice == 5) {
                    heap.clear();
                    cout << "Heap cleared successfully." << endl;
                }
                else if (choice != 6) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 6);
        }
        else if (mainChoice == 2) {
            int value;
            cout << "\nEnter number to search: ";
            read_input(value);
            int index = heap.search(value);
            if (index != -1) {
                cout << "Number found at array index " << index << endl;
            }
            else {
                cout << "Number not found." << endl;
            }
        }
        else if (mainChoice == 3) {
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "                HEAP OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Build Heap From Numbers" << endl;
                cout << "2. Display Heap Array" << endl;
                cout << "3. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    int count;
                    cout << "\nEnter number of values: ";
                    read_input(count);
                    if (count < 0) {
                        cout << "Number of values cannot be negative!" << endl;
                    }
                    else {
                        int* values = (count > 0) ? new int[count] : nullptr;
                        for (int i = 0; i < count; i++) {
                            cout << "Enter value " << (i + 1) << ": ";
                            read_input(values[i]);
                        }
                        heap.build_heap(values, count);
                        delete[] values;
                        cout << "Heap built successfully." << endl;
                        cout << "Heap: ";
                        heap.display();
                    }
                }
                else if (choice == 2) {
                    cout << "Heap: ";
                    heap.display();
                }
                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 3);
        }
        else if (mainChoice == 4) {
            int maximum;
            cout << "\nHeap: ";
            heap.display();
            cout << "Values: " << heap.get_length() << endl;
            cout << "Capacity: " << heap.get_capacity() << endl;
            cout << "Empty: " << (heap.is_empty() ? "Yes" : "No") << endl;
            if (heap.peek(maximum)) {
                cout << "Maximum: " << maximum << endl;
            }
        }
        else if (mainChoice == 5) {
            int* sorted = new int[heap.get_length()];
            if (heap.heap_sort(sorted, heap.get_length())) {
                cout << "\nValues in ascending order: ";
                for (int i = 0; i < heap.get_length(); i++) {
                    cout << sorted[i] << " ";
                }
                cout << endl;
                cout << "Heap remains unchanged: ";
                heap.display();
            }
            else {
                cout << "Unable to sort heap." << endl;
            }
            delete[] sorted;
        }
        else if (mainChoice != 6) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 6);

    cout << "\nThank you for using Binary Heap Application." << endl;
    return 0;
}
