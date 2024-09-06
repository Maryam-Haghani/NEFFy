/**
 * @file converter.cpp
 * @brief MSA File Converter
 *
 * This program converts the format of an input Multiple Sequence Alignment (MSA) file
 * to the format of an output MSA file.
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
 * 
 *  For detailed instructions, please refer to the documentation at https://maryam-haghani.github.io/NEFFy.
 */

const char* docstr = R"(

This program converts the format of an input Multiple Sequence Alignment (MSA) file to the format of an output MSA file.

Usage:
./converter --in_file=<input_file> --out_file=<output_file> [options]

For detailed instructions, please refer to the documentation at https://maryam-haghani.github.io/NEFFy.
)";

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
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

/// @brief Convert the format of the input MSA file to the format of the output MSA file.
/// @param inFile 
/// @param outFile 
/// @param checkValidation 
/// @param alphabet 
/// @return size
int convert(string inFile, string outFile, bool checkValidation, Alphabet alphabet)
{
    string inFormat = getFormat(inFile, "in_file");
    string outFormat = getFormat(outFile, "out_file");

    MSAReader* msaReader;
    if (inFormat == "a2m")
        msaReader = new MSAReader_a2m(inFile, alphabet, checkValidation);
    else if(inFormat == "a3m")
        msaReader = new MSAReader_a3m(inFile, alphabet, checkValidation);
    else if(std::find(FASTA_FORMATS.begin(), FASTA_FORMATS.end(), inFormat) != FASTA_FORMATS.end())
        msaReader = new MSAReader_fasta(inFile, alphabet, checkValidation);
    else if(inFormat == "sto")
        msaReader = new MSAReader_sto(inFile, alphabet, checkValidation);
    else if(inFormat == "clustal")
        msaReader = new MSAReader_clustal(inFile, alphabet, checkValidation);
    else if (inFormat == "aln")
        msaReader = new MSAReader_aln(inFile, alphabet, checkValidation);
    else if (inFormat == "pfam")
        msaReader = new MSAReader_pfam(inFile, alphabet, checkValidation);
    else
        throw runtime_error("Not supported input MSA file format (" + inFormat + ")");

    vector<Sequence> sequences = msaReader->read();

    MSAWriter* msaWriter;
    if (outFormat == "a2m")
        msaWriter = new MSAWriter_a2m(sequences, outFile);
    else if (outFormat == "a3m")
        msaWriter = new MSAWriter_a3m(sequences, outFile);
    else if (std::find(FASTA_FORMATS.begin(), FASTA_FORMATS.end(), outFormat) != FASTA_FORMATS.end())
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

    return sequences.size();
}

/// @brief Get given alphabet by user
/// @param flagHandler 
/// @return 
Alphabet getAlphabet(FlagHandler& flagHandler)
{
    Alphabet alphabet;
    try {
        alphabet = static_cast<Alphabet>(stoi(flagHandler.getFlagValue("alphabet")));
        if (alphabet < Alphabet::protein || alphabet > Alphabet::DNA) {
            throw runtime_error("");
        }
    } catch (const exception& e) {
       throw runtime_error("Invalid 'alphabet' value. It is outside the valid enum range.");
    }
    return alphabet;
}

int main(int argc, char **argv)
{
    /* Handling flags */
    vector<string> args(argv + 1, argv + argc);
    FlagHandler flagHandler(Flags);
    Alphabet alphabet;

    // Check for help flag
    for (const auto& arg : args) {
        if (arg == "-h" || arg == "--help") {
            cout << docstr << endl;
            return 0;
        }
    }

    try
    { 
        flagHandler.processFlags(args);
        flagHandler.checkRequiredFlags();

        // in_file
        string inFile = flagHandler.getFlagValue("in_file");

        // out_file
        string outFile = flagHandler.getFlagValue("out_file");

        // alphabet
        alphabet = getAlphabet(flagHandler);

        // check_validation
        bool checkValidation = flagHandler.getFlagValue("check_validation") == "true";

        int msaDepth = convert(inFile, outFile, checkValidation, alphabet);

        cout << "Converted " << inFile << " with " <<  msaDepth << " sequences from " << getFormat(inFile, "in_file")
        << " to " << getFormat(outFile, "out_file") << " and saved the output as " << outFile << "." << endl;
        
        return 0;
    }
    catch (const runtime_error& e) 
    {
        cerr << "Handled Error: " << e.what() << endl << endl;
        cerr << docstr;
        return 1;
    } 
    catch (const exception& e) 
    {
        cerr << "Error: " << e.what() << "\nPlease review the provided MSA file." << endl << endl;
        cerr << docstr;
        return 1;
    }
}