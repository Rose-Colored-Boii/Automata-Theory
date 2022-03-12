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
