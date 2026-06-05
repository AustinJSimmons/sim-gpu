/*
 * The operand collector will provide operand collecting units
 * to help arbitrate data flow through the crossbar from our
 * banked register file.
 **/

#ifndef CU_OC_H
#define CU_OC_H

#include "systemc.h"
using namespace sc_core;
using namespace sc_dt;

template <int NUM_OC_UNITS>
SC_MODULE(OC){

};

#endif // !CU_OC_H
