#include "PE.h"

void PE::demux() {
  // Check opcode decide where to send PE internal signals
  bool is_fpu = is_fpu_switch.read();
  sc_int<32> a = dc_in_a.read();
  sc_int<32> b = dc_in_b.read();
  sc_int<32> c = dc_in_c.read();

  if (is_fpu) {
    fpu_in_a.write(a);
    fpu_in_b.write(b);
    fpu_in_c.write(c);
    demux_is_fpu.write(is_fpu);
  } else {
    alu_in_a.write(a);
    alu_in_b.write(b);
    demux_is_fpu.write(is_fpu);
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
