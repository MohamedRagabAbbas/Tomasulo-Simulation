#ifndef TABLES_H
#define TABLES_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "OurParser.h"

using namespace std;

#define NReservationStations 12
#define NRegisters 8

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

vector<string> opcodes = {"LOAD1","LOAD2", "STORE",  "BEQ",  "CALL/RET", "ADD/ADDI1", 
                            "ADD/ADDI2","ADD/ADDI3","ADD/ADDI4", "NAND1", "NAND2", "MUL"};

void fillingReservationStation()
{
    for(int i = 0; i < NReservationStations; i++)
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
    mapper["LOAD1"] = {"LOAD"};
    mapper["LOAD2"] = {"LOAD"};
    mapper["STORE"] = {"STORE"};
    mapper["BEQ"] = {"BEQ"};
    mapper["CALL/RET"] = {"CALL", "RET"};
    mapper["ADD/ADDI1"] = {"ADD"};
    mapper["ADD/ADDI2"] = {"ADD"};
    mapper["ADD/ADDI3"] = {"ADD"};
    mapper["ADD/ADDI4"] = {"ADD"};
    mapper["NAND1"] = {"NAND"};
    mapper["NAND2"] = {"NAND"};
    mapper["MUL"] = {"MUL"};
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
    if(!file.is_open())
    {
        cout << "Error opening file" << endl;
        return;
    }
    string line;
    while(getline(file, line))
    {
        instructions_str.push_back(line);
    }
}
vector<Instruction> scheduleStation;
// flling instructions vector
void fillingInstructions()
{
    for(int i = 0; i < instructions_str.size(); i++)
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
    for(int i = 0; i < NRegisters; i++)
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

    for(int i = 0; i < NReservationStations; i++)
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

    for(int i = 0; i < NRegisters; i++)
    {
        cout << setw(15) << registerStatus[i].registerName << setw(10) << registerStatus[i].q << endl;
    }
}

void printScheduleStation()
{
    cout << left; // Left-align the output
    cout << setw(15) << "Instruction ID" << setw(10) << "Name" << setw(10) << "Issue" << setw(10) << "Execute"
         << setw(15) << "Write Result" << endl;

    for(int i = 0; i < scheduleStation.size(); i++)
    {
        cout << setw(15) << scheduleStation[i].instructionId
             << setw(10) << scheduleStation[i].name
             << setw(10) << scheduleStation[i].issuingCycle
             << setw(10) << scheduleStation[i].executionCycle_end
             << setw(15) << scheduleStation[i].writingCycle << endl;
    }
}

#endif // DATA_STRUCTURES_H
