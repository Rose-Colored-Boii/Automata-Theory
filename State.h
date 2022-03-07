//
// Created by Liam on 6/03/2022.
//

#ifndef NFA_STATE_H
#define NFA_STATE_H
#include "map"
#include "set"
#include "iostream"
using namespace std;

class State {
private:

    string name;
    //Map that maps input character to set of states
    map<char, set<State*>> transitions = {};
    bool accepting;
    bool starting;

public:

    State(string name, bool accepting, bool starting){
        this->name = name;
        this->accepting = accepting;
        this->starting = starting;
    }

    void addTransition(char character, State* state);

    //Getters
    const string &getName() const;
    const map<char, set<State *>> &getTransitions() const;
    bool isAccepting() const;
    bool isStarting() const;
};


#endif //NFA_STATE_H
