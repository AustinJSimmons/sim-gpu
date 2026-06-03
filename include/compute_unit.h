/*
 * The Compute Unit acts as a parallel compute core. Each core
 * will contain a control unit, variable numbers of PEs, L1 Cache
 * and a variable size register file made up of register file banks.
 * */

// Include all children
#include "PE.h"
#include "RFB.h"
#include "cu_control_unit.h"
#include "cu_cross_bar_switch.h"

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

#ifndef CU_H
#define CU_H

SC_MODULE(CU) {

  SC_CTOR(CU){

  };

  ~CU();
};

#endif // CU
