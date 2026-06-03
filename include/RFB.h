/*
 * This will hold the definition of our GPUs Register File Banks
 * Multiple RFBs will compose the entire register file.
 *
 * It will contain 32 registers accessable per thread.
 *
 **/

#ifndef RFB_H
#define RFB_H

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

SC_MODULE(RFB) {
  // First we need to define the Size of our register file
  static constexpr int MAX_THREADS{1};
  static constexpr int MAX_REG_PER_THREAD{32};

  // Define the actual physical RFB structure
  // This gives us MAX_THREADS * MAX_REG_PER_THREAD total registers
  // in our register file.
  sc_signal<sc_int<32>> physical_rf[MAX_THREADS]
                                   [MAX_REG_PER_THREAD]; // Register array

  // Ports: How do we connect to the PEs and how do we connect to the
  // LSU/Memory?
  // Our most complex instruction requires 3 registers FMAD
  // so we need a bus to connect 96bits or data to our crossbar.
  //
  // Other questions that need to be answered:
  // How what is the output input bandwidth per bank?
  // How do we deal with bank collisions?
  // How do banks know what data goes to which PE?

  sc_in<sc_int<32>> bank_input;
  sc_out<sc_int<32>> bank_out;

  SC_CTOR(RFB) {}

  ~RFB();
};

#endif
