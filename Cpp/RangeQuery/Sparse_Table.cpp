#include <iostream>
#include <cstdlib>
using namespace std;

struct Sparse_Table_Result {
    long long minimum;
    long long maximum;
    unsigned long long gcd;
};


class Sparse_Table_Number {
private:

    long long** Minimums;
    long long** Maximums;
    unsigned long long** Gcds;
    int* Logarithms;
    int length;
    int levels;

private:

    unsigned long long absolute_value(long long value)
    {
        if (value >= 0)
        {
            return static_cast<unsigned long long>(value);
        }

        return static_cast<unsigned long long>(-(value + 1)) + 1;
    }


    unsigned long long greatest_common_divisor(long long first, long long second)
    {
        unsigned long long a = absolute_value(first);
        unsigned long long b = absolute_value(second);

        while (b != 0)
        {
            unsigned long long remainder = a % b;
            a = b;
            b = remainder;
        }

        return a;
    }


    void delete_tables()
    {
        if (Minimums != nullptr)
        {
            for (int i = 0; i < levels; i++)
            {
                delete[] Minimums[i];
                delete[] Maximums[i];
                delete[] Gcds[i];
            }
        }

        delete[] Minimums;
        delete[] Maximums;
        delete[] Gcds;
        delete[] Logarithms;
    }


    void allocate_tables(int Length)
    {
        length = Length;
        levels = 0;

        if (length > 0)
        {
            levels = 1;
            while ((1 << levels) <= length)
            {
                levels++;
            }
        }

        Minimums = (levels > 0) ? new long long*[levels] : nullptr;
        Maximums = (levels > 0) ? new long long*[levels] : nullptr;
        Gcds = (levels > 0) ? new unsigned long long*[levels] : nullptr;

        for (int level = 0; level < levels; level++)
        {
            Minimums[level] = new long long[length];
            Maximums[level] = new long long[length];
            Gcds[level] = new unsigned long long[length];

            for (int i = 0; i < length; i++)
            {
                Minimums[level][i] = 0;
                Maximums[level][i] = 0;
                Gcds[level][i] = 0;
            }
        }

        Logarithms = new int[length + 1];
        Logarithms[0] = 0;

        if (length > 0)
        {
            Logarithms[1] = 0;
        }

        for (int i = 2; i <= length; i++)
        {
            Logarithms[i] = Logarithms[i / 2] + 1;
        }
    }


    void build_tables(const long long* values)
    {
        for (int i = 0; i < length; i++)
        {
            Minimums[0][i] = values[i];
            Maximums[0][i] = values[i];
            Gcds[0][i] = absolute_value(values[i]);
        }

        for (int level = 1; level < levels; level++)
        {
            int halfLength = 1 << (level - 1);
            int segmentLength = 1 << level;

            for (int start = 0; start + segmentLength <= length; start++)
            {
                long long leftMinimum = Minimums[level - 1][start];
                long long rightMinimum = Minimums[level - 1][start + halfLength];
                Minimums[level][start] = (leftMinimum < rightMinimum)
                    ? leftMinimum
                    : rightMinimum;

                long long leftMaximum = Maximums[level - 1][start];
                long long rightMaximum = Maximums[level - 1][start + halfLength];
                Maximums[level][start] = (leftMaximum > rightMaximum)
                    ? leftMaximum
                    : rightMaximum;

                Gcds[level][start] = gcd_unsigned(
                    Gcds[level - 1][start],
                    Gcds[level - 1][start + halfLength]
                );
            }
        }
    }


    unsigned long long gcd_unsigned(unsigned long long first, unsigned long long second)
    {
        while (second != 0)
        {
            unsigned long long remainder = first % second;
            first = second;
            second = remainder;
        }

        return first;
    }

public:

    // Constructor
    // Creates an empty sparse table
    Sparse_Table_Number()
    {
        Minimums = nullptr;
        Maximums = nullptr;
        Gcds = nullptr;
        Logarithms = nullptr;
        length = 0;
        levels = 0;
        allocate_tables(0);
    }


