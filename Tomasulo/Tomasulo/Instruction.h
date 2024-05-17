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
    int rA, rB, rC;
    int imm;

    int issuingCycle;
    int executionCycleStart;
    int executionCycleEnd;
    int writingCycle;

    Instruction(int instructionId, const string& inst) : instructionId(instructionId), rA(-1), rB(-1), rC(-1), imm(-1), issuingCycle(-1), executionCycleStart(-1), executionCycleEnd(-1), writingCycle(-1) {
        name = inst;

        int index = inst.find(' ');
        string inst_type = inst.substr(0, index);

        if (inst_type == "LOAD" || inst_type == "STORE") {
            parse_LS_inst(inst);
        }
        else if (inst_type == "ADD" || inst_type == "NAND" || inst_type == "MUL") {
            parse_R_inst(inst);
        }
        else if (inst_type == "ADDI") {
            parse_I_inst(inst);
        }
        else if (inst_type == "BEQ") {
            parse_BRANCH_inst(inst);
        }
        else if (inst_type == "CALL") {
            parse_CALL_inst(inst);
        }
        else if (inst_type == "RET") {
            parse_RET_inst(inst);
        }
        else {
            cout << inst << " is an invalid instruction\n";
            exit(1);
        }
    }

    void parse_LS_inst(const string& inst) {
        int index = inst.find(' ');
        op = inst.substr(0, index);

        while (inst[index] == ' ') index++;
        rA = inst[index + 1] - '0';

        index += 2;
        while (inst[index] == ' ' || inst[index] == ',') index++;
        int bracket = inst.find('(');

        imm = stoi(inst.substr(index, bracket - index + 1));

        rB = inst[bracket + 2] - '0';

    }

    void parse_R_inst(const string& inst) {
        int index = inst.find(' ');
        op = inst.substr(0, index);

        while (inst[index] == ' ') index++;
        rA = inst[index + 1] - '0';

        index += 2;
        while (inst[index] == ' ' || inst[index] == ',') index++;
        rB = inst[index + 1] - '0';

        index += 2;
        while (inst[index] == ' ' || inst[index] == ',') index++;
        rC = inst[index + 1] - '0';

        //cout << "rA: " << rA << " rB: " << rB << " rC: " << rC << "\n";
    }

    void parse_I_inst(const string& inst) {
        int index = inst.find(' ');
        op = inst.substr(0, index);

        while (inst[index] == ' ') index++;
        rA = inst[index + 1] - '0';

        index += 2;
        while (inst[index] == ' ' || inst[index] == ',') index++;
        rB = inst[index + 1] - '0';

        index += 2;
        while (inst[index] == ' ' || inst[index] == ',') index++;

        int stop = index;
        while (inst[stop + 1] != ' ') stop++;

        imm = stoi(inst.substr(index, stop - index + 1));

        //cout << "rA: " << rA << " rB: " << rB << " imm: " << imm << "\n";
    }

    void parse_BRANCH_inst(const string& inst) {
        // Implementation for BEQ parsing
    }

    void parse_CALL_inst(const string& inst) {
        // Implementation for CALL parsing
    }

    void parse_RET_inst(const string& inst) {
        // Implementation for RET parsing
    }

    void print() {
        cout << "Instruction ID: " << instructionId << endl;
        cout << "Name: " << name << endl;
        cout << "Operation: " << op << endl;
        cout << "Destination: " << rA << endl;
        cout << "Source 1: " << rB << endl;
        cout << "Source 2: " << rC << endl;
        cout << "Immediate: " << imm << endl;
        cout << "Issuing Cycle: " << issuingCycle << endl;
        cout << "Execution Cycle Start: " << executionCycleStart << endl;
        cout << "Execution Cycle End: " << executionCycleEnd << endl;
        cout << "Writing Cycle: " << writingCycle << endl;
    }
};



