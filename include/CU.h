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

template <int WARP_SIZE, int NUM_INSTRUCTION> SC_MODULE(CU) {
  // Clock in to execute pipeline
  sc_in_clk clk;
  // I-Cache Instruction cache
  sc_bv<32> i_cache[NUM_INSTRUCTION];

  // PC (Program Counter)
  sc_signal<sc_uint<32>> pc_reg;

  sc_out<sc_bv<6>> opcode;

  // M
  sc_out<sc_uint<5>> rs1_addr;
  sc_out<sc_uint<5>> rs2_addr;
  sc_out<sc_uint<5>> rd_addr;

  // S
  sc_out<sc_uint<16>> offset;

  // C
  sc_out<sc_bv<4>> preds;
  sc_out<sc_uint<22>> c_offset;

  SC_CTOR(CU) {
    pc_reg.write(0); // init the pc reg
    SC_METHOD(execute_pipeline);
    sensitive << clk.pos();
  };

  void execute_pipeline() {
    // Fetch
    sc_bv<32> raw_instruction = i_cache[pc_reg.read()];
    sc_bv<6> op_val = raw_instruction.range(5, 0);
    // Decode and dispatch
    opcode.write(raw_instruction.range(5, 0));
    if (op_val.to_uint() <= 0x01) {
      rs1_addr.write(raw_instruction.range(15, 11).to_uint());
      rs2_addr.write(raw_instruction.range(10, 6).to_uint());
      rd_addr.write(raw_instruction.range(10, 6).to_uint());
      offset.write(raw_instruction.range(31, 16).to_uint());

    } else if (0x02 <= op_val.to_uint() && op_val.to_uint() <= 0x13) {
      rs1_addr.write(raw_instruction.range(10, 6).to_uint());
      rs2_addr.write(raw_instruction.range(15, 11).to_uint());
      rd_addr.write(raw_instruction.range(20, 16).to_uint());

    } else {
      preds.write(raw_instruction.range(9, 6));
      c_offset.write(raw_instruction.range(31, 10).to_uint());
    }

    pc_reg.write(pc_reg.read() + 1);
  }

  ~CU(){};
};

#endif // !CU_H
