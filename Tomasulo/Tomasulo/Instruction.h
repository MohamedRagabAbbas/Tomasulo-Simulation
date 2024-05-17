#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <iomanip>

using namespace std;

class Instruction {
public:
    int instructionId;
    string name;
    string op;
    int dest;
    int src1;
    int src2;
    int imm;

    int issuingCycle;
    int executionCycleStart;
    int executionCycleEnd;
    int writingCycle;

    Instruction(int instructionId, const string& inst) : instructionId(instructionId), dest(-1), src1(-1), src2(-1), imm(-1), issuingCycle(-1), executionCycleStart(-1), executionCycleEnd(-1), writingCycle(-1) {
        string inst_op;
        string word;
        name = inst;
        bool first_time = true;
        int comma_count = 0;
        int i = 0;
        for (int j = 0; j < inst.length(); j++)
        {
            if (inst[j] == ' ')
            {
                i = j;
                break;
            }
            inst_op = word;
            word = "";
        }
        for (; i < inst.length(); i++) {
            if ((inst[i] == ' ' && first_time) || (i == inst.length() - 1 && first_time)) {
                inst_op = inst.substr(0, i);
                if (i == inst.length() - 1)
                    inst_op = inst;
                op = inst_op;
                first_time = false;
                continue;
            }
            if (inst_op == "LOAD" || inst_op == "STORE") {
                if (inst[i] != ' ' && inst[i] != ',' && inst[i] != '(' && inst[i] != ')')
                    word += inst[i];
                if (inst[i] == ',') {
                    if (inst_op == "LOAD")
                    {
                        dest = stoi(word.substr(1, word.length() - 1));
                    }
                    else
                    {
                        src1 = stoi(word.substr(1, word.length() - 1));
                    }
                    word = "";
                }
                if (inst[i] == '(') {
                    imm = stoi(word);
                    word = "";
                    i++;
                    while (inst[i] != ')') {
                        word += inst[i];
                        i++;
                    }
                    if (inst_op == "LOAD")
                    {
                        //dest = stoi(word.substr(1, word.length() - 1));  used to overwrite correct destination
                    }
                    else
                    {
                        dest = stoi(word.substr(1, word.length() - 1));
                    }
                    word = "";
                }
            }
            else if (inst_op == "ADD" || inst_op == "MUL" || inst_op == "NAND" || inst_op == "ADDI") {
                if (inst[i] != ' ' && inst[i] != ',')
                    word += inst[i];
                if (inst[i] == ',') {
                    comma_count++;
                    if (comma_count == 1) {
                        dest = stoi(word.substr(1, word.length() - 1));
                    }
                    if (comma_count == 2) {
                        src1 = stoi(word.substr(1, word.length() - 1));
                    }
                    word = "";
                }
                if (i == inst.length() - 1) {
                    if (inst_op == "ADDI") {
                        imm = stoi(word);
                    }
                    else {
                        src2 = stoi(word.substr(1, word.length() - 1));
                    }
                    word = "";
                }
            }
            else if (inst_op == "BEQ") {
                if (inst[i] != ' ' && inst[i] != ',')
                    word += inst[i];
                if (inst[i] == ',') {
                    if (comma_count == 0)
                    {
                        dest = stoi(word.substr(1, word.length() - 1));
                    }
                    else if (comma_count == 1)
                    {
                        src1 = stoi(word.substr(1, word.length() - 1));
                    }
                    word = "";
                }
                if (i == inst.length() - 1) {
                    imm = stoi(word);
                    word = "";
                }
            }
            else if (inst_op == "CALL") {
                if (inst[i] != ' ' && inst[i] != ',')
                    word += inst[i];
                if (i == inst.length() - 1) {
                    imm = stoi(word);
                    word = "";
                }
            }
            else if (inst == "RET") {
                op = inst_op;
            }
            else
            {
                cout << "Invalid instruction" << endl;
            }
        }
    }
    // void issuingCycle_set(int cycle)
    // {
    //     issuingCycle = cycle;
    // }
    // void setExecutionCycleStart(int cycle)
    // {
    //     executionCycleStart = cycle;
    // }
    // void setExecutionCycleEnd(int cycle)
    // {
    //     executionCycleEnd = cycle;
    // }
    // void writingCycle_set(int cycle)
    // {
    //     writingCycle = cycle;
    // }
    void print()
    {
        cout << "Instruction ID: " << instructionId << endl;
        cout << "Name: " << name << endl;
        cout << "Operation: " << op << endl;
        cout << "Destination: " << dest << endl;
        cout << "Source 1: " << src1 << endl;
        cout << "Source 2: " << src2 << endl;
        cout << "Immediate: " << imm << endl;
        cout << "Issuing Cycle: " << issuingCycle << endl;
        cout << "Execution Cycle Start: " << executionCycleStart << endl;
        cout << "Execution Cycle End: " << executionCycleEnd << endl;
        cout << "Writing Cycle: " << writingCycle << endl;
    }
};