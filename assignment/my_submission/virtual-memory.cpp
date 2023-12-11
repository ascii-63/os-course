#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

struct Page
{
    int pageNumber;
    int referenceBit;
};

std::vector<int> calculateWorkingSet(std::vector<Page> &pages, int delta, int numReferenceBits)
{
    std::vector<int> workingSet;

    for (int i = 0; i < pages.size(); ++i)
    {
        // Update the reference bit for the current page
        pages[i].referenceBit = 1;

        // Adjust the reference bits for older pages
        for (int j = i - delta; j >= 0 && j < i; ++j)
        {
            if (j >= 0 && pages[j].referenceBit > 0)
                pages[j].referenceBit--;
        }

        // Calculate the working set size for the current page
        int workingSetSize = 0;
        for (const auto &page : pages)
        {
            if (page.referenceBit > 0)
                workingSetSize++;
        }

        workingSet.push_back(workingSetSize);
    }

    return workingSet;
}

int main()
{
    int delta = 16;           // Delta
    int numReferenceBits = 4; // Number of reference bits
    int numPages = 20;        // Number of pages
    std::vector<Page> pages;  // Create a vector of pages with random reference bits

    std::srand(std::time(0));
    for (int i = 0; i < numPages; ++i)
        pages.push_back({i, rand() % (numReferenceBits)});

    std::cout << "Initial reference bit of pages:";
    for (const auto &page : pages)
        std::cout << " " << page.referenceBit;

    std::vector<int> result = calculateWorkingSet(pages, delta, numReferenceBits);

    // Display the final result (working set sizes)
    std::cout << "\nFinal Working Set Sizes:\n";
    for (int i = 0; i < result.size(); ++i)
        std::cout << "Page " << i << ": " << result[i] << "\n";

    return 0;
}
