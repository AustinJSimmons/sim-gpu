/*
 * The Control Unit Scheduler will handle managing thread resources
 * via control signals. If a instruction is sent out among 32 threads
 * the scheduler will be responsible for keeping track of when thread
 * instructinos are complete and assign threads to specific PEs to maximize
 * hardware utilization.
 **/

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

#ifndef CTU_SCH_H
#define CTU_SCH_H

SC_MODULE(CTU_SCHEDULER) {

  SC_CTOR(CTU_SCHEDULER){

  };

  ~CTU_SCHEDULER();
};

#endif // CU_DE
