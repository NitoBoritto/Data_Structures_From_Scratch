#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
using namespace std;

struct Suffix_Automaton_State {
    int maximumLength;
    int suffixLink;
    int firstPosition;
    unsigned long long occurrenceCount;
    map<unsigned char, int> transitions;

    Suffix_Automaton_State()
    {
        maximumLength = 0;
        suffixLink = -1;
        firstPosition = -1;
        occurrenceCount = 0;
    }
};


class Suffix_Automaton_String {
private:

    string text;
    vector<Suffix_Automaton_State> states;
    int lastState;

private:

    void reset()
    {
        text.clear();
        states.clear();
        states.push_back(Suffix_Automaton_State());
        lastState = 0;
    }


    void extend(unsigned char character)
    {
        int currentState = static_cast<int>(states.size());
        states.push_back(Suffix_Automaton_State());
        states[currentState].maximumLength = states[lastState].maximumLength + 1;
        states[currentState].firstPosition = static_cast<int>(text.length());
        states[currentState].occurrenceCount = 1;

        int state = lastState;
        while (state != -1
            && states[state].transitions.find(character) == states[state].transitions.end())
        {
            states[state].transitions[character] = currentState;
            state = states[state].suffixLink;
        }

        if (state == -1)
        {
            states[currentState].suffixLink = 0;
        }
        else
        {
            int nextState = states[state].transitions[character];

            if (states[state].maximumLength + 1 == states[nextState].maximumLength)
            {
                states[currentState].suffixLink = nextState;
            }
            else
            {
                int cloneState = static_cast<int>(states.size());
                states.push_back(states[nextState]);
                states[cloneState].maximumLength = states[state].maximumLength + 1;
                states[cloneState].occurrenceCount = 0;

                while (state != -1)
                {
                    map<unsigned char, int>::iterator transition =
                        states[state].transitions.find(character);

                    if (transition == states[state].transitions.end()
                        || transition->second != nextState)
                    {
                        break;
                    }

                    transition->second = cloneState;
                    state = states[state].suffixLink;
                }

                states[nextState].suffixLink = cloneState;
                states[currentState].suffixLink = cloneState;
            }
        }

        lastState = currentState;
        text.push_back(static_cast<char>(character));
    }


    vector<unsigned long long> count_occurrences_by_state()
    {
        vector<unsigned long long> counts(states.size(), 0);
        vector<int> order(states.size());

        for (size_t i = 0; i < states.size(); i++)
        {
            counts[i] = states[i].occurrenceCount;
            order[i] = static_cast<int>(i);
        }

        sort(order.begin(), order.end(),
            [this](int first, int second) {
                return states[first].maximumLength > states[second].maximumLength;
            });

        for (size_t i = 0; i < order.size(); i++)
        {
            int state = order[i];
            if (states[state].suffixLink != -1)
            {
                counts[states[state].suffixLink] += counts[state];
            }
        }

        return counts;
    }

public:

    // Constructor
    // Creates an empty suffix automaton
    Suffix_Automaton_String()
    {
        reset();
    }


    // Constructor
    // Builds the automaton for text
    Suffix_Automaton_String(const string& Text)
    {
        reset();
        build(Text);
    }


    // Replace stored text and rebuild the automaton
    int build(const string& Text)
    {
        reset();
        for (size_t i = 0; i < Text.length(); i++)
        {
            extend(static_cast<unsigned char>(Text[i]));
        }

        return 1;
    }


    // Append text while maintaining the automaton
    int append(const string& addition)
    {
        for (size_t i = 0; i < addition.length(); i++)
        {
            extend(static_cast<unsigned char>(addition[i]));
        }

        return 1;
    }


    // Check whether pattern occurs in the text
    bool contains(const string& pattern)
    {
        int state = 0;

        for (size_t i = 0; i < pattern.length(); i++)
        {
            map<unsigned char, int>::iterator transition =
                states[state].transitions.find(
                    static_cast<unsigned char>(pattern[i])
                );

            if (transition == states[state].transitions.end())
            {
                return false;
            }

            state = transition->second;
        }

        return true;
    }


    // Count occurrences of a substring
    unsigned long long count_occurrences(const string& pattern)
    {
        if (pattern.empty())
        {
            return static_cast<unsigned long long>(text.length()) + 1;
        }

        int state = 0;
        for (size_t i = 0; i < pattern.length(); i++)
        {
            map<unsigned char, int>::iterator transition =
                states[state].transitions.find(
                    static_cast<unsigned char>(pattern[i])
                );

            if (transition == states[state].transitions.end())
            {
                return 0;
            }

            state = transition->second;
        }

        vector<unsigned long long> counts = count_occurrences_by_state();
        return counts[state];
    }


    // Return the number of distinct non-empty substrings
    unsigned long long count_distinct_substrings()
    {
        unsigned long long count = 0;

        for (size_t i = 1; i < states.size(); i++)
        {
            count += static_cast<unsigned long long>(
                states[i].maximumLength
                - states[states[i].suffixLink].maximumLength
            );
        }

        return count;
    }


