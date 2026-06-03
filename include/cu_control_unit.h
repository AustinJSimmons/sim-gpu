/*
 * Our control unit is the brain of our cores, it will contain
 *  - Instruction cache: hold instructions for Warps? Or Threads?
 *  - decoder: Decodes instructions and gets ready to send control
 *  signals throughout the Compute Unit for each thread.
 *  - dispatcher: Dispatches control signals to the rest of the Compute
 *  Unit based on decoded instructions.
 *  -
 * */

#include "cu_ctu_decoder.h"
#include "cu_ctu_dispatcher.h"
#include "cu_ctu_instruction_cache.h"
#include "cu_ctu_scheduler.h"

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

#ifndef CU_CTU_H
#define CU_CTU_H

SC_MODULE(CU_CTU) {

  SC_CTOR(CU_CTU){

  };

  ~CU_CTU();
};

#endif // CU
