/**
 * @file msaReader.h
 * @brief This file contains the declaration of the MSAReader class and its derived classes.
 *
 * The MSAReader class provides functionality to read multiple sequence alignments (MSA) from different file formats.
 * It supports reading MSAs in a2m, a3m, stockholm, fasta, CLUSTAL, and aln formats.
 * The MSAReader class is an abstract base class, and the derived classes provide specific implementations for each format.
 */

#ifndef MSAREADER_H
#define MSAREADER_H

#include <vector>
#include <string>
#include <iostream>
#include "common.h"

class MSAReader
{
protected:
    std::string file;
    Alphabet alphabet;
    bool checkValidation;
    bool omitGaps;
    std::vector<Sequence> Sequences; // the sequences read from the file

    /// @brief  Check if the MSA sequences are aligned
    /// @return -1 if aligned, otherwise return the index of the unaligned sequence     
    int isUnaligned();

    /// @brief Check if the sequences contain any invalid characters
    /// @return The sequence number and position of the first invalid character, or an empty string if valid
    std::string containsInvalidLetters(Alphabet alphabet);

    /// @brief Convert all lowercase letters to uppercase in the sequences
    void makeUppercase();

    /// @brief Validate the sequences for any invalid characters or unaligned sequences
    /// @param alphabet 
    void validateSequences(Alphabet alphabet);

    virtual void readFile(std::ifstream& file)=0;  //??

public:
    /// @brief Constructor
    /// @param _file 
    /// @param _alphabet 
    /// @param _checkValidation 
    /// @param _omitGaps 
    MSAReader(std::string _file, Alphabet _alphabet, bool _checkValidation, bool _omitGaps = false);

    /// @brief Read the MSA file
    /// @return The processed sequences in the file
    std::vector<Sequence> read();
};

// Derived class for reading a2m format
class MSAReader_a2m : public MSAReader
{
public:
    using MSAReader::MSAReader;
private:
    void readFile(std::ifstream& file) override;
};

// Derived class for reading a3m format
class MSAReader_a3m : public MSAReader
{
public:
    using MSAReader::MSAReader;
private:
    void readFile(std::ifstream& file) override;
};

// Derived class for reading stockholm format
class MSAReader_sto : public MSAReader
{
public:
    using MSAReader::MSAReader;
private:
    void readFile(std::ifstream& file) override;
};

// Derived class for reading fasta / afa format
class MSAReader_fasta : public MSAReader
{
public:
    using MSAReader::MSAReader;
private:
    void readFile(std::ifstream& file) override;
};

// Derived class for reading clustal format
class MSAReader_clustal : public MSAReader
{
public:
    using MSAReader::MSAReader;
private:
    void readFile(std::ifstream& file) override;
};

// Derived class for reading aln format
class MSAReader_aln : public MSAReader
{
public:
    using MSAReader::MSAReader;
private:
    void readFile(std::ifstream& file) override;
};

// Derived class for reading pfam format
class MSAReader_pfam : public MSAReader
{
public:
    using MSAReader::MSAReader;
private:
    void readFile(std::ifstream& file) override;
};

#endif
