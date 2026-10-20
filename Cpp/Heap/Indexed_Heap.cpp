#include <iostream>
#include <cstdlib>
#include <limits>
#include <stdexcept>

using namespace std;

struct Indexed_Heap_Entry {
    int id;
    int priority;
    unsigned long long order;

    Indexed_Heap_Entry() : id(0), priority(0), order(0) {}

    Indexed_Heap_Entry(int Id, int Priority, unsigned long long Order)
        : id(Id), priority(Priority), order(Order) {}
};


class Indexed_Heap {
private:
    enum { EMPTY = 0, USED = 1, DELETED = 2 };

    Indexed_Heap_Entry* entries;
    int heapCapacity;
    int length;
    unsigned long long nextOrder;

    int* mapKeys;
    int* mapIndexes;
    unsigned char* mapStates;
    int mapCapacity;
    int mapLength;
    int mapDeleted;

    unsigned int hash_id(int id) const
    {
        unsigned int value = static_cast<unsigned int>(id);
        value ^= value >> 16;
        value *= 0x7feb352dU;
        value ^= value >> 15;
        value *= 0x846ca68bU;
        value ^= value >> 16;
        return value;
    }

    int map_slot(int id) const
    {
        int index = static_cast<int>(hash_id(id) % static_cast<unsigned int>(mapCapacity));
        int firstDeleted = -1;

        for (int step = 0; step < mapCapacity; ++step)
        {
            if (mapStates[index] == EMPTY)
            {
                return firstDeleted == -1 ? index : firstDeleted;
            }
            if (mapStates[index] == USED && mapKeys[index] == id)
            {
                return index;
            }
            if (mapStates[index] == DELETED && firstDeleted == -1)
            {
                firstDeleted = index;
            }
            index = (index + 1) % mapCapacity;
        }

        return firstDeleted;
    }

    int find_map_slot(int id) const
    {
        int index = static_cast<int>(hash_id(id) % static_cast<unsigned int>(mapCapacity));

        for (int step = 0; step < mapCapacity; ++step)
        {
            if (mapStates[index] == EMPTY)
            {
                return -1;
            }
            if (mapStates[index] == USED && mapKeys[index] == id)
            {
                return index;
            }
            index = (index + 1) % mapCapacity;
        }

        return -1;
    }

    void rehash_map(int newCapacity)
    {
        int* newKeys = new int[newCapacity];
        int* newIndexes = new int[newCapacity];
        unsigned char* newStates = new unsigned char[newCapacity];
        for (int i = 0; i < newCapacity; ++i)
        {
            newStates[i] = EMPTY;
        }

        for (int i = 0; i < mapCapacity; ++i)
        {
            if (mapStates[i] == USED)
            {
                int index = static_cast<int>(
                    hash_id(mapKeys[i]) % static_cast<unsigned int>(newCapacity));
                while (newStates[index] == USED)
                {
                    index = (index + 1) % newCapacity;
                }
                newKeys[index] = mapKeys[i];
                newIndexes[index] = mapIndexes[i];
                newStates[index] = USED;
            }
        }

        delete[] mapKeys;
        delete[] mapIndexes;
        delete[] mapStates;
        mapKeys = newKeys;
        mapIndexes = newIndexes;
        mapStates = newStates;
        mapCapacity = newCapacity;
        mapDeleted = 0;
    }

    void ensure_map_capacity()
    {
        if ((mapLength + mapDeleted + 1) * 10 >= mapCapacity * 7)
        {
            if (mapCapacity > numeric_limits<int>::max() / 2)
            {
                throw length_error("Indexed heap ID map is too large.");
            }
            rehash_map(mapCapacity * 2);
        }
        else if (mapDeleted > mapCapacity / 4)
        {
            rehash_map(mapCapacity);
        }
    }

    void map_insert(int id, int heapIndex)
    {
        int index = map_slot(id);
        if (index == -1)
        {
            throw length_error("Indexed heap ID map is full.");
        }
        if (mapStates[index] == DELETED)
        {
            --mapDeleted;
        }
        mapKeys[index] = id;
        mapIndexes[index] = heapIndex;
        mapStates[index] = USED;
        ++mapLength;
    }

    void map_erase(int id)
    {
        int index = find_map_slot(id);
        if (index != -1)
        {
            mapStates[index] = DELETED;
            --mapLength;
            ++mapDeleted;
        }
    }

    void map_set_index(int id, int heapIndex)
    {
        int index = find_map_slot(id);
        if (index != -1)
        {
            mapIndexes[index] = heapIndex;
        }
    }

    int parent_index(int index) const
    {
        return (index - 1) / 2;
    }

