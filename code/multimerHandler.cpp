/**
 * @file multimerHandler.cpp
 * @brief This file contains the implementation of the MultimerHandler class to manage and manipulate MSA of a multimer.
 * 
 * - Verify if the MSA is in the format of the given stoichiometry,
 * When multimer is a heteromer(with more than one distinct chain), MSA starts from pairedMSA, followed by the MSA of the chains in the multimer.
 * When multimer is a homomer, MSA is a concatenation of MSAs of the chain.
 * 
 * The implementation includes:
 * - Constructor to initialize the split position.
 * - Method to verify the structure of the MSA with the given stochiometry.
 * - Method to return the MSA sets.
 */

#include "multimerHandler.h"
#include <tuple>
#include <iostream>
#include <string>
#include <regex>

using namespace std;

MultimerHandler::MultimerHandler(string _stoichiom) : stoichiom(_stoichiom) {}

string MultimerHandler::chainIndexToStoichiomLetter(int index) {    
    string result;
    
    while (index >= 0) {
        result = char('A' + (index % 26)) + result;
        index = index / 26 - 1;
    }
    return result;
}

vector<int> MultimerHandler::getSequenceSegment(const vector<int>& row, int chainIndex, int repeat) const
{
    int chainLength = (chainIntervals[chainIndex+1] - chainIntervals[chainIndex]) / numbers[chainIndex];

    int start = chainIntervals[chainIndex] + (chainLength * (repeat-1));
    int end = chainIntervals[chainIndex] + (chainLength * repeat);

    return vector<int>(row.begin()+start, row.begin()+end);
}

bool MultimerHandler::isHomomerFormat()
{
    regex pattern("^A(\\d+)$");
    smatch match;
    
    if (regex_match(stoichiom, match, pattern))
    {
        // Add the number of chains to the 'numbers' vector
        numbers.push_back(stoi(match[1].str()));
        return true;
    }
    return false;
}

bool MultimerHandler::isHeteromerFormat()
{
    regex pattern(
        "^(A[1-9][0-9]*)(B[1-9][0-9]*)?(C[1-9][0-9]*)?(D[1-9][0-9]*)?"
        "(E[1-9][0-9]*)?(F[1-9][0-9]*)?(G[1-9][0-9]*)?(H[1-9][0-9]*)?"
        "(I[1-9][0-9]*)?(J[1-9][0-9]*)?(K[1-9][0-9]*)?(L[1-9][0-9]*)?"
        "(M[1-9][0-9]*)?(N[1-9][0-9]*)?(O[1-9][0-9]*)?(P[1-9][0-9]*)?"
        "(Q[1-9][0-9]*)?(R[1-9][0-9]*)?(S[1-9][0-9]*)?(T[1-9][0-9]*)?"
        "(U[1-9][0-9]*)?(V[1-9][0-9]*)?(W[1-9][0-9]*)?(X[1-9][0-9]*)?"
        "(Y[1-9][0-9]*)?(Z[1-9][0-9]*)?$"
    );

    if (!regex_match(stoichiom, pattern)) {
        return false;
    }

    // Ensure the sequence of letters is valid
    char expected_letter = 'A';
    for (char c : stoichiom) {
        if (isalpha(c)) {
            if (c != expected_letter) {
                return false;
            }
            expected_letter++;
        }
    }

    // Extract numbers from the stoichiometry and store in `numbers` vector
    regex numberPattern("[1-9][0-9]*");
    sregex_iterator numbers_begin(stoichiom.begin(), stoichiom.end(), numberPattern);
    sregex_iterator numbers_end;

    for (auto it = numbers_begin; it != numbers_end; ++it) {
        numbers.push_back(stoi(it->str()));
    }

    chainsCount = numbers.size();

    return true;
}

vector<int> MultimerHandler::checkRepeatedChainAndReturnOneSegment(const vector<int>& row, int rowIndex, int chainIndex) 
{
    vector<int> previousSegment = getSequenceSegment(row, chainIndex);

    for (int i = 1; i < numbers[chainIndex]; i++)
    {
        vector<int> segment = getSequenceSegment(row, chainIndex, i+1);

        if (segment != previousSegment)
        {
            throw runtime_error("MSA for the repeated chain "  + chainIndexToStoichiomLetter(chainIndex) + to_string(numbers[chainIndex])
            + " is not the same in row: " + to_string(rowIndex + 1));
        }
        previousSegment = segment;
    }

    return previousSegment;
}

