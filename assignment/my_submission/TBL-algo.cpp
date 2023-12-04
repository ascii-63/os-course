#include <iostream>
#include <bitset>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int PAGE_BITS = 3;
const int OFFSET_BITS = 5;
const int FRAME_BITS = 3;
const int TLB_SIZE = 4;

// Function to translate logical address to physical address
int translateAddress(int logicalAddress, vector<int> &pageTable, vector<pair<int, int>> &tlb)
{
    // Extract page number and offset
    int pageNumber = (logicalAddress >> OFFSET_BITS) & ((1 << PAGE_BITS) - 1);
    int offset = logicalAddress & ((1 << OFFSET_BITS) - 1);

    // Check TLB for the page
    for (const auto &entry : tlb)
    {
        if (entry.first == pageNumber)
        {
            int frameNumber = entry.second;
            int physicalAddress = (frameNumber << OFFSET_BITS) | offset;
            return physicalAddress;
        }
    }

    // TLB miss, consult the page table
    int frameNumber = pageTable[pageNumber];
    int physicalAddress = (frameNumber << OFFSET_BITS) | offset;

    // Update TLB (replace the oldest entry)
    if (tlb.size() < TLB_SIZE)
    {
        tlb.push_back({pageNumber, frameNumber});
    }
    else
    {
        tlb.erase(tlb.begin());
        tlb.push_back({pageNumber, frameNumber});
    }

    return physicalAddress;
}

// Function to print TLB state
void printTLB(const vector<pair<int, int>> &tlb)
{
    cout << "TLB State: ";
    for (const auto &entry : tlb)
    {
        cout << "(" << bitset<PAGE_BITS>(entry.first) << ", " << bitset<FRAME_BITS>(entry.second) << ") ";
    }
    cout << endl;
}

int main()
{
    srand(time(0));
    vector<int> pageTable(1 << PAGE_BITS);
    for (int i = 0; i < (1 << PAGE_BITS); ++i)
        pageTable[i] = rand() % (1 << FRAME_BITS);

    vector<pair<int, int>> tlb;

    cout << "Initialized Page Table:\n";
    for (int i = 0; i < (1 << PAGE_BITS); ++i)
        cout << "Page " << bitset<PAGE_BITS>(i) << ": Frame " << bitset<FRAME_BITS>(pageTable[i]) << "\n";
    cout << endl;

    //////////////////////////////////////

    while (true)
    {
        string input;
        cout << "Enter logical address in binary form (0 to exit): ";
        cin >> input;

        if (input == "0")
            break;

        int logicalAddress = stoi(input, 0, 2);                                 // Convert binary string to integer
        int physicalAddress = translateAddress(logicalAddress, pageTable, tlb); // Translate the address

        cout << "Logical Address: " << bitset<8>(logicalAddress) << " --> Physical Address: "
             << bitset<8>(physicalAddress) << endl;

        printTLB(tlb);
    }

    return 0;
}
