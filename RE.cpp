//
// Created by Liam on 16/03/2022.
//

#include "RE.h"
#include "NFAState.h"
#include "algorithm"

ENFA RE::toENFA() {
    vector<char> operators;
    vector<ENFA> operands;
    ENFA enfa;
    ENFA* temp1 = nullptr;
    ENFA *temp2 = nullptr;
    vector<char> specialChar = {'(', ')', '+', '*', '.', eps};
    string newRegex;
    for (int i = 0; i < regex.size(); i++){
        newRegex += regex[i];
        if ((find(specialChar.begin(), specialChar.end(), regex[i]) == specialChar.end() and find(specialChar.begin(), specialChar.end(), regex[i+1]) == specialChar.end())
         or (find(specialChar.begin(), specialChar.end(), regex[i]) == specialChar.end() and regex[i+1] == '(') or
          (find(specialChar.begin(), specialChar.end(), regex[i+1]) == specialChar.end() and regex[i] == ')')){
            newRegex += '.';
        }
    }
    set<char> alphabet;
    enfa.setEpsilon(this->eps);
    for (char& i : regex){
        if (find(specialChar.begin(), specialChar.end(), i) == specialChar.end()){
            alphabet.insert(i);
            temp1 = new ENFA();
            auto* state1 = new NFAState(to_string(newName), false, false);
            newName++;
            auto* state2 = new NFAState(to_string(newName), false, false);
            newName++;
            state1->addTransition(i, state2);
            temp1->addState(state1);
            temp1->addState(state2);
            operands.push_back(*temp1);
            delete temp1;
        }
        else{
            if (i == '+' or i == '.' or i == '('){
                operators.push_back(i);
            }
            else if (i == '*'){
                *temp2 = operands[operands.size()-1];
                operands.pop_back();
                operands.push_back(Kleene(*temp2));
            }
            else {

            }
        }
    }
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