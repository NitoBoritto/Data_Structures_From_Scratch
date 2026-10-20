#include <iostream>
#include <cstdlib>
using namespace std;

class Open_Addressing_Hash_Table_Number {
private:

    // Every slot is in one of three states
    // DELETED is a tombstone: the slot is free again, but a search must not stop there
    // (stopping would hide keys that were pushed further by a collision)
    enum { EMPTY = 0, USED = 1, DELETED = 2 };

    int* keys;          // Key of every slot
    int* values;        // Value of every slot
    int* states;        // EMPTY, USED or DELETED for every slot
    int size;           // Number of slots
    int length;         // Number of entries currently stored
    int deleted;        // Number of tombstones

private:

    // Turn a key into the slot where the search starts
    int hash(int key, int count)
    {
        int index = key % count;

        // In C++ the remainder of a negative number is negative
        if (index < 0)
        {
            index += count;
        }

        return index;
    }


    // Find the slot that holds a key
    // Returns -1 if the key does not exist
    int find_slot(int key)
    {
        int index = hash(key, size);

        // Linear probing: if a slot is taken, try the next one
        for (int step = 0; step < size; step++)
        {
            // An empty slot ends the search: the key would have been placed here or before
            if (states[index] == EMPTY)
            {
                return -1;
            }

            if (states[index] == USED && keys[index] == key)
            {
                return index;
            }

            index = (index + 1) % size;
        }

        return -1;
    }


    // Create bigger arrays and insert every entry again
    // The tombstones are not copied, so they disappear
    void rehash(int newSize)
    {
        int* newKeys = new int[newSize];
        int* newValues = new int[newSize];
        int* newStates = new int[newSize];

        for (int i = 0; i < newSize; i++)
        {
            newStates[i] = EMPTY;
        }

        for (int i = 0; i < size; i++)
        {
            if (states[i] == USED)
            {
                int index = hash(keys[i], newSize);

                // Move forward until a free slot is found
                while (newStates[index] == USED)
                {
                    index = (index + 1) % newSize;
                }

                newKeys[index] = keys[i];
                newValues[index] = values[i];
                newStates[index] = USED;
            }
        }

        delete[] keys;
        delete[] values;
        delete[] states;

        keys = newKeys;
        values = newValues;
        states = newStates;
        size = newSize;
        deleted = 0;
    }

public:

    // Constructor
    // Creates an empty table with the given number of slots
    Open_Addressing_Hash_Table_Number(int Size)
    {
        // At least one slot, or the hash function would divide by zero
        size = (Size > 0) ? Size : 1;
        length = 0;
        deleted = 0;

        keys = new int[size];
        values = new int[size];
        states = new int[size];

        for (int i = 0; i < size; i++)
        {
            states[i] = EMPTY;
        }
    }


    // Destructor
    // Releases dynamically allocated memory
    ~Open_Addressing_Hash_Table_Number()
    {
        delete[] keys;
        delete[] values;
        delete[] states;
    }


    // Copy Constructor
    // Creates a deep copy of another table
    Open_Addressing_Hash_Table_Number(const Open_Addressing_Hash_Table_Number& other)
    {
        size = other.size;
        length = other.length;
        deleted = other.deleted;

        keys = new int[size];
        values = new int[size];
        states = new int[size];

        for (int i = 0; i < size; i++)
        {
            keys[i] = other.keys[i];
            values[i] = other.values[i];
            states[i] = other.states[i];
        }
    }


    // Assignment Operator
    // Performs deep copy between objects
    Open_Addressing_Hash_Table_Number& operator=(const Open_Addressing_Hash_Table_Number& other)
    {
        // Avoid self-assignment
        if (this == &other)
        {
            return *this;
        }

        // Allocate and copy first, so nothing is lost if allocation fails
        int* newKeys = new int[other.size];
        int* newValues = new int[other.size];
        int* newStates = new int[other.size];

        for (int i = 0; i < other.size; i++)
        {
            newKeys[i] = other.keys[i];
            newValues[i] = other.values[i];
            newStates[i] = other.states[i];
        }

        // Delete old memory
        delete[] keys;
        delete[] values;
        delete[] states;

        keys = newKeys;
        values = newValues;
        states = newStates;
        size = other.size;
        length = other.length;
        deleted = other.deleted;

        return *this;
    }


    // Store a value under a key
    // Returns 1 if the key is new, 0 if the key existed and its value was updated
    int put(int key, int value)
    {
        int found = find_slot(key);

        if (found != -1)
        {
            values[found] = value;

            return 0;
        }

        // Keep the table at most half full (tombstones count too)
        // With free slots everywhere, probing stays short and never loops forever
        if ((length + deleted + 1) * 2 > size)
        {
            int newSize = size;

            while ((length + 1) * 2 > newSize)
            {
                newSize *= 2;
            }

            // Same size is fine too: it only clears the tombstones
            rehash(newSize);
        }

        int index = hash(key, size);

        // The key is new, so the first slot that is not USED can take it
        while (states[index] == USED)
        {
            index = (index + 1) % size;
        }

        // Reusing a tombstone removes it
        if (states[index] == DELETED)
        {
            deleted--;
        }

        keys[index] = key;
        values[index] = value;
        states[index] = USED;

        length++;

        return 1;
    }


