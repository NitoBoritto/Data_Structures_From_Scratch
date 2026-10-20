#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

class Suffix_Array_String {
private:

    string text;
    vector<int> suffixArray;
    vector<int> inverseRank;
    vector<int> lcpArray;
    vector< vector<int> > lcpTable;
    vector<int> logarithms;

private:

    void build_suffix_array()
    {
        int length = static_cast<int>(text.length());
        suffixArray.resize(length);
        inverseRank.resize(length);

        if (length == 0)
        {
            return;
        }

        vector<int> ranks(length);
        vector<int> nextRanks(length);

        for (int i = 0; i < length; i++)
        {
            suffixArray[i] = i;
            ranks[i] = static_cast<unsigned char>(text[i]);
        }

        for (int width = 1; width < length; width *= 2)
        {
            sort(suffixArray.begin(), suffixArray.end(),
                [&ranks, width, length](int first, int second) {
                    if (ranks[first] != ranks[second])
                    {
                        return ranks[first] < ranks[second];
                    }

                    int firstSecondRank = (first + width < length)
                        ? ranks[first + width]
                        : -1;
                    int secondSecondRank = (second + width < length)
                        ? ranks[second + width]
                        : -1;
                    return firstSecondRank < secondSecondRank;
                });

            nextRanks[suffixArray[0]] = 0;
            for (int i = 1; i < length; i++)
            {
                int previous = suffixArray[i - 1];
                int current = suffixArray[i];
                bool different = ranks[previous] != ranks[current];

                int previousSecondRank = (previous + width < length)
                    ? ranks[previous + width]
                    : -1;
                int currentSecondRank = (current + width < length)
                    ? ranks[current + width]
                    : -1;

                if (previousSecondRank != currentSecondRank)
                {
                    different = true;
                }

                nextRanks[current] = nextRanks[previous] + (different ? 1 : 0);
            }

            ranks.swap(nextRanks);

            if (ranks[suffixArray[length - 1]] == length - 1)
            {
                break;
            }

            if (width > length / 2)
            {
                break;
            }
        }

        for (int rank = 0; rank < length; rank++)
        {
            inverseRank[suffixArray[rank]] = rank;
        }
    }


    void build_lcp_array()
    {
        int length = static_cast<int>(text.length());
        lcpArray.assign(length, 0);
        int commonLength = 0;

        for (int start = 0; start < length; start++)
        {
            int rank = inverseRank[start];
            if (rank == 0)
            {
                commonLength = 0;
                continue;
            }

            int previousStart = suffixArray[rank - 1];
            while (start + commonLength < length
                && previousStart + commonLength < length
                && text[start + commonLength] == text[previousStart + commonLength])
            {
                commonLength++;
            }

            lcpArray[rank] = commonLength;
            if (commonLength > 0)
            {
                commonLength--;
            }
        }
    }


    void build_lcp_table()
    {
        int length = static_cast<int>(text.length());
        logarithms.assign(length + 1, 0);

        for (int i = 2; i <= length; i++)
        {
            logarithms[i] = logarithms[i / 2] + 1;
        }

        int levels = (length > 0) ? logarithms[length] + 1 : 0;
        lcpTable.assign(levels, vector<int>(length, 0));

        if (length == 0)
        {
            return;
        }

        lcpTable[0] = lcpArray;

        for (int level = 1; level < levels; level++)
        {
            int halfLength = 1 << (level - 1);
            int segmentLength = 1 << level;

            for (int start = 0; start + segmentLength <= length; start++)
            {
                lcpTable[level][start] = min(
                    lcpTable[level - 1][start],
                    lcpTable[level - 1][start + halfLength]
                );
            }
        }
    }


