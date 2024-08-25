#include "systemc.h"
#include "driver.h"
#include "monitor.h"
#include "hardware.h"

int sc_main(int argc, char* argv[]) {

    sc_clock TestClk("TestClock", 10, SC_NS, 0.5, 1, SC_NS);
    sc_signal<int> w;
    sc_signal<float> fitness;

    driver driver("Driver");
    driver.clock(TestClk);
    driver.w(w);

    hardware hw("hardware");    
    hw.totalW(w);
    hw.fitness(fitness);
    hw.clock(TestClk);

    monitor mon("Monitor");
    mon.fitness(fitness);
    mon.clock(TestClk);

    sc_start();
    return 0; 
}