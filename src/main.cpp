/**
 * Main executable for simulating a gpu and its components we imagine this
 * being where each component of the gpu is 'soldered' together.
 **/

#include <systemc>       // include the systemC header file
using namespace sc_core; // use namespace

int sc_main(int, char *[]) { // entry point
  // Instantiate the clock
  // params
  // name, period (1/T), duty_cycle, start_time, initial_value
  // This would be a 1GHz sysclk that spends 50% period as high and starts on
  // the negedge
  sc_clock clk("sysclk", 1, 0.5, 0, false);

  return 0;
}
