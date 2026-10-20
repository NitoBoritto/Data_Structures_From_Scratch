#include <iostream>
#include <cstdlib>
using namespace std;

struct Cache_Entry {
    int key;
    int value;
    Cache_Entry* previous;
    Cache_Entry* next;
    Cache_Entry* bucketNext;

    Cache_Entry(int Key, int Value)
    {
        key = Key;
        value = Value;
        previous = nullptr;
        next = nullptr;
        bucketNext = nullptr;
    }
};


class LRU_Cache_Number {
private:

    Cache_Entry** buckets;
    Cache_Entry* mostRecent;
    Cache_Entry* leastRecent;
    int bucketCount;
    int capacity;
    int length;

private:

    int hash(int key)
    {
        unsigned int value = static_cast<unsigned int>(key);
        value ^= value >> 16;
        value *= 0x7feb352dU;
        value ^= value >> 15;
        return static_cast<int>(value % static_cast<unsigned int>(bucketCount));
    }


    Cache_Entry* find_entry(int key)
    {
        int index = hash(key);
        Cache_Entry* current = buckets[index];

        while (current != nullptr)
        {
            if (current->key == key)
            {
                return current;
            }
            current = current->bucketNext;
        }

        return nullptr;
    }


    void add_to_bucket(Cache_Entry* entry)
    {
        int index = hash(entry->key);
        entry->bucketNext = buckets[index];
        buckets[index] = entry;
    }


    void remove_from_bucket(Cache_Entry* entry)
    {
        int index = hash(entry->key);
        Cache_Entry* current = buckets[index];
        Cache_Entry* previous = nullptr;

        while (current != nullptr)
        {
            if (current == entry)
            {
                if (previous == nullptr)
                {
                    buckets[index] = current->bucketNext;
                }
                else
                {
                    previous->bucketNext = current->bucketNext;
                }

                entry->bucketNext = nullptr;
                return;
            }

            previous = current;
            current = current->bucketNext;
        }
    }


    void detach(Cache_Entry* entry)
    {
        if (entry->previous != nullptr)
        {
            entry->previous->next = entry->next;
        }
        else
        {
            mostRecent = entry->next;
        }

        if (entry->next != nullptr)
        {
            entry->next->previous = entry->previous;
        }
        else
        {
            leastRecent = entry->previous;
        }

        entry->previous = nullptr;
        entry->next = nullptr;
    }


    void add_to_front(Cache_Entry* entry)
    {
        entry->previous = nullptr;
        entry->next = mostRecent;

        if (mostRecent != nullptr)
        {
            mostRecent->previous = entry;
        }
        else
        {
            leastRecent = entry;
        }

        mostRecent = entry;
    }


    void mark_recent(Cache_Entry* entry)
    {
        if (entry == mostRecent)
        {
            return;
        }

        detach(entry);
        add_to_front(entry);
    }


    void remove_entry(Cache_Entry* entry)
    {
        remove_from_bucket(entry);
        detach(entry);
        delete entry;
        length--;
    }

public:

    // Constructor
    // Creates an empty cache with a fixed maximum number of entries
    LRU_Cache_Number(int Capacity)
    {
        capacity = (Capacity > 0) ? Capacity : 0;
        bucketCount = (capacity > 0) ? (capacity * 2 + 1) : 1;
        length = 0;
        mostRecent = nullptr;
        leastRecent = nullptr;
        buckets = new Cache_Entry*[bucketCount];

        for (int i = 0; i < bucketCount; i++)
        {
            buckets[i] = nullptr;
        }
    }


    // Destructor
    // Releases every cache entry and bucket
    ~LRU_Cache_Number()
    {
        clear();
        delete[] buckets;
    }


    // Copy Constructor
    // Preserves the original least-recently-used order
    LRU_Cache_Number(const LRU_Cache_Number& other)
    {
        capacity = other.capacity;
        bucketCount = other.bucketCount;
        length = 0;
        mostRecent = nullptr;
        leastRecent = nullptr;
        buckets = new Cache_Entry*[bucketCount];

        for (int i = 0; i < bucketCount; i++)
        {
            buckets[i] = nullptr;
        }

        Cache_Entry* current = other.leastRecent;
        while (current != nullptr)
        {
            Cache_Entry* entry = new Cache_Entry(current->key, current->value);
            add_to_front(entry);
            add_to_bucket(entry);
            length++;
            current = current->previous;
        }
    }


    // Assignment Operator
    // Performs a deep copy and preserves recency order
    LRU_Cache_Number& operator=(const LRU_Cache_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        clear();
        delete[] buckets;

        capacity = other.capacity;
        bucketCount = other.bucketCount;
        buckets = new Cache_Entry*[bucketCount];
        for (int i = 0; i < bucketCount; i++)
        {
            buckets[i] = nullptr;
        }

        Cache_Entry* current = other.leastRecent;
        while (current != nullptr)
        {
            Cache_Entry* entry = new Cache_Entry(current->key, current->value);
            add_to_front(entry);
            add_to_bucket(entry);
            length++;
            current = current->previous;
        }

        return *this;
    }


