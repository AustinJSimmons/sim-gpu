/*
 * cpp files will be used to fill out logic of functions
 * for each module etc...
 **/

// Must include the component header
#include "ALU.h"

void ALU::execute() {
  sc_uint<4> op = opcode.read();
  sc_int<32> a = input_a.read();
  sc_int<32> b = input_b.read();

  sc_int<32> res = 0;
  // Define different actions for each Oppcode
  switch (op) {
  case 0x03:
    res = a + b;
    break; // ADD
  case 0x04:
    res = a - b;
    break; // SUB
  case 0x05:
    res = a * b;
    break; // MUL
  default:
    res = 0;
  }

  result.write(res);
}

ALU::~ALU() {}
