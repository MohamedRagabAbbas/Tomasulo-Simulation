#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void printVector(vector<string> v)
{
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << " ";
    cout << endl;
}

class Instruction {
public:
    int instructionId;
    string name;
    string op;
    int dest;
    int src1;
    int src2;
    int imm;

    Instruction(int instructionId, const string& inst) : instructionId(instructionId), dest(-1), src1(-1), src2(-1), imm(-1) {
        stringstream ss(inst);
        string inst_op;
        vector<string> inst_load_store_vector;
        vector<string> inst_arithmetic_vector;
        vector<string> inst_branch_vector;
        vector<string> inst_call_vector;
        vector<string> inst_ret_vector;
        string word;
        bool first_time = true;
        for (int i = 0; i < inst.length(); i++) {
            if ((inst[i] == ' ' && first_time) || (i == inst.length() - 1 && first_time)) {
                inst_op = inst.substr(0, i);
                if (i == inst.length() - 1)
                    inst_op = inst;
                inst_load_store_vector.push_back(inst_op);
                inst_arithmetic_vector.push_back(inst_op);
                inst_branch_vector.push_back(inst_op);
                inst_call_vector.push_back(inst_op);
                inst_ret_vector.push_back(inst_op);
                first_time = false;
                continue;
            }
            if (inst_op == "LOAD" || inst_op == "STORE") {
                if (inst[i] != ' ' && inst[i] != ',' && inst[i] != '(' && inst[i] != ')')
                    word += inst[i];
                if (inst[i] == ',') {
                    inst_load_store_vector.push_back(word); // dest
                    word = "";
                }
                if (inst[i] == '(') {
                    inst_load_store_vector.push_back(word); // imm
                    word = "";
                    i++;
                    while (inst[i] != ')') {
                        word += inst[i];
                        i++;
                    }
                    inst_load_store_vector.push_back(word); // src1
                    word = "";
                }
            }
            if (inst_op == "ADD" || inst_op == "MUL" || inst_op == "NAND" || inst_op == "ADDI") {
                if (inst[i] != ' ' && inst[i] != ',')
                    word += inst[i];
                if (inst[i] == ',') {
                    inst_arithmetic_vector.push_back(word); // dest or src1
                    word = "";
                }
                if (i == inst.length() - 1) {
                    inst_arithmetic_vector.push_back(word); // src2 or imm
                    word = "";
                }
            }
            if (inst_op == "BEQ") {
                if (inst[i] != ' ' && inst[i] != ',')
                    word += inst[i];
                if (inst[i] == ',') {
                    inst_branch_vector.push_back(word); // src1
                    word = "";
                }
                if (i == inst.length() - 1) {
                    inst_branch_vector.push_back(word); // src2
                    word = "";
                }
                if (i == inst.length() - 1) {
                    inst_branch_vector.push_back(word); // imm
                    word = "";
                }
            }
            if (inst_op == "CALL") {
                if (inst[i] != ' ' && inst[i] != ',')
                    word += inst[i];
                if (i == inst.length() - 1) {
                    inst_call_vector.push_back(word); // imm
                    word = "";
                }
            }
        }
        if (inst_op == "ADD" || inst_op == "MUL" || inst_op == "NAND" || inst_op == "ADDI") {
            printVector(inst_arithmetic_vector);
        } else if (inst_op == "LOAD" || inst_op == "STORE") {
            printVector(inst_load_store_vector);
        } else if (inst_op == "BEQ") {
            printVector(inst_branch_vector);
        } else if (inst_op == "CALL") {
            printVector(inst_call_vector);
        } else if (inst_op == "RET") {
            printVector(inst_ret_vector);
        } else {
            cout << "Invalid instruction" << endl;
        }
        inst_load_store_vector.clear();
        inst_arithmetic_vector.clear();
        inst_branch_vector.clear();
        inst_call_vector.clear();
        inst_ret_vector.clear();
    }
};

#endif
