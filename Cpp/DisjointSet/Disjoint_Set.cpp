#include <iostream>
#include <cstdlib>
using namespace std;

class Disjoint_Set_Number {
private:

    int* Parent;
    int* Size;
    int capacity;
    int length;
    int componentCount;

private:

    bool is_valid_element(int element)
    {
        return element >= 0 && element < length;
    }


    // Grow both arrays while keeping the existing sets
    void resize(int newCapacity)
    {
        if (newCapacity <= capacity)
        {
            return;
        }

        int* newParent = new int[newCapacity];
        int* newSize = new int[newCapacity];

        for (int i = 0; i < length; i++)
        {
            newParent[i] = Parent[i];
            newSize[i] = Size[i];
        }

        delete[] Parent;
        delete[] Size;

        Parent = newParent;
        Size = newSize;
        capacity = newCapacity;
    }


    void ensure_capacity(int requiredCapacity)
    {
        if (requiredCapacity <= capacity)
        {
            return;
        }

        int newCapacity = (capacity > 0) ? capacity : 1;
        while (newCapacity < requiredCapacity)
        {
            newCapacity *= 2;
        }

        resize(newCapacity);
    }

public:

    // Constructor
    // Creates the requested number of separate sets
    Disjoint_Set_Number(int Length = 0)
    {
        length = (Length > 0) ? Length : 0;
        capacity = (length > 0) ? length : 1;
        componentCount = length;

        Parent = new int[capacity];
        Size = new int[capacity];

        for (int i = 0; i < length; i++)
        {
            Parent[i] = i;
            Size[i] = 1;
        }
    }


    // Destructor
    // Releases dynamically allocated memory
    ~Disjoint_Set_Number()
    {
        delete[] Parent;
        delete[] Size;
    }


    // Copy Constructor
    // Creates a deep copy of another disjoint set
    Disjoint_Set_Number(const Disjoint_Set_Number& other)
    {
        capacity = other.capacity;
        length = other.length;
        componentCount = other.componentCount;

        Parent = new int[capacity];
        Size = new int[capacity];

        for (int i = 0; i < length; i++)
        {
            Parent[i] = other.Parent[i];
            Size[i] = other.Size[i];
        }
    }


    // Assignment Operator
    // Performs deep copy between disjoint sets
    Disjoint_Set_Number& operator=(const Disjoint_Set_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        int* newParent = new int[other.capacity];
        int* newSize = new int[other.capacity];

        for (int i = 0; i < other.length; i++)
        {
            newParent[i] = other.Parent[i];
            newSize[i] = other.Size[i];
        }

        delete[] Parent;
        delete[] Size;

        Parent = newParent;
        Size = newSize;
        capacity = other.capacity;
        length = other.length;
        componentCount = other.componentCount;

        return *this;
    }


    // Add a new element as its own set
    // Returns the new element's number
    int add_set()
    {
        ensure_capacity(length + 1);
        Parent[length] = length;
        Size[length] = 1;
        length++;
        componentCount++;
        return length - 1;
    }


    // Find the representative of an element's set
    // Path compression shortens future searches
    int find_set(int element)
    {
        if (!is_valid_element(element))
        {
            return -1;
        }

        if (Parent[element] != element)
        {
            Parent[element] = find_set(Parent[element]);
        }

        return Parent[element];
    }


    // Merge the sets containing first and second
    // Returns false for invalid elements or elements already in the same set
    bool unite(int first, int second)
    {
        int firstRoot = find_set(first);
        int secondRoot = find_set(second);

        if (firstRoot == -1 || secondRoot == -1 || firstRoot == secondRoot)
        {
            return false;
        }

        // Attach the smaller set below the larger set
        if (Size[firstRoot] < Size[secondRoot])
        {
            int temp = firstRoot;
            firstRoot = secondRoot;
            secondRoot = temp;
        }

        Parent[secondRoot] = firstRoot;
        Size[firstRoot] += Size[secondRoot];
        componentCount--;

        return true;
    }


    // Check whether two valid elements belong to the same set
    bool connected(int first, int second)
    {
        int firstRoot = find_set(first);
        int secondRoot = find_set(second);

        return firstRoot != -1 && firstRoot == secondRoot;
    }


    // Return the number of elements in an element's set
    // Returns -1 if the element is invalid
    int get_set_size(int element)
    {
        int root = find_set(element);
        if (root == -1)
        {
            return -1;
        }

        return Size[root];
    }


