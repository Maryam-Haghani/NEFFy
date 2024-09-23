/**
 * @file msaReader.cpp
 * @brief This file contains the implementation of the MSAReader class and its derived classes.
 *
 * The MSAReader class provides functionality to read multiple sequence alignments (MSA) from different file formats.
 * It supports reading MSAs in a2m, a3m, stockholm, fasta, CLUSTAL, pfam and aln formats.
 * The MSAReader class is an abstract base class, and the derived classes provide specific implementations for each format.
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "common.h"
#include "msaReader.h"

using namespace std;

MSAReader::MSAReader(string _file, Alphabet _alphabet,  bool _checkValidation, bool _omitGaps)
: file(_file), alphabet(_alphabet), checkValidation(_checkValidation), omitGaps(_omitGaps) {}

vector<Sequence> MSAReader::read()
{
    ifstream inputFile(file);
    if (!inputFile)
    {
        throw runtime_error( "Failed to open the input file '"+ file + "'.");
    }

    readFile(inputFile);

    inputFile.close();

    makeUppercase();

    if(checkValidation)
    {
        validateSequences(alphabet);
    }
    return Sequences;
}

// void MSAReader::readFile(ifstream& inputFile) {}

void MSAReader::makeUppercase()
{
    for (auto& sequence : Sequences)
    {
        transform(sequence.sequence.begin(), sequence.sequence.end(), sequence.sequence.begin(), ::toupper);
    }
}

void MSAReader::validateSequences(Alphabet alphabet)
{    
    string invalidLetter = containsInvalidLetters(alphabet);
    if (!invalidLetter.empty())
    {
        throw runtime_error("MSA file contains an invalid character in: "+ invalidLetter);    
    }

    int unaligned = isUnaligned();
    if (unaligned != -1)
    {
        throw runtime_error("MSA file expected to be aligned...\nSequence "
                            + to_string(unaligned) + " causes misalignment.");    
    }
}

int MSAReader::isUnaligned()
{
    int index = 0;
    const int length = Sequences[0].sequence.length();           

    for (auto sequence : Sequences)
    {
        index++;
        if (sequence.sequence.length() != length) // Sequence found with a different length
        {
            return index;
        }
    }
    return -1;
}

string MSAReader::containsInvalidLetters(Alphabet alphabet)
{
    const string allowedLetters = getAllowedLetters(alphabet);
    int index = 0;

    for (auto sequence : Sequences)
    {
        index++;
        auto pos = sequence.sequence.find_first_not_of(allowedLetters);
        if (pos != string::npos)
        {
            return "sequence " + to_string(index) + " and position " + to_string(pos+1);  // Invalid letter found
        }
    }
    return "";
}

void MSAReader_a2m::readFile(ifstream& inputFile)
{
    string line, id, sequence, temp, remarks = "";
    int num = 0;

    while (getline(inputFile, line))
    {
        if (!line.empty())
        {
            istringstream iss(line);

            if (line[0] == '>') // Getting ID and remarks
            {            
                remarks.clear();                          
                iss >> id;
                id = id.substr(1);

                while (iss >> temp)
                {
                    remarks =  remarks + " " + temp;
                } 
            }
            else
            {
                iss >> sequence;
                replace(sequence.begin(), sequence.end(), '.', '-'); // Converting all '.'s to '-'s

                // Adding some number at the end of ID, if already exists
                auto it = std::find_if(Sequences.begin(), Sequences.end(), [&](const Sequence& sequence) {
                    return sequence.id == id;
                });

                if (it != Sequences.end())
                {
                    id = id + '_' + to_string(++num);
                }

                Sequences.push_back({id, sequence, remarks});
            }
        }                
    }
}

void MSAReader_a3m::readFile(ifstream& inputFile)
{
    string line, id, sequence, temp, remarks = "";
    int num =  0;

    while (getline(inputFile, line))
    {
        if (!line.empty())
        {
            istringstream iss(line);

             // Getting ID and remarks
            if (line[0] == '>')
            {                  
                remarks.clear();  
                iss >> id;
                id = id.substr(1);

                while (iss >> temp)
                {
                    remarks =  remarks + " " + temp;
                }                             
            }
            else
            {
                iss >> sequence;
                replace(sequence.begin(), sequence.end(), '.', '-'); // Converting all '.'s to '-'s

                // Adding some number at the end of ID, if already exists
                auto it = std::find_if(Sequences.begin(), Sequences.end(), [&](const Sequence& sequence) {
                    return sequence.id == id;
                });

                if (it != Sequences.end())
                {
                    id = id + '_' + to_string(++num);
                }

                Sequences.push_back({id, sequence, remarks});
            }
        }                
    }

    int unaligned = isUnaligned();
    if (unaligned != -1) // unaligned
    {                
        if (omitGaps) // An option for neff calculation: will remove all lowercase letters in all sequences
        {
            string seq;
            for (auto& sequence : Sequences)
            {
                seq.clear();
                for (char res : sequence.sequence)
                {
                    if (!(islower(res)))
                    {
                        seq += res;
                    }
                }
                sequence.sequence = seq;
            }
        } 
        else // Add gaps in the positions of lowercase letters
        {        
            for (int pos = 0; pos < Sequences[0].sequence.size(); pos++)
            {
                // Check if lowercase character found in the position for any sequence
                for (auto seq : Sequences)
                {
                    if (islower(seq.sequence[pos]))
                    {
                        for (auto& sequence : Sequences)
                        {
                            if (!islower(sequence.sequence[pos]))
                            {
                                sequence.sequence.insert(pos, "-");
                            }
                            else
                            {
                                sequence.sequence[pos] = toupper(sequence.sequence[pos]);
                            }
                        }
                    }
                }                               
            }
        }
    }
}

void MSAReader_sto::readFile(ifstream& inputFile)
{
    string id, seq, line, temp, remarks = "";
    int lastGS = 0;
    int lineNo = 0;
    bool lineAfterStockholm = false;
    bool noAlignmetnts = false;
    bool goForward = false;

    /* Only reading sequences whose IDs are present at the beginning of the file */
    while (getline(inputFile, line))
    {        
        lineNo++;
        remarks.clear();
        istringstream iss(line);

        if (line.find("# STOCKHOLM") == 0)
        {
            lineAfterStockholm = true;
            continue;
        }

        if (lineAfterStockholm && !line.empty())
        {
            // if first line after version is not GF or GS, means there are no alignments in MSA
            if ((line.find("#=GF") != 0) and (line.find("#=GS") != 0))
            {
                noAlignmetnts = true;
                Sequences.push_back({"", "", ""});
            }
            lineAfterStockholm = false;
            goForward = true;
        }

        if (goForward)
        {
            if (noAlignmetnts) // get query sequence lines
            {
                if(!(line.empty() || line.rfind("//", 0) == 0 ||  line.rfind("#", 0) == 0))
                {
                    iss >> id >> seq;
                    Sequences[0].id = id;
                    Sequences[0].sequence = Sequences[0].sequence+seq;  
                }
            }
            else
            {
                // Get ID and remarks of sequences
                if (line.substr(0, 4) == "#=GS")
                {
                    iss >> temp >> id;

                    while (iss >> temp)
                    {
                        remarks =  remarks + " " + temp;
                    }            

                    Sequences.push_back({id, "", remarks});
                    lastGS = lineNo;
                }

                // Ignore any other comments
                if (line.empty() || line.rfind("//", 0) == 0 ||  line.rfind("#", 0) == 0)
                {
                    continue;
                }

                // add the first sequence which do not have any GS remark (if not added already)
                if(lineNo == lastGS+2)
                {
                    iss >> id >> seq;
                    auto it = std::find_if(Sequences.begin(), Sequences.end(), [&](const Sequence& sequence) {
                        return sequence.id == id;
                    });

                    if (it == Sequences.end())
                    {
                        Sequences.insert(Sequences.begin(), {id, "", ""});
                    }
                }
                
                if(lineNo >= lastGS+2)
                {
                    // Get sequences
                    iss >> id >> seq;
                    for (auto& sequence : Sequences)
                    {
                        if (sequence.id == id)
                        {
                            sequence.sequence = sequence.sequence+seq;
                            break;
                        }
                    }
                }  
            }
        }
    }
}

