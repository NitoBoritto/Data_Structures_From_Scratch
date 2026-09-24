#include <iostream>
using namespace std;

class Static_Array_Number
{
private:

    int* Array;     // Pointer to dynamically allocated array
    int size;       // Total capacity of the array
    int length;     // Number of elements currently stored

public:

    // Constructor
    // Creates an array with the given capacity
    // and copies the first Length elements from arr
    Static_Array_Number(int Size, int Length, int* arr)
    {
        size = Size;

        // Length cannot be greater than Size
        length = (Length <= Size) ? Length : Size;

        // Allocate memory dynamically
        Array = new int[size];

        // Copy initial elements
        for (int i = 0; i < length; i++)
        {
            Array[i] = arr[i];
        }
    }


    // Destructor
    // Releases dynamically allocated memory
    ~Static_Array_Number()
    {
        delete[] Array;
    }


    // Copy Constructor
    // Creates a deep copy of another object
    Static_Array_Number(const Static_Array_Number& other)
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
    Static_Array_Number& operator=(const Static_Array_Number& other)
    {
        // Avoid self-assignment
        if (this == &other)
        {
            return *this;
        }

        // Delete old memory
        delete[] Array;

        // Copy size and length
        size = other.size;
        length = other.length;

        // Allocate new memory
        Array = new int[size];

        // Copy elements
        for (int i = 0; i < length; i++)
        {
            Array[i] = other.Array[i];
        }

        return *this;
    }


    // Insert value at a specific index
    int insert(int index, int value)
    {
        // Check valid index and available space
        if (index >= 0 && index <= length && length < size)
        {
            // Shift elements to the right
            for (int i = length; i > index; i--)
            {
                Array[i] = Array[i - 1];
            }

            // Insert value
            Array[index] = value;

            // Increase number of elements
            length++;

            return 1;
        }

        return 0;
    }


    // Add value at the end
    int append(int value)
    {
        // Check if there is free space
        if (length < size)
        {
            Array[length] = value;
            length++;

            return 1;
        }

        return 0;
    }


    // Insert value while keeping array sorted
    // The array must already be sorted
    int insert_sorted(int value)
    {
        if (length < size)
        {
            int i = length - 1;

            // Shift larger elements to the right
            while (i >= 0 && Array[i] > value)
            {
                Array[i + 1] = Array[i];
                i--;
            }

            // Insert value
            Array[i + 1] = value;

            length++;

            return 1;
        }

        return 0;
    }


    // Delete element by index
    // Returns deleted value
    int delete_index(int index)
    {
        if (index >= 0 && index < length)
        {
            // Save deleted value
            int x = Array[index];

            // Shift elements to the left
            for (int i = index; i < length - 1; i++)
            {
                Array[i] = Array[i + 1];
            }

            // Decrease length
            length--;

            return x;
        }

        return -1;
    }


    // Linear Search
    // Returns index if found, otherwise -1
    int linear_search(int key)
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


    // Binary Search
    // Array must be sorted
    int binary_search(int key)
    {
        int low = 0;
        int high = length - 1;

        while (low <= high)
        {
            // Avoid potential overflow
            int mid = low + (high - low) / 2;

            if (Array[mid] == key)
            {
                return mid;
            }
            else if (Array[mid] < key)
            {
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }
        }

        return -1;
    }


    // Get element by index
    // Returns false if index is invalid
    bool get(int index, int& value)
    {
        if (index >= 0 && index < length)
        {
            value = Array[index];
            return true;
        }

        return false;
    }


    // Change element at specific index
    int set(int index, int value)
    {
        if (index >= 0 && index < length)
        {
            Array[index] = value;

            return 1;
        }

        return 0;
    }


    // Return maximum value
    int max()
    {
        if (length == 0)
        {
            return -1;
        }

        int max_value = Array[0];

        for (int i = 1; i < length; i++)
        {
            if (Array[i] > max_value)
            {
                max_value = Array[i];
            }
        }

        return max_value;
    }


    // Return minimum value
    int min()
    {
        if (length == 0)
        {
            return -1;
        }

        int min_value = Array[0];

        for (int i = 1; i < length; i++)
        {
            if (Array[i] < min_value)
            {
                min_value = Array[i];
            }
        }

        return min_value;
    }


