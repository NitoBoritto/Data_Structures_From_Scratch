#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstdint>
using namespace std;

class Bloom_Filter_Number {
private:

    unsigned char* Bits;
    int bitCount;
    int hashCount;
    unsigned long long insertions;

private:

    unsigned long long mix(unsigned long long value)
    {
        value ^= value >> 30;
        value *= 0xbf58476d1ce4e5b9ULL;
        value ^= value >> 27;
        value *= 0x94d049bb133111ebULL;
        value ^= value >> 31;
        return value;
    }


    unsigned long long hash_first(int value)
    {
        return mix(static_cast<unsigned int>(value) + 0x9e3779b97f4a7c15ULL);
    }


    unsigned long long hash_second(int value)
    {
        return mix(static_cast<unsigned int>(value) + 0x6a09e667f3bcc909ULL) | 1ULL;
    }


    int get_bit_index(int value, int hashNumber)
    {
        unsigned long long first = hash_first(value);
        unsigned long long second = hash_second(value);
        unsigned long long combined = first + (hashNumber * second);
        return static_cast<int>(combined % static_cast<unsigned long long>(bitCount));
    }


    void set_bit(int index)
    {
        int byteIndex = index / 8;
        int bitOffset = index % 8;
        Bits[byteIndex] |= static_cast<unsigned char>(1U << bitOffset);
    }


    bool get_bit(int index)
    {
        int byteIndex = index / 8;
        int bitOffset = index % 8;
        return (Bits[byteIndex] & static_cast<unsigned char>(1U << bitOffset)) != 0;
    }

public:

    // Constructor
    // Creates a filter with the requested number of bits and hash functions
    Bloom_Filter_Number(int Bit_Count, int Hash_Count)
    {
        bitCount = (Bit_Count > 0) ? Bit_Count : 1;
        hashCount = (Hash_Count > 0) ? Hash_Count : 1;
        int byteCount = (bitCount + 7) / 8;
        Bits = new unsigned char[byteCount];
        insertions = 0;

        for (int i = 0; i < byteCount; i++)
        {
            Bits[i] = 0;
        }
    }


    // Destructor
    // Releases the bit array
    ~Bloom_Filter_Number()
    {
        delete[] Bits;
    }


    // Copy Constructor
    // Creates a deep copy of another filter
    Bloom_Filter_Number(const Bloom_Filter_Number& other)
    {
        bitCount = other.bitCount;
        hashCount = other.hashCount;
        insertions = other.insertions;
        int byteCount = (bitCount + 7) / 8;
        Bits = new unsigned char[byteCount];

        for (int i = 0; i < byteCount; i++)
        {
            Bits[i] = other.Bits[i];
        }
    }


    // Assignment Operator
    // Performs deep copy between filters
    Bloom_Filter_Number& operator=(const Bloom_Filter_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        int byteCount = (other.bitCount + 7) / 8;
        unsigned char* newBits = new unsigned char[byteCount];

        for (int i = 0; i < byteCount; i++)
        {
            newBits[i] = other.Bits[i];
        }

        delete[] Bits;
        Bits = newBits;
        bitCount = other.bitCount;
        hashCount = other.hashCount;
        insertions = other.insertions;
        return *this;
    }


    // Add a value to the filter
    // Bloom filters do not store the original values
    int add(int value)
    {
        for (int i = 0; i < hashCount; i++)
        {
            set_bit(get_bit_index(value, i));
        }

        insertions++;
        return 1;
    }


    // False means definitely absent; true means probably present
    bool possibly_contains(int value)
    {
        for (int i = 0; i < hashCount; i++)
        {
            if (!get_bit(get_bit_index(value, i)))
            {
                return false;
            }
        }

        return true;
    }


    // Estimate false-positive probability from insertions and filter size
    double estimated_false_positive_rate()
    {
        double bits = static_cast<double>(bitCount);
        double hashes = static_cast<double>(hashCount);
        double items = static_cast<double>(insertions);
        return pow(1.0 - exp(-(hashes * items) / bits), hashes);
    }


    // Return configured number of bits
    int get_bit_count()
    {
        return bitCount;
    }


    // Return configured number of hash functions
    int get_hash_count()
    {
        return hashCount;
    }


    // Return total number of add calls (duplicates are counted again)
    unsigned long long get_insertions()
    {
        return insertions;
    }


    // Remove all values by clearing every bit
    int clear()
    {
        int byteCount = (bitCount + 7) / 8;
        for (int i = 0; i < byteCount; i++)
        {
            Bits[i] = 0;
        }

        insertions = 0;
        return 1;
    }


    // Check if no values have been added since the last clear
    bool is_empty()
    {
        return insertions == 0;
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
    cout << "        Welcome To Bloom Filter Application" << endl;
    cout << "============================================" << endl;
    cout << "A positive result means possibly present; a negative is definitely absent." << endl;

    int bitCount;
    int hashCount;

    do {
        cout << "\nEnter number of bits: ";
        read_input(bitCount);
        if (bitCount <= 0) {
            cout << "Bit count must be greater than zero!" << endl;
        }
    } while (bitCount <= 0);

    do {
        cout << "Enter number of hash functions: ";
        read_input(hashCount);
        if (hashCount <= 0) {
            cout << "Hash count must be greater than zero!" << endl;
        }
    } while (hashCount <= 0);

    Bloom_Filter_Number filter(bitCount, hashCount);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Add Number" << endl;
        cout << "2. Check Number" << endl;
        cout << "3. Filter Information" << endl;
        cout << "4. Clear Filter" << endl;
        cout << "5. Exit" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        read_input(mainChoice);

        if (mainChoice == 1) {
            int value;
            cout << "\nEnter number to add: ";
            read_input(value);
            filter.add(value);
            cout << "Number added to the filter." << endl;
        }
        else if (mainChoice == 2) {
            int value;
            cout << "\nEnter number to check: ";
            read_input(value);
            if (filter.possibly_contains(value)) {
                cout << "Number is possibly present (a false positive is possible)." << endl;
            }
            else {
                cout << "Number is definitely absent." << endl;
            }
        }
        else if (mainChoice == 3) {
            cout << "\nBits: " << filter.get_bit_count() << endl;
            cout << "Hash functions: " << filter.get_hash_count() << endl;
            cout << "Insertions: " << filter.get_insertions() << endl;
            cout << "Estimated false-positive rate: "
                 << filter.estimated_false_positive_rate() * 100.0 << "%" << endl;
            cout << "Empty: " << (filter.is_empty() ? "Yes" : "No") << endl;
        }
        else if (mainChoice == 4) {
            filter.clear();
            cout << "Filter cleared successfully." << endl;
        }
        else if (mainChoice != 5) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 5);

    cout << "\nThank you for using Bloom Filter Application." << endl;
    return 0;
}
