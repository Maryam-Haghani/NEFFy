/**
 * @file common.cpp
 * @brief This file contains common utility functions used in the MSA project.
 */

#include <vector>
#include <string>
#include <functional>
#include <stdexcept>
#include "common.h"

using namespace std;

string getFormat(string file, string name)
{
    size_t index = file.find_last_of('.');
    string format;

    if (index != string::npos)
    {
        return file.substr(index + 1);
    }
    else
    {
        throw runtime_error("please specify a format for " + name);
    }
}

void keepNonGapPositionsOfQuerySequence(vector<Sequence>& sequences)
{
    vector<int> gapPositions;
    string querySequence = sequences[0].sequence;
    bool isQuerySeq = true;

    for (auto& sequence : sequences)
    {        
        if (isQuerySeq) // Gathering the positions of non-gaps from the query sequence
        {
            for (int i = querySequence.length()-1; i >= 0; i--)
            {
                if (querySequence[i] == '-')
                {
                    gapPositions.push_back(i);
                }
            }
            isQuerySeq = false;
        }

        for (int pos : gapPositions)
        {
            sequence.sequence.erase(pos, 1);
        }
    }
}

string getAllowedLetters(Alphabet alphabet)
{
    switch(alphabet)
    {
        case protein:
            return STANDARD_AMINO_ACIDS + NON_STANDARD_AMINO_ACIDS + GAP;
        case RNA:
            return STANDARD_RNA_NUCLEOTIDES + NON_STANDARD_NUCLEOTIDES + GAP;
        case DNA:
            return STANDARD_DNA_NUCLEOTIDES + NON_STANDARD_NUCLEOTIDES + GAP;
        default:
            return GAP;
    }
}

string getStandardLetters(Alphabet alphabet)
{
    switch(alphabet)
    {
        case protein:
            return STANDARD_AMINO_ACIDS;
        case RNA:
            return STANDARD_RNA_NUCLEOTIDES; 
        case DNA:
            return STANDARD_DNA_NUCLEOTIDES;
        default:
            return "";
    }
}

string getNonStandardLetters(Alphabet alphabet)
{
    switch(alphabet)
    {
        case protein:
            return NON_STANDARD_AMINO_ACIDS;
        case RNA:
            return NON_STANDARD_NUCLEOTIDES; 
        case DNA:
            return NON_STANDARD_NUCLEOTIDES;
        default:
            return "";
    }
}