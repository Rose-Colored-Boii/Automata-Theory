#include <iostream>
#include "NFA.h"
#include "NFAState.h"
#include "ENFA.h"

int main() {
    ENFA enfa("input-mssc1.json");
    enfa.toDFA().print();
}
