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

template <int WARP_SIZE, int NUM_REG> SC_MODULE(RF) {
  // Registers based on threads and registers
  sc_uint<32> registers[WARP_SIZE][NUM_REG];

  // Control Path
  sc_in<sc_uint<5>> rs1_addr;
  sc_in<sc_uint<5>> rs2_addr;
  sc_in<sc_uint<5>> rd_addr;
  // Per-thread write enable mask
  sc_vector<sc_in<bool>> write_enable;

  // Data Path
  sc_vector<sc_out<sc_int<32>>> data_out_a;
  sc_vector<sc_out<sc_int<32>>> data_out_b;
  sc_vector<sc_out<sc_int<32>>> data_out_c;
  sc_vector<sc_in<sc_int<32>>> data_in_write;

  SC_CTOR(RF)
      : write_enable("write_enable", WARP_SIZE),
        data_out_a("data_out_a", WARP_SIZE),
        data_out_b("data_out_b", WARP_SIZE),
        data_out_c("data_out_c", WARP_SIZE),
        data_in_write("data_in_write", WARP_SIZE) {
    SC_METHOD(read_process);
    sensitive << rs1_addr << rs2_addr << rd_addr;

    SC_METHOD(write_process);
    for (int i = 0; i < WARP_SIZE; i++) {
      sensitive << data_in_write[i] << write_enable[i];
    }
  }

  void read_process() {
    for (int i = 0; i < WARP_SIZE; i++) {
      // Loop through all threads and output their data
      data_out_a[i].write(registers[i][rs1_addr.read()]);
      data_out_b[i].write(registers[i][rs2_addr.read()]);
      data_out_c[i].write(registers[i][rd_addr.read()]);
    }
  }

  void write_process() {
    for (int i = 0; i < WARP_SIZE; i++) {
      if (write_enable[i].read() == true) {
        registers[i][rd_addr.read()] = data_in_write[i].read();
      }
    }
  }

  ~RF();
};

#endif // !RF_H
