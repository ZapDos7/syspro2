#ifndef TRIPLEARRAY_H
#define TRIPLEARRAY_H

#include "Array.h"
#include "Triplette.h"
#include <iostream>

class TripleArray : public Array<Triplette>
{
public:
    TripleArray(int capacity);

    virtual void print();
};

#endif