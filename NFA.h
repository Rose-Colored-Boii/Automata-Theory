//
// Created by Liam on 6/03/2022.
//

#ifndef NFA_NFA_H
#define NFA_NFA_H
#include "iostream"
#include "vector"
#include "set"

using namespace std;

class State;

class NFA {
private:

    vector<State*> states;
    set<char> alphabet;
    set<State*> currStates;

public:

    ~NFA();

    //Method that checks if string is accepted by the NFA
    bool accepts(const string& input);
    //Add state to the NFA
    bool addState(State* state);
    //Set the alphabet of the NFA
    void setAlphabet(set<char> alphabet);
};


#endif //NFA_NFA_H
