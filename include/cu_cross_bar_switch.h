/*
 * This file will layout our Cross bar switch.
 * Essentially a set of multiplex and demux logic to control
 * how the register file banks connect and transfer data to and
 * from our execution units.
 **/

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

#ifndef CU_CBS_H
#define CU_CBS_H

SC_MODULE(CB_SWITCH) {

  SC_CTOR(CB_SWITCH){

  };

  ~CB_SWITCH();
};

#endif // CU_CBS