    // Find the value stored under a key
    // Returns false if the key does not exist
    bool get(int key, int& value)
    {
        int found = find_slot(key);

        if (found != -1)
        {
            value = values[found];

            return true;
        }

        return false;
    }


    // Check if a key exists
    bool contains(int key)
    {
        return find_slot(key) != -1;
    }


    // Remove a key and its value
    // Returns false if the key does not exist
    bool remove(int key)
    {
        int found = find_slot(key);

        if (found != -1)
        {
            // Leave a tombstone: an EMPTY slot here would break searches
            states[found] = DELETED;

            length--;
            deleted++;

            return true;
        }

        return false;
    }


    // Delete every entry
    // The number of slots stays the same
    int clear()
    {
        for (int i = 0; i < size; i++)
        {
            states[i] = EMPTY;
        }

        length = 0;
        deleted = 0;

        return 1;
    }


    // Return the slot where the search for a key starts
    int index_of(int key)
    {
        return hash(key, size);
    }


    // Return entries / slots
    double load_factor()
    {
        return static_cast<double>(length) / size;
    }


    // Return number of tombstones
    int get_deleted()
    {
        return deleted;
    }


    // Display every slot
    int display()
    {
        for (int i = 0; i < size; i++)
        {
            cout << "[" << i << "] ";

            if (states[i] == USED)
            {
                cout << keys[i] << ":" << values[i] << endl;
            }
            else if (states[i] == DELETED)
            {
                cout << "DELETED" << endl;
            }
            else
            {
                cout << "EMPTY" << endl;
            }
        }

        return 1;
    }


    // Return number of slots
    int get_size()
    {
        return size;
    }


    // Return number of stored entries
    int get_length()
    {
        return length;
    }
};


// ============================================
// Applications
// ============================================

// Count how many times every number appears
// The table maps number -> count, so there is no nested loop
void print_frequencies(int* numbers, int n)
{
    Open_Addressing_Hash_Table_Number counts(n);

    for (int i = 0; i < n; i++)
    {
        int count;

        if (counts.get(numbers[i], count))
        {
            counts.put(numbers[i], count + 1);
        }
        else
        {
            counts.put(numbers[i], 1);
        }
    }

    // Print in input order, and remove a number after printing it
    // so a repeated number is not printed twice
    for (int i = 0; i < n; i++)
    {
        int count;

        if (counts.get(numbers[i], count))
        {
            cout << numbers[i] << " appears " << count << " time(s)" << endl;

            counts.remove(numbers[i]);
        }
    }
}


