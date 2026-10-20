#include <iostream>
#include <cstdlib>
#include <climits>
using namespace std;

struct Segment_Tree_Result {
    long long sum;
    long long minimum;
    long long maximum;
    bool valid;

    Segment_Tree_Result()
    {
        sum = 0;
        minimum = LLONG_MAX;
        maximum = LLONG_MIN;
        valid = false;
    }

    Segment_Tree_Result(long long Sum, long long Minimum, long long Maximum)
    {
        sum = Sum;
        minimum = Minimum;
        maximum = Maximum;
        valid = true;
    }
};


class Segment_Tree_Number {
private:

    long long* Sums;
    long long* Minimums;
    long long* Maximums;
    long long* Lazy;
    int length;
    int treeSize;

private:

    int get_tree_size(int Length)
    {
        return (Length > 0) ? 4 * Length : 1;
    }


    Segment_Tree_Result combine(const Segment_Tree_Result& left,
        const Segment_Tree_Result& right)
    {
        if (!left.valid)
        {
            return right;
        }
        if (!right.valid)
        {
            return left;
        }

        long long minimum = (left.minimum < right.minimum)
            ? left.minimum
            : right.minimum;
        long long maximum = (left.maximum > right.maximum)
            ? left.maximum
            : right.maximum;

        return Segment_Tree_Result(left.sum + right.sum, minimum, maximum);
    }


    void build_tree(int node, int left, int right, const long long* values)
    {
        if (left == right)
        {
            Sums[node] = values[left];
            Minimums[node] = values[left];
            Maximums[node] = values[left];
            Lazy[node] = 0;
            return;
        }

        int middle = left + (right - left) / 2;
        build_tree(node * 2, left, middle, values);
        build_tree((node * 2) + 1, middle + 1, right, values);
        update_parent(node);
    }


    void update_parent(int node)
    {
        Sums[node] = Sums[node * 2] + Sums[(node * 2) + 1];
        Minimums[node] = (Minimums[node * 2] < Minimums[(node * 2) + 1])
            ? Minimums[node * 2]
            : Minimums[(node * 2) + 1];
        Maximums[node] = (Maximums[node * 2] > Maximums[(node * 2) + 1])
            ? Maximums[node * 2]
            : Maximums[(node * 2) + 1];
    }


    void apply_add(int node, int left, int right, long long delta)
    {
        Sums[node] += delta * (right - left + 1);
        Minimums[node] += delta;
        Maximums[node] += delta;
        Lazy[node] += delta;
    }


    void push_down(int node, int left, int right)
    {
        if (Lazy[node] == 0 || left == right)
        {
            return;
        }

        int middle = left + (right - left) / 2;
        apply_add(node * 2, left, middle, Lazy[node]);
        apply_add((node * 2) + 1, middle + 1, right, Lazy[node]);
        Lazy[node] = 0;
    }


    void add_range(int node, int left, int right, int queryLeft,
        int queryRight, long long delta)
    {
        if (queryLeft <= left && right <= queryRight)
        {
            apply_add(node, left, right, delta);
            return;
        }

        push_down(node, left, right);
        int middle = left + (right - left) / 2;

        if (queryLeft <= middle)
        {
            add_range(node * 2, left, middle, queryLeft, queryRight, delta);
        }
        if (queryRight > middle)
        {
            add_range((node * 2) + 1, middle + 1, right,
                queryLeft, queryRight, delta);
        }

        update_parent(node);
    }


    Segment_Tree_Result query_range(int node, int left, int right,
        int queryLeft, int queryRight)
    {
        if (queryLeft <= left && right <= queryRight)
        {
            return Segment_Tree_Result(Sums[node], Minimums[node], Maximums[node]);
        }

        push_down(node, left, right);
        int middle = left + (right - left) / 2;
        Segment_Tree_Result result;

        if (queryLeft <= middle)
        {
            result = combine(result,
                query_range(node * 2, left, middle, queryLeft, queryRight));
        }
        if (queryRight > middle)
        {
            result = combine(result,
                query_range((node * 2) + 1, middle + 1, right,
                    queryLeft, queryRight));
        }

        return result;
    }


    bool is_valid_index(int index)
    {
        return index >= 0 && index < length;
    }


    bool is_valid_range(int left, int right)
    {
        return left >= 0 && right >= 0 && left <= right && right < length;
    }


