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

map<pair<DFAState*, DFAState*>, char> DFA::generateTable(){
    map<pair<DFAState*, DFAState*>, char> Table;
    for (int i = 0; i < this->states.size(); i++){
        for (int j = i + 1; j < this->states.size(); j++){
            vector<string> names = {this->states[i]->getName(), this->states[j]->getName()};
            std::sort(names.begin(), names.end());
            if (states[i]->getName() == names[0]){
                Table[{states[i], states[j]}] = '-';
            }
            else{
                Table[{states[j], states[i]}] = '-';
            }
        }
    }
    for (auto entry : Table){
        if ((entry.first.first->isAccepting() and !entry.first.second->isAccepting()) or (!entry.first.first->isAccepting() and entry.first.second->isAccepting())){
            Table[entry.first] = 'X';
        }
    }
    bool done = false;
    while (!done) {
        bool found = false;
        for (auto entry: Table) {
            if (entry.second == 'X') {
                continue;
            }
            for (auto character : this->alphabet){
                pair<DFAState*, DFAState*> toPair;
                DFAState* toState1 = entry.first.first->getTransitions().find(character)->second;
                DFAState* toState2 = entry.first.second->getTransitions().find(character)->second;
                if (toState1 == toState2){
                    continue;
                }
                vector<string> names = {toState1->getName(), toState2->getName()};
                std::sort(names.begin(), names.end());
                if (toState1->getName() == names[0]){
                    toPair = {toState1, toState2};
                }
                else{
                    toPair = {toState2, toState1};
                }
                if (Table[toPair] == 'X'){
                    Table[entry.first] = 'X';
                    found = true;
                }
            }
        }
        if (!found){
            done = true;
        }
    }
    return Table;
}

DFA DFA::minimize() {
    DFA minDFA;
    minDFA.setAlphabet(this->alphabet);
    map<pair<DFAState*, DFAState*>, char> Table = generateTable();
    set<set<DFAState*>> equivalences;
    for (auto entry : Table){
        if (entry.second == '-'){
            DFAState* state1 = entry.first.first;
            DFAState* state2 = entry.first.second;
            set<DFAState*> equivalence = {state1, state2};
            for (auto i : Table){
                if (i != entry and i.second == '-' and (i.first.first == state1 or i.first.second == state1 or i.first.first == state2 or i.first.second == state2)){
                    if (i.first.first == state1 or i.first.first == state2){
                        equivalence.insert(i.first.second);
                    }
                    else if (i.first.second == state1 or i.first.second == state2){
                        equivalence.insert(i.first.first);
                    }
                }
            }
            equivalences.insert(equivalence);
        }
    }
    for (auto state : this->states){
        bool found = false;
        for (const auto& equivalence : equivalences){
            if (equivalence.find(state) != equivalence.end()){
                found = true;
            }
        }
        if (!found){
            equivalences.insert({state});
        }
    }
    for (const auto& equivalence : equivalences){
        bool starting = false;
        bool accepting = false;
        vector<string> names;
        for (auto state : equivalence){
            if (state->isAccepting()){
                accepting = true;
            }
            if (state->isBeginning()){
                starting = true;
            }
            names.push_back(state->getName());
        }
        std::sort(names.begin(), names.end());
        string name = "{";
        for (const auto& i : names){
            name += i;
            name += ',';
        }
        name[name.size()-1] = '}';
        auto* dfaState = new DFAState(name, accepting, starting);
        minDFA.addState(dfaState);
    }
    for (auto state : minDFA.states){
        string stateName;
        for (auto character : state->getName()){
            if (character == '{'){
                continue;
            }
            else if (character == ',' or character == '}'){
                break;
            }
            else{
                stateName += character;
            }
        }
        DFAState* fromState = nullptr;
        for (auto s : this->states){
            if (s->getName() == stateName){
                fromState = s;
                break;
            }
        }
        for (auto transition : fromState->getTransitions()){
            string toName = transition.second->getName();
            for (auto toState : minDFA.states){
                string tempName;
                for (auto character : toState->getName()){
                    if (tempName == toName){
                        state->addTransition(transition.first, toState);
                    }
                    else if (character == ',' or character == '{'){
                        tempName = "";
                    }
                    else if (character == '}'){
                        break;
                    }
                    else{
                        tempName += character;
                    }
                }
            }
        }
    }
    return minDFA;
}

void DFA::printTable() {
    map<pair<DFAState*, DFAState*>, char> Table = generateTable();
    set<DFAState*> tableStates;
    for (auto entry : Table){
        tableStates.insert(entry.first.first);
        tableStates.insert(entry.first.second);
    }
    vector<string> stateNameVector;
    for (auto state : tableStates){
        stateNameVector.push_back(state->getName());
    }
    std::sort(stateNameVector.begin(), stateNameVector.end());
    vector<DFAState*> stateVector(stateNameVector.size());
    for (int i = 0; i < stateNameVector.size(); i++){
        for (auto state : states){
            if (stateNameVector[i] == state->getName()){
                stateVector[i] = state;
                break;
            }
        }
    }
    for (int i = 1; i < stateVector.size(); i++){
        cout << stateVector[i]->getName() << '\t';
        for (int j = 0; j < i; j++){
            DFAState* state1 = stateVector[i];
            DFAState* state2 = stateVector[j];
            for (auto entry : Table){
                if ((entry.first.first == state1 and entry.first.second == state2) or (entry.first.first == state2 and entry.first.second == state1)){
                    cout << entry.second << '\t';
                    break;
                }
            }
        }
        cout << endl;
    }
    cout << '\t';
    for (int i = 0; i < stateVector.size()-1; i++){
        cout << stateVector[i]->getName() << '\t';
    }
    cout << endl;
}
