#include <iostream>
#include <limits>
#include <new>

using namespace std;

class Bitset {
private:
    unsigned long long* words;
    size_t bitCount;
    size_t wordCount;

    static size_t words_for(size_t bits)
    {
        return bits / 64 + (bits % 64 == 0 ? 0 : 1);
    }

    bool valid_index(size_t index) const
    {
        return index < bitCount;
    }

    void mask_unused_bits()
    {
        if (wordCount != 0 && bitCount % 64 != 0)
        {
            const size_t used = bitCount % 64;
            words[wordCount - 1] &= (1ULL << used) - 1ULL;
        }
    }

public:
    explicit Bitset(size_t bits = 0)
        : words(nullptr), bitCount(bits), wordCount(words_for(bits))
    {
        if (wordCount != 0)
        {
            words = new unsigned long long[wordCount];
            for (size_t i = 0; i < wordCount; ++i)
            {
                words[i] = 0;
            }
        }
    }

    ~Bitset()
    {
        delete[] words;
    }

    Bitset(const Bitset& other)
        : words(nullptr), bitCount(other.bitCount), wordCount(other.wordCount)
    {
        if (wordCount != 0)
        {
            words = new unsigned long long[wordCount];
            for (size_t i = 0; i < wordCount; ++i)
            {
                words[i] = other.words[i];
            }
        }
    }

    Bitset& operator=(const Bitset& other)
    {
        if (this != &other)
        {
            Bitset copy(other);
            swap(copy);
        }
        return *this;
    }

    void swap(Bitset& other)
    {
        unsigned long long* savedWords = words;
        words = other.words;
        other.words = savedWords;

        size_t savedBits = bitCount;
        bitCount = other.bitCount;
        other.bitCount = savedBits;

        size_t savedWordCount = wordCount;
        wordCount = other.wordCount;
        other.wordCount = savedWordCount;
    }

    size_t size() const
    {
        return bitCount;
    }

    bool set(size_t index)
    {
        if (!valid_index(index))
        {
            return false;
        }
        words[index / 64] |= 1ULL << (index % 64);
        return true;
    }

    bool reset(size_t index)
    {
        if (!valid_index(index))
        {
            return false;
        }
        words[index / 64] &= ~(1ULL << (index % 64));
        return true;
    }

    bool flip(size_t index)
    {
        if (!valid_index(index))
        {
            return false;
        }
        words[index / 64] ^= 1ULL << (index % 64);
        return true;
    }

    bool test(size_t index) const
    {
        return valid_index(index) &&
            (words[index / 64] & (1ULL << (index % 64))) != 0;
    }

    bool test(size_t index, bool& value) const
    {
        if (!valid_index(index))
        {
            return false;
        }
        value = test(index);
        return true;
    }

    size_t count() const
    {
        size_t total = 0;
        for (size_t i = 0; i < wordCount; ++i)
        {
            unsigned long long value = words[i];
            while (value != 0)
            {
                value &= value - 1ULL;
                ++total;
            }
        }
        return total;
    }

    bool any() const
    {
        for (size_t i = 0; i < wordCount; ++i)
        {
            if (words[i] != 0)
            {
                return true;
            }
        }
        return false;
    }

    bool none() const
    {
        return !any();
    }

    bool all() const
    {
        if (bitCount == 0)
        {
            return true;
        }

        for (size_t i = 0; i + 1 < wordCount; ++i)
        {
            if (words[i] != ~0ULL)
            {
                return false;
            }
        }

        const size_t remaining = bitCount % 64;
        const unsigned long long expected =
            remaining == 0 ? ~0ULL : (1ULL << remaining) - 1ULL;
        return words[wordCount - 1] == expected;
    }

    bool union_with(const Bitset& other)
    {
        if (bitCount != other.bitCount)
        {
            return false;
        }
        for (size_t i = 0; i < wordCount; ++i)
        {
            words[i] |= other.words[i];
        }
        mask_unused_bits();
        return true;
    }

    bool intersection_with(const Bitset& other)
    {
        if (bitCount != other.bitCount)
        {
            return false;
        }
        for (size_t i = 0; i < wordCount; ++i)
        {
            words[i] &= other.words[i];
        }
        mask_unused_bits();
        return true;
    }

