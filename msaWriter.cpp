/**
 * @file msaWriter.cpp
 * @brief This file contains the implementation of the MSAWriter class and its derived classes.
 *
 * The MSAWriter class provides functionality to write multiple sequence alignments (MSA) to different file formats.
 * It supports writing MSAs in a2m, a3m, stockholm, fasta, clustal, pfam and aln formats.
 * The MSAWriter class is an abstract base class, and the derived classes provide specific implementations for each format.
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include "common.h"
#include "msaWriter.h"
#include <set>

using namespace std;

MSAWriter::MSAWriter(std::vector<Sequence> _sequences, std::string _file, std::set<int> _maskedIndices)
    : Sequences(_sequences), file(_file), maskedIndices(_maskedIndices) {}

void MSAWriter::generateIdForSequences()
{
    int index = 1;
    for (auto& sequence : Sequences)
    {
        sequence.id = "sequence_"+to_string(index++);
    }
}

void MSAWriter::write()
{
    if (Sequences[0].id.empty())
    {
        generateIdForSequences();
    }
    
    IDspace = 0;
    for (auto sequence : Sequences)
    {
        IDspace = max(IDspace, int(sequence.id.size()));
    }
    IDspace +=1;

    ofstream outputFile(file);
    if (!outputFile)
    {
        throw runtime_error("Failed to create file: " + file);
    }

    writeFile(outputFile);
    outputFile.close();
}

void MSAWriter_a2m::writeFile(ofstream& outputFile)
{
    string querySequence = Sequences[0].sequence;

    // Replace '-' to '.' for gaps aligned to insersions if query sequence contains any gaps
    int gapPosition = querySequence.find('-');
    if (gapPosition != string::npos)
    {
        int length = querySequence.size();

        for (int pos = length - 1; pos >= 0; pos--)
        {
            if(querySequence[pos] == '-')
            {
                for (auto& sequence : Sequences)
                {
                    if (sequence.sequence[pos] == '-')
                    {
                        sequence.sequence[pos] = '.';         
                    }
                    else //converting to lowercase 
                    {                    
                        sequence.sequence[pos] = tolower(sequence.sequence[pos]);
                    }
                }
            }                                        
        }
    }

    for (auto sequence : Sequences)
    {
        outputFile << '>' << sequence.id << sequence.remarks << endl; // Writing identifier and remarks
        outputFile << sequence.sequence << endl;
    }
}

void MSAWriter_a3m::writeFile(ofstream& outputFile)
{
    string querySequence = Sequences[0].sequence;

    // Removing gaps aligned to insersions in query sequence if contains any
    int gapPosition = querySequence.find('-');
    if (gapPosition != string::npos)
    {
        int length = querySequence.size();

        for (int pos = length - 1; pos >= 0; pos--)
        {
            if(querySequence[pos] == '-')
            {
                for (auto& sequence : Sequences)
                {
                    if (sequence.sequence[pos] == '-')
                    {
                        sequence.sequence.erase(pos, 1);         
                    }
                    else //converting to lowercase 
                    {                    
                        sequence.sequence[pos] = tolower(sequence.sequence[pos]);
                    }
                }
            }                                        
        }
    }

    for (auto sequence : Sequences)
    {
        outputFile << '>' << sequence.id << sequence.remarks << endl; // Writing identifier and remarks
        outputFile << sequence.sequence << endl;
    }
}

void MSAWriter_sto::writeFile(ofstream& outputFile)
{
    string querySequence = Sequences[0].sequence;

    outputFile << "# STOCKHOLM 1.0" <<endl << endl;

    // Writing identifiers and remarks
    for (auto sequence : Sequences)
    {
        if(sequence.sequence == querySequence)
        {
            outputFile << left << setw(5) << "#=GF" << left << setw(3) << "ID"  << sequence.id << endl << endl;
        }

        outputFile << left << setw(5) << "#=GS" << left << setw(IDspace) << sequence.id << sequence.remarks << endl;   
    }
    outputFile<< endl;

    // Writing sequences
    for (int i = 0; i < querySequence.length(); i += 200)
    {  
        for (auto sequence : Sequences)
        {
            outputFile << left << setw(IDspace) << sequence.id << sequence.sequence.substr(i, 200) << endl;
        }
        outputFile << endl;
    }

    outputFile << "// "<< endl;
}

void MSAWriter_fasta::writeFile(ofstream& outputFile)
{
    for (int index = 0; index < Sequences.size(); ++index)
    {
        if (maskedIndices.find(index) == maskedIndices.end())
        {
            outputFile << '>' << Sequences[index].id << Sequences[index].remarks << endl; // Writing identifier and remarks
            outputFile << Sequences[index].sequence << endl;
        }
    }
}

void MSAWriter_clustal::writeFile(ofstream& outputFile)
{
    outputFile << "Generated CLUSTAL format" << endl << endl;

    for (size_t i = 0; i < Sequences[0].sequence.length(); i += 60)
    {
        for (auto sequence : Sequences)
        {
            outputFile << left << setw(IDspace) << sequence.id << sequence.sequence.substr(i, 60)  << endl;
        }
    }
}

void MSAWriter_aln::writeFile(std::ofstream& outputFile)
{
    keepNonGapPositionsOfQuerySequence(Sequences);

    for (auto sequence : Sequences)
    {
        outputFile << sequence.sequence << endl;
    }
}

void MSAWriter_pfam::writeFile(ofstream& outputFile)
{
    for (auto sequence : Sequences)
    {
        outputFile << sequence.id << '\t' << sequence.sequence << endl;
    }
}
