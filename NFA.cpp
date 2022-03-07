//
// Created by Liam on 6/03/2022.
//

#include "NFA.h"
#include "algorithm"
#include "NFAState.h"

bool NFA::accepts(const string &input) {
    currStates = {};
    //Find start state of the NFA
    for (auto state : states){
        if (state->isStarting()) {
            currStates.insert(state);
            break;
        }
    }
    //Traverse over input string
    for (auto character : input){
        //If character not in alphabet, string is not accepted
        if (find(alphabet.begin(), alphabet.end(), character) == alphabet.end()){
            return false;
        }
        //Temporary set to hold next states that the NFA goes to
        set<NFAState*> temp;
        //Traverse over currStates and add correct states to temp set
        for (auto state : currStates){
            //If transtion not in transitions, check next state in currState
            if (state->getTransitions().find(character) == state->getTransitions().end()){
                continue;
            }
            //If transition does exist, add all states in set of states to temp
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
    //At end of string, check if any state in currStates is a final state, if so, string is accepted
    for (auto state : currStates){
        if (state->isAccepting()){
            return true;
        }
    }
    return false;
}

bool NFA::addState(NFAState *state) {
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

DFA *NFA::toDFA() {
    return nullptr;
}