vector<vector<int>> MultimerHandler::getHomomerIndividualMSA(const vector<vector<int>>& sequences2num) 
{
    vector<vector<int>> msa;
    int length = sequences2num[0].size();

    chainIntervals.resize(2, 0);
    chainIntervals[1]= length;

    if (length % numbers[0] != 0) {
        throw invalid_argument("MSA is not in the form of stoichiometry A" + to_string(numbers[0]));
    }

    for (int index = 0; index < sequences2num.size(); ++index)
    {
        vector<int> part = checkRepeatedChainAndReturnOneSegment(sequences2num[index], index, 0);
        msa.push_back(part);
    }
    return msa;
}

int MultimerHandler::checkRowCorrespondsOnlyToOneChainAndReturnChainIndex(const vector<int>& row, int row_index) 
{
    // Tracks whether each chain has non-gap elements in the current row (false for no, true for yes)
    vector<bool> chainHasNonGap(chainIntervals.size() - 1, false); 

    // iterate over each chain
    for (int i = 1; i < chainIntervals.size(); i++)
    {
        // itrate over each position of the current chain
        for (int pos = chainIntervals[i - 1]; pos < chainIntervals[i]; pos++)
        {
            if (row[pos] != 0)
            {
                chainHasNonGap[i - 1] = true;
                break;
            }
        }

        // soon raise error if more than one non-gap chain is found in the current row
        int trueCount = 0;
        for (bool val : chainHasNonGap)
        {
            if (val && ++trueCount > 1) {
                throw runtime_error("MSA is not in the form of stoichiometry: " + stoichiom
                + "\n More than one chain has non-gap elements in row: " + to_string(row_index+1));
            }
        }
    }

    // Return the index of the chain, if the row contains exactly one chain with non-gap elements
    for (int i = 0; i < chainHasNonGap.size(); ++i) {
        if (chainHasNonGap[i]) {
            return i;
        }
    }

    // Raise error if the current row only contain gaps
    throw runtime_error("MSA is not in the form of stoichiometry: " + stoichiom
                + "\n No chain has non-gap elements in row: " + to_string(row_index+1));
                
    return -1;  // This never would happen
}

vector<int> MultimerHandler::checkMSAFormatAndReturnRowIndexIntervalsCorrespondingToChains(const vector<int>& chainIndexOfRows) 
{
    vector<int> individualMSAIndexIntervals(chainsCount + 1, 0);
    int currentChain = chainIndexOfRows[0];

    // itrate over each row
    int rowIndex;
    for (rowIndex = 1; rowIndex < chainIndexOfRows.size(); ++rowIndex)
    {
        // chain indexes should be ascending
        if (chainIndexOfRows[rowIndex] < currentChain)
        {
            throw runtime_error("The order of the individual MSAs is incorrect.");
        }
        
        if (chainIndexOfRows[rowIndex] != currentChain)
        {
            individualMSAIndexIntervals[currentChain+1] = rowIndex;
            currentChain = chainIndexOfRows[rowIndex];
        }
    }

    // set end index of the current chain
    individualMSAIndexIntervals[currentChain+1] = rowIndex;

    return individualMSAIndexIntervals;
}

