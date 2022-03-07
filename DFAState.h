//
// Created by Liam on 16/02/2022.
//
#ifndef DFA_STATE_H
#define DFA_STATE_H

#include "map"
#include "iostream"
using namespace std;

class DFAState {
private:
    string name;
    //If state is a final state, accepting = true
    bool accepting;
    //If state is the start state, beginning = true
    bool beginning;

    map<char, DFAState*> transitions;
public:
    //Constructor that sets accepting and beginning to true or false
    DFAState(string& name, bool accepting, bool beginning);
    //Method to add a transition to the state
    bool addTransition(char label, DFAState* state);
    //Getters
    bool isAccepting() const;

    bool isBeginning() const;

    const map<char, DFAState *> &getTransitions() const;

    const string &getName() const;
};


#endif //DFA_STATE_H