    // Constructor
    // Builds the sparse table from the supplied values
    Sparse_Table_Number(const long long* values, int count)
    {
        Minimums = nullptr;
        Maximums = nullptr;
        Gcds = nullptr;
        Logarithms = nullptr;
        length = 0;
        levels = 0;
        build(values, count);
    }


    // Destructor
    // Releases all sparse-table levels
    ~Sparse_Table_Number()
    {
        delete_tables();
    }


    // Copy Constructor
    // Creates a deep copy of another sparse table
    Sparse_Table_Number(const Sparse_Table_Number& other)
    {
        Minimums = nullptr;
        Maximums = nullptr;
        Gcds = nullptr;
        Logarithms = nullptr;
        length = 0;
        levels = 0;

        allocate_tables(other.length);
        for (int level = 0; level < levels; level++)
        {
            for (int i = 0; i < length; i++)
            {
                Minimums[level][i] = other.Minimums[level][i];
                Maximums[level][i] = other.Maximums[level][i];
                Gcds[level][i] = other.Gcds[level][i];
            }
        }

        for (int i = 0; i <= length; i++)
        {
            Logarithms[i] = other.Logarithms[i];
        }
    }


    // Assignment Operator
    // Performs deep copy between sparse tables
    Sparse_Table_Number& operator=(const Sparse_Table_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        long long** newMinimums = (other.levels > 0)
            ? new long long*[other.levels]
            : nullptr;
        long long** newMaximums = (other.levels > 0)
            ? new long long*[other.levels]
            : nullptr;
        unsigned long long** newGcds = (other.levels > 0)
            ? new unsigned long long*[other.levels]
            : nullptr;

        for (int level = 0; level < other.levels; level++)
        {
            newMinimums[level] = new long long[other.length];
            newMaximums[level] = new long long[other.length];
            newGcds[level] = new unsigned long long[other.length];

            for (int i = 0; i < other.length; i++)
            {
                newMinimums[level][i] = other.Minimums[level][i];
                newMaximums[level][i] = other.Maximums[level][i];
                newGcds[level][i] = other.Gcds[level][i];
            }
        }

        int* newLogarithms = new int[other.length + 1];
        for (int i = 0; i <= other.length; i++)
        {
            newLogarithms[i] = other.Logarithms[i];
        }

        delete_tables();
        Minimums = newMinimums;
        Maximums = newMaximums;
        Gcds = newGcds;
        Logarithms = newLogarithms;
        length = other.length;
        levels = other.levels;
        return *this;
    }


    // Build or rebuild from values in O(n log n) time
    bool build(const long long* values, int count)
    {
        if (count < 0 || (count > 0 && values == nullptr))
        {
            return false;
        }

        long long** newMinimums = nullptr;
        long long** newMaximums = nullptr;
        unsigned long long** newGcds = nullptr;
        int newLevels = 0;

        if (count > 0)
        {
            newLevels = 1;
            while ((1 << newLevels) <= count)
            {
                newLevels++;
            }

            newMinimums = new long long*[newLevels];
            newMaximums = new long long*[newLevels];
            newGcds = new unsigned long long*[newLevels];

            for (int level = 0; level < newLevels; level++)
            {
                newMinimums[level] = new long long[count];
                newMaximums[level] = new long long[count];
                newGcds[level] = new unsigned long long[count];

                for (int i = 0; i < count; i++)
                {
                    newMinimums[level][i] = 0;
                    newMaximums[level][i] = 0;
                    newGcds[level][i] = 0;
                }
            }
        }

        int* newLogarithms = new int[count + 1];
        newLogarithms[0] = 0;
        if (count > 0)
        {
            newLogarithms[1] = 0;
        }
        for (int i = 2; i <= count; i++)
        {
            newLogarithms[i] = newLogarithms[i / 2] + 1;
        }

        delete_tables();
        Minimums = newMinimums;
        Maximums = newMaximums;
        Gcds = newGcds;
        Logarithms = newLogarithms;
        length = count;
        levels = newLevels;

        if (length > 0)
        {
            build_tables(values);
        }

        return true;
    }


