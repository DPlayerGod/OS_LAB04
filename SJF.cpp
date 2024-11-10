#include<bits/stdc++.h>

using namespace std;

typedef struct {
    int iPID;               // Process ID
    int iArrival;           // Arrival Time
    int iBurst;             // Burst Time
    int iStart;             // Start Time
    int iFinish;            // Finish Time
    int iWaiting;           // Waiting Time
    int iResponse;          // Response Time
    int iTaT;               // Turnaround Time
} PCB;

struct GANT {
    int start;
    int finish;
    int iPID;

    GANT(int _start = 0, int _finish = 0, int _iPID = 0) 
        : start(_start), finish(_finish), iPID(_iPID) {}
} ;

bool compareArrival(PCB a, PCB b) {
    return a.iArrival < b.iArrival;
}

bool compareBurst(PCB a, PCB b) {
    return a.iBurst < b.iBurst;
}
bool compareID(PCB a, PCB b) {
    return a.iPID < b.iPID;
}

void calculateTimes(vector<PCB>& processes, vector<GANT>& Gantt) {
    int currentTime = 0;
    for (int i = 0; i < processes.size(); i++) {
        int index = -1;
        int minBurst = INT_MAX;

        for (int j = 0; j < processes.size(); j++) {
            if (processes[j].iArrival <= currentTime && processes[j].iBurst < minBurst && processes[j].iFinish == -1) {
                minBurst = processes[j].iBurst;
                index = j;
            }
        }

        if (index == -1) {
            currentTime++;
            i--;
            continue;
        }

        processes[index].iStart = currentTime;
        processes[index].iFinish = processes[index].iStart + processes[index].iBurst;
        processes[index].iWaiting = processes[index].iStart - processes[index].iArrival;
        processes[index].iResponse = processes[index].iStart - processes[index].iArrival;
        processes[index].iTaT = processes[index].iFinish - processes[index].iArrival;

        currentTime += processes[index].iBurst;
        processes[index].iFinish = currentTime;

        Gantt.push_back(GANT(processes[index].iStart, processes[index].iFinish, processes[index].iPID));
    }
}

void printResults(vector<PCB>& processes, vector<GANT>& gantt) {
    float totalWaitingTime = 0, totalTurnaroundTime = 0;
    cout << "Gantt Chart:\n";
    for (const auto& process : gantt) {
        cout << "| P" << process.iPID << " ";
    }
    cout << "|\n";

    cout << "\n" 
     << left << setw(10) << "PID"
     << setw(10) << "Arrival"
     << setw(10) << "Burst"
     << setw(10) << "Start"
     << setw(10) << "Finish"
     << setw(10) << "Waiting"
     << setw(10) << "Response"
     << setw(10) << "Turnaround" << "\n";
     
    sort(processes.begin(), processes.end(), compareID);
    for (const auto& process : processes) {
        cout << left << setw(10) << process.iPID
            << setw(10) << process.iArrival
            << setw(10) << process.iBurst
            << setw(10) << process.iStart
            << setw(10) << process.iFinish
            << setw(10) << process.iWaiting
            << setw(10) << process.iResponse
            << setw(10) << process.iTaT << "\n";

        totalWaitingTime += process.iWaiting;
        totalTurnaroundTime += process.iTaT;
    }

    cout << "\nAverage Waiting Time: " << totalWaitingTime / processes.size() << "\n";
    cout << "Average Turnaround Time: " << totalTurnaroundTime / processes.size() << "\n";
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<PCB> processes(n);
    vector<GANT> gantt;
    for (int i = 0; i < n; i++) {
        processes[i].iPID = i + 1;
        cout << "Enter arrival time and burst time for process " << i + 1 << ": ";
        cin >> processes[i].iArrival >> processes[i].iBurst;
        processes[i].iStart = processes[i].iFinish = -1; // Initialize start and finish times
    }

    sort(processes.begin(), processes.end(), compareArrival); // Sort processes by arrival time

    calculateTimes(processes, gantt);
    printResults(processes, gantt);

    return 0;
}
