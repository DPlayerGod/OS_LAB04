#include <bits/stdc++.h>
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
    bool isCompleted = false; // Completion status
} PCB;

struct GANT {
    int start;
    int finish;
    int iPID;

    GANT(int _start = 0, int _finish = 0, int _iPID = 0) 
        : start(_start), finish(_finish), iPID(_iPID) {}
};

void calculateSRTF(vector<PCB>& processes, vector<GANT>& gantt) {
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();
    int shortestRemainingTime = INT_MAX;
    int shortestIndex = -1;
    bool foundProcess = false;

    // Initialize remainingBurst for each process
    for (int i = 0; i < n; i++) {
        processes[i].remainingBurst = processes[i].iBurst;
    }

    while (completed != n) {        // Execute the selected process for 1 unit of time

        foundProcess = false;

        // Find the process with the shortest remaining time at the current time
        for (int i = 0; i < n; i++) {
            if (processes[i].iArrival <= currentTime && !processes[i].isCompleted &&
                processes[i].remainingBurst < shortestRemainingTime && processes[i].remainingBurst > 0) {
                
                shortestRemainingTime = processes[i].remainingBurst;
                shortestIndex = i;
                foundProcess = true;
            }
        }

        if (!foundProcess) {
            currentTime++;
            continue;
        }

        // Record the start time of the process if this is its first execution
        if (processes[shortestIndex].remainingBurst == processes[shortestIndex].iBurst) {
            processes[shortestIndex].iStart = currentTime;
            processes[shortestIndex].iResponse = processes[shortestIndex].iStart - processes[shortestIndex].iArrival;
        }

        // Execute the selected process for 1 unit of time
        processes[shortestIndex].remainingBurst--;
        
        if (gantt.size() && processes[shortestIndex].iPID == gantt.back().iPID) gantt.back().finish = currentTime + 1;
        else gantt.push_back(GANT(currentTime, currentTime + 1, processes[shortestIndex].iPID)); // Record Gantt chart entry
        currentTime++;

        // If the process is completed
        if (processes[shortestIndex].remainingBurst == 0) {
            processes[shortestIndex].iFinish = currentTime;
            processes[shortestIndex].iTaT = processes[shortestIndex].iFinish - processes[shortestIndex].iArrival;
            processes[shortestIndex].iWaiting = processes[shortestIndex].iTaT - processes[shortestIndex].iBurst;
            processes[shortestIndex].isCompleted = true;
            completed++;
            shortestRemainingTime = INT_MAX; // Reset the shortest remaining time value
        }
    }
}

void printResults(vector<PCB>& processes, vector<GANT>& gantt) {
    float totalWaitingTime = 0, totalTurnaroundTime = 0;

    // Print Gantt Chart
    cout << "Gantt Chart:\n";
    for (const auto& entry : gantt) {
        cout << "| P" << entry.iPID << " (" << entry.start << "," << entry.finish << ") ";
    }
    cout << "|\n";

    // Print Process Information
    cout << "\n"
        << left << setw(10) << "PID"
        << setw(10) << "Arrival"
        << setw(10) << "Burst"
        << setw(10) << "Start"
        << setw(10) << "Finish"
        << setw(10) << "Waiting"
        << setw(10) << "Response"
        << setw(10) << "Turnaround" << "\n";

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

    calculateSRTF(processes, gantt);
    printResults(processes, gantt);

    return 0;
}
