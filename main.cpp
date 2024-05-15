#include <iostream>
#include "Instruction.h"

using namespace std;

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
