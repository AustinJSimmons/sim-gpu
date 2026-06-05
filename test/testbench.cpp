/*
 * This file will be used to write test logic for each
 * component as needed.
 **/

#include "testbench.h"
#include "helpers.h"
#include "sysc/kernel/sc_time.h"
#include <iostream>
#include <ostream>
#include <stdio.h>

/*
 * testBench() constructor to make my ctest more intuitive.
 *
 */
TestBench::TestBench(sc_module_name name, std::string testMode)
    : sc_module(name), testMode(testMode) {

  errorCount = 0;
  alu1 = new ALU("ALU_1");
  alu1->op_switch(opcode_alu);
  alu1->input_a(a_alu);
  alu1->input_b(b_alu);
  alu1->result(result_alu);

  fpu1 = new FPU("FPU_1");
  fpu1->op_switch(opcode_fpu);
  fpu1->src1(a_fpu);
  fpu1->src2(b_fpu);
  fpu1->src3(c_fpu);
  fpu1->result(result_fpu);

  pe1 = new PE("PE_1");
  pe1->is_fpu_switch(is_fpu);
  pe1->active_mask_bit(is_active_pe);
  pe1->opcode_in(opcode_pe);
  pe1->dc_in_a(a_pe);
  pe1->dc_in_b(b_pe);
  pe1->dc_in_c(c_pe);
  pe1->dc_out(result_pe);
  pe1->write_enable_out(write_out_enable);

  rf0 = new RF<32, 4>("RF_0");
  rf0->global_read_addr(global_read_addr);
  rf0->global_write_addr(global_write_addr);
  rf0->global_write_enable(global_write_enable);
  rf0->global_write_mask(global_write_mask);
  for (int i = 0; i < 4; i++) {
    rf0->global_read_data_out[i](global_read_data_out[i]);
    rf0->global_write_data_in[i](global_write_data_in[i]);
  }
  rf0->clk(this->clk);

  SC_THREAD(test_alu);
  SC_THREAD(test_fpu);
  SC_THREAD(test_pe);
  SC_THREAD(test_rf);
  sensitive << clk.pos();
}

void TestBench::test_rf() {
  if (testMode == "RF") {
    global_write_enable.write(true);
    global_write_addr.write(13);
    global_read_addr.write(13);

    sc_int<32> data_in[4] = {1, 4, 2, 7};
    sc_bv<4> mask = 0b1111;

    global_write_mask.write(mask);
    for (int i = 0; i < 4; i++) {
      global_write_data_in[i].write(data_in[i]);
    }
    wait();
    wait();

    global_write_enable.write(false);
    global_read_addr.write(13);

    wait();
    wait();
    for (int i = 0; i < 4; i++) {
      if (global_read_data_out[i].read() != data_in[i]) {
        errorCount++;
        std::cout << global_read_data_out[i].read() << " != " << data_in[i]
                  << std::endl;
      }
    }
  }
  sc_stop();
}

/*
 * test_alu()
 * a function used to test each opcode the ALU
 * might receive.
 * */
