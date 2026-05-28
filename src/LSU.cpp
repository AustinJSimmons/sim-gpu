#include "LSU.h"

void LSU::execute() {
  sc_uint<6> op = opcode.read();
  sc_int<32> addr = mem_addr.read();
  sc_int<32> r = sr_dr.read();

  switch (op) {
  case 0x00:
    // Load register from memory
    break;
  case 0x01:
    // Store register data to memory
    break;
  }
}
