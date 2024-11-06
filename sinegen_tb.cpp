#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vsinegen.h"

#include "vbuddy.cpp"     // include vbuddy code
#define MAX_SIM_CYC 1000000
#define ADDRESS_WIDTH 8
#define ROM_SZ 256

int main(int argc, char **argv, char **env) {
  int simcyc;     // simulation clock count
  int tick;       // each clk cycle has two ticks for two edges

  Verilated::commandArgs(argc, argv);
  // init top verilog instance
  Vsinegen* top = new Vsinegen;
  // init trace dump
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;
  top->trace (tfp, 99);
  tfp->open ("sinegen.vcd");
 
  // init Vbuddy
  if (vbdOpen()!=1) return(-1);
  vbdHeader("L2T1: SigGen");

  // initialize simulation inputs
  top->clk = 1;
  top->rst = 0;
  top->incr = 1;

  // run simulation for MAX_SIM_CYC clock cycles
  for (simcyc=0; simcyc<MAX_SIM_CYC; simcyc++) {
    // dump variables into VCD file and toggle clock
    for (tick=0; tick<2; tick++) {
      tfp->dump (2*simcyc+tick);
      top->clk = !top->clk;
      top->eval ();
    }
    
    // Set increment and phase offset from Vbuddy
    top->incr = 1;  // Increment value for counter
    top->phase_offset = vbdValue();  // Phase offset for second sinusoid

    // Plot ROM output for both sinusoids
    vbdPlot(int(top->dout1), 0, 255);
    vbdPlot(int(top->dout2), 0, 255);
    vbdCycle(simcyc);

    // Exit if 'q' pressed or simulation finishes
    if ((Verilated::gotFinish()) || (vbdGetkey() == 'q')) 
      exit(0);
  }

  vbdClose();
  tfp->close(); 
  exit(0);
}