    // Return one longest repeated substring
    bool get_longest_repeated_substring(string& result)
    {
        vector<unsigned long long> counts = count_occurrences_by_state();
        int bestState = -1;

        for (size_t i = 1; i < states.size(); i++)
        {
            if (counts[i] >= 2
                && (bestState == -1
                    || states[i].maximumLength > states[bestState].maximumLength))
            {
                bestState = static_cast<int>(i);
            }
        }

        if (bestState == -1)
        {
            result.clear();
            return false;
        }

        int start = states[bestState].firstPosition
            - states[bestState].maximumLength + 1;
        result = text.substr(start, states[bestState].maximumLength);
        return true;
    }


    // Find the longest substring shared with another text
    // Returns its length and start positions in both strings
    int longest_common_substring(const string& other, int& textStart,
        int& otherStart)
    {
        int state = 0;
        int currentLength = 0;
        int bestLength = 0;
        int bestEnd = -1;
        int bestOtherEnd = -1;

        for (size_t i = 0; i < other.length(); i++)
        {
            unsigned char character = static_cast<unsigned char>(other[i]);
            map<unsigned char, int>::iterator transition =
                states[state].transitions.find(character);

            while (state != 0 && transition == states[state].transitions.end())
            {
                state = states[state].suffixLink;
                currentLength = states[state].maximumLength;
                transition = states[state].transitions.find(character);
            }

            if (transition != states[state].transitions.end())
            {
                state = transition->second;
                currentLength++;

                if (currentLength > bestLength)
                {
                    bestLength = currentLength;
                    bestEnd = states[state].firstPosition;
                    bestOtherEnd = static_cast<int>(i);
                }
            }
            else
            {
                state = 0;
                currentLength = 0;
            }
        }

        if (bestLength == 0)
        {
            textStart = -1;
            otherStart = -1;
            return 0;
        }

        textStart = bestEnd - bestLength + 1;
        otherStart = bestOtherEnd - bestLength + 1;
        return bestLength;
    }


    // Return original text
    string get_text()
    {
        return text;
    }


    // Return number of text bytes
    int get_length()
    {
        return static_cast<int>(text.length());
    }


    // Return number of states
    int get_state_count()
    {
        return static_cast<int>(states.size());
    }


    // Check whether the automaton represents empty text
    bool is_empty()
    {
        return text.empty();
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
    cout << "     Welcome To Suffix Automaton Application" << endl;
    cout << "============================================" << endl;
    cout << "Indices and lengths count bytes; matching is case-sensitive." << endl;

    string text;
    read_text(text);
    Suffix_Automaton_String automaton(text);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Search Substring" << endl;
        cout << "2. Count Substring Occurrences" << endl;
        cout << "3. Count Distinct Substrings" << endl;
        cout << "4. Longest Repeated Substring" << endl;
        cout << "5. Longest Common Substring" << endl;
        cout << "6. Append Text" << endl;
        cout << "7. Rebuild For New Text" << endl;
        cout << "8. Text Information" << endl;
        cout << "9. Exit" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        read_input(mainChoice);

        if (mainChoice == 1 || mainChoice == 2) {
            string pattern;
            read_text(pattern);
            if (mainChoice == 1) {
                cout << (automaton.contains(pattern)
                    ? "Substring found."
                    : "Substring not found.") << endl;
            }
            else {
                cout << "Occurrences: "
                     << automaton.count_occurrences(pattern) << endl;
            }
        }
        else if (mainChoice == 3) {
            cout << "\nDistinct non-empty substrings: "
                 << automaton.count_distinct_substrings() << endl;
        }
        else if (mainChoice == 4) {
            string repeated;
            if (automaton.get_longest_repeated_substring(repeated)) {
                cout << "Longest repeated substring: " << repeated << endl;
            }
            else {
                cout << "No non-empty repeated substring." << endl;
            }
        }
        else if (mainChoice == 5) {
            string other;
            int textStart;
            int otherStart;
            read_text(other);
            int length = automaton.longest_common_substring(other, textStart, otherStart);

            if (length == 0) {
                cout << "No common non-empty substring." << endl;
            }
            else {
                cout << "Longest common substring: "
                     << automaton.get_text().substr(textStart, length) << endl;
                cout << "Start positions: " << textStart
                     << " and " << otherStart << endl;
            }
        }
        else if (mainChoice == 6) {
            string addition;
            read_text(addition);
            automaton.append(addition);
            cout << "Text appended successfully." << endl;
        }
        else if (mainChoice == 7) {
            read_text(text);
            automaton.build(text);
            cout << "Automaton rebuilt successfully." << endl;
        }
        else if (mainChoice == 8) {
            cout << "\nText length: " << automaton.get_length() << endl;
            cout << "Automaton states: " << automaton.get_state_count() << endl;
            cout << "Distinct substrings: "
                 << automaton.count_distinct_substrings() << endl;
            cout << "Empty: " << (automaton.is_empty() ? "Yes" : "No") << endl;
        }
        else if (mainChoice != 9) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 9);

    cout << "\nThank you for using Suffix Automaton Application." << endl;
    return 0;
}