void TestBench::test_alu() {
  // Init inputs
  a_alu.write(0);
  b_alu.write(0);
  opcode_alu.write(0b0000);
  wait(10, SC_NS);

  // Test ADD 0b0001
  if (testMode == "ADD") {

    a_alu.write(0);
    b_alu.write(1);
    opcode_alu.write(0b0001);
    wait(10, SC_NS);

    if (result_alu.read() == 1) {
      std::cout << "Test Passed 0 + 1 = " << result_alu.read() << std::endl;
    } else {
      std::cout << "Test Failed 0 + 1 != " << result_alu.read() << std::endl;
      errorCount++;
    }

    a_alu.write(-15);
    b_alu.write(20);
    opcode_alu.write(0b0001);
    wait(10, SC_NS);

    if (result_alu.read() == 5) {
      std::cout << "Test Passed -15 + 20 = " << result_alu.read() << std::endl;
    } else {
      std::cout << "Test Failed -15 + 20 !=" << result_alu.read() << std::endl;
      errorCount++;
    }
  }

  // Test SUB 0b0010
  if (testMode == "SUB") {
    a_alu.write(2);
    b_alu.write(1);
    opcode_alu.write(0b0010);
    wait(10, SC_NS);

    if (result_alu.read() == 1) {
      std::cout << "Test Passed 2 - 1 = " << result_alu.read() << std::endl;
    } else {
      std::cout << "Test Failed 2 - 1 != " << result_alu.read() << std::endl;
      errorCount++;
    }
  }

  // Test MUL 0b0011
  if (testMode == "MUL") {
    a_alu.write(0);
    b_alu.write(2147483647); // max twos compliment
    opcode_alu.write(0b0011);
    wait(10, SC_NS);

    if (result_alu.read() != 0) {
      std::cout << "Test Failed MUL by 0" << std::endl;
      errorCount++;
    }
  }

  // Test AND 0b0100
  if (testMode == "ANND") {
    a_alu.write(1);
    b_alu.write(1);
    opcode_alu.write(0b0100);
    wait(10, SC_NS);

    if (result_alu.read() != 1) {
      std::cout << "Test Failed 1 AND 1 = 1" << std::endl;
      errorCount++;
    }

    a_alu.write(1);
    b_alu.write(2);
    opcode_alu.write(0b0100);
    wait(10, SC_NS);

    if (result_alu.read() != 0) {
      std::cout << "Test Failed 01 AND 10 = 00" << std::endl;
      errorCount++;
    }
  }

  // Test OR 0b0101
  if (testMode == "ORR") {
    a_alu.write(1);
    b_alu.write(2);
    opcode_alu.write(0b0101);
    wait(10, SC_NS);

    if (result_alu.read() != 3) {
      std::cout << "Test Failed 01 OR 10 = 11" << std::endl;
      errorCount++;
    }
  }
  // Test NOT 0b0110
  if (testMode == "NOTT") {
    a_alu.write(0);
    opcode_alu.write(0b0110);
    wait(10, SC_NS);

    if (result_alu.read() != -1) {
      std::cout << "Test Failed NOT 0 = -1 != " << result_alu.read()
                << std::endl;
      errorCount++;
    }
  }
  // Test XOR 0b0111
  if (testMode == "XORR") {
    a_alu.write(1);
    b_alu.write(2);
    opcode_alu.write(0b0111);
    wait(10, SC_NS);

    if (result_alu.read() != 3) {
      std::cout << "Test Failed 01 XOR 10 = 11" << std::endl;
      errorCount++;
    }

    a_alu.write(1);
    b_alu.write(1);
    opcode_alu.write(0b0111);
    wait(10, SC_NS);

    if (result_alu.read() != 0) {
      std::cout << "Test Failed 01 XOR 01 = 11" << std::endl;
      errorCount++;
    }
  }
  // Test LS 0b1000
  if (testMode == "LS") {
    a_alu.write(1);
    b_alu.write(2);
    opcode_alu.write(0b1000);
    wait(10, SC_NS);

    if (result_alu.read() != 4) {
      std::cout << "Test Failed 0001 << 1 = 0100 = 4" << std::endl;
      errorCount++;
    }
  }
  // Test RS 0b1001
  if (testMode == "RS") {
    a_alu.write(4);
    b_alu.write(2);
    opcode_alu.write(0b1001);
    wait(10, SC_NS);

    if (result_alu.read() != 1) {
      std::cout << "Test Failed 0100 >> 2 = 0001 = 1" << std::endl;
      errorCount++;
    }
  }

  sc_stop();
}

bool rough_equal(float a, float b, float eps = 1e-5f) {
  if (a == b)
    return true;

  if (std::abs(a - b) < eps) {
    return true;
  }

  return false;
}

