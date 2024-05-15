#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Instruction.h"

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
                            "ADD/ADDI2","ADD/ADDI3","ADD/ADDI4" "NAND1", "NAND2", "MUL"};
void fillingReservationStation()
{
        for(int i =0 ; i < NReservationStations; i++)
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
map<std::string, std::vector<std::string>> mapper;
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
vector<string> instructions;
vector<Instruction> scheduleStation;
struct RegisterStatus {
    string registerName;
    int q;
};
vector<RegisterStatus> registerStatus;

