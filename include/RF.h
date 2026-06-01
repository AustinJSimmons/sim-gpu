/*
 * This will hold the definition of our GPUs Register File.
 *
 * It will contain 32 registers accessable per thread.
 *
 **/

#ifndef RF_H
#define RF_H

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

SC_MODULE(RF) {
  // First we need to define the Size of our register file
  static constexpr int MAX_THREADS{1};
  static constexpr int MAX_REG_PER_THREAD{32};

  // Define the actual physical RF structure
  // This gives us MAX_THREADS * MAX_REG_PER_THREAD total registers
  // in our register file.
  sc_signal<sc_int<32>> physical_rf[MAX_THREADS][MAX_REG_PER_THREAD];

  SC_CTOR(RF) {}

  ~RF();
};

#endif
