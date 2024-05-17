#include "Instruction.h"

#define NReservationStations 12
#define NRegisters 8

short int memory[1 << 16];             // 2 bytes * 2^16 ~ 128kb (short int is 2 bytes)
int PC = 0;

vector<int> registers(NRegisters, 0);

int jumpTo = 0; // the instruction to jump to
bool isJump = false; // if the program is jumping
int numberOfJumps = 0; // number of jumps

int cycle = 1;
bool isFinished = false;
int writng_counter = 0;

struct ReservationStation {
    string name;
    string op;
    int id;
    int instructionId;
    bool busy;
    int vj, vk;
    int qj, qk;
    int A;
    int result;
};

vector<ReservationStation> reservationStation;

vector<string> opcodes = { "LOAD1", "LOAD2", "STORE", "BEQ", "CALL/RET", "ADD/ADDI1",
                            "ADD/ADDI2", "ADD/ADDI3", "ADD/ADDI4", "NAND1", "NAND2", "MUL" };

void fillingReservationStation() {
    for (int i = 0; i < NReservationStations; i++) {
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

map<string, vector<string>> mapper;

void fillingMapper() {
    mapper["LOAD"] = { "LOAD1", "LOAD2" };
    mapper["STORE"] = { "STORE" };
    mapper["BEQ"] = { "BEQ" };
    mapper["CALL"] = { "CALL/RET" };
    mapper["RET"] = { "CALL/RET" };
    mapper["ADD"] = { "ADD/ADDI1", "ADD/ADDI2", "ADD/ADDI3", "ADD/ADDI4" };
    mapper["ADDI"] = { "ADD/ADDI1", "ADD/ADDI2", "ADD/ADDI3", "ADD/ADDI4" };
    mapper["NAND"] = { "NAND1", "NAND2" };
    mapper["MUL"] = { "MUL" };
}

map<string, int> numberOfcycles = { {"LOAD", 6}, {"STORE", 6}, {"BEQ", 1}, {"CALL", 1},{"RET", 1},
                                    {"ADD", 2}, {"ADDI", 2}, {"NAND", 1}, {"MUL", 8} };

// read instructions from file and fill the scheduleStation vector
vector<string> instructions_str;

vector<Instruction> scheduleStation;

// flling instructions vector
void fillingInstructions() {
    for (int i = 0; i < instructions_str.size(); i++) {
        Instruction inst(i, instructions_str[i]);           // initialize instruction with all its attributes
        scheduleStation.push_back(inst);
    }
}

struct RegisterStatus {
    string registerName;
    string q;
};

vector<RegisterStatus> fillingRegisterStatus() {

    vector<RegisterStatus> registerStatus;

    for (int i = 0; i < NRegisters; i++) {
        RegisterStatus rs;
        rs.registerName = "R" + to_string(i);
        rs.q = "";
        registerStatus.push_back(rs);
    }
    return registerStatus;
}

vector<RegisterStatus> registerStatus = fillingRegisterStatus();


// take image form the reservation station and status register and wait to know whether the branch is taken or not
vector<ReservationStation> reservationStation_image;
vector<RegisterStatus> registerStatus_image;

void takeImage() {
    reservationStation_image = reservationStation;
    registerStatus_image = registerStatus;
}

// restore the reservation station and status register
void restoreImage() {
    reservationStation = reservationStation_image;
    registerStatus = registerStatus_image;
}


int get_reservation_station_id(string func) {

    for (int i = 0; i < NReservationStations; i++) {
        if (reservationStation[i].name == func)
            return i;
    }
    return -1;
}

pair<int, bool> isBusy(string func) {

    pair<int, bool> isBusy_var;

    for (int i = 0; i < NReservationStations; i++) {
        if (reservationStation[i].name == func)
            isBusy_var = { i, reservationStation[i].busy };
    }
    return isBusy_var;
}

//----------------------------------------------------------------------------ISSUE----------------------------------------------------------------------------

void issue()
{
    //for (int i = PC; i < scheduleStation.size(); i++)
    //{
    //	if (registers[1] == -1) // means the return address is set
 //       {
    if (PC >= scheduleStation.size()) {
        return;
    }
    int i = PC;
    //if (scheduleStation[i].issuingCycle == -1) // means not issued yet
    //{
    for (int j = 0; j < mapper[scheduleStation[i].op].size(); j++)              // iterate over all RSs for current instruction type
    {
        pair<int, bool> isBusy_var = isBusy(mapper[scheduleStation[i].op][j]);
        if (!isBusy_var.second) // if the reservation station is not busy
        {
            //scheduleStation[i].issuingCycle_set(cycle); // set the issuing cycle
            scheduleStation[i].issuingCycle = cycle;
            PC++;
            reservationStation[isBusy_var.first].instructionId = scheduleStation[i].instructionId; // mark RS is responsible for which instruction
            reservationStation[isBusy_var.first].busy = true; // mark RS as busy
            reservationStation[isBusy_var.first].op = scheduleStation[i].op; // set what operation RS will do

            if (scheduleStation[i].rB != -1) // if the source 1 is a register
            {
                if (registerStatus[scheduleStation[i].rB].q == "") // if the register is ready
                {
                    reservationStation[isBusy_var.first].vj = registers[scheduleStation[i].rB]; // set the value of vj
                }
                else
                {
                    reservationStation[isBusy_var.first].qj = get_reservation_station_id(registerStatus[scheduleStation[i].rB].q); // set RS q to RS name 
                }
            }

            if (scheduleStation[i].rC != -1) // if the source 2 is a register
            {
                if (registerStatus[scheduleStation[i].rC].q == "")
                {
                    reservationStation[isBusy_var.first].vk = registers[scheduleStation[i].rC];
                }
                else
                {
                    reservationStation[isBusy_var.first].qk = get_reservation_station_id(registerStatus[scheduleStation[i].rC].q);
                }
            }
            if (scheduleStation[i].rA != -1) // if the destination is a register
            {
                registerStatus[scheduleStation[i].rA].q = reservationStation[isBusy_var.first].name; // set register status Q to RS name
            }
            if (scheduleStation[i].op == "LOAD" || scheduleStation[i].op == "STORE" || scheduleStation[i].op == "BEQ" || scheduleStation[i].op == "ADDI") // if the operation is load or store
            {
                reservationStation[isBusy_var.first].A = scheduleStation[i].imm;
            }
            //                 if (scheduleStation[i].op == "CALL")
            //                 {
            //                     // take image
                                 //takeImage();
            //                 }
            return;     // we found a free RS so we return
        }
        //   }
   //    }
   //}

    }
}

int getInstructionIndex(int instructionId) {

    for (int i = 0; i < scheduleStation.size(); i++) {
        if (scheduleStation[i].instructionId == instructionId)
            return i;
    }
    return -1;
}

int pointer = 0;

//2d vector to store the instructions that are executed in the functions stack
vector<vector<bool>> isExecuted_in_functionsStack;
void filling_isExecuted_in_functionsStack()
{
    for (int i = 0; i < scheduleStation.size(); i++)
    {
        vector<bool> v;
        for (int j = 0; j < scheduleStation.size(); j++) {
            v.push_back(false);
        }
        isExecuted_in_functionsStack.push_back(v);
    }
}


// check if the branch is taken or not
bool isBranchTaken(int instructionId)
{
    int instruction_index = getInstructionIndex(instructionId);
    if (scheduleStation[instruction_index].op == "BEQ")
    {
        if (registers[scheduleStation[instruction_index].rB] == registers[scheduleStation[instruction_index].rC])
        {
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------EXECUTE----------------------------------------------------------------------------

void executeInstLogic(ReservationStation& station, string op) {

    // compute address for LOAD & STORE
    if (op == "LOAD") {
        station.result = station.A + station.vj; // imm + rB
    }
    else if (op == "STORE") {
        station.result = station.A + station.vj; // imm + rB
    }
    else if (op == "BEQ") {
        //station.result = (station.vj != station.vk);
    }
    else if (op == "CALL") {

    }
    else if (op == "RET") {

    }
    else if (op == "ADD") {
        station.result = station.vj + station.vk;
    }
    else if (op == "ADDI") {
        station.result = station.A + station.vj;
    }
    else if (op == "NAND") {
        station.result = ~(station.vk & station.vj);
    }
    else if (op == "MUL") {
        station.result = station.vj * station.vk;
    }
}

void execute()
{
    for (int i = 0; i < NReservationStations; i++)
    {
        if (reservationStation[i].busy)
        {
            // if the instruction is not executed yet or the program jumped 
            if (isExecuted_in_functionsStack[numberOfJumps][reservationStation[i].instructionId] == false &&
                scheduleStation[reservationStation[i].instructionId].issuingCycle != cycle)
            {
                if (reservationStation[i].qj == -1 && reservationStation[i].qk == -1) // if the sources are ready
                {
                    scheduleStation[reservationStation[i].instructionId].executionCycleStart = cycle;
                    scheduleStation[reservationStation[i].instructionId].executionCycleEnd = cycle + numberOfcycles[reservationStation[i].op] - 1;

                    isExecuted_in_functionsStack[numberOfJumps][reservationStation[i].instructionId] = true;
                    executeInstLogic(reservationStation[i], scheduleStation[reservationStation[i].instructionId].op);
                    //map<int, bool> isExecuted = isExecuted_in_functionsStack[reservationStation[i].instructionId];
                }
                if (isJump)
                {
                    pointer = i + 1;
                }
            }

        }
    }
}

//flush the two instructions after the call
void flush(int call_id)
{
    for (int i = call_id; i < call_id + 2; i++)
    {
        scheduleStation[i].issuingCycle = -1;
        scheduleStation[i].executionCycleStart = -1;
        scheduleStation[i].executionCycleEnd = -1;
        scheduleStation[i].writingCycle = -1;
        registerStatus[scheduleStation[i].rA].q = "";
        for (int j = 0; j < NReservationStations; j++) {
            if (reservationStation[j].instructionId == scheduleStation[i].instructionId)
            {
                reservationStation[j].busy = false;
                reservationStation[j].op = "";
                reservationStation[j].vj = -1;
                reservationStation[j].vk = -1;
                reservationStation[j].qj = -1;
                reservationStation[j].qk = -1;
                reservationStation[j].A = -1;
            }
        }
    }

}

//----------------------------------------------------------------------------WRITE----------------------------------------------------------------------------


void writeResult()
{
    // if call.write == cycle + 2 flush the two instructions after the call
    for (int b = 0; b < scheduleStation.size(); b++)
    {
        if (scheduleStation[b].op == "CALL" && scheduleStation[b].writingCycle != -1 && scheduleStation[b].writingCycle + 1 == cycle)
        {
            flush(b + 1);
        }
    }
    for (int i = 0; i < NReservationStations; i++)
    {
        if (reservationStation[i].busy)
        {
            if (scheduleStation[reservationStation[i].instructionId].executionCycleEnd + 1 == cycle
                && scheduleStation[reservationStation[i].instructionId].executionCycleEnd != -1) // if the instruction is executed
            {
                int instruction_index = getInstructionIndex(reservationStation[i].instructionId);
                if (instruction_index != -1)
                {
                    if (scheduleStation[instruction_index].writingCycle == -1) // only update the writing cycle once
                        writng_counter++;
                    // if it is call set registers[1] to the return address which is the next instruction ctyle + 1
                    if (scheduleStation[instruction_index].op == "CALL")
                    {
                        registers[1] = cycle - 1;
                        // registers[1] = cycle - numberOfcycles["CALL"] ;
                        isJump = true;
                        jumpTo = scheduleStation[instruction_index].imm;
                        PC = jumpTo;
                        for (int l = 0; l <= instruction_index; l++)
                        {
                            isExecuted_in_functionsStack[numberOfJumps + 1][l] = true;
                        }

                    }
                    string op = scheduleStation[instruction_index].op;

                    if (op == "LOAD") {
                        int dest = scheduleStation[reservationStation[i].instructionId].rA;
                        if (dest != 0) registers[dest] = memory[reservationStation[i].result];
                    }
                    else if (op == "STORE") {
                        memory[reservationStation[i].result] = registers[scheduleStation[reservationStation[i].instructionId].rA];
                    }
                    else if (op == "ADD" || op == "ADDI" || op == "NAND" || op == "MUL") {
                        int dest = scheduleStation[reservationStation[i].instructionId].rA;
                        if (dest != 0) registers[dest] = reservationStation[i].result;
                    }
                    else if (op == "BEQ") {

                    }
                    else if (op == "RET") {
                        PC = registers[1];
                        registers[1] = -1;
                        numberOfJumps++;
                        // flush the two instructions after the call
                    }
                    else if (op == "CALL") {

                    }

                    // I moved this a bit downwards as I needed the A for the store (nvm i don't need now but i left it here)
                    scheduleStation[instruction_index].writingCycle = cycle;
                    reservationStation[i].busy = false;
                    reservationStation[i].op = "";
                    reservationStation[i].vj = -1;
                    reservationStation[i].vk = -1;
                    reservationStation[i].qj = -1;
                    reservationStation[i].qk = -1;
                    reservationStation[i].A = -1;

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
                                    reservationStation[j].vj = registers[scheduleStation[k].rB];
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
                                    reservationStation[j].vk = registers[scheduleStation[k].rC];
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

//----------------------------------------------------------------------------PRINT----------------------------------------------------------------------------

void printReservationStation()
{
    cout << left; // Left-align the output
    cout << setw(12) << "Id" << setw(18) << "Instruction ID" << setw(18) << "Name" << setw(12) << "Busy" << setw(12) << "Op"
        << setw(12) << "Vj" << setw(12) << "Vk" << setw(12) << "Qj" << setw(12) << "Qk" << setw(12) << "A" << endl;

    cout << "\n";
    for (int i = 0; i < NReservationStations; i++)
    {
        cout << setw(12) << reservationStation[i].id
            << setw(18) << reservationStation[i].instructionId
            << setw(18) << reservationStation[i].name
            << setw(12) << reservationStation[i].busy
            << setw(12) << reservationStation[i].op
            << setw(12) << reservationStation[i].vj
            << setw(12) << reservationStation[i].vk
            << setw(12) << reservationStation[i].qj
            << setw(12) << reservationStation[i].qk
            << setw(12) << reservationStation[i].A << endl;
    }
}

void printRegisterStatus() {
    cout << "Register:    ";
    for (int i = 0; i < NRegisters; i++) {
        cout << setw(15) << registerStatus[i].registerName;
    }
    cout << "\n\nQ:           ";
    for (int i = 0; i < NRegisters; i++) {
        cout << setw(15) << registerStatus[i].q;
    }
}

void printScheduleStation()
{
    cout << left; // Left-align the output
    cout << setw(20) << "Instruction ID" << setw(30) << "Name" << setw(20) << "Issue" << setw(20) << "Start Execution" <<
        setw(20) << "End Execution" << setw(20) << "Write Result" << endl;

    cout << "\n";
    for (int i = 0; i < scheduleStation.size(); i++)
    {
        cout << setw(20) << scheduleStation[i].instructionId
            << setw(30) << scheduleStation[i].name
            << setw(20) << scheduleStation[i].issuingCycle
            << setw(20) << scheduleStation[i].executionCycleStart
            << setw(20) << scheduleStation[i].executionCycleEnd
            << setw(20) << scheduleStation[i].writingCycle << endl;
    }
}
void printVector(vector<string>& v)
{
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << " ";
    cout << endl;
}

void print() {
    cout << "\n\n----------------------------------------------------------" << "Cycle " << cycle - 1
        << "----------------------------------------------------------" << endl;
    cout << endl;
    printScheduleStation();
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
    printReservationStation();
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
    printRegisterStatus();
    cout << endl;
}

//--------------------------------------------------------------------------READ FILES--------------------------------------------------------------------------


void read_instructions_file(const string& filename) {
    fstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        instructions_str.push_back(line);
    }
}

vector<string> split_line(string line, char delimiter = ' ') {
    vector<string> words;
    string cur_word = "";
    for (auto& u : line) {
        if (u == delimiter) {
            if (cur_word.length()) {
                words.push_back(cur_word);
                cur_word = "";
            }
        }
        else cur_word += u;
    }
    if (cur_word.length()) words.push_back(cur_word);
    return words;
}

void read_memory_file(string mem_file) {
    fstream f_mem(mem_file);
    string line;
    while (!f_mem.eof()) {
        int address, value;
        getline(f_mem, line);
        vector<string> line_data = split_line(line);
        address = stoi(line_data[0]);
        value = stoi(line_data[1]);
        memory[address] = value;
    }
}

void taskManager() {
    read_instructions_file("instructions2.txt");
    read_memory_file("memory.txt");
    fillingInstructions();
    fillingReservationStation();
    fillingMapper();
    filling_isExecuted_in_functionsStack();

    while (writng_counter != scheduleStation.size()) {
        issue();
        execute();
        writeResult();
        //print();
        cycle++;
    }
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
    printScheduleStation();
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < 8; i++) {
        cout << "R" << i << ": " << registers[i] << "\n";
    }
    cout << "Memory[40]: " << memory[40];
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

int main() {

    taskManager();
    return 0;
}