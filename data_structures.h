#include <iostream>
#include <string>
#include <stringstream>
using namespace std;
struct ReservationStation {
    int instructionId;
    string name;
    bool busy;
    string op;
    int vj;
    int vk;
    int qj;
    int qk;
    int A;
};
struct Scheduler{
    string instruction;
    int issue;
    pair<int, int> execute;
    int read;
};
struct RegisterStatus {
    string registerName;
    int q;
};

class Instruction {
    int instructionId;
    string name;
    string op;
    int dest;
    int src1;
    int src2;
    int imm;
    Instruction(int instructionId, string inst) {
        stringstream stream(inst);
        string inst_op, inst_dest, inst_src1, inst_src2;
        ss >> inst_op;

        // -->Load/store instructions
        if(inst_op == "LOAD") {
            ss >> dest >> src1;
            op = given_op;
        }
        else if(inst_op == "STORE") {
            ss >> src1 >> src2;
            op = given_op;
        }
        // --> Conditional branch instructions
        else if (inst_op == "BEQ") {
            ss >> src1 >> src2 >> imm;
            op = given_op;
        }
        // --> Call/Return instructions
        else if (inst_op == "CALL") {
            ss >> imm;
            op = given_op;
        }
        else if (inst_op == "RET") {
            op = given_op;
        }
        // --> Arithmetic instructions
        else if (inst_op == "ADD" || inst_op == "MUL"|| inst_op == "NAND") {
            ss >> dest >> src1 >> src2;
            op = given_op;
        }
        else if (inst_op == "ADDI") {
            ss >> dest >> src1 >> imm;
            op = given_op;
        }
        else {
            cout << "Invalid instruction" << endl;
        }
    }
        
    
    
};
