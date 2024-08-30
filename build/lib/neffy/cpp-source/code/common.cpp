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

/// @brief Get format of the given file
/// @param file 
/// @param name 
/// @return 
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
        throw runtime_error("Please specify a format for " + name);
    }
}

/// @brief Remove gap positions of the query sequence from all sequences
/// @param sequences 
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

/// @brief Return all standard + non-standard + gap letters for the given alphabet
/// @param alphabet 
/// @return 
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

/// @brief Return standard letters of the given alphabet
/// @param alphabet 
/// @return 
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

/// @brief Return non-standard letters of the given alphabet
/// @param alphabet 
/// @return 
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