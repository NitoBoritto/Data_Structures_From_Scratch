#include <iostream>
#include <cstdlib>
#include <limits>
#include <stdexcept>

using namespace std;

struct Cuckoo_Hash_Entry {
    int key;
    int value;

    Cuckoo_Hash_Entry() : key(0), value(0) {}
    Cuckoo_Hash_Entry(int Key, int Value) : key(Key), value(Value) {}
};


class Cuckoo_Hash_Table {
private:
    enum { EMPTY = 0, USED = 1 };

    int* keys;
    int* values;
    unsigned char* states;
    int tableCapacity;
    int length;

    unsigned int mix(unsigned int value) const
    {
        value ^= value >> 16;
        value *= 0x7feb352dU;
        value ^= value >> 15;
        value *= 0x846ca68bU;
        value ^= value >> 16;
        return value;
    }

    int hash_one(int key, int capacity) const
    {
        unsigned int value = static_cast<unsigned int>(key);
        return static_cast<int>(mix(value ^ 0x9e3779b9U)
                                % static_cast<unsigned int>(capacity));
    }

    int hash_two(int key, int capacity) const
    {
        unsigned int value = static_cast<unsigned int>(key);
        return static_cast<int>(mix(value ^ 0x85ebca6bU)
                                % static_cast<unsigned int>(capacity));
    }

    int find_index(int key) const
    {
        int first = hash_one(key, tableCapacity);
        if (states[first] == USED && keys[first] == key)
        {
            return first;
        }
        int second = hash_two(key, tableCapacity);
        if (states[second] == USED && keys[second] == key)
        {
            return second;
        }
        return -1;
    }

    bool place_entry(int* targetKeys, int* targetValues,
                     unsigned char* targetStates, int capacity,
                     Cuckoo_Hash_Entry entry,
                     Cuckoo_Hash_Entry& pending) const
    {
        int previousIndex = -1;
        int maxKicks = capacity > numeric_limits<int>::max() / 2
            ? numeric_limits<int>::max() : capacity * 2;

        for (int kick = 0; kick < maxKicks; ++kick)
        {
            int first = hash_one(entry.key, capacity);
            int second = hash_two(entry.key, capacity);
            int index = previousIndex == -1 || previousIndex != first
                ? first : second;

            if (targetStates[index] == EMPTY)
            {
                targetKeys[index] = entry.key;
                targetValues[index] = entry.value;
                targetStates[index] = USED;
                return true;
            }

            Cuckoo_Hash_Entry displaced(targetKeys[index], targetValues[index]);
            targetKeys[index] = entry.key;
            targetValues[index] = entry.value;
            entry = displaced;
            previousIndex = index;
        }
        pending = entry;
        return false;
    }

    void rebuild(const Cuckoo_Hash_Entry* allEntries, int count, int minimumCapacity)
    {
        int newCapacity = tableCapacity;
        if (newCapacity < minimumCapacity)
        {
            newCapacity = minimumCapacity;
        }
        if (newCapacity < 2)
        {
            newCapacity = 2;
        }
        while (count > newCapacity / 2)
        {
            if (newCapacity > numeric_limits<int>::max() / 2)
            {
                throw length_error("Cuckoo hash table is too large.");
            }
            newCapacity *= 2;
        }

        while (true)
        {
            int* newKeys = new int[newCapacity];
            int* newValues = new int[newCapacity];
            unsigned char* newStates = new unsigned char[newCapacity];
            for (int i = 0; i < newCapacity; ++i)
            {
                newStates[i] = EMPTY;
            }

            bool successful = true;
            for (int i = 0; i < count; ++i)
            {
                Cuckoo_Hash_Entry pending;
                if (!place_entry(newKeys, newValues, newStates, newCapacity,
                                 allEntries[i], pending))
                {
                    successful = false;
                    break;
                }
            }

            if (successful)
            {
                delete[] keys;
                delete[] values;
                delete[] states;
                keys = newKeys;
                values = newValues;
                states = newStates;
                tableCapacity = newCapacity;
                length = count;
                return;
            }

            delete[] newKeys;
            delete[] newValues;
            delete[] newStates;
            if (newCapacity > numeric_limits<int>::max() / 2)
            {
                throw length_error("Unable to find a cuckoo-hash placement.");
            }
            newCapacity *= 2;
        }
    }