    int compare_suffix_with_pattern(int suffixStart, const string& pattern)
    {
        int offset = 0;

        while (offset < static_cast<int>(pattern.length())
            && suffixStart + offset < static_cast<int>(text.length()))
        {
            unsigned char textCharacter =
                static_cast<unsigned char>(text[suffixStart + offset]);
            unsigned char patternCharacter =
                static_cast<unsigned char>(pattern[offset]);

            if (textCharacter < patternCharacter)
            {
                return -1;
            }
            if (textCharacter > patternCharacter)
            {
                return 1;
            }

            offset++;
        }

        if (offset == static_cast<int>(pattern.length()))
        {
            return 0;
        }

        return -1;
    }


    int lower_bound_pattern(const string& pattern)
    {
        int low = 0;
        int high = static_cast<int>(suffixArray.size());

        while (low < high)
        {
            int middle = low + (high - low) / 2;
            if (compare_suffix_with_pattern(suffixArray[middle], pattern) < 0)
            {
                low = middle + 1;
            }
            else
            {
                high = middle;
            }
        }

        return low;
    }


    int upper_bound_pattern(const string& pattern)
    {
        int low = 0;
        int high = static_cast<int>(suffixArray.size());

        while (low < high)
        {
            int middle = low + (high - low) / 2;
            if (compare_suffix_with_pattern(suffixArray[middle], pattern) <= 0)
            {
                low = middle + 1;
            }
            else
            {
                high = middle;
            }
        }

        return low;
    }

public:

    // Constructor
    // Creates an empty suffix array
    Suffix_Array_String()
    {
        build("");
    }


    // Constructor
    // Builds suffix and LCP arrays for text
    Suffix_Array_String(const string& Text)
    {
        build(Text);
    }


    // Rebuild for new text; suffix arrays are static between builds
    int build(const string& Text)
    {
        text = Text;
        build_suffix_array();
        build_lcp_array();
        build_lcp_table();
        return 1;
    }


    // Count occurrences of pattern using binary search over suffixes
    int count_occurrences(const string& pattern)
    {
        if (pattern.empty())
        {
            return static_cast<int>(text.length()) + 1;
        }

        int first = lower_bound_pattern(pattern);
        int last = upper_bound_pattern(pattern);
        return last - first;
    }


    // Return all matching start positions in suffix-array order
    // If output is too small, set requiredCount and return false
    bool find_occurrences(const string& pattern, int* output,
        int outputCapacity, int& requiredCount)
    {
        requiredCount = count_occurrences(pattern);

        if (outputCapacity < 0
            || (requiredCount > 0 && outputCapacity > 0 && output == nullptr))
        {
            return false;
        }
        if (outputCapacity < requiredCount)
        {
            return false;
        }
        if (requiredCount > 0 && output == nullptr)
        {
            return false;
        }

        if (pattern.empty())
        {
            for (int i = 0; i < requiredCount; i++)
            {
                output[i] = i;
            }
            return true;
        }

        int first = lower_bound_pattern(pattern);
        for (int i = 0; i < requiredCount; i++)
        {
            output[i] = suffixArray[first + i];
        }

        if (requiredCount > 1)
        {
            sort(output, output + requiredCount);
        }
        return true;
    }


    // Return the LCP length of suffixes beginning at two positions
    bool get_lcp(int firstStart, int secondStart, int& result)
    {
        int length = static_cast<int>(text.length());
        if (firstStart < 0 || secondStart < 0
            || firstStart >= length || secondStart >= length)
        {
            return false;
        }

        if (firstStart == secondStart)
        {
            result = length - firstStart;
            return true;
        }

        int leftRank = inverseRank[firstStart];
        int rightRank = inverseRank[secondStart];
        if (leftRank > rightRank)
        {
            int temp = leftRank;
            leftRank = rightRank;
            rightRank = temp;
        }

        int left = leftRank + 1;
        int rangeLength = rightRank - leftRank;
        int level = logarithms[rangeLength];
        int segmentLength = 1 << level;
        result = min(
            lcpTable[level][left],
            lcpTable[level][rightRank - segmentLength + 1]
        );
        return true;
    }


