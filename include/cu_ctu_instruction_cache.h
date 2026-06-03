/*
 * The instruction cache is a part of localized instruction memory
 * that holds program instructions for specific compute units. Not
 * sure how it works yet entirely but I think it would hold thread
 * level instructions.
 * */

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

#ifndef CTU_ICACHE_H
#define CTU_ICACHE_H

SC_MODULE(CTU_ICACHE) {

  SC_CTOR(CTU_ICACHE){

  };

  ~CTU_ICACHE();
};

#endif // CTU_DE
