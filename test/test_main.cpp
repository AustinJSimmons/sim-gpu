/*
 * This file runs the simulation and testbench suite.
 **/
#include "systemc"
#include "testbench.h"

int sc_main(int argc, char **argv) {
  // Init testbench
  std::string test_mode;

  if (argc > 1) {
    test_mode = argv[1];
  }

  TestBench testbench_instance("testbench_instance", test_mode);

  // Start Sim
  sc_start();

  if (testbench_instance.errorCount != 0) {
    return 1;
  }

  return 0;
}
