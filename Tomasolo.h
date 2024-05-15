#ifndef TOMASULO_H
#define TOMASULO_H

#include <iostream>
#include "Instruction.h"
#include "data_structures.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> reading_from_file(string file_name)
{
    ifstream file(file_name);
    vector<string> instructions;
    string line;
    while (getline(file, line))
    {
        instructions.push_back(line);
    }
    return instructions;
}
class Tomasulo 
{
    public:
        // instruction queue
        vector<string> instructionQueue = reading_from_file("instructions.txt");


    private:
    void issue(Instruction instr)
    {
        
    }
    void execute(Instruction instr)
    {
        cout << "Executing instruction: " << instr.name << endl;
    }
    void write(Instruction instr)
    {
        cout << "Writing instruction: " << instr.name << endl;
    }
};


#endif