    void delete_arrays()
    {
        delete[] Sums;
        delete[] Minimums;
        delete[] Maximums;
        delete[] Lazy;
    }

public:

    // Constructor
    // Creates a segment tree with Length zero-valued elements
    Segment_Tree_Number(int Length = 0)
    {
        length = (Length > 0) ? Length : 0;
        treeSize = get_tree_size(length);

        Sums = new long long[treeSize];
        Minimums = new long long[treeSize];
        Maximums = new long long[treeSize];
        Lazy = new long long[treeSize];

        for (int i = 0; i < treeSize; i++)
        {
            Sums[i] = 0;
            Minimums[i] = 0;
            Maximums[i] = 0;
            Lazy[i] = 0;
        }

        if (length > 0)
        {
            long long* values = new long long[length];
            for (int i = 0; i < length; i++)
            {
                values[i] = 0;
            }
            build_tree(1, 0, length - 1, values);
            delete[] values;
        }
    }


    // Destructor
    // Releases dynamically allocated memory
    ~Segment_Tree_Number()
    {
        delete_arrays();
    }


    // Copy Constructor
    // Creates a deep copy of another segment tree
    Segment_Tree_Number(const Segment_Tree_Number& other)
    {
        length = other.length;
        treeSize = other.treeSize;
        Sums = new long long[treeSize];
        Minimums = new long long[treeSize];
        Maximums = new long long[treeSize];
        Lazy = new long long[treeSize];

        for (int i = 0; i < treeSize; i++)
        {
            Sums[i] = other.Sums[i];
            Minimums[i] = other.Minimums[i];
            Maximums[i] = other.Maximums[i];
            Lazy[i] = other.Lazy[i];
        }
    }


    // Assignment Operator
    // Performs deep copy between segment trees
    Segment_Tree_Number& operator=(const Segment_Tree_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        long long* newSums = new long long[other.treeSize];
        long long* newMinimums = new long long[other.treeSize];
        long long* newMaximums = new long long[other.treeSize];
        long long* newLazy = new long long[other.treeSize];

        for (int i = 0; i < other.treeSize; i++)
        {
            newSums[i] = other.Sums[i];
            newMinimums[i] = other.Minimums[i];
            newMaximums[i] = other.Maximums[i];
            newLazy[i] = other.Lazy[i];
        }

        delete_arrays();
        Sums = newSums;
        Minimums = newMinimums;
        Maximums = newMaximums;
        Lazy = newLazy;
        length = other.length;
        treeSize = other.treeSize;
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

        int newTreeSize = get_tree_size(count);
        long long* newSums = new long long[newTreeSize];
        long long* newMinimums = new long long[newTreeSize];
        long long* newMaximums = new long long[newTreeSize];
        long long* newLazy = new long long[newTreeSize];

        for (int i = 0; i < newTreeSize; i++)
        {
            newSums[i] = 0;
            newMinimums[i] = 0;
            newMaximums[i] = 0;
            newLazy[i] = 0;
        }

        delete_arrays();
        Sums = newSums;
        Minimums = newMinimums;
        Maximums = newMaximums;
        Lazy = newLazy;
        length = count;
        treeSize = newTreeSize;

        if (length > 0)
        {
            build_tree(1, 0, length - 1, values);
        }

        return true;
    }


    // Add delta to every value in the inclusive range
    bool add_range(int left, int right, long long delta)
    {
        if (!is_valid_range(left, right))
        {
            return false;
        }

        add_range(1, 0, length - 1, left, right, delta);
        return true;
    }


    // Set one value
    bool set(int index, long long value)
    {
        if (!is_valid_index(index))
        {
            return false;
        }

        long long currentValue;
        long long minimum;
        long long maximum;
        query_range(index, index, currentValue, minimum, maximum);
        return add_range(index, index, value - currentValue);
    }


    // Add delta to one value
    bool add(int index, long long delta)
    {
        return add_range(index, index, delta);
    }


    // Return the value at index through value
    bool get(int index, long long& value)
    {
        if (!is_valid_index(index))
        {
            return false;
        }

        long long minimum;
        long long maximum;
        return query_range(index, index, value, minimum, maximum);
    }