void TestBench::test_fpu() {
  // Test FADD 0b0001
  if (testMode == "FADD") {
    float a, b;

    a = 0.1f;
    b = 0.1f;

    int raw_a = typePunning<int>(a);
    int raw_b = typePunning<int>(b);

    a_fpu.write(raw_a);
    b_fpu.write(raw_b);
    opcode_fpu.write(0b0001);
    wait(10, SC_NS);

    int raw_res = result_fpu.read();
    float res = typePunning<float>(raw_res);

    if (!::rough_equal(res, 0.2f)) {
      std::cout << "Test Failed 0.1 + 0.1 != " << res << std::endl;
      std::cout << "a:" << a_fpu.read() << " b:" << b_fpu.read() << std::endl;
      errorCount++;
    }
  }
  // Test FSUB 0b0010
  if (testMode == "FSUB") {
    float a, b;
    a = 0.1f;
    b = 0.1f;

    int raw_a = typePunning<int>(a);
    int raw_b = typePunning<int>(b);
    a_fpu.write(raw_a);
    b_fpu.write(raw_b);
    opcode_fpu.write(0b0010);
    wait(10, SC_NS);

    int raw_res = result_fpu.read();
    float res = typePunning<float>(raw_res);

    if (!::rough_equal(res, 0.0f)) {
      std::cout << "Test Failed 0.1 - 0.1 != " << res << std::endl;
      errorCount++;
    }
  }
  // Test FMUL 0b0011
  if (testMode == "FMUL") {
    float a, b;
    a = 0.1f;
    b = 0.1f;

    int raw_a = typePunning<int>(a);
    int raw_b = typePunning<int>(b);
    a_fpu.write(raw_a);
    b_fpu.write(raw_b);
    opcode_fpu.write(0b0011);
    wait(10, SC_NS);

    int raw_res = result_fpu.read();
    float res = typePunning<float>(raw_res);
    std::cout << "res = " << res << std::endl;

    if (!::rough_equal(res, 0.01)) {
      std::cout << "Test Failed 0.1 * 0.1 != " << res << std::endl;
      errorCount++;
    }
  }
  // Test FDIV 0b0100
  if (testMode == "FDIV") {
    float a, b;
    a = 0.1f;
    b = 0.1f;

    int raw_a = typePunning<int>(a);
    int raw_b = typePunning<int>(b);
    a_fpu.write(raw_a);
    b_fpu.write(raw_b);
    opcode_fpu.write(0b0100);
    wait(10, SC_NS);

    int raw_res = result_fpu.read();
    float res = typePunning<float>(raw_res);

    if (res != 1.0f) {
      std::cout << "Test Failed 0.1/0.1 != " << res << std::endl;
      errorCount++;
    }
  }
  // Test FMAD 0b0101
  if (testMode == "FMAD") {
    float a, b, c;
    a = 1.0f;
    b = 0.1f;
    c = 0.1f;

    int raw_a = typePunning<int>(a);
    int raw_b = typePunning<int>(b);
    int raw_c = typePunning<int>(c);
    a_fpu.write(raw_a);
    b_fpu.write(raw_b);
    c_fpu.write(raw_c);
    opcode_fpu.write(0b0101);
    wait(10, SC_NS);

    int raw_res = result_fpu.read();
    float res = typePunning<float>(raw_res);

    if (!::rough_equal(res, 0.2f)) {
      std::cout << "Test Failed (1.0 * 0.1) + 0.1 != " << res << std::endl;
      errorCount++;
    }
  }
  // Test FMIN 0b0110
  if (testMode == "FMIN") {
    float a, b;
    a = 0.01f;
    b = 0.1f;

    int raw_a = typePunning<int>(a);
    int raw_b = typePunning<int>(b);
    a_fpu.write(raw_a);
    b_fpu.write(raw_b);
    opcode_fpu.write(0b0110);
    wait(10, SC_NS);

    int raw_res = result_fpu.read();
    float res = typePunning<float>(raw_res);

    if (!::rough_equal(res, 0.01f)) {
      std::cout << "Test Failed Min(0.01, 0.1) != " << res << std::endl;
      errorCount++;
    }
  }
  // Test FMAX 0b0111
  if (testMode == "FMAX") {
    float a, b;
    a = 0.01f;
    b = 0.1f;

    int raw_a = typePunning<int>(a);
    int raw_b = typePunning<int>(b);
    a_fpu.write(raw_a);
    b_fpu.write(raw_b);
    opcode_fpu.write(0b0111);
    wait(10, SC_NS);

    int raw_res = result_fpu.read();
    float res = typePunning<float>(raw_res);

    if (!::rough_equal(res, 0.1f)) {
      std::cout << "Test Failed Max(0.01, 0.1) != " << res << std::endl;
      errorCount++;
    }
  }
}

void TestBench::test_pe() {
  if (testMode == "PADD") {
    is_fpu.write(false);
    is_active_pe.write(true);
    a_pe.write(12);
    b_pe.write(30);
    opcode_pe.write(0b0001);
    wait(30, SC_NS);

    if (result_pe.read() != 42) {
      std::cout << "Test Failed 12 + 30 = 42 != " << result_pe.read()
                << std::endl;
      errorCount++;
    }
  }

  if (testMode == "PFAD") {
    float a, b;
    a = 12.30f;
    b = 30.12f;

    int raw_a = typePunning<int>(a);
    int raw_b = typePunning<int>(b);

    is_fpu.write(true);
    a_pe.write(raw_a);
    b_pe.write(raw_b);
    opcode_pe.write(0b0001);
    wait(30, SC_NS);

    int raw_res = result_pe.read();
    float res = typePunning<float>(raw_res);

    if (!::rough_equal(res, 42.42f)) {
      std::cout << "Test Faild 12.30 + 30.12 = 42.42 != " << res << std::endl;
      errorCount++;
    }
  }
}
