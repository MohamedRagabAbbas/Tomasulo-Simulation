#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <iomanip>
using namespace std;

#define NReservationStations 12
#define NRegisters 8
int cycle = 0;

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

    int issuingCycle;
    int executionCycle_start;
    int executionCycle_end;
    int writingCycle;

    Instruction(int instructionId, const string& inst) : instructionId(instructionId), dest(-1), src1(-1), src2(-1), imm(-1), issuingCycle(-1), executionCycle_start(-1), executionCycle_end(-1), writingCycle(-1) {
        string inst_op;
        string word;
        bool first_time = true;
        int comma_count = 0;
        for (int i = 0; i < inst.length(); i++) {
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
                        dest = stoi(word.substr(1, word.length() - 1));
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
            else if (inst_op == "RET") {
                // Do nothing
            }
            else
            {
                cout << "Invalid instruction" << endl;
            }
        }
    }
    void issuingCycle_set(int cycle)
    {
        issuingCycle = cycle;
    }
    void executionCycle_start_set(int cycle)
    {
        executionCycle_start = cycle;
    }
    void executionCycle_end_set(int cycle)
    {
        executionCycle_end = cycle;
    }
    void writingCycle_set(int cycle)
    {
        writingCycle = cycle;
    }
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
        cout << "Execution Cycle Start: " << executionCycle_start << endl;
        cout << "Execution Cycle End: " << executionCycle_end << endl;
        cout << "Writing Cycle: " << writingCycle << endl;
    }
};

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
vector<ReservationStation> reservationStation;

vector<string> opcodes = { "LOAD1","LOAD2", "STORE",  "BEQ",  "CALL/RET", "ADD/ADDI1",
                            "ADD/ADDI2","ADD/ADDI3","ADD/ADDI4", "NAND1", "NAND2", "MUL" };

void fillingReservationStation()
{
    for (int i = 0; i < NReservationStations; i++)
    {
        ReservationStation rs;
        rs.instructionId = -1;
        rs.name = opcodes[i];
        rs.busy = false;
        rs.op = "";
        rs.vj = 0; rs.vk = 0;
        rs.qj = 0; rs.qk = 0;
        rs.A = 0;
        reservationStation.push_back(rs);
    }
}

map<string, vector<string>> mapper;

void fillingMapper()
{
    mapper["LOAD1"] = { "LOAD" };
    mapper["LOAD2"] = { "LOAD" };
    mapper["STORE"] = { "STORE" };
    mapper["BEQ"] = { "BEQ" };
    mapper["CALL/RET"] = { "CALL", "RET" };
    mapper["ADD/ADDI1"] = { "ADD" };
    mapper["ADD/ADDI2"] = { "ADD" };
    mapper["ADD/ADDI3"] = { "ADD" };
    mapper["ADD/ADDI4"] = { "ADD" };
    mapper["NAND1"] = { "NAND" };
    mapper["NAND2"] = { "NAND" };
    mapper["MUL"] = { "MUL" };
}

enum instructionType {
    LOAD,
    STORE,
    ADD,
    ADDI,
    MUL,
    BEQ,
    CALL,
    RET
};
vector<string> instructions_str;
void reading_from_file(const string& filename)
{
    fstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        return;
    }
    string line;
    while (getline(file, line))
    {
        instructions_str.push_back(line);
    }
}
vector<Instruction> scheduleStation;
// flling instructions vector
void fillingInstructions()
{
    for (int i = 0; i < instructions_str.size(); i++)
    {
        Instruction inst(i, instructions_str[i]);
        scheduleStation.push_back(inst);
    }
}

struct RegisterStatus {
    string registerName;
    string q;
};
vector<RegisterStatus> fillingRegisterStatus()
{
    vector<RegisterStatus> registerStatus;
    for (int i = 0; i < NRegisters; i++)
    {
        RegisterStatus rs;
        rs.registerName = "" + to_string(i);
        rs.q = "";
        registerStatus.push_back(rs);
    }
    return registerStatus;
}
vector<RegisterStatus> registerStatus = fillingRegisterStatus();
void printReservationStation()
{
    cout << left; // Left-align the output
    cout << setw(15) << "Instruction ID" << setw(10) << "Name" << setw(5) << "Busy" << setw(5) << "Op"
        << setw(5) << "Vj" << setw(5) << "Vk" << setw(5) << "Qj" << setw(5) << "Qk" << setw(5) << "A" << endl;

    for (int i = 0; i < NReservationStations; i++)
    {
        cout << setw(15) << reservationStation[i].instructionId
            << setw(10) << reservationStation[i].name
            << setw(5) << reservationStation[i].busy
            << setw(5) << reservationStation[i].op
            << setw(5) << reservationStation[i].vj
            << setw(5) << reservationStation[i].vk
            << setw(5) << reservationStation[i].qj
            << setw(5) << reservationStation[i].qk
            << setw(5) << reservationStation[i].A << endl;
    }
}

void printRegisterStatus()
{
    cout << left; // Left-align the output
    cout << setw(15) << "Register Name" << setw(10) << "Q" << endl;

    for (int i = 0; i < NRegisters; i++)
    {
        cout << setw(15) << registerStatus[i].registerName << setw(10) << registerStatus[i].q << endl;
    }
}

void printScheduleStation()
{
    cout << left; // Left-align the output
    cout << setw(15) << "Instruction ID" << setw(10) << "Name" << setw(10) << "Issue" << setw(10) << "Execute"
        << setw(15) << "Write Result" << endl;

    for (int i = 0; i < scheduleStation.size(); i++)
    {
        cout << setw(15) << scheduleStation[i].instructionId
            << setw(10) << scheduleStation[i].name
            << setw(10) << scheduleStation[i].issuingCycle
            << setw(10) << scheduleStation[i].executionCycle_end
            << setw(15) << scheduleStation[i].writingCycle << endl;
    }
}

void issue()
{

}
void execute()
{

}
void write()
{

}

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



