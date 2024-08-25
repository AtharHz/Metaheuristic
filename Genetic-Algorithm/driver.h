#include "systemc.h"

SC_MODULE(driver) {

    sc_out<int> w;
    sc_in<bool> clock;

    void StimGen() {
        w.write(10);
        wait();
        sc_stop();
    }

    SC_CTOR(driver) {
        SC_THREAD(StimGen);
        sensitive << w;
    }
};