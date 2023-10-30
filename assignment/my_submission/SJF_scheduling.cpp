#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

#define PATH_TO_OUTPUT_TXT_FILE "./output.txt"

class Process
{
public:
    unsigned int id;           // Process ID
    unsigned int arrival_time; // Arrival time
    unsigned int burst_time;   // Burst time
public:
    unsigned int remaining_time;              // Remaining time
    std::vector<unsigned int> start_time_vec; // Vector containing timestamps when the process starts running
    unsigned int run_time;                    // Run time

public:
    Process();                                                                               // Default Constructor
    Process(const unsigned int _id, const unsigned int _arr_t, const unsigned int _burst_t); // Full Constructor
    ~Process();                                                                              // Destructor
};

Process::Process() {}
Process::Process(const unsigned int _id, const unsigned int _arr_t, const unsigned int _burst_t) : id(_id),
                                                                                                   arrival_time(_arr_t),
                                                                                                   burst_time(_burst_t)
{
    remaining_time = burst_time;
}
Process::~Process() {}

std::vector<Process> process_list; // List of input processes

inline void processInit()
{
    process_list.push_back(Process(1, 0, 8));
    process_list.push_back(Process(2, 1, 4));
    process_list.push_back(Process(3, 2, 9));
    process_list.push_back(Process(4, 3, 5));
}

void SJF()
{
    std::vector<Process> completed_processes;
    std::priority_queue<Process,
                        std::vector<Process>,
                        bool (*)(const Process &, const Process &)>
        ready_queue([](const Process &p1, const Process &p2)
                    { return p1.remaining_time > p2.remaining_time; });

    unsigned int current_time = 0;

    while (!process_list.empty() || !ready_queue.empty())
    {
        // Move processes that have arrived to the ready queue
        while (!process_list.empty() && process_list.front().arrival_time <= current_time)
        {
            ready_queue.push(process_list.front());
            process_list.erase(process_list.begin());
        }

        if (!ready_queue.empty())
        {
            Process current_process = ready_queue.top();
            ready_queue.pop();

            // Update start time if it's the first time this process runs
            if (current_process.start_time_vec.empty())
            {
                current_process.start_time_vec.push_back(current_time);
            }

            current_process.remaining_time--;
            current_time++;

            // Check if the process has completed
            if (current_process.remaining_time == 0)
            {
                current_process.run_time = current_time - current_process.start_time_vec[0];
                completed_processes.push_back(current_process);
            }
            else
            {
                ready_queue.push(current_process);
            }
        }
        else
        {
            current_time++;
        }
    }

    // Update the process list with the completed processes
    process_list = completed_processes;
}

void output()
{
    std::ofstream outputFile(PATH_TO_OUTPUT_TXT_FILE);
    if (!outputFile)
    {
        std::cerr << "Error: Unable to open the output file." << std::endl;
        return;
    }

    std::cout << "Process ID\tStart Time\tRun Time" << std::endl;
    outputFile << "Process ID\tStart Time\tRun Time" << std::endl;

    for (const Process &process : process_list)
    {
        std::cout << process.id << "\t\t";
        outputFile << process.id << "\t\t";

        for (unsigned int i = 0; i < process.start_time_vec.size(); ++i)
        {
            std::cout << process.start_time_vec[i];
            outputFile << process.start_time_vec[i];
        }

        std::cout << "\t\t" << process.run_time << std::endl;
        outputFile << "\t\t" << process.run_time << std::endl;
    }

    outputFile.close();
}

int main()
{
    processInit();
    SJF();
    output();

    return 0;
}
