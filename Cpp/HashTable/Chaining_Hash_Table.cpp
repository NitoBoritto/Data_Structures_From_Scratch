#include <iostream>
#include <cstdlib>
using namespace std;

struct Entry {
    int key;        // The key used to find this entry
    int value;      // The value stored for this key
    Entry* next;    // Next entry in the same bucket (nullptr = last one)

    Entry(int k, int v)
    {
        key = k;
        value = v;
        next = nullptr;
    }
};


class Chaining_Hash_Table_Number {
private:

    Entry** buckets;    // Array of bucket heads, every bucket is a linked list
    int size;           // Number of buckets
    int length;         // Number of entries currently stored

private:

    // Turn a key into a bucket number between 0 and count - 1
    // The hash function is the whole idea: key -> index, no searching
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


    // Create a bigger bucket array and move every entry into it
    // No entry is copied: only the links change
    // Every key needs a new index because the number of buckets changed
    void rehash(int newSize)
    {
        Entry** newBuckets = new Entry*[newSize];

        for (int i = 0; i < newSize; i++)
        {
            newBuckets[i] = nullptr;
        }

        for (int i = 0; i < size; i++)
        {
            Entry* current = buckets[i];

            while (current != nullptr)
            {
                // Save next before we change the link
                Entry* nextEntry = current->next;

                // Put the entry at the start of its new bucket
                int index = hash(current->key, newSize);

                current->next = newBuckets[index];
                newBuckets[index] = current;

                current = nextEntry;
            }
        }

        // Delete old bucket array (the entries now live in the new one)
        delete[] buckets;

        buckets = newBuckets;
        size = newSize;
    }

public:

    // Constructor
    // Creates an empty table with the given number of buckets
    Chaining_Hash_Table_Number(int Size)
    {
        // At least one bucket, or the hash function would divide by zero
        size = (Size > 0) ? Size : 1;
        length = 0;

        buckets = new Entry*[size];

        for (int i = 0; i < size; i++)
        {
            buckets[i] = nullptr;
        }
    }


    // Destructor
    // Every entry was created with new, so every entry must be deleted
    ~Chaining_Hash_Table_Number()
    {
        clear();

        delete[] buckets;
    }


    // Copy Constructor
    // Creates a deep copy of another table
    Chaining_Hash_Table_Number(const Chaining_Hash_Table_Number& other)
    {
        size = other.size;
        length = 0;

        buckets = new Entry*[size];

        for (int i = 0; i < size; i++)
        {
            buckets[i] = nullptr;

            Entry* current = other.buckets[i];
            Entry* last = nullptr;

            // Copy the chain in the same order
            while (current != nullptr)
            {
                Entry* newEntry = new Entry(current->key, current->value);

                if (last == nullptr)
                {
                    buckets[i] = newEntry;
                }
                else
                {
                    last->next = newEntry;
                }

                last = newEntry;
                length++;

                current = current->next;
            }
        }
    }


    // Assignment Operator
    // Performs deep copy between objects
    Chaining_Hash_Table_Number& operator=(const Chaining_Hash_Table_Number& other)
    {
        // Avoid self-assignment
        if (this == &other)
        {
            return *this;
        }

        // Build the copy first, so nothing is lost if something fails
        Chaining_Hash_Table_Number temp(other);

        // Swap contents, temp will delete our old entries
        Entry** oldBuckets = buckets;
        buckets = temp.buckets;
        temp.buckets = oldBuckets;

        int oldSize = size;
        size = temp.size;
        temp.size = oldSize;

        int oldLength = length;
        length = temp.length;
        temp.length = oldLength;

        return *this;
    }


    // Store a value under a key
    // Returns 1 if the key is new, 0 if the key existed and its value was updated
    int put(int key, int value)
    {
        int index = hash(key, size);

        // Look for the key in its bucket
        Entry* current = buckets[index];

        while (current != nullptr)
        {
            if (current->key == key)
            {
                current->value = value;

                return 0;
            }

            current = current->next;
        }

        // Keep the chains short: more than 75% full means grow
        if ((length + 1) * 4 > size * 3)
        {
            rehash(size * 2);

            // The number of buckets changed, so the index changed too
            index = hash(key, size);
        }

        // Add the new entry at the start of the bucket
        Entry* newEntry = new Entry(key, value);

        newEntry->next = buckets[index];
        buckets[index] = newEntry;

        length++;

        return 1;
    }


