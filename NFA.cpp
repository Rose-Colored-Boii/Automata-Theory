//
// Created by Liam on 6/03/2022.
//

#include "NFA.h"
#include "DFAState.h"
#include "DFA.h"
#include "algorithm"
#include "NFAState.h"
#include "json.hpp"
#include "fstream"
#include "iomanip"
using json = nlohmann::json;

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
    DFA* dfa = new DFA();
    for (auto state : states){
        if (state->isStarting()){
            string name = '{' + state->getName() + '}';
            auto* s = new DFAState(name, state->isAccepting(), true);
            for (auto transition : state->getTransitions()){
                continue;
            }
        }
    }
}

NFA::NFA(string filename) {
    ifstream input(filename);
    json j;
    input >> j;
    set<char> alphabet;
    for (auto state : j["states"]){
        string name = state["name"];
        bool starting = state["starting"];
        bool accepting = state["accepting"];
        auto* s = new NFAState(name, accepting, starting);
        addState(s);
    }
    for (auto transition : j["transitions"]){
        NFAState* fromState = nullptr;
        NFAState* toState = nullptr;
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
}

void NFA::print() {
    json j;
    j["type"] = "NFA";
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
        j["states"][i]["starting"] = state->isStarting();
        j["states"][i]["accepting"] = state->isAccepting();
        i++;
    }
    j["transitions"] = {};
    i = 0;
    for (auto state : states){
        for (auto transition : state->getTransitions()){
            for (auto toState : transition.second){
                j["transitions"].push_back({});
                j["transitions"][i]["from"] = state->getName();
                j["transitions"][i]["to"] = toState->getName();
                string output = " ";
                output[0] = transition.first;
                j["transitions"][i]["input"] = output;
                i++;
            }
        }
    }
    cout << setw(4) << j << endl;
}