    void grow_with_entry(Cuckoo_Hash_Entry extra)
    {
        Cuckoo_Hash_Entry* allEntries = new Cuckoo_Hash_Entry[length + 1];
        int count = 0;
        for (int i = 0; i < tableCapacity; ++i)
        {
            if (states[i] == USED)
            {
                allEntries[count++] = Cuckoo_Hash_Entry(keys[i], values[i]);
            }
        }
        allEntries[count++] = extra;

        try
        {
            int minimumCapacity = tableCapacity;
            if (minimumCapacity > numeric_limits<int>::max() / 2)
            {
                throw length_error("Cuckoo hash table is too large.");
            }
            minimumCapacity *= 2;
            rebuild(allEntries, count, minimumCapacity);
        }
        catch (...)
        {
            delete[] allEntries;
            throw;
        }
        delete[] allEntries;
    }

    void grow_after_kick(Cuckoo_Hash_Entry pending)
    {
        Cuckoo_Hash_Entry* allEntries = new Cuckoo_Hash_Entry[length + 1];
        int count = 0;
        for (int i = 0; i < tableCapacity; ++i)
        {
            if (states[i] == USED)
            {
                allEntries[count++] = Cuckoo_Hash_Entry(keys[i], values[i]);
            }
        }
        allEntries[count++] = pending;

        try
        {
            int minimumCapacity = tableCapacity;
            if (minimumCapacity > numeric_limits<int>::max() / 2)
            {
                throw length_error("Cuckoo hash table is too large.");
            }
            minimumCapacity *= 2;
            rebuild(allEntries, count, minimumCapacity);
        }
        catch (...)
        {
            delete[] allEntries;
            throw;
        }
        delete[] allEntries;
    }

public:
    explicit Cuckoo_Hash_Table(int initialCapacity = 8)
        : keys(0), values(0), states(0),
          tableCapacity(initialCapacity > 1 ? initialCapacity : 2), length(0)
    {
        keys = new int[tableCapacity];
        values = new int[tableCapacity];
        states = new unsigned char[tableCapacity];
        for (int i = 0; i < tableCapacity; ++i)
        {
            states[i] = EMPTY;
        }
    }

    ~Cuckoo_Hash_Table()
    {
        delete[] keys;
        delete[] values;
        delete[] states;
    }

    Cuckoo_Hash_Table(const Cuckoo_Hash_Table& other)
        : keys(new int[other.tableCapacity]),
          values(new int[other.tableCapacity]),
          states(new unsigned char[other.tableCapacity]),
          tableCapacity(other.tableCapacity), length(other.length)
    {
        for (int i = 0; i < tableCapacity; ++i)
        {
            keys[i] = other.keys[i];
            values[i] = other.values[i];
            states[i] = other.states[i];
        }
    }

    Cuckoo_Hash_Table& operator=(const Cuckoo_Hash_Table& other)
    {
        if (this == &other)
        {
            return *this;
        }

        int* newKeys = new int[other.tableCapacity];
        int* newValues = 0;
        unsigned char* newStates = 0;
        try
        {
            newValues = new int[other.tableCapacity];
            newStates = new unsigned char[other.tableCapacity];
        }
        catch (...)
        {
            delete[] newKeys;
            delete[] newValues;
            delete[] newStates;
            throw;
        }

        for (int i = 0; i < other.tableCapacity; ++i)
        {
            newKeys[i] = other.keys[i];
            newValues[i] = other.values[i];
            newStates[i] = other.states[i];
        }

        delete[] keys;
        delete[] values;
        delete[] states;
        keys = newKeys;
        values = newValues;
        states = newStates;
        tableCapacity = other.tableCapacity;
        length = other.length;
        return *this;
    }

