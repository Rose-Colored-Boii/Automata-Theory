//
// Created by Liam on 13/03/2022.
//

#ifndef NFA_ENFA_H
#define NFA_ENFA_H
#include "NFA.h"
#include "set"
using namespace std;

class NFAState;

class ENFA{
private:
    //Vector that holds all states
    vector<NFAState*> states;
    //Set that holds the alphabet
    set<char> alphabet;
    //Set that contains all current states of the ENFA
    set<NFAState*> currStates;
    //Epsilon character
    char epsilon;
public:
    ENFA(string filename);
    //Destructor that deletes all states
    ~ENFA();
    //Method that checks if string is accepted by the ENFA
    bool accepts(const string& input);
    //Add state to the ENFA
    bool addState(NFAState* state);
    //Set the alphabet of the ENFA
    void setAlphabet(set<char> alphabet);
    //Convert ENFA to DFA via SSC
    DFA toDFA();
    //Print ENFA to JSON
    void print();
    //Calculate E-Closure of state
    set<NFAState*> ECLOSE(NFAState* state) const;
};


#endif //NFA_ENFA_H
