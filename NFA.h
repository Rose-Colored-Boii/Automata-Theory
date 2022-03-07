//
// Created by Liam on 6/03/2022.
//

#ifndef NFA_NFA_H
#define NFA_NFA_H
#include "iostream"
#include "vector"
#include "set"

using namespace std;

class NFAState;

class NFA {
private:
    //Vector that holds all states
    vector<NFAState*> states;
    //Set that holds the alphabet
    set<char> alphabet;
    //Set that contains all current states of the NFA
    set<NFAState*> currStates;

public:
    //Destructor that deletes all states
    ~NFA();

    //Method that checks if string is accepted by the NFA
    bool accepts(const string& input);
    //Add state to the NFA
    bool addState(NFAState* state);
    //Set the alphabet of the NFA
    void setAlphabet(set<char> alphabet);
};


#endif //NFA_NFA_H
