//
// Created by Liam on 6/03/2022.
//

#include "NFAState.h"

const string &NFAState::getName() const {
    return name;
}

const map<char, set<NFAState *>> &NFAState::getTransitions() const {
    return transitions;
}

bool NFAState::isAccepting() const {
    return accepting;
}

bool NFAState::isStarting() const {
    return starting;
}

void NFAState::addTransition(char character, NFAState *state) {
    //Check if transition already exists. If not, map character to singleton of that state
    if (transitions.find(character) == transitions.end()){
        transitions[character] = {state};
    }
    //else, insert into set
    else{
        transitions[character].insert(state);
    }
}

void NFAState::setAccepting(bool accepting) {
    NFAState::accepting = accepting;
}

void NFAState::setStarting(bool starting) {
    NFAState::starting = starting;
}