    // Add or update a key-value pair
    // When full, the least recently used entry is removed
    bool put(int key, int value)
    {
        if (capacity == 0)
        {
            return false;
        }

        Cache_Entry* entry = find_entry(key);
        if (entry != nullptr)
        {
            entry->value = value;
            mark_recent(entry);
            return true;
        }

        if (length == capacity)
        {
            remove_entry(leastRecent);
        }

        entry = new Cache_Entry(key, value);
        add_to_front(entry);
        add_to_bucket(entry);
        length++;
        return true;
    }


    // Read a value and mark its key as recently used
    bool get(int key, int& value)
    {
        Cache_Entry* entry = find_entry(key);
        if (entry == nullptr)
        {
            return false;
        }

        value = entry->value;
        mark_recent(entry);
        return true;
    }


    // Check for a key without changing its recency
    bool contains(int key)
    {
        return find_entry(key) != nullptr;
    }


    // Remove a key
    bool remove(int key)
    {
        Cache_Entry* entry = find_entry(key);
        if (entry == nullptr)
        {
            return false;
        }

        remove_entry(entry);
        return true;
    }


    // Display entries from most recent to least recent
    int display()
    {
        if (length == 0)
        {
            cout << "Cache is empty." << endl;
            return 0;
        }

        Cache_Entry* current = mostRecent;
        while (current != nullptr)
        {
            cout << "(Key: " << current->key << ", Value: " << current->value << ")";
            if (current->next != nullptr)
            {
                cout << " -> ";
            }
            current = current->next;
        }
        cout << endl;
        return 1;
    }


    // Remove all entries; the capacity stays the same
    int clear()
    {
        Cache_Entry* current = mostRecent;
        while (current != nullptr)
        {
            Cache_Entry* nextEntry = current->next;
            delete current;
            current = nextEntry;
        }

        mostRecent = nullptr;
        leastRecent = nullptr;
        length = 0;

        for (int i = 0; i < bucketCount; i++)
        {
            buckets[i] = nullptr;
        }

        return 1;
    }


    // Check if the cache has no entries
    bool is_empty()
    {
        return length == 0;
    }


    // Return number of entries
    int get_length()
    {
        return length;
    }


    // Return maximum number of entries
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
    cout << "          Welcome To LRU Cache Application" << endl;
    cout << "============================================" << endl;

    int capacity;
    do {
        cout << "\nEnter cache capacity: ";
        read_input(capacity);
        if (capacity < 0) {
            cout << "Capacity cannot be negative!" << endl;
        }
    } while (capacity < 0);

    LRU_Cache_Number cache(capacity);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Search Operations" << endl;
        cout << "3. Cache Operations" << endl;
        cout << "4. Cache Information" << endl;
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
                cout << "1. Put Key And Value" << endl;
                cout << "2. Get Value" << endl;
                cout << "3. Remove Key" << endl;
                cout << "4. Clear Cache" << endl;
                cout << "5. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    int key;
                    int value;
                    cout << "\nEnter key: ";
                    read_input(key);
                    cout << "Enter value: ";
                    read_input(value);
                    if (cache.put(key, value)) {
                        cout << "Value stored successfully." << endl;
                    }
                    else {
                        cout << "Cache capacity is zero; no values can be stored." << endl;
                    }
                }
                else if (choice == 2) {
                    int key;
                    int value;
                    cout << "\nEnter key: ";
                    read_input(key);
                    if (cache.get(key, value)) {
                        cout << "Value: " << value << endl;
                    }
                    else {
                        cout << "Key not found." << endl;
                    }
                }
                else if (choice == 3) {
                    int key;
                    cout << "\nEnter key to remove: ";
                    read_input(key);
                    cout << (cache.remove(key) ? "Key removed successfully." : "Key not found.") << endl;
                }
                else if (choice == 4) {
                    cache.clear();
                    cout << "Cache cleared successfully." << endl;
                }
                else if (choice != 5) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 5);
        }
        else if (mainChoice == 2) {
            int key;
            cout << "\nEnter key to search: ";
            read_input(key);
            cout << (cache.contains(key) ? "Key found." : "Key not found.") << endl;
        }
        else if (mainChoice == 3) {
            cout << "\nCache (most recent to least recent):" << endl;
            cache.display();
        }
        else if (mainChoice == 4) {
            cout << "\nEntries: " << cache.get_length() << endl;
            cout << "Capacity: " << cache.get_capacity() << endl;
            cout << "Empty: " << (cache.is_empty() ? "Yes" : "No") << endl;
        }
        else if (mainChoice != 5) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 5);

    cout << "\nThank you for using LRU Cache Application." << endl;
    return 0;
}
