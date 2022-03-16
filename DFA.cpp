//
// Created by Liam on 16/02/2022.
//

#include "DFA.h"
#include "DFAState.h"
#include <algorithm>
#include <string>
#include "iomanip"
#include "json.hpp"
#include "fstream"
#include "iostream"
using namespace std;
using json = nlohmann::json;

DFA::DFA(string filename) {
    ifstream input(filename);
    json j;
    input >> j;
    set<char> alphabet;
    for (auto state : j["states"]){
        string name = state["name"];
        bool starting = state["starting"];
        bool accepting = state["accepting"];
        auto* s = new DFAState(name, accepting, starting);
        addState(s);
    }
    for (auto transition : j["transitions"]){
        DFAState* fromState = nullptr;
        DFAState* toState = nullptr;
        string fromStateName = transition["from"];
        string toStateName = transition["to"];
        for (auto s : states){
            if (fromStateName == s->getName()){
                fromState = s;
            }
            if (toStateName == s->getName()){
                toState = s;
            }
        }
        string inputchar = transition["input"];
        fromState->addTransition(inputchar[0], toState);
    }
    for (string i : j["alphabet"]){
        alphabet.insert(i[0]);
    }
    setAlphabet(alphabet);
    for (auto state : states){
        if (state->isBeginning()){
            currState = state;
        }
    }
}

void DFA::print() {
    json j;
    j["type"] = "DFA";
    vector<string> alphabet;
    for (char character : this->alphabet) {
        string output = " ";
        output[0] = character;
        alphabet.push_back(output);
    }
    j["alphabet"] = alphabet;
    j["states"] = {};
    int i = 0;
    for (auto state : states){
        j["states"].push_back({});
        j["states"][i]["name"] = state->getName();
        j["states"][i]["starting"] = state->isBeginning();
        j["states"][i]["accepting"] = state->isAccepting();
        i++;
    }
    j["transitions"] = {};
    i = 0;
    for (auto state : states){
        for (auto transition : state->getTransitions()){
            j["transitions"].push_back({});
            j["transitions"][i]["from"] = state->getName();
            j["transitions"][i]["to"] = transition.second->getName();
            string output = " ";
            output[0] = transition.first;
            j["transitions"][i]["input"] = output;
            i++;
        }
    }
    cout << setw(4) << j << endl;
}

bool DFA::accepts(const string& input){
    for (auto character : input){
        //If character not in alphabet, return false
        if (find(alphabet.begin(), alphabet.end(), character) == alphabet.end()){
            return false;
        }
        //If label of a transition == character, set currState to the next state
        for (auto transition : currState->getTransitions()){
            if (transition.first == character){
                currState = transition.second;
                break;
            }
        }
    }
    //If the currState at the end of the string is accepting, string is accepted by the DFA
    if (currState->isAccepting()){
        return true;
    }
    //In all other cases, the string is not accepted
    return false;
}

bool DFA::addState(DFAState *state) {
    //If state already in states, return false
    if (find(states.begin(), states.end(), state) != states.end()){
        return false;
    }
    states.push_back(state);
    return true;
}

void DFA::setAlphabet(set<char> alphabet) {
    this->alphabet = alphabet;
}

DFA::~DFA() {
    //Delete all states
    for (auto state : states){
        delete state;
    }
}

DFA::DFA() {

}

const vector<DFAState *> &DFA::getStates() const {
    return states;
}

DFA::DFA(const DFA& dfa1, const DFA& dfa2, bool intersection) {
    this->alphabet = dfa1.alphabet;
    for (auto state1 : dfa1.states){
        for (auto state2 : dfa2.states){
            if (state1->isBeginning() and state2->isBeginning()){
                bool accepting = false;
                if (intersection){
                    if (state1->isAccepting() and state2->isAccepting()){
                        accepting = true;
                    }
                }
                else{
                    if (state1->isAccepting() or state2->isAccepting()){
                        accepting = true;
                    }
                }
                string name = '(' + state1->getName() + ',' + state2->getName() + ')';
                auto newState = new DFAState(name, accepting, true);
                addState(newState);
            }
        }
    }
    for (int i = 0; i != states.size(); i++){
        vector<string> newNames;
        string temp;
        DFAState* dfaState1 = nullptr;
        DFAState* dfaState2 = nullptr;
        for (int j = 1; j != states[i]->getName().size(); j++){
            if (states[i]->getName()[j] == ','){
                newNames.push_back(temp);
                temp = "";
                continue;
            }
            if (states[i]->getName()[j]  == ')'){
                newNames.push_back(temp);
                break;
            }
            temp += states[i]->getName()[j];
        }
        for (auto state1 : dfa1.getStates()){
            if (state1->getName() == newNames[0]){
                dfaState1 = state1;
                break;
            }
        }
        for (auto state2 : dfa2.getStates()){
            if (state2->getName() == newNames[1]){
                dfaState2 = state2;
                break;
            }
        }
        for (auto transition1 : dfaState1->getTransitions()){
            for (auto transition2 : dfaState2->getTransitions()){
                if (transition1.first == transition2.first){
                    DFAState* s1 = nullptr;
                    string newName = '(' + transition1.second->getName() + ',' + transition2.second->getName() + ')';
                    for (auto toState : states){
                        if (toState->getName() == newName){
                            s1 = toState;
                            break;
                        }
                    }
                    if (s1 == nullptr){
                        bool accepting = false;
                        if (intersection){
                            if (transition1.second->isAccepting() and transition2.second->isAccepting()){
                                accepting = true;
                            }
                        }
                        else{
                            if (transition1.second->isAccepting() or transition2.second->isAccepting()){
                                accepting = true;
                            }
                        }
                        s1 = new DFAState(newName, accepting, false);
                        addState(s1);
                    }
                    states[i]->addTransition(transition1.first, s1);
                }
            }
        }
    }
}
