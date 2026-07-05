/*
 * CU stands for Control Unit in this case.
 *
 * Defines a systemc module that holds parts of our control unit,
 * which is part of the greater Streaming Multiprocessor or SM.
 **/

#ifndef CU_H
#define CU_H

#include "systemc.h"
using namespace sc_core;
using namespace sc_dt;

template <int WARP_SIZE> SC_MODULE(CU) {

  SC_CTOR(CU){};

  ~CU(){};
};

#endif // !CU_H