vector<vector<vector<int>>> MultimerHandler::getHetoromerMSAs(const vector<vector<int>>& sequences2num, const vector<int>& chainLengths) 
{
    if (chainLengths.size() != numbers.size()) {
        throw runtime_error("Size of 'chainLengths' should be equal to the number of unique chains in the multimer.");
    }

    vector<vector<vector<int>>> msas(chainsCount + 1);

    chainIntervals.resize(chainsCount + 1, 0);

    for (int i = 1; i < chainIntervals.size(); i++)
    {
        chainIntervals[i] = chainIntervals[i - 1] + chainLengths[i - 1] * numbers[i - 1];
    }

    // The index for the end of the last chain should be the length of the MSA sequence.
    if(chainIntervals[chainsCount] != sequences2num[0].size())
    {
        throw runtime_error("The provided 'chain_lengths' and 'stoich' do not match the length of the MSA sequences.");
    }
    
    // Get paired MSA
    int individualMSAStartIndex = 0;
    for (int i = 0; i < sequences2num.size(); i++)
    {
        if (includesInPairedMSA(sequences2num[i]))
        {
            msas[0].push_back(sequences2num[i]);
            individualMSAStartIndex = msas[0].size();
        }
        else
        {   
            break;
        }
    }

    // Check repeated chains similarity in paired MSA
    for (int chainIndex = 1; chainIndex <= chainsCount; chainIndex++)
    {
        for (int rowIndex = 0; rowIndex < individualMSAStartIndex; rowIndex++)
        {
            if (numbers[chainIndex-1] > 1) // This chain is repeated more than once
            {
                vector<int> segment = checkRepeatedChainAndReturnOneSegment(sequences2num[rowIndex], rowIndex, chainIndex-1);
            }
        }
    }

    // Get individual MSAs when there are some rows corresponding to individual MSAs
    if(sequences2num.size() > individualMSAStartIndex)
    {
        vector<int> chainIndexOfRows(sequences2num.size()-individualMSAStartIndex, -1); //to keep chain index that each row in individual MSA part corresponds to

        for (int i = individualMSAStartIndex; i < sequences2num.size(); i++)
        {
            chainIndexOfRows[i-individualMSAStartIndex] =
            checkRowCorrespondsOnlyToOneChainAndReturnChainIndex(sequences2num[i], i); 
        }

        vector<int> individualMSAIndexIntervals = checkMSAFormatAndReturnRowIndexIntervalsCorrespondingToChains(chainIndexOfRows);

        //get MSAs beased on the their intervals

        // iterate over each chain
        for (int chainIndex = 0; chainIndex < chainsCount; ++chainIndex)
        {
            // Add the chain sequence from the query sequence to chain MSA, if there is an individual MSA for the chain
            if(individualMSAIndexIntervals[chainIndex+1] != 0)
            {
                if (numbers[chainIndex] > 1) // This chain is repeated more than once
                {  
                    vector<int> segment = checkRepeatedChainAndReturnOneSegment(sequences2num[0], 0, chainIndex);
                    msas[chainIndex+1].push_back(segment);
                }
                else
                {
                    msas[chainIndex+1].push_back(getSequenceSegment(sequences2num[0], chainIndex));
                }
            }

            // iterate over rows in the individual MSA of the current chain
            for (int rowIndex = individualMSAIndexIntervals[chainIndex]; rowIndex <  individualMSAIndexIntervals[chainIndex+1]; ++rowIndex)
            {
                int adjustedRowIndex = rowIndex + individualMSAStartIndex;
                
                // add chain interval of the row to chain MSA

                if (numbers[chainIndex] > 1) // This chain is repeated more than once
                {  
                    vector<int> chainInterval = checkRepeatedChainAndReturnOneSegment(sequences2num[adjustedRowIndex], adjustedRowIndex, chainIndex);
                    msas[chainIndex+1].push_back(chainInterval);
                }
                else
                {
                    msas[chainIndex+1].push_back(getSequenceSegment(sequences2num[adjustedRowIndex], chainIndex));
                }
            }
        }
    }
    return msas;
}

bool MultimerHandler::includesInPairedMSA(const vector<int>& row) 
{
    // Tracks whether each chain has non-gap elements in the current row (false for no, true for yes)
    vector<bool> chainHasNonGap(chainIntervals.size() - 1, false);

    // iterate over each chain
    for (int i = 1; i < chainIntervals.size(); i++)
    {
        // itrate over each position of the chain
        for (int pos = chainIntervals[i - 1]; pos < chainIntervals[i]; pos++)
        {
            if (row[pos] != 0) {
                chainHasNonGap[i - 1] = true;
                break;
            }
        }

        // soon return true if row has 2 non-gap chains
        int trueCount = 0;
        for (bool value : chainHasNonGap) {
            if (value && ++trueCount == 2) {
                return true;
            }
        }
    }

    return false;
}