    // Get sum, minimum, and maximum for an inclusive range
    bool query_range(int left, int right, long long& sum,
        long long& minimum, long long& maximum)
    {
        if (!is_valid_range(left, right))
        {
            return false;
        }

        Segment_Tree_Result result = query_range(1, 0, length - 1, left, right);
        sum = result.sum;
        minimum = result.minimum;
        maximum = result.maximum;
        return true;
    }


    // Print every value in index order
    int display()
    {
        if (length == 0)
        {
            cout << "Segment tree is empty." << endl;
            return 0;
        }

        for (int i = 0; i < length; i++)
        {
            long long value;
            long long minimum;
            long long maximum;
            query_range(i, i, value, minimum, maximum);
            cout << value << " ";
        }
        cout << endl;
        return 1;
    }


    // Set every value to zero
    int clear()
    {
        for (int i = 0; i < treeSize; i++)
        {
            Sums[i] = 0;
            Minimums[i] = 0;
            Maximums[i] = 0;
            Lazy[i] = 0;
        }

        if (length > 0)
        {
            long long* values = new long long[length];
            for (int i = 0; i < length; i++)
            {
                values[i] = 0;
            }
            build_tree(1, 0, length - 1, values);
            delete[] values;
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
    cout << "       Welcome To Segment Tree Application" << endl;
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

    Segment_Tree_Number tree(length);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Range Queries" << endl;
        cout << "3. Tree Operations" << endl;
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
                cout << "1. Set Value" << endl;
                cout << "2. Add To Value" << endl;
                cout << "3. Add To Range" << endl;
                cout << "4. Clear All Values" << endl;
                cout << "5. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(choice);

                if (choice == 1 || choice == 2) {
                    int index;
                    long long value;
                    cout << "\nEnter index: ";
                    read_input(index);
                    cout << (choice == 1 ? "Enter new value: " : "Enter amount to add: ");
                    read_input(value);

                    bool success = (choice == 1)
                        ? tree.set(index, value)
                        : tree.add(index, value);

                    if (success) {
                        cout << "Value updated successfully." << endl;
                    }
                    else {
                        cout << "Invalid index." << endl;
                    }
                }
                else if (choice == 3) {
                    int left;
                    int right;
                    long long delta;
                    cout << "\nEnter left index: ";
                    read_input(left);
                    cout << "Enter right index: ";
                    read_input(right);
                    cout << "Enter amount to add: ";
                    read_input(delta);
                    if (tree.add_range(left, right, delta)) {
                        cout << "Range updated successfully." << endl;
                    }
                    else {
                        cout << "Invalid range or index." << endl;
                    }
                }
                else if (choice == 4) {
                    tree.clear();
                    cout << "All values set to zero." << endl;
                }
                else if (choice != 5) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 5);
        }
        else if (mainChoice == 2) {
            int left;
            int right;
            long long sum;
            long long minimum;
            long long maximum;
            cout << "\nEnter left index: ";
            read_input(left);
            cout << "Enter right index: ";
            read_input(right);

            if (tree.query_range(left, right, sum, minimum, maximum)) {
                cout << "Range sum: " << sum << endl;
                cout << "Range minimum: " << minimum << endl;
                cout << "Range maximum: " << maximum << endl;
            }
            else {
                cout << "Invalid range or index." << endl;
            }
        }
        else if (mainChoice == 3) {
            int choice;

            do {
                cout << "\n\n============================================" << endl;
                cout << "               TREE OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Build From Values" << endl;
                cout << "2. Display Values" << endl;
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
                        long long* values = (count > 0)
                            ? new long long[count]
                            : nullptr;

                        for (int i = 0; i < count; i++) {
                            cout << "Enter value " << i << ": ";
                            read_input(values[i]);
                        }

                        tree.build(values, count);
                        delete[] values;
                        cout << "Segment tree built successfully." << endl;
                    }
                }
                else if (choice == 2) {
                    cout << "\nValues: ";
                    tree.display();
                }
                else if (choice != 3) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (choice != 3);
        }
        else if (mainChoice == 4) {
            cout << "\nValues: " << tree.get_length() << endl;
            cout << "Empty: " << (tree.is_empty() ? "Yes" : "No") << endl;
            cout << "Array values: ";
            tree.display();
        }
        else if (mainChoice != 5) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 5);

    cout << "\nThank you for using Segment Tree Application." << endl;
    return 0;
}
