#include <iostream>
#include <vector>
#include <algorithm>

const int NUM_RESOURCES = 3;
const int NUM_PROCESSES = 5;
const int A = 0;
const int B = 1;
const int C = 2;

// Function to check if the requested resources can be granted
bool isSafe(std::vector<std::vector<int>> &max, std::vector<std::vector<int>> &allocation,
            std::vector<int> &available, int process, std::vector<int> &request)
{
    // Check if the request is less than or equal to the need
    for (int i = 0; i < NUM_RESOURCES; ++i)
    {
        if (request[i] > max[process][i] - allocation[process][i])
        {
            std::cerr << "Request exceeds the maximum need\n";
            return false;
        }
    }

    // Simulate resource allocation
    for (int i = 0; i < NUM_RESOURCES; ++i)
    {
        allocation[process][i] += request[i];
        available[i] -= request[i];
    }

    // Check if the system is in a safe state
    bool finish[NUM_PROCESSES] = {false};
    for (int i = 0; i < NUM_PROCESSES; ++i)
    {
        if (!finish[i])
        {
            bool canFinish = true;
            for (int j = 0; j < NUM_RESOURCES; ++j)
            {
                if (max[i][j] - allocation[i][j] > available[j])
                {
                    canFinish = false;
                    break;
                }
            }
            if (canFinish)
            {
                finish[i] = true;
                for (int j = 0; j < NUM_RESOURCES; ++j)
                {
                    available[j] += allocation[i][j];
                }
                i = -1; // Restart the loop
            }
        }
    }

    // Restore the state
    for (int i = 0; i < NUM_RESOURCES; ++i)
    {
        allocation[process][i] -= request[i];
        available[i] += request[i];
    }

    // Check if the request can be granted without leading to an unsafe state
    return std::all_of(std::begin(finish), std::end(finish), [](bool val)
                       { return val; });
}

// Banker's Algorithm
void bankerAlgorithm(std::vector<std::vector<int>> &max, std::vector<std::vector<int>> &allocation,
                     std::vector<int> &available, int process, std::vector<int> &request)
{
    if (isSafe(max, allocation, available, process, request))
    {
        std::cout << "Request granted. Resource allocation successful.\n";
        for (int i = 0; i < NUM_RESOURCES; ++i)
        {
            allocation[process][i] += request[i];
            available[i] -= request[i];
        }

        // Print the updated allocation matrix
        std::cout << "Updated Allocation Matrix:\n";
        for (int i = 0; i < NUM_PROCESSES; ++i)
        {
            for (int j = 0; j < NUM_RESOURCES; ++j)
            {
                std::cout << allocation[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    else
    {
        std::cout << "Request denied. Process must wait to avoid deadlock.\n";
    }
}

// Function to get user input for resources
std::vector<int> getUserInputResources()
{
    std::vector<int> request(NUM_RESOURCES);
    std::cout << "Enter the requested resources (e.g., 1 1 1): ";
    for (int i = 0; i < NUM_RESOURCES; ++i)
    {
        std::cin >> request[i];
    }
    return request;
}

int main()
{
    std::vector<std::vector<int>> max = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
    std::vector<std::vector<int>> allocation = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};
    std::vector<int> available = {3, 3, 2};

    int process;
    std::vector<int> request;

    // Input the process
    std::cout << "Enter the process (0 to 4): ";
    std::cin >> process;

    // Get user input for resources
    request = getUserInputResources();

    // Run the Banker's Algorithm
    bankerAlgorithm(max, allocation, available, process, request);

    return 0;
}
