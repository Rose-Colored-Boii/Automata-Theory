//
// Created by Liam on 16/02/2022.
//

#include "DFAState.h"
#include "algorithm"

DFAState::DFAState(string& name, bool accepting, bool beginning) {
    this->transitions = {};
    this->name = name;
    this->accepting = accepting;
    this->beginning = beginning;
}

bool DFAState::addTransition(const char label, DFAState *state) {
    //If transition already exists, return false
    if (transitions.find(label) != transitions.end()){
        return false;
    } else{
        this->transitions[label] = state;
        return true;
    }
}

bool DFAState::isAccepting() const {
    return accepting;
}

bool DFAState::isBeginning() const {
    return beginning;
}

const map<char, DFAState *> &DFAState::getTransitions() const {
    return transitions;
}

const string &DFAState::getName() const {
    return name;
}