    // Print each representative and the elements in its set
    int display()
    {
        if (length == 0)
        {
            cout << "There are no elements." << endl;
            return 0;
        }

        bool* displayed = new bool[length];
        for (int i = 0; i < length; i++)
        {
            displayed[i] = false;
        }

        for (int i = 0; i < length; i++)
        {
            int root = find_set(i);

            if (!displayed[root])
            {
                displayed[root] = true;
                cout << "Set " << root << ": ";

                for (int element = 0; element < length; element++)
                {
                    if (find_set(element) == root)
                    {
                        cout << element << " ";
                    }
                }

                cout << endl;
            }
        }

        delete[] displayed;
        return 1;
    }


    // Restore every element as a separate set
    int clear()
    {
        for (int i = 0; i < length; i++)
        {
            Parent[i] = i;
            Size[i] = 1;
        }

        componentCount = length;
        return 1;
    }


    // Check if there are no elements
    bool is_empty()
    {
        return length == 0;
    }


    // Return number of elements
    int get_length()
    {
        return length;
    }


    // Return number of disjoint sets
    int get_component_count()
    {
        return componentCount;
    }


    // Return total capacity
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
    cout << "    Welcome To Disjoint Set Application" << endl;
    cout << "============================================" << endl;

    int size;
    do {
        cout << "\nEnter starting number of elements: ";
        read_input(size);
        if (size < 0) {
            cout << "Number of elements cannot be negative!" << endl;
        }
    } while (size < 0);

    Disjoint_Set_Number sets(size);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Search Operations" << endl;
        cout << "3. Set Operations" << endl;
        cout << "4. Set Information" << endl;
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
                cout << "1. Add New Element" << endl;
                cout << "2. Unite Two Sets" << endl;
                cout << "3. Reset All Sets" << endl;
                cout << "4. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    int element = sets.add_set();
                    cout << "Element added successfully. Element number: " << element << endl;
                }
                else if (choice == 2) {
                    int first;
                    int second;
                    cout << "\nEnter first element (0 to " << (sets.get_length() - 1) << "): ";
                    read_input(first);
                    cout << "Enter second element: ";
                    read_input(second);

                    if (sets.unite(first, second)) {
                        cout << "Sets united successfully." << endl;
                    }
                    else if (sets.connected(first, second)) {
                        cout << "These elements are already in the same set." << endl;
                    }
                    else {
                        cout << "Invalid element number." << endl;
                    }
                }
                else if (choice == 3) {
                    sets.clear();
                    cout << "All elements are now separate sets." << endl;
                }
                else if (choice != 4) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 4);
        }
        else if (mainChoice == 2) {
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "              SEARCH OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Check If Two Elements Are Connected" << endl;
                cout << "2. Find Set Representative" << endl;
                cout << "3. Get Set Size" << endl;
                cout << "4. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    int first;
                    int second;
                    cout << "\nEnter first element: ";
                    read_input(first);
                    cout << "Enter second element: ";
                    read_input(second);

                    if (sets.find_set(first) == -1 || sets.find_set(second) == -1) {
                        cout << "Invalid element number." << endl;
                    }
                    else if (sets.connected(first, second)) {
                        cout << "The elements are connected." << endl;
                    }
                    else {
                        cout << "The elements are not connected." << endl;
                    }
                }
                else if (choice == 2 || choice == 3) {
                    int element;
                    cout << "\nEnter element: ";
                    read_input(element);

                    if (choice == 2) {
                        int representative = sets.find_set(element);
                        if (representative != -1) {
                            cout << "Set representative: " << representative << endl;
                        }
                        else {
                            cout << "Invalid element number." << endl;
                        }
                    }
                    else {
                        int setSize = sets.get_set_size(element);
                        if (setSize != -1) {
                            cout << "Set size: " << setSize << endl;
                        }
                        else {
                            cout << "Invalid element number." << endl;
                        }
                    }
                }
                else if (choice != 4) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 4);
        }
        else if (mainChoice == 3) {
            cout << "\nCurrent sets:" << endl;
            sets.display();
        }
        else if (mainChoice == 4) {
            cout << "\nElements: " << sets.get_length() << endl;
            cout << "Disjoint sets: " << sets.get_component_count() << endl;
            cout << "Capacity: " << sets.get_capacity() << endl;
            cout << "Empty: " << (sets.is_empty() ? "Yes" : "No") << endl;
        }
        else if (mainChoice != 5) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 5);

    cout << "\nThank you for using Disjoint Set Application." << endl;
    return 0;
}