    // Calculate sum of elements
    int sum()
    {
        int total = 0;

        for (int i = 0; i < length; i++)
        {
            total += Array[i];
        }

        return total;
    }


    // Calculate average
    double average()
    {
        if (length == 0)
        {
            return 0.0;
        }

        // Convert to double to avoid integer division
        return static_cast<double>(sum()) / length;
    }


    // Reverse array
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


    // Check whether array is sorted
    // Returns 1 if sorted, 0 otherwise
    int is_sorted()
    {
        for (int i = 0; i < length - 1; i++)
        {
            if (Array[i] > Array[i + 1])
            {
                return 0;
            }
        }

        return 1;
    }


    // Rearrange negative and non-negative values
    // Negative values move toward the beginning
    int rearrange()
    {
        int i = 0;
        int j = length - 1;

        while (i < j)
        {
            // Find first non-negative value
            while (i < length && Array[i] < 0)
            {
                i++;
            }

            // Find last negative value
            while (j >= 0 && Array[j] >= 0)
            {
                j--;
            }

            // Swap if positions are valid
            if (i < j)
            {
                swap(Array[i], Array[j]);
            }
        }

        return 1;
    }


    // Merge two sorted arrays
    int merge(Static_Array_Number& arr2)
    {
        // Create enough space for both arrays
        int* mergedArray = new int[size + arr2.size];

        int i = 0;
        int j = 0;
        int k = 0;

        // Merge while both arrays have elements
        while (i < length && j < arr2.length)
        {
            if (Array[i] < arr2.Array[j])
            {
                mergedArray[k++] = Array[i++];
            }
            else
            {
                mergedArray[k++] = arr2.Array[j++];
            }
        }

        // Copy remaining elements from first array
        while (i < length)
        {
            mergedArray[k++] = Array[i++];
        }

        // Copy remaining elements from second array
        while (j < arr2.length)
        {
            mergedArray[k++] = arr2.Array[j++];
        }

        // Delete old array
        delete[] Array;

        // Replace with merged array
        Array = mergedArray;

        // Update capacity and length
        size += arr2.size;
        length = k;

        return 1;
    }


    // Union of two sorted arrays
    // Duplicate values are stored once
    int union_array(Static_Array_Number& arr2)
    {
        int* unionArray = new int[size + arr2.size];

        int i = 0;
        int j = 0;
        int k = 0;

        // Compare elements from both arrays
        while (i < length && j < arr2.length)
        {
            if (Array[i] < arr2.Array[j])
            {
                unionArray[k++] = Array[i++];
            }
            else if (Array[i] > arr2.Array[j])
            {
                unionArray[k++] = arr2.Array[j++];
            }
            else
            {
                // Equal values → store once
                unionArray[k++] = Array[i++];

                j++;
            }
        }

        // Copy remaining elements
        while (i < length)
        {
            unionArray[k++] = Array[i++];
        }

        while (j < arr2.length)
        {
            unionArray[k++] = arr2.Array[j++];
        }

        // Replace old array
        delete[] Array;

        Array = unionArray;

        // Update capacity and length
        size += arr2.size;
        length = k;

        return 1;
    }


    // Intersection of two sorted arrays
    // Keeps values existing in both arrays
    int intersection(Static_Array_Number& arr2)
    {
        // Maximum possible intersection size
        int newSize = std::min(size, arr2.size);

        int* intersectionArray = new int[newSize];

        int i = 0;
        int j = 0;
        int k = 0;

        // Find common elements
        while (i < length && j < arr2.length)
        {
            if (Array[i] < arr2.Array[j])
            {
                i++;
            }
            else if (Array[i] > arr2.Array[j])
            {
                j++;
            }
            else
            {
                intersectionArray[k++] = Array[i++];

                j++;
            }
        }

        // Replace old array
        delete[] Array;

        Array = intersectionArray;

        // Update capacity and length
        size = newSize;
        length = k;

        return 1;
    }


