#include "PE.h"

void PE::demux() {
  // Check opcode decide where to send PE internal signals
  sc_uint<6> op = opcode.read();
  sc_int<32> a = dc_in_a.read();
  sc_int<32> b = dc_in_b.read();

  switch (op) {
  case 0x0D:
  case 0x0E:
  case 0x0F:
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
    fpu_in_a.write(a);
    fpu_in_b.write(b);
    fpu_op.write(op);
    demux_is_fpu.write(true);
    break;
  default:
    alu_in_a.write(a);
    alu_in_b.write(b);
    alu_op.write(op);
    demux_is_fpu.write(false);
    break;
  }
}

void PE::mux() {
  if (demux_is_fpu.read()) {
    dc_out.write(fpu_result.read());
  } else {
    dc_out.write(alu_result.read());
  }
}

PE::~PE() {}
