#include "systemc.h"
#include <iostream>
#include <iomanip>

SC_MODULE(monitor){

    // sc_in<bool> reset;
    // sc_in<bool> read;
    // sc_in<bool> write;
    //sc_in<sc_int<4>> data_in;
    // sc_in<int> data_in;
    // sc_in<int> data_out;
    sc_in<bool> clock;
    sc_in<float> fitness;

    void prc_monitor(){
        // wait();
        cout << std::setw(15) << "Fitness";
        cout << std::setw(15) << sc_time_stamp();
        cout << std::setw(15) << fitness.read() << endl;
        // wait();
    } 

    SC_CTOR(monitor){
        SC_THREAD(prc_monitor);
        sensitive << clock.neg();
    }
};  