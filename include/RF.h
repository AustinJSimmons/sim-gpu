/*
 * RF stands for Register File
 *
 * Defines a systemc module meant to represent our
 * SMs register file.
 **/

#ifndef RF_H
#define RF_H

#include "systemc.h"
using namespace sc_core;
using namespace sc_dt;

SC_MODULE(RF) {

  SC_CTOR(RF) {}

  ~RF();
};

#endif // !RF_H
