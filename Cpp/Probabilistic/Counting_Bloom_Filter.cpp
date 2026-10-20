#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <limits>
#include <unordered_map>
#include <vector>
using namespace std;

class Counting_Bloom_Filter
{
private:
    vector<unsigned char> counters;
    int hashCount;
    unsigned long long insertions;
    unordered_map<int, unsigned long long> keyCounts;

private:
    unsigned long long mix(unsigned long long value) const
    {
        value ^= value >> 30;
        value *= 0xbf58476d1ce4e5b9ULL;
        value ^= value >> 27;
        value *= 0x94d049bb133111ebULL;
        value ^= value >> 31;
        return value;
    }

    unsigned long long hash_first(int value) const
    {
        return mix(static_cast<unsigned int>(value) + 0x9e3779b97f4a7c15ULL);
    }

    unsigned long long hash_second(int value) const
    {
        return mix(static_cast<unsigned int>(value) + 0x6a09e667f3bcc909ULL) | 1ULL;
    }

    size_t get_counter_index(int value, int hashNumber) const
    {
        unsigned long long first = hash_first(value);
        unsigned long long second = hash_second(value);
        unsigned long long combined =
            first + static_cast<unsigned long long>(hashNumber) * second;
        return static_cast<size_t>(combined % static_cast<unsigned long long>(counters.size()));
    }

public:
    // Creates a filter with the requested number of counters and hash functions.
    Counting_Bloom_Filter(size_t Counter_Count, int Hash_Count)
        : counters(Counter_Count > 0 ? Counter_Count : 1, 0),
          hashCount(Hash_Count > 0 ? Hash_Count : 1),
          insertions(0)
    {
    }

    // vector and unordered_map copy their owned storage, so this is a deep copy.
    Counting_Bloom_Filter(const Counting_Bloom_Filter& other)
        : counters(other.counters),
          hashCount(other.hashCount),
          insertions(other.insertions),
          keyCounts(other.keyCounts)
    {
    }

    // Assignment also copies the owned counters and key multiplicities.
    Counting_Bloom_Filter& operator=(const Counting_Bloom_Filter& other)
    {
        if (this != &other)
        {
            counters = other.counters;
            hashCount = other.hashCount;
            insertions = other.insertions;
            keyCounts = other.keyCounts;
        }
        return *this;
    }

    // Adds one occurrence of a key. Counters saturate rather than wrapping.
    bool add(int value)
    {
        if (insertions == numeric_limits<unsigned long long>::max())
        {
            return false;
        }

        unordered_map<int, unsigned long long>::iterator found = keyCounts.find(value);
        if (found == keyCounts.end())
        {
            keyCounts.emplace(value, 1);
        }
        else
        {
            if (found->second == numeric_limits<unsigned long long>::max())
            {
                return false;
            }
            ++found->second;
        }

        const unsigned char maximum = numeric_limits<unsigned char>::max();
        for (int i = 0; i < hashCount; ++i)
        {
            size_t index = get_counter_index(value, i);
            if (counters[index] < maximum)
            {
                ++counters[index];
            }
        }

        ++insertions;
        return true;
    }

    // Removes one occurrence only when that key has previously been added.
    bool remove(int value)
    {
        unordered_map<int, unsigned long long>::iterator found = keyCounts.find(value);
        if (found == keyCounts.end())
        {
            return false;
        }

        const unsigned char maximum = numeric_limits<unsigned char>::max();
        for (int i = 0; i < hashCount; ++i)
        {
            size_t index = get_counter_index(value, i);
            // A saturated counter may represent a larger count. Leave it saturated
            // rather than risk making a still-present key appear definitely absent.
            if (counters[index] < maximum && counters[index] > 0)
            {
                --counters[index];
            }
        }

        --insertions;
        if (found->second == 1)
        {
            keyCounts.erase(found);
        }
        else
        {
            --found->second;
        }
        return true;
    }