    // Returns 1 for a new key and 0 when an existing value was updated.
    int insert(int key, int value)
    {
        int found = find_index(key);
        if (found != -1)
        {
            values[found] = value;
            return 0;
        }

        Cuckoo_Hash_Entry entry(key, value);
        if (length + 1 > tableCapacity / 2)
        {
            grow_with_entry(entry);
            return 1;
        }

        Cuckoo_Hash_Entry pending;
        if (place_entry(keys, values, states, tableCapacity, entry, pending))
        {
            ++length;
            return 1;
        }

        // The bounded kick loop may leave one entry displaced; rebuild from all
        // occupied slots plus that pending entry, retrying at larger capacities.
        grow_after_kick(pending);
        return 1;
    }

    bool search(int key, int& value) const
    {
        int index = find_index(key);
        if (index == -1)
        {
            return false;
        }
        value = values[index];
        return true;
    }

    bool get(int key, int& value) const
    {
        return search(key, value);
    }

    bool contains(int key) const
    {
        return find_index(key) != -1;
    }

    bool remove(int key)
    {
        int index = find_index(key);
        if (index == -1)
        {
            return false;
        }
        states[index] = EMPTY;
        --length;
        return true;
    }

    void clear()
    {
        for (int i = 0; i < tableCapacity; ++i)
        {
            states[i] = EMPTY;
        }
        length = 0;
    }

    int size() const { return length; }
    int capacity() const { return tableCapacity; }
    int get_length() const { return length; }
    int get_size() const { return tableCapacity; }

    void display() const
    {
        for (int i = 0; i < tableCapacity; ++i)
        {
            cout << "[" << i << "] ";
            if (states[i] == USED)
            {
                cout << keys[i] << ":" << values[i] << endl;
            }
            else
            {
                cout << "EMPTY" << endl;
            }
        }
    }
};


static void read_integer(int& value)
{
    while (!(cin >> value))
    {
        if (cin.eof())
        {
            exit(EXIT_SUCCESS);
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input error: expected an integer. Try again: ";
    }
}


#ifndef CUCKOO_HASH_TABLE_TEST
int main()
{
    cout << "============================================" << endl;
    cout << "    Welcome To Cuckoo Hash Table Application" << endl;
    cout << "============================================" << endl;

    int initialCapacity;
    do
    {
        cout << "Enter starting capacity (0 or greater): ";
        read_integer(initialCapacity);
        if (initialCapacity < 0)
        {
            cout << "Capacity cannot be negative." << endl;
        }
    } while (initialCapacity < 0);

    Cuckoo_Hash_Table table(initialCapacity);
    int choice;
    do
    {
        cout << "\n1. Insert / update (key, value)\n"
             << "2. Search key\n"
             << "3. Remove key\n"
             << "4. Clear\n"
             << "5. Display table\n"
             << "6. Show size and capacity\n"
             << "0. Exit\nChoice: ";
        read_integer(choice);

        int key;
        int value;
        switch (choice)
        {
        case 1:
            cout << "Key: ";
            read_integer(key);
            cout << "Value: ";
            read_integer(value);
            cout << (table.insert(key, value) ? "Key inserted." : "Value updated.") << endl;
            break;
        case 2:
            cout << "Key: ";
            read_integer(key);
            if (table.search(key, value))
            {
                cout << "Value: " << value << endl;
            }
            else
            {
                cout << "Key not found." << endl;
            }
            break;
        case 3:
            cout << "Key: ";
            read_integer(key);
            cout << (table.remove(key) ? "Key removed." : "Key not found.") << endl;
            break;
        case 4:
            table.clear();
            cout << "Cuckoo hash table cleared." << endl;
            break;
        case 5:
            table.display();
            break;
        case 6:
            cout << "Size: " << table.size() << ", capacity: "
                 << table.capacity() << endl;
            break;
        case 0:
            cout << "Goodbye." << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
        }
    } while (choice != 0);

    return 0;
}
#endif
