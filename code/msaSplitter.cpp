/**
 * @file msaSplitter.cpp
 * @brief This file contains the implementation of the MSASplitter class to manage and manipulate MSAs.
 * 
 * The MSASplitter class provides methods to:
 * - Verify if the MSA is in a paired form, where sequences are followed by the MSA of the first monomer, followed by the MSA of the second monomer.
 * - Split the MSA into three sets: paired MSA, MSA of the first monomer, and MSA of the second monomer.
 * 
 * The implementation includes:
 * - Constructor to initialize the split position.
 * - Method to verify the structure of the MSA.
 * - Method to return the split sets.
 * - Private helper functions to determine set membership and extract non-zero parts of the MSA rows.
 */

#include "msaSplitter.h"
#include <tuple>

using namespace std;

MSASplitter::MSASplitter(vector<int> _splitPositions) : splitPositions(_splitPositions) {}

bool MSASplitter::hasBlockForm(const vector<vector<int>>& vectors) {
    bool set2Started = false;
    bool set3Started = false;

    for (const auto& row : vectors) {
        if (!set2Started && !set3Started) {
            // We are in set 1 or set 2/ set 3 can start
            if (isSet2(row))
            {
                set2Started = true;
                set2.push_back(getNonZeroPart(row, 0, splitPositions[0]));
            } else if (isSet3(row))
            {
                set3Started = true;
                set3.push_back(getNonZeroPart(row, splitPositions[0], row.size())); // Set 3 can start if set 2 is empty  
            }
            else {
                set1.push_back(row);
            }
        } else if (set2Started && !set3Started) {
            // We are in set 2 or set 3 can start
            if (isSet2(row)) {
                set2.push_back(getNonZeroPart(row, 0, splitPositions[0]));
            } else if (isSet3(row)) {
                set3Started = true;
                set3.push_back(getNonZeroPart(row, splitPositions[0], row.size()));
            } else {
                return false; // Invalid row for set 2 or set 3
            }
        } else if (set3Started) {
            // We are in set 3
            if (!isSet3(row)) {
                return false; // Invalid row for set 3
            }
            set3.push_back(getNonZeroPart(row, splitPositions[0], row.size()));
        }
    }

    return set2Started || set3Started; // If it passes all checks, it's valid
}

bool MSASplitter::isSet2(const vector<int>& row) const {
    if (row.size() <= splitPositions[0]) return false;  // check the row has enough elements
    for (int i = splitPositions[0]; i < row.size(); ++i)
    {
        if (row[i] != 0) return false;  // elements from position splitPosition onward should be 0
    }
    return true;
}

bool MSASplitter::isSet3(const vector<int>& row) const {
    if (row.size() <= splitPositions[0]) return false;  // check the row has enough elements
    for (int i = 0; i < splitPositions[0]; ++i)
    {
        if (row[i] != 0) return false;  // elements up to position splitPosition should be 0
    }
    return true;
}

vector<int> MSASplitter::getNonZeroPart(const vector<int>& row, int start, int end) const {
    vector<int> nonZeroPart;
    for (int i = start; i < end; i++)
    {
        nonZeroPart.push_back(row[i]);
    }
    return nonZeroPart;
}

tuple<vector<vector<int>>, vector<vector<int>>, vector<vector<int>>> MSASplitter::returnSets()
{
    // Insert the elements from position 0 to splitPosition of the first row of set1 at the beginning of set2
    set2.insert(set2.begin(), vector<int>(set1[0].begin(), set1[0].begin() + splitPositions[0]));

    // Insert the elements from position splitPosition+1 to the last position of the first row of set1 at the beginning of set3
    set3.insert(set3.begin(), vector<int>(set1[0].begin() + splitPositions[0], set1[0].end()));

    return make_tuple(set1, set2, set3);
}