    bool comes_before(const Indexed_Heap_Entry& first,
                      const Indexed_Heap_Entry& second) const
    {
        if (first.priority != second.priority)
        {
            return first.priority > second.priority;
        }
        return first.order < second.order;
    }

    void swap_entries(int first, int second)
    {
        Indexed_Heap_Entry temp = entries[first];
        entries[first] = entries[second];
        entries[second] = temp;
        map_set_index(entries[first].id, first);
        map_set_index(entries[second].id, second);
    }

    void sift_up(int index)
    {
        while (index > 0 && comes_before(entries[index], entries[parent_index(index)]))
        {
            int parent = parent_index(index);
            swap_entries(index, parent);
            index = parent;
        }
    }

    void sift_down(int index)
    {
        while (true)
        {
            int best = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < length && comes_before(entries[left], entries[best]))
            {
                best = left;
            }
            if (right < length && comes_before(entries[right], entries[best]))
            {
                best = right;
            }
            if (best == index)
            {
                return;
            }
            swap_entries(index, best);
            index = best;
        }
    }

    void restore_heap(int index)
    {
        if (index > 0 && comes_before(entries[index], entries[parent_index(index)]))
        {
            sift_up(index);
        }
        else
        {
            sift_down(index);
        }
    }

    void ensure_heap_capacity(int required)
    {
        if (required <= heapCapacity)
        {
            return;
        }

        int newCapacity = heapCapacity;
        while (newCapacity < required)
        {
            if (newCapacity > numeric_limits<int>::max() / 2)
            {
                newCapacity = required;
                break;
            }
            newCapacity *= 2;
        }

        Indexed_Heap_Entry* newEntries = new Indexed_Heap_Entry[newCapacity];
        for (int i = 0; i < length; ++i)
        {
            newEntries[i] = entries[i];
        }
        delete[] entries;
        entries = newEntries;
        heapCapacity = newCapacity;
    }

    void remove_index(int index)
    {
        map_erase(entries[index].id);
        --length;
        if (index == length)
        {
            return;
        }

        entries[index] = entries[length];
        map_set_index(entries[index].id, index);
        restore_heap(index);
    }