    // False means definitely absent; true means possibly present.
    bool possibly_contains(int value) const
    {
        for (int i = 0; i < hashCount; ++i)
        {
            if (counters[get_counter_index(value, i)] == 0)
            {
                return false;
            }
        }
        return true;
    }

    // Estimates the false-positive rate from the fraction of occupied counters.
    double estimated_false_positive_rate() const
    {
        size_t occupied = 0;
        for (size_t i = 0; i < counters.size(); ++i)
        {
            if (counters[i] != 0)
            {
                ++occupied;
            }
        }

        double occupiedFraction =
            static_cast<double>(occupied) / static_cast<double>(counters.size());
        return pow(occupiedFraction, static_cast<double>(hashCount));
    }

    size_t get_counter_count() const
    {
        return counters.size();
    }

    int get_hash_count() const
    {
        return hashCount;
    }

    unsigned long long get_insertions() const
    {
        return insertions;
    }

    size_t get_distinct_key_count() const
    {
        return keyCounts.size();
    }

    bool is_empty() const
    {
        return insertions == 0;
    }

    // Removes all keys and resets every counter.
    int clear()
    {
        for (size_t i = 0; i < counters.size(); ++i)
        {
            counters[i] = 0;
        }
        keyCounts.clear();
        insertions = 0;
        return 1;
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
    cout << "   Welcome To Counting Bloom Filter" << endl;
    cout << "============================================" << endl;
    cout << "A positive result means possibly present, not certainly present." << endl;

    int counterCount;
    int hashCount;

    do {
        cout << "\nEnter number of counters: ";
        read_input(counterCount);
        if (counterCount <= 0) {
            cout << "Counter count must be greater than zero!" << endl;
        }
    } while (counterCount <= 0);

    do {
        cout << "Enter number of hash functions: ";
        read_input(hashCount);
        if (hashCount <= 0) {
            cout << "Hash count must be greater than zero!" << endl;
        }
    } while (hashCount <= 0);

    Counting_Bloom_Filter filter(static_cast<size_t>(counterCount), hashCount);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Add Number" << endl;
        cout << "2. Remove Number" << endl;
        cout << "3. Check Number" << endl;
        cout << "4. Filter Information" << endl;
        cout << "5. Clear Filter" << endl;
        cout << "6. Exit" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        read_input(mainChoice);

        if (mainChoice == 1) {
            int value;
            cout << "\nEnter number to add: ";
            read_input(value);
            if (filter.add(value)) {
                cout << "Number added to the filter." << endl;
            }
            else {
                cout << "Unable to add: insertion count limit reached." << endl;
            }
        }
        else if (mainChoice == 2) {
            int value;
            cout << "\nEnter number to remove: ";
            read_input(value);
            if (filter.remove(value)) {
                cout << "One occurrence removed." << endl;
            }
            else {
                cout << "Number was not inserted; the filter was not changed." << endl;
            }
        }
        else if (mainChoice == 3) {
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
        else if (mainChoice == 4) {
            cout << "\nCounters: " << filter.get_counter_count() << endl;
            cout << "Hash functions: " << filter.get_hash_count() << endl;
            cout << "Insertions: " << filter.get_insertions() << endl;
            cout << "Distinct keys tracked: " << filter.get_distinct_key_count() << endl;
            cout << "Estimated false-positive rate: "
                 << filter.estimated_false_positive_rate() * 100.0 << "%" << endl;
            cout << "Estimate is probabilistic; a positive query is never proof of membership."
                 << endl;
            cout << "Saturated counters may stay high until clear, increasing false positives."
                 << endl;
            cout << "Empty: " << (filter.is_empty() ? "Yes" : "No") << endl;
        }
        else if (mainChoice == 5) {
            filter.clear();
            cout << "Filter cleared successfully." << endl;
        }
        else if (mainChoice != 6) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 6);

    cout << "\nThank you for using Counting Bloom Filter." << endl;
    return 0;
}
