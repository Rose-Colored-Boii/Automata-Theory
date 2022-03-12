#include <iostream>
#include "NFA.h"
#include "NFAState.h"

int main() {
    NFA nfa("input-ssc1.json");
    nfa.toDFA().print();
    return 0;
}
