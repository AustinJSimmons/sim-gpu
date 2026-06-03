/*
 * The Control Unit decoder will be responsible for fetching instructions
 * from the instruction cache, decoding them into control signals before handing
 * off to the cu_scheduler.
 **/

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

#ifndef CTU_DE_H
#define CTU_DE_H

SC_MODULE(CTU_DECODER) {

  SC_CTOR(CTU_DECODER){

  };

  ~CTU_DECODER();
};

#endif // CU_DE
