/**
 * @file converter.cpp
 * @brief MSA File Converter
 *
 * This program converts the format of an input Multiple Sequence Alignment (MSA) file
 * to the format of an output MSA file. It supports the following formats:
 *      -A2M (FASTA-like format with additional annotations)
 *      -A3M (Extended A2M format)
 *      -FASTA (Standard FASTA format)
 *      -STO (Stockholm format)
 *      -Clustal (CLUSTAL format)
 *      -ALN (ALN format)
 *      -AFA
 *      -PFAM

 *
 * Usage:
 *   ./converter --in_file=<input_file> --out_file=<output_file> [options]
 *
 * Options:
 *   --in_file=<input_file>           Path to the input MSA file
 *   --out_file=<output_file>         Path to the output MSA file
 *   --alphabet=<value>               Valid alphabet of MSA;  alphabet option (0: Protein, 1: RNA, 2: DNA) (default: 0)
 *   --check_validation=<true/false>  Perform validation on sequences (default: true)
 *
 * The input MSA file is converted to the format specified by the output file. The converted
 * file is saved in the specified output file path.
 */

const char* docstr = ""
"\n"
"./converter --in_file=<input_file> --out_file=<output_file> [options]\n"
"Options:\n"
"   --in_file=<input_file>            Path to the input MSA file\n"
"   --out_file=<output_file>          Path to the output MSA file\n"
"    --alphabet=<value>               Valid alphabet of MSA; alphabet option (0: Protein, 1: RNA, 2: DNA) (default: 0)\n"
"    --check_validation=<true/false>  Perform validation on sequences (default: true)\n"
"Supported formats: A2M, A3M, FASTA,  STO, CLUSTAL, ALN, AFA, PFAM\n"
;

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "flagHandler.h"
#include "msaReader.h"
#include "msaWriter.h"

using namespace std;

unordered_map<string, FlagInfo> Flags =
{
    {"in_file", {true, ""}},                // in_file path
    {"out_file", {true, ""}},               // out_file path
    {"alphabet", {false, "0"}},             // alphabet of MSA
    {"check_validation", {false, "true"}},  // whether to perform validation on sequences to include only alphabet letters
};

/**
 * @brief Convert the format of the input MSA file to the format of the output MSA file.
 */
void convert(string inFile, string outFile, bool checkValidation, Alphabet alphabet)
{
    string inFormat = getFormat(inFile, "in_file");
    string outFormat = getFormat(outFile, "out_file");

    MSAReader* msa_reader;
    if (inFormat == "a2m")
        msa_reader = new MSAReader_a2m(inFile, alphabet, checkValidation);
    else if(inFormat == "a3m")
        msa_reader = new MSAReader_a3m(inFile, alphabet, checkValidation);
    else if(inFormat == "fasta" || inFormat == "afa")
        msa_reader = new MSAReader_fasta(inFile, alphabet, checkValidation);
    else if(inFormat == "sto")
        msa_reader = new MSAReader_sto(inFile, alphabet, checkValidation);
    else if(inFormat == "clustal")
        msa_reader = new MSAReader_clustal(inFile, alphabet, checkValidation);
    else if (inFormat == "aln")
        msa_reader = new MSAReader_aln(inFile, alphabet, checkValidation);
    else if (inFormat == "pfam")
        msa_reader = new MSAReader_pfam(inFile, alphabet, checkValidation);
    else
        throw runtime_error("Not supported input MSA file format (" + inFormat + ")");

    vector<Sequence> sequences = msa_reader->read();

    MSAWriter* msaWriter;
    if (outFormat == "a2m")
        msaWriter = new MSAWriter_a2m(sequences, outFile);
    else if (outFormat == "a3m")
        msaWriter = new MSAWriter_a3m(sequences, outFile);
    else if (outFormat == "fasta" || outFormat == "afa")
        msaWriter = new MSAWriter_fasta(sequences, outFile);
    else if (outFormat == "sto")
        msaWriter = new MSAWriter_sto(sequences, outFile);
    else if (outFormat == "clustal")
        msaWriter = new MSAWriter_clustal(sequences, outFile);
    else if (outFormat == "aln")
        msaWriter = new MSAWriter_aln(sequences, outFile);
    else if (outFormat == "pfam")
        msaWriter = new MSAWriter_pfam(sequences, outFile);
    else
    {
        throw runtime_error("Not supported output MSA file format (" + outFormat + ")");
    }

    msaWriter->write();
}

int main(int argc, char **argv)
{
    /* Handling flags */
    vector<string> args(argv + 1, argv + argc);
    FlagHandler flagHandler(Flags);
    Alphabet alphabet;

    try
    { 
        flagHandler.processFlags(args);
        flagHandler.checkRequiredFlags();

        // in_file
        string inFile = flagHandler.getFlagValue("in_file");

        // out_file
        string outFile = flagHandler.getFlagValue("out_file");

        // alphabet
        alphabet = static_cast<Alphabet>(stoi(flagHandler.getFlagValue("alphabet")));

        // check_validation
        bool checkValidation = flagHandler.getFlagValue("check_validation") == "true";

        convert(inFile, outFile, checkValidation, alphabet);
        
        return 0;
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
        cerr << docstr;
        return 1;
    }
}