    // Return a longest repeated substring through result
    bool get_longest_repeated_substring(string& result)
    {
        if (text.empty())
        {
            return false;
        }

        int bestRank = 0;
        for (int rank = 1; rank < static_cast<int>(lcpArray.size()); rank++)
        {
            if (lcpArray[rank] > lcpArray[bestRank])
            {
                bestRank = rank;
            }
        }

        if (lcpArray[bestRank] == 0)
        {
            result.clear();
            return false;
        }

        result = text.substr(suffixArray[bestRank], lcpArray[bestRank]);
        return true;
    }


    // Print suffixes in sorted order
    int display()
    {
        if (suffixArray.empty())
        {
            cout << "Suffix array is empty." << endl;
            return 0;
        }

        for (size_t rank = 0; rank < suffixArray.size(); rank++)
        {
            int start = suffixArray[rank];
            cout << "[" << start << "] " << text.substr(start)
                 << " (LCP: " << lcpArray[rank] << ")" << endl;
        }
        return 1;
    }


    // Return text length
    int get_length()
    {
        return static_cast<int>(text.length());
    }


    // Check whether text is empty
    bool is_empty()
    {
        return text.empty();
    }


    // Return a suffix-array rank's original start position
    bool get_suffix_start(int rank, int& start)
    {
        if (rank < 0 || rank >= static_cast<int>(suffixArray.size()))
        {
            return false;
        }

        start = suffixArray[rank];
        return true;
    }
};


bool read_text(string& value)
{
    cout << "\nEnter text: ";
    if (!getline(cin >> ws, value))
    {
        cerr << "Input error: expected text." << endl;
        std::exit(EXIT_FAILURE);
    }
    return true;
}


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
    cout << "       Welcome To Suffix Array Application" << endl;
    cout << "============================================" << endl;
    cout << "Indices and lengths count bytes; matching is case-sensitive." << endl;

    string text;
    read_text(text);
    Suffix_Array_String suffixArray(text);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Search Pattern" << endl;
        cout << "2. Get LCP Of Two Suffixes" << endl;
        cout << "3. Longest Repeated Substring" << endl;
        cout << "4. Display Suffix Array" << endl;
        cout << "5. Rebuild For New Text" << endl;
        cout << "6. Text Information" << endl;
        cout << "7. Exit" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        read_input(mainChoice);

        if (mainChoice == 1) {
            string pattern;
            read_text(pattern);
            int count = suffixArray.count_occurrences(pattern);
            cout << "Occurrences: " << count << endl;
            int* positions = (count > 0) ? new int[count] : nullptr;
            int requiredCount;
            if (suffixArray.find_occurrences(pattern, positions, count, requiredCount)) {
                cout << "Start positions: ";
                for (int i = 0; i < requiredCount; i++) {
                    cout << positions[i] << " ";
                }
                cout << endl;
            }
            delete[] positions;
        }
        else if (mainChoice == 2) {
            int first;
            int second;
            int length;
            cout << "\nEnter first suffix start: ";
            read_input(first);
            cout << "Enter second suffix start: ";
            read_input(second);
            if (suffixArray.get_lcp(first, second, length)) {
                cout << "Common prefix length: " << length << endl;
            }
            else {
                cout << "Invalid suffix start." << endl;
            }
        }
        else if (mainChoice == 3) {
            string repeated;
            if (suffixArray.get_longest_repeated_substring(repeated)) {
                cout << "Longest repeated substring: " << repeated << endl;
            }
            else {
                cout << "No non-empty repeated substring." << endl;
            }
        }
        else if (mainChoice == 4) {
            suffixArray.display();
        }
        else if (mainChoice == 5) {
            read_text(text);
            suffixArray.build(text);
            cout << "Suffix array rebuilt successfully." << endl;
        }
        else if (mainChoice == 6) {
            cout << "\nText length: " << suffixArray.get_length() << endl;
            cout << "Empty: " << (suffixArray.is_empty() ? "Yes" : "No") << endl;
        }
        else if (mainChoice != 7) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 7);

    cout << "\nThank you for using Suffix Array Application." << endl;
    return 0;
}
