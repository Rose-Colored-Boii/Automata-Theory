//
// Created by Liam on 16/03/2022.
//

#include "RE.h"
#include "NFAState.h"

ENFA RE::toENFA() {
    ENFA enfa;
    ENFA temp1;
    ENFA temp2;
    auto* startState = new NFAState(to_string(newName), false, true);
    temp1.addState(startState);
    newName++;
    set<char> alphabet;
    enfa.setEpsilon(this->eps);
    for (char& i : regex){
        if (i != '(' and i != ')' and i != '+' and i != '*' and i != '.' and i != this->eps){
            alphabet.insert(i);
        }
    }
    for (char& i : regex){
        if (i == '('){

        }
        else if (i == ')'){

        }
        else if (i == '*'){
            temp1 = Kleene(temp1);
        }
        else if (i == '.'){
            temp1 = Concatenation(temp1, temp2);
        }
        else if (i == '+'){
            temp1 = Union(temp1, temp2);
        }
    }
    temp1.setEpsilon(eps);
    temp1.setAlphabet(alphabet);
    enfa = temp1;
    return enfa;
}

ENFA RE::Union(ENFA enfa1, ENFA enfa2){
    ENFA enfa;
    NFAState* final1 = nullptr;
    NFAState* start1 = nullptr;
    NFAState* final2 = nullptr;
    NFAState* start2 = nullptr;
    for (auto state : enfa1.getStates()){
        enfa.addState(state);
        if (state->isAccepting()){
            final1 = state;
        }
        if (state->isStarting()){
            start1 = state;
        }
    }
    for (auto state : enfa2.getStates()){
        enfa.addState(state);
        if (state->isAccepting()){
            final2 = state;
        }
        if (state->isStarting()){
            start2 = state;
        }
    }
    auto* newStarting = new NFAState(to_string(newName), false, true);
    enfa.addState(newStarting);
    newName++;
    auto* newFinal = new NFAState(to_string(newName), true, false);
    enfa.addState(newFinal);
    newName++;
    newStarting->addTransition(eps, start1);
    newStarting->addTransition(eps, start2);
    final1->addTransition(eps, newFinal);
    final2->addTransition(eps, newFinal);
    final1->setAccepting(false);
    final2->setAccepting(false);
    start1->setStarting(false);
    start2->setStarting(false);
    return enfa;
}

ENFA RE::Concatenation(ENFA enfa1, ENFA enfa2) const{
    ENFA enfa;
    NFAState* final = nullptr;
    NFAState* start = nullptr;
    for (auto state : enfa1.getStates()){
        enfa.addState(state);
        if (state->isAccepting()){
            final = state;
        }
    }
    for (auto state : enfa2.getStates()){
        enfa.addState(state);
        if( state->isStarting()){
            final = state;
        }
    }
    final->addTransition(eps, start);
    final->setAccepting(false);
    return enfa;
}

ENFA RE::Kleene(ENFA enfa){
    ENFA newEnfa;
    NFAState* final = nullptr;
    NFAState* start = nullptr;
    for (auto state : enfa.getStates()){
        if (state->isStarting()){
            start = state;
        }
        if (state->isAccepting()){
            final = state;
        }
    }
    auto* newStarting = new NFAState(to_string(newName), false, true);
    enfa.addState(newStarting);
    newName++;
    auto* newFinal = new NFAState(to_string(newName), true, false);
    enfa.addState(newFinal);
    newName++;
    newStarting->addTransition(eps, start);
    final->addTransition(eps, newFinal);
    final->setAccepting(false);
    start->setStarting(false);
    newStarting->addTransition(eps, newFinal);
    final->addTransition(eps, start);
    return newEnfa;
}
