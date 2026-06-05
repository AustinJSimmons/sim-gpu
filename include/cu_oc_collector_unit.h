/*
 * The collector units are the piece of silicon physically
 * responsible for latching operands over multiple cycles if
 * needed.
 **/
#ifndef OC_UNIT_H
#define OC_UNIT_H

#include "systemc.h"
using namespace sc_core;
using namespace sc_dt;

template <int NUM_OPERANDS, int NUM_THREADS>
SC_MODULE(OC_UNIT){

    SC_CTOR(OC_UNIT){

    }

    ~OC_UNIT(){}

};
#endif // !OC_UNIT_H
