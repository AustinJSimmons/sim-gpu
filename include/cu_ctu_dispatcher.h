/*
 * The Control Unit dispatcher actually changes the control signals based
 * decodes instructions and available resources as determined by the scheduler.
 **/

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

#ifndef CTU_DIS_H
#define CTU_DIS_H

SC_MODULE(CTU_DISPATCHER) {

  SC_CTOR(CTU_DISPATCHER){

  };

  ~CTU_DISPATCHER();
};

#endif // CTU_DIS
