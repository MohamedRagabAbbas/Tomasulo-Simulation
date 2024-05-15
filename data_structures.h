#include <iostream>
#include <string>
using namespace std;

struct ReservationStation {
    string name;
    bool busy;
    string op;
    int vj;
    int vk;
    int qj;
    int qk;
    int A;
};


struct Scheduler{
    string instruction;
    int issue;
    pair<int, int> execute;
    int read;
};


struct RegisterStatus {
    string registerName;
    int q;
};