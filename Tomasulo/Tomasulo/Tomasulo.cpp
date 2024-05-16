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
// isntruction class
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

// reservation station 
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
        rs.vj = -1; rs.vk = -1;
        rs.qj = -1; rs.qk = -1;
        rs.A = -1;
        reservationStation.push_back(rs);
    }
}

// filling mapper 
map<string, vector<string>> mapper;
void fillingMapper()
{
    mapper["LOAD"] = { "LOAD1","LOAD2"};
    mapper["STORE"] = { "STORE" };
    mapper["BEQ"] = { "BEQ" };
    mapper["CALL"] = { "CALL/RET" };
    mapper["RET"] = { "CALL/RET" };
    mapper["ADD"] = { "ADD/ADDI1","ADD/ADDI2","ADD/ADDI3","ADD/ADDI4" };
    mapper["ADDI"] = { "ADD/ADDI1","ADD/ADDI2","ADD/ADDI3","ADD/ADDI4" };
    mapper["NAND"] = { "NAND1","NAND2"};
    mapper["MUL"] = { "MUL" };
}
map<string, int> numberOfcycles = { {"LOAD", 6}, {"STORE", 6}, {"BEQ", 1}, {"CALL/RET", 1}, {"ADD", 2},
									{"ADDI", 2}, {"NAND", 2}, {"MUL", 10} };

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

// read instructions from file and fill the scheduleStation vector
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

// filling register status
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

// print functions
void printReservationStation()
{
    cout << left; // Left-align the output
    cout << setw(15) << "Instruction ID" << setw(10) << "Name" << setw(5) << "Busy" << setw(10) << "Op"
        << setw(5) << "Vj" << setw(5) << "Vk" << setw(5) << "Qj" << setw(5) << "Qk" << setw(5) << "A" << endl;

    for (int i = 0; i < NReservationStations; i++)
    {
        cout << setw(15) << reservationStation[i].instructionId
            << setw(10) << reservationStation[i].name
            << setw(5) << reservationStation[i].busy
            << setw(10) << reservationStation[i].op
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
    for (int i = 0; i < NRegisters; i++) {
        registerStatus[i].registerName = "Reg" + to_string(i + 1);
    }
    cout << setw(20) << "Register Name";
    for (int i = 0; i < NRegisters; i++) {
        cout << setw(15)<< registerStatus[i].registerName;
    }
	cout << endl;
    cout << setw(15) << "Q";
    for (int i = 0; i < NRegisters; i++)
    {
        cout<< registerStatus[i].q<< setw(20);
    }
    cout << endl;
}
void printScheduleStation()
{
    cout << left; // Left-align the output
    cout << setw(15) << "Instruction ID" << setw(30) << "Name" << setw(10) << "Issue" << setw(10) << "Execute"
        << setw(15) << "Write Result" << endl;

    for (int i = 0; i < scheduleStation.size(); i++)
    {
        cout << setw(15) << scheduleStation[i].instructionId
            << setw(30) << scheduleStation[i].name
            << setw(10) << scheduleStation[i].issuingCycle
            << setw(10) << scheduleStation[i].executionCycle_end
            << setw(15) << scheduleStation[i].writingCycle << endl;
    }
}
void printVector(vector<string> v)
{
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << " ";
    cout << endl;
}

pair<int, bool> isBusy(string func)
{
    pair<int, bool> isBusy_var;
	for (int i = 0; i < NReservationStations; i++)
		if (reservationStation[i].name == func)
            isBusy_var = { i,reservationStation[i].busy };
	return isBusy_var;
}
void issue()
{
	for (int i = 0; i < scheduleStation.size(); i++)
	{
		if (scheduleStation[i].issuingCycle == -1) // means not issued yet
		{
			for (int j = 0; j < mapper[scheduleStation[i].op].size(); j++)
			{
				pair<int, bool> isBusy_var = isBusy(mapper[scheduleStation[i].op][j]);
				if (!isBusy_var.second) // if the reservation station is not busy
				{
					scheduleStation[i].issuingCycle_set(cycle); // set the issuing cycle
					reservationStation[isBusy_var.first].instructionId = scheduleStation[i].instructionId; // set the instruction id
					reservationStation[isBusy_var.first].busy = true; // set the reservation station to busy
					reservationStation[isBusy_var.first].op = scheduleStation[i].op; // set the operation
					if (scheduleStation[i].src1 != -1) // if the source 1 is a register
					{
						if (registerStatus[scheduleStation[i].src1].q == "") // if the register is not busy
						{
							reservationStation[isBusy_var.first].vj = scheduleStation[i].src1; // set the value of the source 1
						}
						else
						{
							reservationStation[isBusy_var.first].qj = scheduleStation[i].src1; // set the reservation station of the source 1
						}
					}
					if (scheduleStation[i].src2 != -1) // if the source 2 is a register
					{
						if (registerStatus[scheduleStation[i].src2].q == "") // if the register is not busy
						{
							reservationStation[isBusy_var.first].vk = scheduleStation[i].src2; // set the value of the source 2
						}
						else
						{
							reservationStation[isBusy_var.first].qk = scheduleStation[i].src2; // set the reservation station of the source 2
						}
					}
					if (scheduleStation[i].dest != -1) // if the destination is a register
					{
						registerStatus[scheduleStation[i].dest].q = reservationStation[isBusy_var.first].name; // set the reservation station of the destination
					}
					return;
				}
			}
		}
	}
}

void execute()
{
	for (int i = 0; i < NReservationStations; i++)
	{
		if (reservationStation[i].busy)
		{
			if (reservationStation[i].qj == -1 && reservationStation[i].qk == -1) // if the sources are ready
			{
				scheduleStation[reservationStation[i].instructionId].executionCycle_start_set(cycle);
				scheduleStation[reservationStation[i].instructionId].executionCycle_end_set(cycle + numberOfcycles[reservationStation[i].op]);
			}
		}
	}
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
	issue();
	cycle++;
    issue();




    printScheduleStation();
	cout << "---------------------------------------------" << endl;
	printReservationStation();
    cout << "---------------------------------------------" << endl;
	printRegisterStatus();
}

int main()
{
	taskManager();
	return 0;
}

