#include <iostream>
#include <cstdlib>
using namespace std;

class Fenwick_Tree_Number {
private:

    long long* Array;
    long long* Tree;
    int length;

private:

    bool is_valid_index(int index)
    {
        return index >= 0 && index < length;
    }


    int least_significant_bit(int index)
    {
        return index & -index;
    }


    void build_tree()
    {
        for (int i = 1; i <= length; i++)
        {
            Tree[i] += Array[i - 1];

            int parent = i + least_significant_bit(i);
            if (parent <= length)
            {
                Tree[parent] += Tree[i];
            }
        }
    }

public:

    // Constructor
    // Creates a Fenwick tree with Length zero-valued elements
    Fenwick_Tree_Number(int Length = 0)
    {
        length = (Length > 0) ? Length : 0;
        Array = (length > 0) ? new long long[length] : nullptr;
        Tree = new long long[length + 1];

        for (int i = 0; i < length; i++)
        {
            Array[i] = 0;
        }
        for (int i = 0; i <= length; i++)
        {
            Tree[i] = 0;
        }
    }


    // Destructor
    // Releases dynamically allocated memory
    ~Fenwick_Tree_Number()
    {
        delete[] Array;
        delete[] Tree;
    }


    // Copy Constructor
    // Creates a deep copy of another Fenwick tree
    Fenwick_Tree_Number(const Fenwick_Tree_Number& other)
    {
        length = other.length;
        Array = (length > 0) ? new long long[length] : nullptr;
        Tree = new long long[length + 1];

        for (int i = 0; i < length; i++)
        {
            Array[i] = other.Array[i];
        }
        for (int i = 0; i <= length; i++)
        {
            Tree[i] = other.Tree[i];
        }
    }


    // Assignment Operator
    // Performs deep copy between Fenwick trees
    Fenwick_Tree_Number& operator=(const Fenwick_Tree_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        long long* newArray = (other.length > 0)
            ? new long long[other.length]
            : nullptr;
        long long* newTree = new long long[other.length + 1];

        for (int i = 0; i < other.length; i++)
        {
            newArray[i] = other.Array[i];
        }
        for (int i = 0; i <= other.length; i++)
        {
            newTree[i] = other.Tree[i];
        }

        delete[] Array;
        delete[] Tree;
        Array = newArray;
        Tree = newTree;
        length = other.length;
        return *this;
    }


    // Build the tree from values in O(n) time
    // Existing values are replaced
    bool build(const long long* values, int count)
    {
        if (count < 0 || (count > 0 && values == nullptr))
        {
            return false;
        }

        long long* newArray = (count > 0) ? new long long[count] : nullptr;
        long long* newTree = new long long[count + 1];

        for (int i = 0; i < count; i++)
        {
            newArray[i] = values[i];
        }
        for (int i = 0; i <= count; i++)
        {
            newTree[i] = 0;
        }

        delete[] Array;
        delete[] Tree;
        Array = newArray;
        Tree = newTree;
        length = count;

        build_tree();
        return true;
    }


    // Add delta to the value at index
    bool add(int index, long long delta)
    {
        if (!is_valid_index(index))
        {
            return false;
        }

        Array[index] += delta;

        int treeIndex = index + 1;
        while (treeIndex <= length)
        {
            Tree[treeIndex] += delta;
            treeIndex += least_significant_bit(treeIndex);
        }

        return true;
    }


    // Set the value at index
    bool set(int index, long long value)
    {
        if (!is_valid_index(index))
        {
            return false;
        }

        return add(index, value - Array[index]);
    }


    // Get the value at index through value
    bool get(int index, long long& value)
    {
        if (!is_valid_index(index))
        {
            return false;
        }

        value = Array[index];
        return true;
    }


    // Get the sum of values from index 0 through index, inclusive
    bool prefix_sum(int index, long long& sum)
    {
        if (index < -1 || index >= length)
        {
            return false;
        }

        sum = 0;
        int treeIndex = index + 1;

        while (treeIndex > 0)
        {
            sum += Tree[treeIndex];
            treeIndex -= least_significant_bit(treeIndex);
        }

        return true;
    }


