#include <bits/stdc++.h>

#include "../util/filereadwrite.h"

using namespace std;

FileReadWrite frw;

// Data structures
map<char, vector<string>> productionMap;
map<string, int> productionRuleNumberingMap;
map<int, string> RuleProductionNumberingMapForReduction;
map<set<string>, int> RuleToItemNumberMap;
map<int, set<string>> ItemNumberToRuleMap;
queue<int> itemProcessQueue;
map<pair<int, char>, string> ActionTable;
map<pair<int, char>, string> GotoTable;
map<char, set<char>> followSetMap;
set<int> ss;
char start;
map<char, set<char>> firstSetMap;

// dot will always be at the end for this function to be called
bool FirstSet(char i, char org, char last) {
    bool rtake = false;
    for (auto r : productionMap[i]) {
        bool take = true;
        for (auto s : r) {
            if (s == i) break;
            if (!take) break;
            if (!(s >= 'A' && s <= 'Z') && s != '#') {
                ss.insert(s);
                break;
            } else if (s == '#') {
                if (org == i || i == last) ss.insert(s);
                rtake = true;
                break;
            } else {
                take = FirstSet(s, org, r[r.size() - 1]);
                rtake |= take;
            }
        }
    }
    return rtake;
}

void prepareFirstSet() {
    for (auto q : productionMap) {
        ss.clear();
        FirstSet(q.first, q.first, q.first);
        for (auto g : ss) firstSetMap[q.first].insert(g);
    }
}

