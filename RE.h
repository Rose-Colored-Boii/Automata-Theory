//
// Created by Liam on 16/03/2022.
//

#ifndef NFA_RE_H
#define NFA_RE_H
#include "iostream"
#include "ENFA.h"
using namespace std;

class RE {
private:
    string regex;
    char eps;
    char newName;
public:
    RE(string regex, char eps){
        this->regex = regex;
        this->eps = eps;
        this->newName = 'a';
    }

    ENFA toENFA();

    ENFA Union(ENFA enfa1, ENFA enfa2);

    ENFA Concatenation(ENFA enfa1, ENFA enfa2) const;

    ENFA Kleene(ENFA enfa);
};


#endif //NFA_RE_H