public:
    explicit Indexed_Heap(int initialCapacity = 10)
        : entries(0), heapCapacity(initialCapacity > 0 ? initialCapacity : 1),
          length(0), nextOrder(0), mapKeys(0), mapIndexes(0), mapStates(0),
          mapCapacity(8), mapLength(0), mapDeleted(0)
    {
        entries = new Indexed_Heap_Entry[heapCapacity];
        mapKeys = new int[mapCapacity];
        mapIndexes = new int[mapCapacity];
        mapStates = new unsigned char[mapCapacity];
        for (int i = 0; i < mapCapacity; ++i)
        {
            mapStates[i] = EMPTY;
        }
    }

    ~Indexed_Heap()
    {
        delete[] entries;
        delete[] mapKeys;
        delete[] mapIndexes;
        delete[] mapStates;
    }

    Indexed_Heap(const Indexed_Heap& other)
        : entries(new Indexed_Heap_Entry[other.heapCapacity]),
          heapCapacity(other.heapCapacity), length(other.length),
          nextOrder(other.nextOrder), mapKeys(new int[other.mapCapacity]),
          mapIndexes(new int[other.mapCapacity]),
          mapStates(new unsigned char[other.mapCapacity]),
          mapCapacity(other.mapCapacity), mapLength(other.mapLength),
          mapDeleted(other.mapDeleted)
    {
        for (int i = 0; i < length; ++i)
        {
            entries[i] = other.entries[i];
        }
        for (int i = 0; i < mapCapacity; ++i)
        {
            mapKeys[i] = other.mapKeys[i];
            mapIndexes[i] = other.mapIndexes[i];
            mapStates[i] = other.mapStates[i];
        }
    }

    Indexed_Heap& operator=(const Indexed_Heap& other)
    {
        if (this == &other)
        {
            return *this;
        }

        Indexed_Heap_Entry* newEntries = new Indexed_Heap_Entry[other.heapCapacity];
        int* newKeys = 0;
        int* newIndexes = 0;
        unsigned char* newStates = 0;
        try
        {
            newKeys = new int[other.mapCapacity];
            newIndexes = new int[other.mapCapacity];
            newStates = new unsigned char[other.mapCapacity];
        }
        catch (...)
        {
            delete[] newEntries;
            delete[] newKeys;
            delete[] newIndexes;
            delete[] newStates;
            throw;
        }

        for (int i = 0; i < other.length; ++i)
        {
            newEntries[i] = other.entries[i];
        }
        for (int i = 0; i < other.mapCapacity; ++i)
        {
            newKeys[i] = other.mapKeys[i];
            newIndexes[i] = other.mapIndexes[i];
            newStates[i] = other.mapStates[i];
        }

        delete[] entries;
        delete[] mapKeys;
        delete[] mapIndexes;
        delete[] mapStates;
        entries = newEntries;
        mapKeys = newKeys;
        mapIndexes = newIndexes;
        mapStates = newStates;
        heapCapacity = other.heapCapacity;
        length = other.length;
        nextOrder = other.nextOrder;
        mapCapacity = other.mapCapacity;
        mapLength = other.mapLength;
        mapDeleted = other.mapDeleted;
        return *this;
    }

    bool insert(int id, int priority)
    {
        if (contains(id))
        {
            return false;
        }

        ensure_heap_capacity(length + 1);
        ensure_map_capacity();
        entries[length] = Indexed_Heap_Entry(id, priority, nextOrder++);
        map_insert(id, length);
        sift_up(length);
        ++length;
        return true;
    }

    bool peek(int& id, int& priority) const
    {
        if (length == 0)
        {
            return false;
        }
        id = entries[0].id;
        priority = entries[0].priority;
        return true;
    }

    bool extract_max(int& id, int& priority)
    {
        if (!peek(id, priority))
        {
            return false;
        }
        remove_index(0);
        return true;
    }

    bool update_priority(int id, int newPriority)
    {
        int mapIndex = find_map_slot(id);
        if (mapIndex == -1)
        {
            return false;
        }
        int index = mapIndexes[mapIndex];
        entries[index].priority = newPriority;
        restore_heap(index);
        return true;
    }

    bool remove(int id)
    {
        int mapIndex = find_map_slot(id);
        if (mapIndex == -1)
        {
            return false;
        }
        remove_index(mapIndexes[mapIndex]);
        return true;
    }

    bool contains(int id) const
    {
        return find_map_slot(id) != -1;
    }

    void clear()
    {
        length = 0;
        nextOrder = 0;
        mapLength = 0;
        mapDeleted = 0;
        for (int i = 0; i < mapCapacity; ++i)
        {
            mapStates[i] = EMPTY;
        }
    }

    int size() const { return length; }
    int capacity() const { return heapCapacity; }
    int get_length() const { return length; }
    int get_capacity() const { return heapCapacity; }

    void display() const
    {
        if (length == 0)
        {
            cout << "Indexed heap is empty." << endl;
            return;
        }
        for (int i = 0; i < length; ++i)
        {
            cout << "(ID: " << entries[i].id << ", Priority: "
                 << entries[i].priority << ") ";
        }
        cout << endl;
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


#ifndef INDEXED_HEAP_TEST
int main()
{
    cout << "============================================" << endl;
    cout << "       Welcome To Indexed Heap Application" << endl;
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

    Indexed_Heap heap(initialCapacity);
    int choice;
    do
    {
        cout << "\n1. Insert (ID, priority)\n"
             << "2. Peek maximum\n"
             << "3. Extract maximum\n"
             << "4. Update priority\n"
             << "5. Remove by ID\n"
             << "6. Check ID\n"
             << "7. Clear\n"
             << "8. Display heap\n"
             << "9. Show size and capacity\n"
             << "0. Exit\nChoice: ";
        read_integer(choice);

        int id;
        int priority;
        switch (choice)
        {
        case 1:
            cout << "ID: ";
            read_integer(id);
            cout << "Priority: ";
            read_integer(priority);
            cout << (heap.insert(id, priority) ? "Inserted." : "ID already exists.") << endl;
            break;
        case 2:
        case 3:
            if ((choice == 2 ? heap.peek(id, priority) : heap.extract_max(id, priority)))
            {
                cout << (choice == 2 ? "Maximum" : "Extracted") << ": ID "
                     << id << ", priority " << priority << endl;
            }
            else
            {
                cout << "Indexed heap is empty." << endl;
            }
            break;
        case 4:
            cout << "ID: ";
            read_integer(id);
            cout << "New priority: ";
            read_integer(priority);
            cout << (heap.update_priority(id, priority) ? "Priority updated." : "ID not found.") << endl;
            break;
        case 5:
            cout << "ID: ";
            read_integer(id);
            cout << (heap.remove(id) ? "ID removed." : "ID not found.") << endl;
            break;
        case 6:
            cout << "ID: ";
            read_integer(id);
            cout << (heap.contains(id) ? "ID is present." : "ID is not present.") << endl;
            break;
        case 7:
            heap.clear();
            cout << "Indexed heap cleared." << endl;
            break;
        case 8:
            heap.display();
            break;
        case 9:
            cout << "Size: " << heap.size() << ", capacity: " << heap.capacity() << endl;
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