    // Difference: elements in current array
    // that are not present in arr2
    int difference(Static_Array_Number& arr2)
    {
        int* differenceArray = new int[size];

        int i = 0;
        int j = 0;
        int k = 0;

        // Compare both sorted arrays
        while (i < length && j < arr2.length)
        {
            if (Array[i] < arr2.Array[j])
            {
                differenceArray[k++] = Array[i++];
            }
            else if (Array[i] > arr2.Array[j])
            {
                j++;
            }
            else
            {
                // Same element exists in both arrays
                i++;
                j++;
            }
        }

        // Copy remaining elements
        while (i < length)
        {
            differenceArray[k++] = Array[i++];
        }

        // Replace old array
        delete[] Array;

        Array = differenceArray;

        // Capacity stays the same
        length = k;

        return 1;
    }


    // Display all stored elements
    int display()
    {
        for (int i = 0; i < length; i++)
        {
            cout << Array[i] << " ";
        }

        cout << endl;

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


    // Return pointer to array for read-only access
    const int* get_array() const
    {
        return Array;
    }


    // Replace array contents by copying values
    int set_array(int* arr, int newLength)
    {
        // Check if new data fits
        if (newLength > size)
        {
            return 0;
        }

        // Copy elements
        for (int i = 0; i < newLength; i++)
        {
            Array[i] = arr[i];
        }

        // Update length
        length = newLength;

        return 1;
    }
};


int main() {

    // ============================================
    // Welcome
    // ============================================

    cout << "============================================" << endl;
    cout << "     Welcome To Array Application" << endl;
    cout << "============================================" << endl;


    // ============================================
    // Create Array
    // ============================================

    int size;

    cout << "\nEnter Array Capacity: ";
    cin >> size;

    Static_Array_Number arr(size, 0, nullptr);


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
        cout << "4. Array Operations" << endl;
        cout << "5. Set Operations" << endl;
        cout << "6. Array Information" << endl;
        cout << "7. Exit" << endl;

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
                cout << "             BASIC OPERATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Append Number" << endl;
                cout << "2. Insert Number" << endl;
                cout << "3. Insert Sorted" << endl;
                cout << "4. Delete Number" << endl;
                cout << "5. Get Number" << endl;
                cout << "6. Set Number" << endl;
                cout << "7. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                cin >> choice;


                // Append
                if (choice == 1) {

                    int value;

                    cout << "\nEnter number: ";
                    cin >> value;

                    if (arr.append(value)) {
                        cout << "Number added successfully." << endl;
                    }
                    else {
                        cout << "Array is full!" << endl;
                    }
                }


                // Insert
                else if (choice == 2) {

                    int index;
                    int value;

                    cout << "\nEnter index: ";
                    cin >> index;

                    cout << "Enter number: ";
                    cin >> value;

                    if (arr.insert(index, value)) {
                        cout << "Number inserted successfully." << endl;
                    }
                    else {
                        cout << "Invalid index or Array is full!" << endl;
                    }
                }


                // Insert Sorted
                else if (choice == 3) {

                    int value;

                    cout << "\nEnter number: ";
                    cin >> value;

                    if (arr.insert_sorted(value)) {
                        cout << "Number inserted successfully." << endl;
                    }
                    else {
                        cout << "Array is full!" << endl;
                    }
                }


                // Delete
                else if (choice == 4) {

                    int index;

                    cout << "\nEnter index: ";
                    cin >> index;

                    int deletedValue = arr.delete_index(index);

                    if (deletedValue != -1) {
                        cout << "Deleted number: "
                             << deletedValue << endl;
                    }
                    else {
                        cout << "Invalid index!" << endl;
                    }
                }


                // Get
                else if (choice == 5) {

                    int index;
                    int value;

                    cout << "\nEnter index: ";
                    cin >> index;

                    if (arr.get(index, value)) {
                        cout << "Value = " << value << endl;
                    }
                    else {
                        cout << "Invalid index!" << endl;
                    }
                }


                // Set
                else if (choice == 6) {

                    int index;
                    int value;

                    cout << "\nEnter index: ";
                    cin >> index;

                    cout << "Enter new value: ";
                    cin >> value;

                    if (arr.set(index, value)) {
                        cout << "Value updated successfully." << endl;
                    }
                    else {
                        cout << "Invalid index!" << endl;
                    }
                }


                else if (choice == 7) {

                    cout << "\nReturning to Main Menu..." << endl;

                }

                else {

                    cout << "\nInvalid choice!" << endl;

                }

            } while (choice != 7);
        }


        // ==================================================
        // 2. SEARCH OPERATIONS
        // ==================================================

        else if (mainChoice == 2) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "              SEARCH OPERATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Linear Search" << endl;
                cout << "2. Binary Search" << endl;
                cout << "3. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                cin >> choice;


                // Linear Search
                if (choice == 1) {

                    int key;

                    cout << "\nEnter number to search: ";
                    cin >> key;

                    int index = arr.linear_search(key);

                    if (index != -1) {
                        cout << "Number found at index: "
                             << index << endl;
                    }
                    else {
                        cout << "Number not found." << endl;
                    }
                }


                // Binary Search
                else if (choice == 2) {

                    int key;

                    cout << "\nEnter number to search: ";
                    cin >> key;

                    cout << "\nNOTE: Binary Search requires "
                         << "a sorted array." << endl;

                    int index = arr.binary_search(key);

                    if (index != -1) {
                        cout << "Number found at index: "
                             << index << endl;
                    }
                    else {
                        cout << "Number not found." << endl;
                    }
                }


                else if (choice == 3) {

                    cout << "\nReturning to Main Menu..." << endl;

                }

                else {

                    cout << "\nInvalid choice!" << endl;

                }

            } while (choice != 3);
        }


        // ==================================================
        // 3. MATHEMATICAL OPERATIONS
        // ==================================================

        else if (mainChoice == 3) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "           MATHEMATICAL OPERATIONS" << endl;
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

                    if (arr.get_length() == 0) {
                        cout << "\nArray is empty!" << endl;
                    }
                    else {
                        cout << "\nMaximum = "
                             << arr.max() << endl;
                    }
                }


                // Minimum
                else if (choice == 2) {

                    if (arr.get_length() == 0) {
                        cout << "\nArray is empty!" << endl;
                    }
                    else {
                        cout << "\nMinimum = "
                             << arr.min() << endl;
                    }
                }


                // Sum
                else if (choice == 3) {

                    cout << "\nSum = "
                         << arr.sum() << endl;
                }


                // Average
                else if (choice == 4) {

                    if (arr.get_length() == 0) {
                        cout << "\nArray is empty!" << endl;
                    }
                    else {
                        cout << "\nAverage = "
                             << arr.average() << endl;
                    }
                }


                else if (choice == 5) {

                    cout << "\nReturning to Main Menu..." << endl;

                }

                else {

                    cout << "\nInvalid choice!" << endl;

                }

            } while (choice != 5);
        }


        // ==================================================
        // 4. ARRAY OPERATIONS
        // ==================================================

        else if (mainChoice == 4) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "               ARRAY OPERATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Reverse Array" << endl;
                cout << "2. Check If Sorted" << endl;
                cout << "3. Rearrange Negative / Positive" << endl;
                cout << "4. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                cin >> choice;


                // Reverse
                if (choice == 1) {

                    arr.reverse();

                    cout << "\nArray reversed successfully." << endl;

                    cout << "Array: ";
                    arr.display();
                }


                // Is Sorted
                else if (choice == 2) {

                    if (arr.is_sorted()) {
                        cout << "\nArray is sorted." << endl;
                    }
                    else {
                        cout << "\nArray is NOT sorted." << endl;
                    }
                }


                // Rearrange
                else if (choice == 3) {

                    arr.rearrange();

                    cout << "\nArray rearranged successfully." << endl;

                    cout << "Array: ";
                    arr.display();
                }


                else if (choice == 4) {

                    cout << "\nReturning to Main Menu..." << endl;

                }

                else {

                    cout << "\nInvalid choice!" << endl;

                }

            } while (choice != 4);
        }


        // ==================================================
        // 5. SET OPERATIONS
        // ==================================================

        else if (mainChoice == 5) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "                SET OPERATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Merge Arrays" << endl;
                cout << "2. Union Arrays" << endl;
                cout << "3. Intersection Arrays" << endl;
                cout << "4. Difference Arrays" << endl;
                cout << "5. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                cin >> choice;


                // ------------------------------------------
                // Merge
                // ------------------------------------------

                if (choice == 1) {

                    int size2;

                    cout << "\nEnter second array capacity: ";
                    cin >> size2;

                    Static_Array_Number arr2(size2, 0, nullptr);

                    int n;

                    cout << "How many numbers do you want to add? ";
                    cin >> n;

                    for (int i = 0; i < n; i++) {

                        int value;

                        cout << "Enter number " << i + 1 << ": ";
                        cin >> value;

                        arr2.append(value);
                    }

                    arr.merge(arr2);

                    cout << "\nArrays merged successfully." << endl;

                    cout << "Result: ";
                    arr.display();
                }


                // ------------------------------------------
                // Union
                // ------------------------------------------

                else if (choice == 2) {

                    int size2;

                    cout << "\nEnter second array capacity: ";
                    cin >> size2;

                    Static_Array_Number arr2(size2, 0, nullptr);

                    int n;

                    cout << "How many numbers do you want to add? ";
                    cin >> n;

                    for (int i = 0; i < n; i++) {

                        int value;

                        cout << "Enter number " << i + 1 << ": ";
                        cin >> value;

                        arr2.append(value);
                    }

                    arr.union_array(arr2);

                    cout << "\nUnion completed successfully." << endl;

                    cout << "Result: ";
                    arr.display();
                }


                // ------------------------------------------
                // Intersection
                // ------------------------------------------

                else if (choice == 3) {

                    int size2;

                    cout << "\nEnter second array capacity: ";
                    cin >> size2;

                    Static_Array_Number arr2(size2, 0, nullptr);

                    int n;

                    cout << "How many numbers do you want to add? ";
                    cin >> n;

                    for (int i = 0; i < n; i++) {

                        int value;

                        cout << "Enter number " << i + 1 << ": ";
                        cin >> value;

                        arr2.append(value);
                    }

                    arr.intersection(arr2);

                    cout << "\nIntersection completed successfully." << endl;

                    cout << "Result: ";
                    arr.display();
                }


                // ------------------------------------------
                // Difference
                // ------------------------------------------

                else if (choice == 4) {

                    int size2;

                    cout << "\nEnter second array capacity: ";
                    cin >> size2;

                    Static_Array_Number arr2(size2, 0, nullptr);

                    int n;

                    cout << "How many numbers do you want to add? ";
                    cin >> n;

                    for (int i = 0; i < n; i++) {

                        int value;

                        cout << "Enter number " << i + 1 << ": ";
                        cin >> value;

                        arr2.append(value);
                    }

                    arr.difference(arr2);

                    cout << "\nDifference completed successfully." << endl;

                    cout << "Result: ";
                    arr.display();
                }


                else if (choice == 5) {

                    cout << "\nReturning to Main Menu..." << endl;

                }

                else {

                    cout << "\nInvalid choice!" << endl;

                }

            } while (choice != 5);
        }


        // ==================================================
        // 6. ARRAY INFORMATION
        // ==================================================

        else if (mainChoice == 6) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "             ARRAY INFORMATION" << endl;
                cout << "============================================" << endl;

                cout << "1. Display Array" << endl;
                cout << "2. Get Capacity" << endl;
                cout << "3. Get Length" << endl;
                cout << "4. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                cin >> choice;


                // Display
                if (choice == 1) {

                    cout << "\nArray: ";

                    if (arr.get_length() == 0) {
                        cout << "Array is empty!" << endl;
                    }
                    else {
                        arr.display();
                    }
                }


                // Capacity
                else if (choice == 2) {

                    cout << "\nArray Capacity = "
                         << arr.get_size() << endl;
                }


                // Length
                else if (choice == 3) {

                    cout << "\nArray Length = "
                         << arr.get_length() << endl;
                }


                else if (choice == 4) {

                    cout << "\nReturning to Main Menu..." << endl;

                }

                else {

                    cout << "\nInvalid choice!" << endl;

                }

            } while (choice != 4);
        }


        // ==================================================
        // 7. EXIT
        // ==================================================

        else if (mainChoice == 7) {

            cout << "\n============================================" << endl;
            cout << "      Thank You For Using Array App!" << endl;
            cout << "============================================" << endl;
        }


        // ==================================================
        // Invalid Main Choice
        // ==================================================

        else {

            cout << "\nInvalid choice!" << endl;

        }

    } while (mainChoice != 7);


    return 0;
}