    // Find the value stored under a key
    // Returns false if the key does not exist
    bool get(int key, int& value)
    {
        Entry* current = buckets[hash(key, size)];

        while (current != nullptr)
        {
            if (current->key == key)
            {
                value = current->value;

                return true;
            }

            current = current->next;
        }

        return false;
    }


    // Check if a key exists
    bool contains(int key)
    {
        int ignored;

        return get(key, ignored);
    }


    // Remove a key and its value
    // Returns false if the key does not exist
    bool remove(int key)
    {
        int index = hash(key, size);

        Entry* previous = nullptr;
        Entry* current = buckets[index];

        while (current != nullptr)
        {
            if (current->key == key)
            {
                // Skip over the entry we want to remove
                if (previous == nullptr)
                {
                    buckets[index] = current->next;
                }
                else
                {
                    previous->next = current->next;
                }

                delete current;

                length--;

                return true;
            }

            previous = current;
            current = current->next;
        }

        return false;
    }


    // Delete every entry
    // The number of buckets stays the same
    int clear()
    {
        for (int i = 0; i < size; i++)
        {
            Entry* current = buckets[i];

            while (current != nullptr)
            {
                // Save next before deleting, or we lose the rest of the chain
                Entry* nextEntry = current->next;

                delete current;

                current = nextEntry;
            }

            buckets[i] = nullptr;
        }

        length = 0;

        return 1;
    }


    // Return the bucket that a key belongs to
    int index_of(int key)
    {
        return hash(key, size);
    }


    // Return the length of the longest chain
    // 1 or 0 means no collisions, a big number means a bad hash or a full table
    int longest_chain()
    {
        int longest = 0;

        for (int i = 0; i < size; i++)
        {
            int count = 0;
            Entry* current = buckets[i];

            while (current != nullptr)
            {
                count++;
                current = current->next;
            }

            if (count > longest)
            {
                longest = count;
            }
        }

        return longest;
    }


    // Return entries / buckets
    double load_factor()
    {
        return static_cast<double>(length) / size;
    }


    // Display every bucket with its chain
    int display()
    {
        for (int i = 0; i < size; i++)
        {
            cout << "[" << i << "] ";

            Entry* current = buckets[i];

            while (current != nullptr)
            {
                cout << "(" << current->key << ":" << current->value << ") -> ";

                current = current->next;
            }

            cout << "NULL" << endl;
        }

        return 1;
    }


    // Return number of buckets
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
    Chaining_Hash_Table_Number counts(n);

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
    Chaining_Hash_Table_Number seen(n);

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
    cout << "    Welcome To Chaining Hash Table Application" << endl;
    cout << "============================================" << endl;


    // ============================================
    // Create Hash Table
    // ============================================

    int size;

    do {
        cout << "\nEnter Starting Number Of Buckets: ";
        read_input(size);

        if (size < 1) {
            cout << "It must be at least 1!" << endl;
        }
    } while (size < 1);

    Chaining_Hash_Table_Number table(size);


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
                cout << "3. Which Bucket Is A Key In" << endl;
                cout << "4. Get Length" << endl;
                cout << "5. Get Number Of Buckets" << endl;
                cout << "6. Get Load Factor" << endl;
                cout << "7. Get Longest Chain" << endl;
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


                // Which Bucket Is A Key In
                else if (choice == 3) {

                    int key;

                    cout << "\nEnter key: ";
                    read_input(key);

                    cout << "Key " << key << " starts at Bucket " << table.index_of(key) << endl;
                }


                // Get Length
                else if (choice == 4) {

                    cout << "\nTable Length = " << table.get_length() << endl;
                }


                // Get Number Of Buckets
                else if (choice == 5) {

                    cout << "\nNumber Of Buckets = " << table.get_size() << endl;
                }


                // Get Load Factor
                else if (choice == 6) {

                    cout << "\nLoad Factor = " << table.load_factor() << endl;
                }


                // Get Longest Chain
                else if (choice == 7) {

                    cout << "\nLongest Chain = " << table.longest_chain() << endl;
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