void prepareFollowSet() {
    // we need to compute the follow set for the symbols of the given grammar
    int i;
    followSetMap[start].insert('$');

    // performing 10 iterations to avoid any chances of symbols which are missed
    int count = 10;
    while (count--) {
        for (auto q : productionMap) {
            for (auto r : q.second) {
                for (i = 0; i < r.size() - 1; i++) {
                    if (r[i] >= 'A' && r[i] <= 'Z') {
                        // checking if the char followed by the current
                        // non-terminal is a terminal or not
                        if (!(r[i + 1] >= 'A' && r[i + 1] <= 'Z'))
                            followSetMap[r[i]].insert(r[i + 1]);
                        // if not a terminal again take the first set of this
                        // following non-terminal
                        else {
                            char temp = r[i + 1];
                            int j = i + 1;
                            while (temp >= 'A' && temp <= 'Z') {
                                if (*firstSetMap[temp].begin() == '#') {
                                    for (auto g : firstSetMap[temp]) {
                                        if (g == '#') continue;
                                        // insert all the characters in the
                                        // first set of the non-terminal in
                                        // follow set
                                        followSetMap[r[i]].insert(g);
                                    }
                                    j++;
                                    if (j < r.size()) {
                                        temp = r[j];
                                        // check if any terminal is visited yet
                                        if (!(temp >= 'A' && temp <= 'Z')) {
                                            followSetMap[r[i]].insert(temp);
                                            break;
                                        }
                                    }
                                    // if the current non-terminal say A->B,
                                    // then follow(B)=follow(A)
                                    else {
                                        for (auto g : followSetMap[q.first])
                                            followSetMap[r[i]].insert(g);
                                        break;
                                    }
                                } else {
                                    // insert the symbol in the first set of the
                                    // symbol appearing next
                                    for (auto g : firstSetMap[temp]) {
                                        followSetMap[r[i]].insert(g);
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
                // if the current non-terminal say A->B, then
                // follow(B)=follow(A)
                if (r[r.size() - 1] >= 'A' && r[r.size() - 1] <= 'Z') {
                    for (auto g : followSetMap[q.first])
                        followSetMap[r[i]].insert(g);
                }
            }
        }
    }
}

// PROCESS THE PRODUCTION RULES AND ADD EACH OF THEM IN PROPER DATA STRUCTURES
void processGrammar() {
    ifstream fin("grammar/finalGrammar.txt");
    string num;
    bool flag = 0;

    while (getline(fin, num)) {
        if (flag == 0) start = num[0], flag = 1;
        // cout << start << '\n'; // start symbol of the grammar

        // vector<char> temp;
        string temp = "";
        char s = num[0];
        for (int i = 3; i < num.size(); i++) {
            if (num[i] == '|') {
                productionMap[s].push_back(temp);
                temp.clear();
            } else
                temp.push_back(num[i]);
        }
        productionMap[s].push_back(temp);
    }
}

void insertIntoReduceRule(string str, int current) {
    cout << "Called insertIntoReduceRule for:";
    cout << str << " ";
    char leftNonTerminal =
        str[0];  // it is the left-non-terminal of the rule whose followset will
                 // contain reduce action
    // cout << "Inserted into Reduced table , Rule:" << str << "\n";
    string rule = str.substr(0, str.size() - 1);
    set<char> s =
        followSetMap[rule[0]];  // find the follow of the LHS and insert all the
                                // reduce rule number into it
    int num = productionRuleNumberingMap[rule];
    cout << "Production rule number for the rule :" << rule << "is :" << num
         << "\n";
    cout << "Start Symbol of the Grammar :" << start << "\n";

    cout << "Follow set map for the character:" << leftNonTerminal << "is:\n";
    for (char c : s) cout << c << " ";
    cout << "\n";

    for (char c : s) {
        cout << "Character c :" << c << " Character start:" << start << "\n";
        if (leftNonTerminal == start) {
            cout << "Inserted into accept state \n";
            ActionTable[{current, c}] = "AC";
        } else
            ActionTable[{current, c}] = "R" + to_string(num);
    }
}

void insertIntoActionTable(int current, char ch, int counter) {
    // cout << "Inserted into action table ,Current:" << current << "->" << ch
    // << "->" << counter << "\n";
    if (ch >= 'A' && ch <= 'Z')  // if the character is not a non-terminal ,
                                 // simply move to that state
        GotoTable[{current, ch}] = to_string(counter);
    else
        ActionTable[{current, ch}] = "S" + to_string(counter);
}

void printClosure(set<string> &RuleList) {
    cout << "Current closure Item set:\n";
    for (string s : RuleList) cout << s << "\n";

    cout << "\n";
}

void prepareClosure(set<string> &RuleList) {
    int iteration = 5;
    set<string>
        st;  // used to store the production rule closure to avoid repetition
    // set<char> presentNonTerminals;
    for (string s : RuleList) {
        // cout << "Current Rule inserted in set :" << s << "\n";
        st.insert(s);
    }

    while (iteration--) {
        for (string str : st) {
            // considering each RuleList
            // cout << "Rule under observation:" << str << "\n";
            for (int i = 0; i < str.size(); i++) {
                if ((i + 1) == str.size()) break;
                if (str[i] == '.' && str[i + 1] >= 'A' && str[i + 1] <= 'Z') {
                    // cout << "Current non-terminal is:" << str[i + 1] << "\n";

                    vector<string> rules;
                    if (productionMap.find(str[i + 1]) != productionMap.end())
                        rules = productionMap[str[i + 1]];

                    // for (string str1 : rules)
                    //     cout << "Rule:" << str1 << "\n";

                    for (string str1 : rules) {
                        // each rule is a string we need to insert in the form
                        // <nonterminal>->.rule
                        string temp = "";
                        temp += str[i + 1];
                        temp += '-';
                        temp += '>';
                        temp += '.';
                        temp += str1;
                        // cout << "New production rule insertion :" << temp <<
                        // "\n";
                        st.insert(temp);
                    }
                    break;
                }
            }
        }
    }
    // after performing a maximum of 10 checks the closure must be prepared and
    // no more non terminals should be generated
    for (string s : st) {
        RuleList.insert(s);
    }
    // cout<<"Printing the ClosureList\n";
    // for(string s:RuleList)cout<<s<<"\n";
    // Now the rulelist contains the closure of the item set
}

int main(int argc, char *argv[]) {
    set<string> RuleList;
    RuleList.insert(
        "G->.P");  // Augmenting G to the start symbol of the grammar
    processGrammar();
    prepareFirstSet();
    prepareFollowSet();  // read the grammar and store the production rules
    prepareClosure(RuleList);

    int ruleCountInitially = 0;
    for (auto itr : productionMap) {
        string currentProductionNonTerminal = "";
        currentProductionNonTerminal += itr.first;
        currentProductionNonTerminal += '-';
        currentProductionNonTerminal += '>';
        for (string s : itr.second) {
            // take out the strings and put one by one into the
            // productionRuleNumberingMap
            productionRuleNumberingMap[currentProductionNonTerminal + s] =
                ruleCountInitially;
            RuleProductionNumberingMapForReduction[ruleCountInitially++] =
                currentProductionNonTerminal + s;
        }
    }

    for (auto itr : productionRuleNumberingMap) {
        cout << "r" << itr.second << ":" << itr.first << "\n";
    }
    for (auto itr : RuleProductionNumberingMapForReduction) {
        cout << "Rule:" << itr.second << "Number:" << itr.first << "\n";
    }

    int counter = 0;
    RuleToItemNumberMap[RuleList] = counter;
    ItemNumberToRuleMap[counter] = RuleList;

    itemProcessQueue.push(
        counter);  // initially the first closure set is pushed into the queue

    while (!itemProcessQueue.empty()) {
        // while the queue is not empty
        int current = itemProcessQueue.front();
        printClosure(ItemNumberToRuleMap[current]);
        // if (current >= 5)
        //     exit(0);
        // cout << "Current Item Set :" << current << "\n";
        itemProcessQueue.pop();

        set<string> currentRuleList = ItemNumberToRuleMap[current];
        set<char> characters;
        // prepare the set of characters which are there after dot so that
        // transitions can be done
        for (string str : currentRuleList) {
            for (int i = 0; i < str.size(); i++) {
                if (str[i] == '.') {
                    if ((i + 1) != str.size())
                        characters.insert(str[i + 1]);

                    else {
                        cout << "Called for rule :" << str << "\n";
                        insertIntoReduceRule(str, current);
                    }

                    break;
                }
            }
        }
        // now st contains the characters for transitions
        queue<char> transitionQueue;
        for (char c : characters) {
            cout << "Pushed character:" << c << "\n";
            transitionQueue.push(c);
        }
        // now I have the queue for transtions of the characters

        while (!transitionQueue.empty()) {
            set<string> newRuleList;
            char ch = transitionQueue.front();
            transitionQueue.pop();

            // observing transitions for a particular character ch
            for (string s : currentRuleList) {
                for (int i = 0; i < s.size(); i++) {
                    if (s[i] == '.' && (i + 1) != s.size() && s[i + 1] == ch) {
                        string rule = s;
                        swap(rule[i], rule[i + 1]);
                        newRuleList.insert(rule);
                        break;
                    }
                }
            }
            // now new rulelist contains all the rules such that the dot if just
            // after the character ch
            prepareClosure(newRuleList);

            // check the existence of this rule list in the maps if not simply
            // create a new counter and insert
            if (RuleToItemNumberMap.find(newRuleList) ==
                RuleToItemNumberMap.end()) {
                // if not present then make a new item set for this transition
                counter++;
                ItemNumberToRuleMap[counter] = newRuleList;
                RuleToItemNumberMap[newRuleList] = counter;
                insertIntoActionTable(
                    current, ch,
                    counter);  // current->ch->shift to counter state number
                itemProcessQueue.push(
                    counter);  // also store this new item in item process set
            } else {
                // if the item set is seen , simply store the action table
                insertIntoActionTable(current, ch,
                                      RuleToItemNumberMap[newRuleList]);
            }
        }
        // printClosure(newRuleList); // prepare the closure
    }
    // Now we have with us the Action and GotoTable and also the Item Sets and
    // their corresponding count we can perform the shift reduce operation using
    // that table;

    // print all the item sets that are generated in this grammar
    cout << "\n--------The Item Sets generated in the grammar are "
            ":-----------\n";
    for (auto itr : ItemNumberToRuleMap) {
        cout << "I" << itr.first << ":\n";
        for (string s : itr.second) {
            cout << s << "\n";
        }
        cout << "\n";
    }

    for (auto itr : ActionTable) {
        int number = itr.first.first;
        char c = itr.first.second;
        string s = itr.second;
        cout << "Action:I" << number << "->" << c << "->" << s << "\n";
    }
    cout << "\n";
    for (auto itr : GotoTable) {
        int number = itr.first.first;
        char c = itr.first.second;
        string s = itr.second;
        cout << "Goto:I" << number << "->" << c << "->" << s << "\n";
    }

    // Final code segment for the parsing of the input string which is generated
    // from the given grammar

    //  FINAL CODE WHICH WILL DO THE PARSING OF THE INPUT TOKEN
    ifstream tokenListFile;
    frw.prepareRead(tokenListFile, argv[1]);
    string input;
    getline(tokenListFile, input);
    tokenListFile.close();
    input += "$";
    stack<string>
        stack;  // create a stack of string for storing the value at any time
    stack.push("0");
    int currentInputIndex = 0;
    while (1) {
        string currStr = stack.top();
        int currStateNumber = stoi(currStr);
        if (ActionTable.find({currStateNumber, input[currentInputIndex]}) ==
            ActionTable.end()) {
            cout << "The input string can't be parsed,No entry in Action "
                    "Table\n";
            exit(1);
        } else {
            string action =
                ActionTable[{currStateNumber, input[currentInputIndex]}];
            if (action == "AC") {
                cout << "\n---ACCEPTED----\n";
                break;
            }
            if (action[0] == 'S') {
                // if the action is shift
                string rem = action.substr(1);
                int state = stoi(rem);
                cout << "Shift " << input[currentInputIndex]
                     << " and move to :" << state << "\n";
                // increase the current input index only in shift operation
                stack.push(input[currentInputIndex++] + "");
                stack.push(rem);
            } else {
                // if the action is reduce
                string rem = action.substr(1);
                int reduceRuleNumber = stoi(rem);
                string rule = "";

                // the given value of cc which is basically reduceRuleNumber
                if (RuleProductionNumberingMapForReduction.find(
                        reduceRuleNumber) ==
                    RuleProductionNumberingMapForReduction.end()) {
                    cout << "No reduction rule found in the grammar \n";
                    exit(0);
                }

                else {
                    string p = RuleProductionNumberingMapForReduction
                        [reduceRuleNumber];
                    cout << "Reduce: " << p << "\n";
                    int len = 2 * (p.size() - 3);
                    while (len--) {
                        // pop characters till 2*|beta|
                        if (stack.empty()) {
                            cout << "Stack became empty \n";
                            exit(0);
                        }
                        stack.pop();
                    }
                    string currentTop = stack.top();
                    int state = stoi(currentTop);  // make it an integer to
                                                   // perform correct operation
                    stack.push(p[0] + "");  // push the character and check goto
                                            // table for this entry
                    if (GotoTable.find({state, p[0]}) == GotoTable.end()) {
                        cout << "No state found for GOTO\n";
                        exit(0);
                    } else {
                        stack.push(GotoTable[{state, p[0]}]);
                    }
                }
            }
        }
    }
    return 0;
}
