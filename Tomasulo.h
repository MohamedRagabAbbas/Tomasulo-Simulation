#ifndef TOMASULO_H
#define TOMASULO_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "tabels.h"
using namespace std;


// Tomasulo class
class Tomasulo 
{
    int cycle = 0;
    public:
        // instruction queue
        
        void taskManager()
        {
            reading_from_file("instructions.txt");
            fillingInstructions();
            fillingReservationStation();
            fillingMapper();
            printScheduleStation();
            //while()
            // for(int i=0; i < 5; i++)
            // {
            //     cout << "Cycle: " << cycle << endl;
            //     issue();
            //     execute();
            //     write();
            //     printScheduleStation();
            //     printReservationStation();
            //     printRegisterStatus();
            // }
        }


    private:
    void issue()
    {
        
    }
    void execute()
    {

    }
    void write()
    {

    }

    
};


#endif