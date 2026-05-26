#include "FPU.h"
#include <cmath>
#include <cstring>

void FPU::execute() {
  sc_uint<6> op = opcode.read();
  int a = src1.read();
  int b = src2.read();

  float f_a;
  float f_b;
  float f_r = 0.0f;

  std::memcpy(&f_a, &a, 4);
  std::memcpy(&f_b, &b, 4);
  switch (op) {
  case 0x0D:
    f_r = f_a + f_b;
    break;
  case 0x0E:
    f_r = f_a - f_b;
    break;
  case 0x0F:
    f_r = f_a * f_b;
    break;
  case 0x10:
    f_r = f_a / f_b;
    break;
  case 0x11:
    f_r = std::fmaf(f_r, f_a, f_b);
    break;
  case 0x12:
    f_r = std::min(f_a, f_b);
    break;
  case 0x13:
    f_r = std::max(f_a, f_b);
    break;
  default:
    f_r = 0.0f;
  };

  int res;
  std::memcpy(&res, &f_r, 4);

  result.write(res);
}

FPU::~FPU() {}