// Find two numbers whose sum is the target
// The table remembers every number we passed: number -> index
// For each number we ask "did I already see target - number?" in O(1)
bool find_two_sum(int* numbers, int n, int target, int& first, int& second)
{
    Open_Addressing_Hash_Table_Number seen(n);

    for (int i = 0; i < n; i++)
    {
        int index;

        if (seen.get(target - numbers[i], index))
        {
            first = index;
            second = i;

            return true;
        }

        seen.put(numbers[i], i);
    }

    return false;
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
    cout << "  Welcome To Open Addressing Hash Table Application" << endl;
    cout << "============================================" << endl;


    // ============================================
    // Create Hash Table
    // ============================================

    int size;

    do {
        cout << "\nEnter Starting Number Of Slots: ";
        read_input(size);

        if (size < 1) {
            cout << "It must be at least 1!" << endl;
        }
    } while (size < 1);

    Open_Addressing_Hash_Table_Number table(size);


    // ============================================
    // Main Menu
    // ============================================

    int mainChoice;

    do {

        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;

        cout << "1. Basic Operations" << endl;
        cout << "2. Table Information" << endl;
        cout << "3. Applications" << endl;
        cout << "4. Exit" << endl;

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

                cout << "1. Put Key And Value" << endl;
                cout << "2. Get Value By Key" << endl;
                cout << "3. Remove Key" << endl;
                cout << "4. Clear Table" << endl;
                cout << "5. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Put Key And Value
                if (choice == 1) {

                    int key;
                    int value;

                    cout << "\nEnter key: ";
                    read_input(key);

                    cout << "Enter value: ";
                    read_input(value);

                    if (table.put(key, value)) {
                        cout << "New key added." << endl;
                    }
                    else {
                        cout << "Key already existed, value updated." << endl;
                    }

                    cout << "Table: ";
                    table.display();
                }


                // Get Value By Key
                else if (choice == 2) {

                    int key;
                    int value;

                    cout << "\nEnter key: ";
                    read_input(key);

                    if (table.get(key, value)) {
                        cout << "Value of key " << key << " = " << value << endl;
                    }
                    else {
                        cout << "Key not found." << endl;
                    }
                }


                // Remove Key
                else if (choice == 3) {

                    int key;

                    cout << "\nEnter key: ";
                    read_input(key);

                    if (table.remove(key)) {
                        cout << "Key removed successfully." << endl;
                    }
                    else {
                        cout << "Key not found." << endl;
                    }

                    cout << "Table: ";
                    table.display();
                }


                // Clear Table
                else if (choice == 4) {

                    table.clear();

                    cout << "\nTable cleared successfully." << endl;

                    cout << "Table: ";
                    table.display();
                }


                else if (choice != 5) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 5);
        }


        // ==================================================
        // 2. TABLE INFORMATION
        // ==================================================

        else if (mainChoice == 2) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "             TABLE INFORMATION" << endl;
                cout << "============================================" << endl;

                cout << "1. Display Table" << endl;
                cout << "2. Check If Key Exists" << endl;
                cout << "3. Which Slot Is A Key In" << endl;
                cout << "4. Get Length" << endl;
                cout << "5. Get Number Of Slots" << endl;
                cout << "6. Get Load Factor" << endl;
                cout << "7. Get Deleted Slots" << endl;
                cout << "8. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Display Table
                if (choice == 1) {

                    cout << "\n";
                    table.display();
                }


                // Check If Key Exists
                else if (choice == 2) {

                    int key;

                    cout << "\nEnter key: ";
                    read_input(key);

                    if (table.contains(key)) {
                        cout << "Key exists." << endl;
                    }
                    else {
                        cout << "Key does NOT exist." << endl;
                    }
                }


                // Which Slot Is A Key In
                else if (choice == 3) {

                    int key;

                    cout << "\nEnter key: ";
                    read_input(key);

                    cout << "Key " << key << " starts at Slot " << table.index_of(key) << endl;
                }


                // Get Length
                else if (choice == 4) {

                    cout << "\nTable Length = " << table.get_length() << endl;
                }


                // Get Number Of Slots
                else if (choice == 5) {

                    cout << "\nNumber Of Slots = " << table.get_size() << endl;
                }


                // Get Load Factor
                else if (choice == 6) {

                    cout << "\nLoad Factor = " << table.load_factor() << endl;
                }


                // Get Deleted Slots
                else if (choice == 7) {

                    cout << "\nDeleted Slots (tombstones) = " << table.get_deleted() << endl;
                }


                else if (choice != 8) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 8);
        }


        // ==================================================
        // 3. APPLICATIONS
        // ==================================================

        else if (mainChoice == 3) {

            int choice;

            do {

                cout << "\n\n============================================" << endl;
                cout << "               APPLICATIONS" << endl;
                cout << "============================================" << endl;

                cout << "1. Count Number Frequencies" << endl;
                cout << "2. Two Sum" << endl;
                cout << "3. Back To Main Menu" << endl;

                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);


                // Count Number Frequencies
                if (choice == 1) {

                    int n;

                    cout << "\nHow many numbers (1 to 1000): ";
                    read_input(n);

                    if (n < 1 || n > 1000) {
                        cout << "Count must be between 1 and 1000!" << endl;
                    }
                    else {
                        int* numbers = new int[n];

                        for (int i = 0; i < n; i++) {
                            cout << "Enter number " << i + 1 << ": ";
                            read_input(numbers[i]);
                        }

                        cout << endl;
                        print_frequencies(numbers, n);

                        delete[] numbers;
                    }
                }


                // Two Sum
                else if (choice == 2) {

                    int n;

                    cout << "\nHow many numbers (1 to 1000): ";
                    read_input(n);

                    if (n < 1 || n > 1000) {
                        cout << "Count must be between 1 and 1000!" << endl;
                    }
                    else {
                        int* numbers = new int[n];
                        int target;

                        for (int i = 0; i < n; i++) {
                            cout << "Enter number " << i + 1 << ": ";
                            read_input(numbers[i]);
                        }

                        cout << "Enter target sum: ";
                        read_input(target);

                        int first;
                        int second;

                        if (find_two_sum(numbers, n, target, first, second)) {
                            cout << "Found: " << numbers[first] << " (index " << first << ") + "
                                 << numbers[second] << " (index " << second << ") = " << target << endl;
                        }
                        else {
                            cout << "No two numbers add up to " << target << "." << endl;
                        }

                        delete[] numbers;
                    }
                }


                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }

            } while (choice != 3);
        }


        // ==================================================
        // 4. EXIT
        // ==================================================

        else if (mainChoice == 4) {
            cout << "\nThank you for using the Hash Table Application!" << endl;
        }

        else {
            cout << "\nInvalid choice!" << endl;
        }

    } while (mainChoice != 4);

    return 0;
}
