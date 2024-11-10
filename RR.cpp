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
    int remainingBurst;     // Remaining Burst Time
} PCB;

struct GANT {
    int start;
    int finish;
    int iPID;

    GANT(int _start = 0, int _finish = 0, int _iPID = 0) 
        : start(_start), finish(_finish), iPID(_iPID) {}
} ;

void calculateRR(vector<PCB>& processes, vector<GANT>& gantt, int timeQuantum) {
    int currentTime = 0;
    queue<int> readyQueue;
    vector<bool> inQueue(processes.size(), false);
    int completed = 0;
    
    // Sort by Arrival Time
    sort(processes.begin(), processes.end(), [](PCB a, PCB b) {
        return a.iArrival < b.iArrival;
    });
    cout << '\n';

    readyQueue.push(0); // Add the first process to the queue
    inQueue[0] = true;
    processes[0].iStart = max(currentTime, processes[0].iArrival);
    
    while (completed < processes.size()) {
        int index = readyQueue.front();
        readyQueue.pop();
        inQueue[index] = false;
        
        if (processes[index].remainingBurst == processes[index].iBurst) {
            processes[index].iStart = max(currentTime, processes[index].iArrival);
            processes[index].iResponse = processes[index].iStart - processes[index].iArrival;
        }
        
        int executeTime = min(timeQuantum, processes[index].remainingBurst);
        int lastTime = currentTime;
        currentTime = max(currentTime, processes[index].iArrival) + executeTime;
        processes[index].remainingBurst -= executeTime;
        
        if (gantt.size() && processes[index].iPID == gantt.back().iPID) gantt.back().finish = currentTime; 
        else gantt.push_back(GANT(lastTime, currentTime, processes[index].iPID));

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].iArrival <= currentTime && processes[i].remainingBurst > 0 && !inQueue[i] && i != index) {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        if (processes[index].remainingBurst == 0) {
            processes[index].iFinish = currentTime;
            processes[index].iTaT = processes[index].iFinish - processes[index].iArrival;
            processes[index].iWaiting = processes[index].iTaT - processes[index].iBurst;
            completed++;
        } else {
            readyQueue.push(index);
            inQueue[index] = true;
        }
    }
}

void printResults(vector<PCB>& processes, vector<GANT>& gantt) {
    float totalWaitingTime = 0, totalTurnaroundTime = 0;

    cout << "Gantt Chart:\n";
    for (const auto& process : gantt) {
        cout << "| P" << process.iPID << " " << "(" << process.start << ',' << process.finish << ") ";
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

    sort(processes.begin(), processes.end(), [](PCB a, PCB b) {
        return a.iPID < b.iPID;
    });

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
    int n, timeQuantum;
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter time quantum: ";
    cin >> timeQuantum;

    vector<PCB> processes(n);
    vector<GANT> gantt;
    for (int i = 0; i < n; i++) {
        processes[i].iPID = i + 1;
        cout << "Enter arrival time and burst time for process " << i + 1 << ": ";
        cin >> processes[i].iArrival >> processes[i].iBurst;
        processes[i].remainingBurst = processes[i].iBurst; // Initialize remaining burst time
        processes[i].iStart = processes[i].iFinish = -1;    // Initialize start and finish times
    }

    calculateRR(processes, gantt, timeQuantum);
    printResults(processes, gantt);

    return 0;
}