    // Get the sum between left and right, inclusive
    bool range_sum(int left, int right, long long& sum)
    {
        if (!is_valid_index(left) || !is_valid_index(right) || left > right)
        {
            return false;
        }

        long long rightSum;
        long long leftSum;
        prefix_sum(right, rightSum);
        prefix_sum(left - 1, leftSum);
        sum = rightSum - leftSum;
        return true;
    }


    // Print every value and the Fenwick tree's one-based array
    int display()
    {
        if (length == 0)
        {
            cout << "Fenwick tree is empty." << endl;
            return 0;
        }

        cout << "Values (zero-based indices): ";
        for (int i = 0; i < length; i++)
        {
            cout << Array[i] << " ";
        }
        cout << endl;

        cout << "Fenwick array (one-based indices): ";
        for (int i = 1; i <= length; i++)
        {
            cout << Tree[i] << " ";
        }
        cout << endl;
        return 1;
    }


    // Set every value to zero
    int clear()
    {
        for (int i = 0; i < length; i++)
        {
            Array[i] = 0;
        }
        for (int i = 0; i <= length; i++)
        {
            Tree[i] = 0;
        }

        return 1;
    }


    // Check if there are no values
    bool is_empty()
    {
        return length == 0;
    }


    // Return number of stored values
    int get_length()
    {
        return length;
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


void read_input(long long& value)
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
    cout << "      Welcome To Fenwick Tree Application" << endl;
    cout << "============================================" << endl;
    cout << "Array indices start at 0. Range endpoints are inclusive." << endl;

    int length;
    do {
        cout << "\nEnter number of values: ";
        read_input(length);
        if (length < 0) {
            cout << "Number of values cannot be negative!" << endl;
        }
    } while (length < 0);

    Fenwick_Tree_Number tree(length);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Search Operations" << endl;
        cout << "3. Range Sum Operations" << endl;
        cout << "4. Tree Information" << endl;
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
                cout << "1. Add To Value" << endl;
                cout << "2. Set Value" << endl;
                cout << "3. Clear All Values" << endl;
                cout << "4. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1 || choice == 2) {
                    int index;
                    long long value;
                    cout << "\nEnter index: ";
                    read_input(index);
                    cout << (choice == 1 ? "Enter amount to add: " : "Enter new value: ");
                    read_input(value);

                    bool success = (choice == 1)
                        ? tree.add(index, value)
                        : tree.set(index, value);

                    if (success) {
                        cout << (choice == 1 ? "Value updated successfully." : "Value set successfully.") << endl;
                    }
                    else {
                        cout << "Invalid index." << endl;
                    }
                }
                else if (choice == 3) {
                    tree.clear();
                    cout << "All values set to zero." << endl;
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
                cout << "1. Get Value At Index" << endl;
                cout << "2. Get Prefix Sum" << endl;
                cout << "3. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1) {
                    int index;
                    long long value;
                    cout << "\nEnter index: ";
                    read_input(index);
                    if (tree.get(index, value)) {
                        cout << "Value: " << value << endl;
                    }
                    else {
                        cout << "Invalid index." << endl;
                    }
                }
                else if (choice == 2) {
                    int index;
                    long long sum;
                    cout << "\nEnter ending index (-1 for an empty prefix): ";
                    read_input(index);
                    if (tree.prefix_sum(index, sum)) {
                        cout << "Prefix sum: " << sum << endl;
                    }
                    else {
                        cout << "Invalid index." << endl;
                    }
                }
                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 3);
        }
        else if (mainChoice == 3) {
            int left;
            int right;
            long long sum;
            cout << "\nEnter left index: ";
            read_input(left);
            cout << "Enter right index: ";
            read_input(right);
            if (tree.range_sum(left, right, sum)) {
                cout << "Range sum: " << sum << endl;
            }
            else {
                cout << "Invalid range or index." << endl;
            }
        }
        else if (mainChoice == 4) {
            cout << "\nValues: " << tree.get_length() << endl;
            cout << "Empty: " << (tree.is_empty() ? "Yes" : "No") << endl;
            tree.display();
        }
        else if (mainChoice != 5) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 5);

    cout << "\nThank you for using Fenwick Tree Application." << endl;
    return 0;
}
