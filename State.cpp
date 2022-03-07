//
// Created by Liam on 6/03/2022.
//

#include "State.h"

const string &State::getName() const {
    return name;
}

const map<char, set<State *>> &State::getTransitions() const {
    return transitions;
}

bool State::isAccepting() const {
    return accepting;
}

bool State::isStarting() const {
    return starting;
}

void State::addTransition(char character, State *state) {
    //Check if transition already exists. If not, map character to singleton of that state
    if (transitions.find(character) == transitions.end()){
        transitions[character] = {state};
    }
    //else, insert into set
    else{
        transitions[character].insert(state);
    }
}