void MSAReader_fasta::readFile(ifstream& inputFile)
{
    int num = 0;
    string line, id, sequence, temp, remarks = "";

    // Read the file line by line
    while (std::getline(inputFile, line))
    {
        if (!line.empty())
        {           
            if (line[0] == '>')
            {
                // If it's a header line, store the previous data (if any)
                if (!id.empty() && !sequence.empty())
                {
                    // Adding some number at the end of ID, if already exists
                    auto it = std::find_if(Sequences.begin(), Sequences.end(), [&](const Sequence& sequence) {
                        return sequence.id == id;
                    });

                    if (it != Sequences.end())
                    {
                        id = id + '_' + to_string(++num);
                    }

                    Sequences.push_back({id, sequence, remarks});
                }

                // set id and clear remark and sequence
                istringstream iss(line);                                               
                iss >> id;
                id = id.substr(1);

                remarks.clear();
                while (iss >> temp)
                {
                    remarks =  remarks + " " + temp;
                }

                sequence.clear();
            }
            else
            {
                // Append the line to the sequence
                sequence += line;
            }
        }
    }
    // Store the last entry (if any)
    if (!id.empty() && !sequence.empty())
    {
        Sequences.push_back({id, sequence, remarks});
    }
}

void MSAReader_clustal::readFile(ifstream& inputFile)
{
    string line, id, seq;
    while (getline(inputFile, line))
    {        
        if (line.empty() || line.find("CLUSTAL") == 0) // Removing comments
        {
            continue;
        }                
        istringstream iss(line);
        iss >> id >> seq;

        bool idExists = false;
        for (auto& sequence : Sequences)
        {
            if (sequence.id == id)
            {
                sequence.sequence = sequence.sequence + seq;
                idExists = true;
                break;
            }
        }
        if (!idExists)
        {
            Sequences.push_back({id, seq, {}});
        }
    }
}

void MSAReader_aln::readFile(ifstream& inputFile)
{
    string line, sequence;
    
    while (getline(inputFile, line))
    {
        if (!line.empty())
        {
            istringstream iss(line);            
            iss >> sequence;
            Sequences.push_back({"", sequence, {}});
        }                
    }
}

void MSAReader_pfam::readFile(ifstream& inputFile)
{
    string line, id, seq;
    while (getline(inputFile, line))
    {        
        if (line.empty()) // Removing comments
        {
            continue;
        }                
        istringstream iss(line);
        iss >> id >> seq;

        bool idExists = false;
        for (auto& sequence : Sequences)
        {
            if (sequence.id == id)
            {
                sequence.sequence = seq;
                idExists = true;
                break;
            }
        }
        if (!idExists)
        {
            Sequences.push_back({id, seq, {}});
        }
    }
}
