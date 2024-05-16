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
int PC = 0; // program counter
vector<int>R(NRegisters, -1);
int jumpTo = 0; // the instruction to jump to
bool isJump = false; // if the program is jumping
int cycle = 0;
bool isFinished = false;
int writng_counter = 0;
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
	int id;
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
		rs.id = i;
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
    cout << setw(5) << "Id" <<setw(15) << "Instruction ID" << setw(10) << "Name" << setw(5) << "Busy" << setw(10) << "Op"
        << setw(5) << "Vj" << setw(5) << "Vk" << setw(5) << "Qj" << setw(5) << "Qk" << setw(5) << "A" << endl;

    for (int i = 0; i < NReservationStations; i++)
    {
		cout<< setw(5) << reservationStation[i].id
            << setw(15) << reservationStation[i].instructionId
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
    cout << setw(20) << "Register Name" << setw(20) << "Q" << endl;
    for (int i = 0; i < NRegisters; i++) {
		cout << setw(20) << registerStatus[i].registerName << setw(20) << registerStatus[i].q << endl;
    }
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


int get_reservation_station_id(string func)
{
	for (int i = 0; i < NReservationStations; i++)
		if (reservationStation[i].name == func)
			return i;
	return -1;
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
	//for (int i = PC; i < scheduleStation.size(); i++)
	//{
	//	if (R[1] == -1) // means the return address is set
 //       {
            if (PC >= scheduleStation.size())
	        {
		        return;
	        }
	        int i = PC;
            //if (scheduleStation[i].issuingCycle == -1) // means not issued yet
            //{
                for (int j = 0; j < mapper[scheduleStation[i].op].size(); j++)
                {
                    pair<int, bool> isBusy_var = isBusy(mapper[scheduleStation[i].op][j]);
                    if (!isBusy_var.second) // if the reservation station is not busy
                    {
                        scheduleStation[i].issuingCycle_set(cycle); // set the issuing cycle
                        PC++;
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
                                reservationStation[isBusy_var.first].qj = get_reservation_station_id(registerStatus[scheduleStation[i].src1].q); // set the reservation station of the source 1
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
                                reservationStation[isBusy_var.first].qk = get_reservation_station_id(registerStatus[scheduleStation[i].src2].q);; // set the reservation station of the source 2
                            }
                        }
                        if (scheduleStation[i].dest != -1) // if the destination is a register
                        {
                            registerStatus[scheduleStation[i].dest].q = reservationStation[isBusy_var.first].name; // set the reservation station of the destination
                        }
                        if (scheduleStation[i].op == "LOAD" || scheduleStation[i].op == "STORE") // if the operation is load or store
                        {
                            reservationStation[isBusy_var.first].A = scheduleStation[i].imm;
                        }
                        return;
                    }
             //   }
        //    }
        //}
		
	}
}

int get_instruction_index(int instructionId)
{
	for (int i = 0; i < scheduleStation.size(); i++)
		if (scheduleStation[i].instructionId == instructionId)
			return i;
	return -1;
}
void execute()
{
	for (int i = 0; i < NReservationStations; i++)
	{
		if (reservationStation[i].busy)
		{
			if (scheduleStation[reservationStation[i].instructionId].executionCycle_start == -1) // if the instruction is not executed yet
			{
                if (reservationStation[i].qj == -1 && reservationStation[i].qk == -1) // if the sources are ready
                {
                    scheduleStation[reservationStation[i].instructionId].executionCycle_start_set(cycle);
                    scheduleStation[reservationStation[i].instructionId].executionCycle_end_set(cycle + numberOfcycles[reservationStation[i].op]);
                }
			}
		}
	}
}

void writeResult()
{
	for (int i = 0; i < NReservationStations; i++)
	{
		if (reservationStation[i].busy)
		{
			if (scheduleStation[reservationStation[i].instructionId].executionCycle_end + 1  == cycle) // if the instruction is executed
			{
				int instruction_index = get_instruction_index(reservationStation[i].instructionId);
                if (instruction_index != -1)
                {
                    scheduleStation[instruction_index].writingCycle_set(cycle);
                    writng_counter++;
                    reservationStation[i].busy = false;
                    reservationStation[i].op = "";
                    reservationStation[i].vj = -1;
                    reservationStation[i].vk = -1;
                    reservationStation[i].qj = -1;
                    reservationStation[i].qk = -1;
                    reservationStation[i].A = -1;
					// if it is call set R[1] to the return address which is the next instruction ctyle + 1
					if (scheduleStation[instruction_index].op == "CALL")
					{
						R[1] = cycle + 1;
						bool isJump = true;
						jumpTo = scheduleStation[instruction_index].imm;
						PC = jumpTo;
					}
					// if it is ret set the cycle to the return address
                    if (scheduleStation[instruction_index].op == "RET")
                    {
                        PC = R[1];
                    }
                    // update the reservation stations
                    for (int j = 0; j < NReservationStations; j++)
                    {
                        if (reservationStation[j].qj == reservationStation[i].id)
                        {
                            reservationStation[j].qj = -1;
                            // update vj
                            for (int k = 0; k < scheduleStation.size(); k++)
                            {
                                if (scheduleStation[k].instructionId == reservationStation[j].instructionId)
                                {
                                    reservationStation[j].vj = scheduleStation[k].src1;
                                    break;
                                }
                            }
                        }
                        if (reservationStation[j].qk == reservationStation[i].id)
                        {
                            reservationStation[j].qk = -1;
                            // update vk
                            for (int k = 0; k < scheduleStation.size(); k++)
                            {
                                if (scheduleStation[k].instructionId == reservationStation[j].instructionId)
                                {
                                    reservationStation[j].vk = scheduleStation[k].src2;
                                    break;
                                }
                            }
                        }
                    }

                    // update the register status
                    for (int j = 0; j < NRegisters; j++)
                    {
                        if (registerStatus[j].q == reservationStation[i].name)
                        {
                            registerStatus[j].q = "";
                        }
                    }
                    

                }
				
			}
		}
	}
}

void runOneStep()
{
	issue();
	execute();
	writeResult();
	cycle++;
}
void print()
{
	cout << "------------------------------"<< "Cycle: " << cycle-1<<"------------------------------" << endl;
	cout << "---------------------------------------------" << endl;
	printScheduleStation();
    cout << "---------------------------------------------" << endl;
	printReservationStation();
    cout << "---------------------------------------------" << endl;
	printRegisterStatus();
    cout << "---------------------------------------------" << endl;
}
void taskManager()
{
    reading_from_file("instructions.txt");
    fillingInstructions();
    fillingReservationStation();
    fillingMapper();

    while (writng_counter != scheduleStation.size())
    {
        issue();
        execute(); 
        writeResult();
        cycle++;
    }
    printScheduleStation();
   /* runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();
    print();
    runOneStep();*/
  
	/*while (writng_counter != scheduleStation.size())
	{
		issue();
		execute();
		writeResult();
		cycle++;
	}*/
	
}

int main()
{
	taskManager();
	return 0;
}

