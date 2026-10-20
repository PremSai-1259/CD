
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

int main() {
    string input;
    cout << "Enter input (use id, +, *, (, )): ";
    cin >> input;
    vector<string> tokens;
    for (int i = 0; i < (int)input.size();) {
        if (input.substr(i, 2) == "id") {
            tokens.push_back("id");
            i += 2;
        } else if (input[i] == ' ') {
            i++;
        } else {
            tokens.push_back(string(1, input[i]));
            i++;
        }
    }
    tokens.push_back("$");
    int action[12][6] = {
        {5, 0, 0, 4, 0, 0},
        {0, 6, 0, 0, 0, 999},
        {0, -2, 7, 0, -2, -2},
        {0, -4, -4, 0, -4, -4},
        {5, 0, 0, 4, 0, 0},
        {0, -6, -6, 0, -6, -6},
        {5, 0, 0, 4, 0, 0},
        {5, 0, 0, 4, 0, 0},
        {0, 6, 0, 0, 11, 0},
        {0, -1, 7, 0, -1, -1},
        {0, -3, -3, 0, -3, -3},
        {0, -5, -5, 0, -5, -5}
    };
    int goTo[12][3] = {
        {1, 2, 3},
        {-1, -1, -1},
        {-1, -1, -1},
        {-1, -1, -1},
        {8, 2, 3},
        {-1, -1, -1},
        {-1, 9, 3},
        {-1, -1, 10},
        {-1, -1, -1},
        {-1, -1, -1},
        {-1, -1, -1},
        {-1, -1, -1}
    };
    string productions[] = {
        "",
        "E -> E + T",
        "E -> T",
        "T -> T * F",
        "T -> F",
        "F -> ( E )",
        "F -> id"
    };
    int length[] = {0, 3, 1, 3, 1, 3, 1};
    char lhs[] = {' ', 'E', 'E', 'T', 'T', 'F', 'F'};
    auto terminalIndex = [](string s) {
        if (s == "id") return 0;
        if (s == "+") return 1;
        if (s == "*") return 2;
        if (s == "(") return 3;
        if (s == ")") return 4;
        if (s == "$") return 5;
        return -1;
    };
    vector<int> states = {0};
    vector<string> symbols;
    int pos = 0;
    int step = 1;
    cout << "\n";
    cout << left
         << setw(7) << "Step"
         << setw(17) << "STACK"
         << setw(15) << "SYMBOLS"
         << setw(20) << "INPUT"
         << "ACTION\n";
    cout << string(85, '-') << "\n";
    while (true) {
        string stackText;
        string symbolText;
        string remaining;
        for (int i = 0; i < (int)states.size(); i++) {
            stackText += to_string(states[i]) + " ";
        }
        for (int i = 0; i < (int)symbols.size(); i++) {
            symbolText += symbols[i] + " ";
        }
        for (int i = pos; i < (int)tokens.size(); i++) {
            remaining += tokens[i] + " ";
        }
        int state = states.back();
        int col = terminalIndex(tokens[pos]);
        cout << left
             << setw(7) << step
             << setw(17) << stackText
             << setw(15) << symbolText
             << setw(20) << remaining;
        if (col == -1) {
            cout << "Invalid input\n";
            break;
        }
        int act = action[state][col];
        if (act == 999) {
            cout << "Accept\n";
            break;
        }
        if (act > 0) {
            cout << "Shift " << act << "\n";
            symbols.push_back(tokens[pos]);
            states.push_back(act);
            pos++;
        } else if (act < 0) {
            int rule = -act;
            cout << "Reduce by " << productions[rule] << "\n";
            for (int i = 0; i < length[rule]; i++) {
                if (symbols.empty() || states.size() <= 1) {
                    cout << "Parsing error\n";
                    return 0;
                }
                symbols.pop_back();
                states.pop_back();
            }
            int index = lhs[rule] == 'E' ? 0 :
                        lhs[rule] == 'T' ? 1 : 2;
            int nextState = goTo[states.back()][index];
            if (nextState == -1) {
                cout << "Parsing error\n";
                break;
            }
            symbols.push_back(string(1, lhs[rule]));
            states.push_back(nextState);
        } else {
            cout << "Error\n";
            break;
        }
        step++;
    }
    return 0;
}
