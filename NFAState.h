//
// Created by Liam on 6/03/2022.
//

#ifndef NFA_NFASTATE_H
#define NFA_NFASTATE_H
#include "map"
#include "set"
#include "iostream"
using namespace std;

class NFAState {
private:

    string name;
    //Map that maps input character to set of states
    map<char, set<NFAState*>> transitions = {};
    bool accepting;
    bool starting;

public:

    NFAState(string name, bool accepting, bool starting){
        this->name = name;
        this->accepting = accepting;
        this->starting = starting;
    }

    void addTransition(char character, NFAState* state);

    //Getters
    const string &getName() const;
    const map<char, set<NFAState *>> &getTransitions() const;
    bool isAccepting() const;
    bool isStarting() const;
};


#endif //NFA_NFASTATE_H
