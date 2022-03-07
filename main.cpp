#include <iostream>
#include "NFA.h"
#include "State.h"

int main() {
    auto* q0 = new State("q0", false, true);
    auto* q1 = new State("q1", false, false);
    auto* q2 = new State("q2", true, false);
    q0->addTransition('0', q0);
    q0->addTransition('1', q0);
    q0->addTransition('0', q1);
    q1->addTransition('1', q2);
    NFA* nfa = new NFA();
    nfa->addState(q0);
    nfa->addState(q1);
    nfa->addState(q2);
    cout << boolalpha << nfa->accepts("01") << endl;
    cout << boolalpha << nfa->accepts("10") << endl;
    cout << boolalpha << nfa->accepts("1000101001") << endl;
    cout << boolalpha << nfa->accepts("01111110110011") << endl;
    delete nfa;
    return 0;
}
