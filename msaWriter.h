/**
 * @file msaWriter.h
 * @brief This file contains the declaration of the MSAWriter class and its derived classes.
 *
 * The MSAWriter class provides functionality to write multiple sequence alignments (MSA) to different file formats.
 * It supports writing MSAs in a2m, a3m, stockholm, fasta, CLUSTAL, and aln formats.
 * The MSAWriter class is an abstract base class, and the derived classes provide specific implementations for each format.
 * 
 */

#ifndef MSAWRITER_H
#define MSAWRITER_H

#include <vector>
#include <string>
#include <iostream>
#include "common.h"

class MSAWriter
{
    protected:
        std::string file;
        std::string fileName;
        std::string format;
        std::vector<Sequence> Sequences;
        int IDspace; // the max space needed to write ID, in order to all sequences be aligned in output file

        virtual void writeFile(std::ofstream& file) = 0;

        /// @brief Generate IDs for sequences that do not have any ID.
        void generateIdForSequences();

    public:
       
        /// @brief Constructor
        /// @param sequences 
        /// @param _file 
        MSAWriter(std::vector<Sequence> sequences, std::string _file);

         /**
         * @brief Write sequences in the MSA file, based on the format of the output file.
         * @param file The output file path to write to.
         */
        void write(); 
};

// Derived class for writing in a2m format
class MSAWriter_a2m : public MSAWriter
{
    public:
        using MSAWriter::MSAWriter;
    private:   
        void writeFile(std::ofstream& file) override;
};

// Derived class for writing in a3m format
class MSAWriter_a3m : public MSAWriter
{
    public:
        using MSAWriter::MSAWriter;
    private:
        void writeFile(std::ofstream& file) override;
};

// Derived class for writing in stockholm format
class MSAWriter_sto : public MSAWriter
{
    public:
        using MSAWriter::MSAWriter;
    private:     
        void writeFile(std::ofstream& file) override;
};

// Derived class for writing in fasta / afa format
class MSAWriter_fasta : public MSAWriter
{
    public:
        using MSAWriter::MSAWriter;
    private:
        void writeFile(std::ofstream& file) override;
};

// Derived class for writing in CLUSTAL format
class MSAWriter_clustal : public MSAWriter
{
    public:
        using MSAWriter::MSAWriter;
    private:
        void writeFile(std::ofstream& file) override;
};

// Derived class for writing in aln format
class MSAWriter_aln : public MSAWriter
{
    public:
        using MSAWriter::MSAWriter;
    private:      
        void writeFile(std::ofstream& file) override;
};

// Derived class for writing in pfam format
class MSAWriter_pfam : public MSAWriter
{
    public:
        using MSAWriter::MSAWriter;
    private:      
        void writeFile(std::ofstream& file) override;
};

#endif
