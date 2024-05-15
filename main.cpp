#include <iostream>
#include "Instruction.h"
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

int main() {
    cout << "Hello World" << std::endl;
    // For testing
    Instruction instr1(1, "LOAD R1,10(R2)");
    Instruction instr2(2, "BEQ R1,R2,15");
    Instruction instr3(3, "ADD R1,R2,R3");
    Instruction instr4(4, "ADDI R1,R2,10");
    Instruction instr5(5, "CALL 100");
    Instruction instr6(6, "RET");

    return 0;
}
