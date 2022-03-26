#include <iostream>
#include "NFA.h"
#include "NFAState.h"
#include "ENFA.h"
#include "RE.h"

int main() {
    DFA dfa("input-tfa1.json");
    DFA mindfa = dfa.minimize();
    dfa.printTable();
    mindfa.print();
    cout << boolalpha << (dfa == mindfa) << endl;    // zijn ze equivalent? Zou hier zeker moeten. Dit wordt getest in de volgende vraag, maar hiermee kan je al eens proberen
    return 0;
}
