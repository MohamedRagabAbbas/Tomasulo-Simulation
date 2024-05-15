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
            if ((inst[i] == ' ') && first_time || (i == inst.length() - 1 && first_time )) {
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
            if (inst_op == "LOAD" || inst_op == "STORE")
            {
				if (inst[i] != ' ' && inst[i] != ',' && inst[i] != '(' && inst[i] != ')')
                    word += inst[i];
				if (inst[i] == ',')
				{
                    inst_load_store_vector.push_back(word); // dest
                    word = "";
				}
				if (inst[i] == '(')
                {
                    inst_load_store_vector.push_back(word); // imm
					word = "";
                    i++;
					while (inst[i] != ')')
                    {
                        word += inst[i];
						i++;
					}
                    inst_load_store_vector.push_back(word); // src1
					word = "";

				}

            }
            if (inst_op == "ADD" || inst_op == "MUL" || inst_op == "NAND" || inst_op == "ADDI")
            {
				if (inst[i] != ' ' && inst[i] != ',')
					word += inst[i];
                if (inst[i] == ',')
				{
					inst_arithmetic_vector.push_back(word); // dest or src1
					word = "";
				}
				if (i == inst.length() - 1)
				{
					inst_arithmetic_vector.push_back(word); // src2 or imm
					word = "";
				}
            }
			if (inst_op == "BEQ")
			{
				if (inst[i] != ' ' && inst[i] != ',')
					word += inst[i];
				if (inst[i] == ',')
				{
					inst_branch_vector.push_back(word); // src1
					word = "";
				}
				if (i == inst.length() - 1)
				{
					inst_branch_vector.push_back(word); // src2
					word = "";
				}
                if (i == inst.length() - 1)
                {
                    inst_branch_vector.push_back(word); // imm
                    word = "";
                }

			}
			if (inst_op == "CALL")
			{
				if (inst[i] != ' ' && inst[i] != ',')
					word += inst[i];
				if (i == inst.length() - 1)
				{
					inst_call_vector.push_back(word); // imm
					word = "";
				}
			}
		}
		if (inst_op == "ADD" || inst_op == "MUL" || inst_op == "NAND" || inst_op == "ADDI")
        {
            printVector(inst_arithmetic_vector);
		}
		else if (inst_op == "LOAD" || inst_op == "STORE")
		{
			printVector(inst_load_store_vector);
		}
		else if (inst_op == "BEQ")
		{
			printVector(inst_branch_vector);
		}
		else if (inst_op == "CALL")
		{
			printVector(inst_call_vector);
		}
        else if (inst_op == "RET")
        {
			printVector(inst_ret_vector);
        }
        inst_load_store_vector.clear();
        inst_arithmetic_vector.clear();
		inst_branch_vector.clear();
        inst_call_vector.clear();
		inst_ret_vector.clear();



        ss >> inst_op;

        // Remove commas and parentheses
        string cleanedInst;
        char c;
        while (ss >> c) {
            if (c != ',') {
                cleanedInst += c;
                
            }
        }
        stringstream stream(cleanedInst);

        // Parse based on instruction type
        if (inst_op == "LOAD") {
            parseLoadStore(stream, true);
        }
        else if (inst_op == "STORE") {
            parseLoadStore(stream, false);
        }
        else if (inst_op == "BEQ") {
            stream >> src1 >> src2 >> imm;
            op = inst_op;
        }
        else if (inst_op == "CALL") {
            stream >> imm;
            op = inst_op;
        }
        else if (inst_op == "RET") {
            op = inst_op;
        }
        else if (inst_op == "ADD" || inst_op == "MUL" || inst_op == "NAND") {
            stream >> dest >> src1 >> src2;
            op = inst_op;
        }
        else if (inst_op == "ADDI") {
            stream >> dest >> src1 >> imm;
            op = inst_op;
        }
        else {
            cout << "Invalid instruction" << endl;
        }
    }

private:
    void parseLoadStore(stringstream& stream, bool isLoad) {
        string reg;
        if (isLoad) {
            stream >> reg >> imm;
        }
        else {
            stream >> reg >> imm;
        }
        dest = stoi(reg.substr(1));
        string addr;
        stream >> addr;
        size_t pos1 = addr.find('(');
        size_t pos2 = addr.find(')');
        if (pos1 != string::npos && pos2 != string::npos) {
            imm = stoi(addr.substr(0, pos1));
            string reg = addr.substr(pos1 + 1, pos2 - pos1 - 1);
            src1 = stoi(reg.substr(1));
        }
        op = isLoad ? "LOAD" : "STORE";
    }
};

// For testing
int main() {
    Instruction instr1(1, "LOAD R1,10(R2)");
    cout << "Instruction 1: " << instr1.op << " " << instr1.dest << " " << instr1.imm << "(" << instr1.src1 << ")" << endl;

    Instruction instr2(2, "BEQ R1,R2,15");
    cout << "Instruction 2: " << instr2.op << " " << instr2.src1 << " " << instr2.src2 << " " << instr2.imm << endl;

    Instruction instr3(3, "ADD R1,R2,R3");
    cout << "Instruction 3: " << instr3.op << " " << instr3.dest << " " << instr3.src1 << " " << instr3.src2 << endl;

    Instruction instr4(4, "ADDI R1,R2,10");
    cout << "Instruction 4: " << instr4.op << " " << instr4.dest << " " << instr4.src1 << " " << instr4.imm << endl;

    Instruction instr5(5, "CALL 100");
    cout << "Instruction 5: " << instr5.op << " " << instr5.imm << endl;

    Instruction instr6(6, "RET");
    cout << "Instruction 6: " << instr6.op << endl;

    return 0;
}
