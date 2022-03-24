//
// Created by Liam on 16/02/2022.
//

#ifndef DFA_DFA_H
#define DFA_DFA_H

#include <map>
#include "iostream"
#include "set"
#include "vector"
using namespace std;

class DFAState;

class DFA {
private:
    //Set that contains all characters in the alphabet of the DFA
    set<char> alphabet;
    //Vector that contains all states
    vector<DFAState*> states;
    //Object that contains the current state
    DFAState* currState;
public:
    DFA(string filename);
    ~DFA();
    //Method that checks if string is accepted by the DFA
    bool accepts(const string& input);
    //Add state to the DFA
    bool addState(DFAState* state);
    //Set the alphabet of the DFA
    void setAlphabet(set<char> alphabet);
    //Convert DFA to json
    void print();

    const vector<DFAState *> &getStates() const;

    DFA();

    DFA(const DFA& dfa1, const DFA& dfa2, bool intersection);

    DFA minimize();

    std::map<pair<DFAState *, DFAState *>, char> generateTable();

    void printTable();
};


#endif //DFA_DFA_H