    bool xor_with(const Bitset& other)
    {
        if (bitCount != other.bitCount)
        {
            return false;
        }
        for (size_t i = 0; i < wordCount; ++i)
        {
            words[i] ^= other.words[i];
        }
        mask_unused_bits();
        return true;
    }

    bool difference_with(const Bitset& other)
    {
        if (bitCount != other.bitCount)
        {
            return false;
        }
        for (size_t i = 0; i < wordCount; ++i)
        {
            words[i] &= ~other.words[i];
        }
        mask_unused_bits();
        return true;
    }

    void clear()
    {
        for (size_t i = 0; i < wordCount; ++i)
        {
            words[i] = 0;
        }
    }

    void display() const
    {
        cout << "Bitset (" << bitCount << " bits; index 0 is rightmost): ";
        if (bitCount == 0)
        {
            cout << "(empty)\n";
            return;
        }
        for (size_t i = bitCount; i > 0; --i)
        {
            cout << (test(i - 1) ? '1' : '0');
        }
        cout << '\n';
    }
};

#ifndef BITSET_NO_MAIN
int main()
{
    size_t bitCount;
    cout << "Enter bitset size: ";
    if (!(cin >> bitCount))
    {
        return 0;
    }

    Bitset bits(bitCount);
    int choice = 0;
    do
    {
        cout << "\n--- Bitset ---\n"
             << "1. Set bit\n"
             << "2. Reset bit\n"
             << "3. Flip bit\n"
             << "4. Test bit\n"
             << "5. Count set bits\n"
             << "6. any / none / all\n"
             << "7. Bitwise operation with another bitset\n"
             << "8. Clear\n"
             << "9. Display\n"
             << "0. Exit\n"
             << "Choice: ";
        if (!(cin >> choice))
        {
            return 0;
        }

        if (choice >= 1 && choice <= 4)
        {
            size_t index;
            cout << "Enter bit index (0 to " << (bitCount == 0 ? 0 : bitCount - 1)
                 << "): ";
            if (!(cin >> index))
            {
                return 0;
            }
            if (choice == 1)
            {
                cout << (bits.set(index) ? "Bit set.\n" : "Index out of range.\n");
            }
            else if (choice == 2)
            {
                cout << (bits.reset(index) ? "Bit reset.\n"
                                           : "Index out of range.\n");
            }
            else if (choice == 3)
            {
                cout << (bits.flip(index) ? "Bit flipped.\n"
                                          : "Index out of range.\n");
            }
            else
            {
                bool value;
                if (bits.test(index, value))
                {
                    cout << "Bit " << index << " is " << (value ? 1 : 0) << ".\n";
                }
                else
                {
                    cout << "Index out of range.\n";
                }
            }
        }
        else if (choice == 5)
        {
            cout << "Set bits: " << bits.count() << '\n';
        }
        else if (choice == 6)
        {
            cout << "any: " << (bits.any() ? "true" : "false")
                 << ", none: " << (bits.none() ? "true" : "false")
                 << ", all: " << (bits.all() ? "true" : "false") << '\n';
        }
        else if (choice == 7)
        {
            cout << "Operation (1=union, 2=intersection, 3=xor, 4=difference): ";
            int operation;
            if (!(cin >> operation))
            {
                return 0;
            }

            Bitset other(bitCount);
            size_t numberToSet;
            cout << "Number of set bits in the other bitset: ";
            if (!(cin >> numberToSet))
            {
                return 0;
            }
            cout << "Enter its bit indices: ";
            for (size_t i = 0; i < numberToSet; ++i)
            {
                size_t index;
                if (!(cin >> index))
                {
                    return 0;
                }
                if (!other.set(index))
                {
                    cout << "Ignored out-of-range index " << index << ".\n";
                }
            }

            bool success = false;
            if (operation == 1)
            {
                success = bits.union_with(other);
            }
            else if (operation == 2)
            {
                success = bits.intersection_with(other);
            }
            else if (operation == 3)
            {
                success = bits.xor_with(other);
            }
            else if (operation == 4)
            {
                success = bits.difference_with(other);
            }

            if (success)
            {
                cout << "Operation complete.\n";
                bits.display();
            }
            else
            {
                cout << "Invalid operation or incompatible bitset lengths.\n";
            }
        }
        else if (choice == 8)
        {
            bits.clear();
            cout << "Bitset cleared.\n";
        }
        else if (choice == 9)
        {
            bits.display();
        }
        else if (choice != 0)
        {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
#endif