    // Query min, max, and gcd over the inclusive range [left, right]
    bool query(int left, int right, Sparse_Table_Result& result)
    {
        if (left < 0 || right < left || right >= length)
        {
            return false;
        }

        int rangeLength = right - left + 1;
        int level = Logarithms[rangeLength];
        int segmentLength = 1 << level;
        int secondStart = right - segmentLength + 1;

        long long firstMinimum = Minimums[level][left];
        long long secondMinimum = Minimums[level][secondStart];
        result.minimum = (firstMinimum < secondMinimum)
            ? firstMinimum
            : secondMinimum;

        long long firstMaximum = Maximums[level][left];
        long long secondMaximum = Maximums[level][secondStart];
        result.maximum = (firstMaximum > secondMaximum)
            ? firstMaximum
            : secondMaximum;

        result.gcd = gcd_unsigned(
            Gcds[level][left],
            Gcds[level][secondStart]
        );

        return true;
    }


    // Print the original values
    int display()
    {
        if (length == 0)
        {
            cout << "Sparse table is empty." << endl;
            return 0;
        }

        for (int i = 0; i < length; i++)
        {
            cout << Minimums[0][i] << " ";
        }
        cout << endl;
        return 1;
    }


    // Check whether the table has no values
    bool is_empty()
    {
        return length == 0;
    }


    // Return number of stored values
    int get_length()
    {
        return length;
    }


    // Return number of precomputed levels
    int get_level_count()
    {
        return levels;
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
    cout << "       Welcome To Sparse Table Application" << endl;
    cout << "============================================" << endl;
    cout << "This table is static: rebuild it after changing the input values." << endl;

    int length;
    do {
        cout << "\nEnter number of values: ";
        read_input(length);
        if (length < 0) {
            cout << "Number of values cannot be negative!" << endl;
        }
    } while (length < 0);

    long long* values = (length > 0) ? new long long[length] : nullptr;
    for (int i = 0; i < length; i++) {
        cout << "Enter value " << i << ": ";
        if (!(cin >> values[i])) {
            cerr << "Input error: expected an integer." << endl;
            delete[] values;
            std::exit(EXIT_FAILURE);
        }
    }

    Sparse_Table_Number table(values, length);
    delete[] values;
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Query Range" << endl;
        cout << "2. Rebuild Table" << endl;
        cout << "3. Display Values" << endl;
        cout << "4. Table Information" << endl;
        cout << "5. Exit" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        read_input(mainChoice);

        if (mainChoice == 1) {
            int left;
            int right;
            cout << "\nEnter left index: ";
            read_input(left);
            cout << "Enter right index: ";
            read_input(right);

            Sparse_Table_Result result;
            if (table.query(left, right, result)) {
                cout << "Minimum: " << result.minimum << endl;
                cout << "Maximum: " << result.maximum << endl;
                cout << "GCD: " << result.gcd << endl;
            }
            else {
                cout << "Invalid range or index." << endl;
            }
        }
        else if (mainChoice == 2) {
            int count;
            cout << "\nEnter number of values: ";
            read_input(count);

            if (count < 0) {
                cout << "Number of values cannot be negative!" << endl;
            }
            else {
                long long* newValues = (count > 0) ? new long long[count] : nullptr;
                bool inputValid = true;

                for (int i = 0; i < count; i++) {
                    cout << "Enter value " << i << ": ";
                    if (!(cin >> newValues[i])) {
                        inputValid = false;
                        break;
                    }
                }

                if (!inputValid) {
                    cerr << "Input error: expected an integer." << endl;
                    delete[] newValues;
                    std::exit(EXIT_FAILURE);
                }

                table.build(newValues, count);
                delete[] newValues;
                cout << "Sparse table rebuilt successfully." << endl;
            }
        }
        else if (mainChoice == 3) {
            cout << "\nValues: ";
            table.display();
        }
        else if (mainChoice == 4) {
            cout << "\nValues: " << table.get_length() << endl;
            cout << "Precomputed levels: " << table.get_level_count() << endl;
            cout << "Empty: " << (table.is_empty() ? "Yes" : "No") << endl;
        }
        else if (mainChoice != 5) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 5);

    cout << "\nThank you for using Sparse Table Application." << endl;
    return 0;
}
