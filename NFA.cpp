//
// Created by Liam on 6/03/2022.
//

#include "NFA.h"
#include "algorithm"
#include "State.h"

bool NFA::accepts(const string &input) {
    currStates = {};
    for (auto state : states){
        if (state->isStarting()) {
            currStates.insert(state);
            break;
        }
    }
    for (auto character : input){
        set<State*> temp;
        for (auto state : currStates){
            if (state->getTransitions().find(character) == state->getTransitions().end()){
                continue;
            }
            for (auto transition : state->getTransitions()){
                if (transition.first == character){
                    for (auto toState : transition.second){
                        temp.insert(toState);
                    }
                }
            }
        }
        currStates = temp;
    }
    for (auto state : currStates){
        if (state->isAccepting()){
            return true;
        }
    }
    return false;
}

bool NFA::addState(State *state) {
    //If state already in states, return false
    if (find(states.begin(), states.end(), state) != states.end()){
        return false;
    }
    states.push_back(state);
    return true;
}

void NFA::setAlphabet(set<char> alphabet) {
    this->alphabet = alphabet;
}

NFA::~NFA() {
    for (auto state : states)
        delete state;
}
