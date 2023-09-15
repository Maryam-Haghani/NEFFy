/**
 * @file neff.cpp
 * @brief Neff Computation
 *
 * This program computes Number of EFFective sequences (NEFF) for a multiple sequence alignment (MSA) file.
 * NEFF is a measure of effective sequence number that takes into account the redundancy
 * and similarity of sequences in the MSA. It is commonly used in bioinformatics to
 * assess the diversity of a set of sequences.
 *
 * Usage:
 *   ./neff --file=<input_file> [options]
 *
 * Options:
 *   --file=<input_file>             Path to the input MSA file (required)
 *   --alphabet=<value>              Valid alphabet of MSA;  alphabet option (0: Protein, 1: RNA, 2: DNA) (default: 0)
 *   --check_validation=<true/false> Perform validation on sequences (default: false)
 *   --threshold=<value>             Threshold value of considering two sequences similar (default: 0.8)
 *   --norm=<value>                  Normalization option (0: sqrt(Length of alignment), 1: Length of alignment, 2: No normalization) (default: 0)
 *   --omit_query_gaps=<true/false>  Omit gap positions of query sequence from all sequences for NEFF computation (default: false)
 *   --is_symmetric=<true/false>     Consider gaps in number of computation when checking sequence semilarities (default: true)
 *   --non_standard_option=<value>   Handling non-standard letters in the specified alphabet (0: AsStandard, 1: ConsiderGapInCutoff, 2: ConsiderGap)
 *   --depth=<value>                 Depth of MSA to be cosidered in computation (default: 0)
 *   --pos_start=<value>             Start position of each sequence to be considered in neff (inclusive (default: 1))
 *   --pos_end=<value>               Last position of each sequence to be considered in neff (inclusive (default: depth og given MSA))
 *   --gap_cutoff=<value>            Cutoff value for removing gappy positions, when #gaps in position >= gap_cutoff (default=1 : does not remove anything), will be rounded to the upper int number
 * 
 * In symmetric version, threshold for considering a pair of sequence as homolog simply depends on length of alignment, and it would be equal for all sequences and we see a symmetry in similarities
 * While in asymmetric version, threshold depends on number of non-gap residues; therefore, threshold may be different for sequences)
 *
 * NEFF is computed based on the given options.
 */

const char* docstr = ""
"\n"
"./neff --file=<input_file> [options]\n"
"Options:\n"
"    --file=<input_file>             Path to the input MSA file (required)\n"
"    --alphabet=<value>              Valid alphabet of MSA; alphabet option (0: Protein, 1: RNA, 2: DNA) (default: 0)\n"
"    --check_validation=<true/false> Perform validation on sequences (default: false)\n"
"    --threshold=<value>             Threshold value of considering two sequences similar (default: 0.8)\n"
"    --norm=<value>                  Normalization option (0: sqrt(Length of alignment), 1: Length of alignment, 2: No normalization) (default: 0)\n"
"    --omit_query_gaps=<true/false>  Omit gap positions of query sequence from all sequences for NEFF computation (default: false)\n"
"    --is_symmetric=<true/false>     Consider gaps in number of differences when checking sequence semilarities (default: true)\n"
"    --non_standard_option=<value>   Handling non-standard letters in the specified alphabet (0: AsStandard, 1: ConsiderGapInCutoff, 2: ConsiderGap)\n"
"    --depth=<value>                 Depth of MSA to be cosidered in computation (default: 0)\n"
"    --pos_start=<value>             Start position of each sequence to be considered in neff (inclusive (default: 1))\n"
"    --pos_end=<value>               Last position of each sequence to be considered in neff (inclusive (default: depth og given MSA))\n"
"    --gap_cutoff=<value>            Cutoff value for removing gappy positions, when #gaps in position >= gap_cutoff (default=1 : does not remove anything)\n"
;

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include "flagHandler.h"
#include "msaReader.h"
#include "common.h"
#include <cmath>
#include <fstream>
#include <algorithm>

using namespace std;

unordered_map<string, FlagInfo> Flags =
{
    {"file", {true, ""}},                   // file path
    {"alphabet", {false, "0"}},             // alphabet of MSA
    {"check_validation", {false, "false"}}, // whether to perform validation on sequences to include only alphabet letters
    {"threshold", {false, "0.8"}},          // threshold value for similarity
    {"norm", {false, "0"}},                 // neff normalization option
    {"omit_query_gaps", {false, "true"}},   // whether to omit gap positions of query sequence from all sequences for NEFF computation
    {"is_symmetric", {false, "true"}},      // whether to consider gaps in similarity cutoff computation (asymmetric) or not (symmetric)
    {"non_standard_option", {false, "0"}},  // handling non-standard letters in the given alphabet
    {"depth", {false, "inf"}},              // depth of MSA to be cosidered in computation
    {"pos_start", {false, "1"}},            // start position of each sequence to be considered in NEFF (inclusive)
    {"pos_end", {false, "inf"}},            // last position of each sequence to be considered in NEFF (inclusive)
    {"gap_cutoff", {false, "1"}}            // cutoff value for considering a position as gappy and removing that
 };

/// @brief map residues from char to digit
/// @param c input letter
/// @param standardLetters 
/// @param nonStandardLetters 
/// @param nonStandardOption 
/// @return 
int char2num(char c, string standardLetters, string nonStandardLetters, NonStandardHandler nonStandardOption)
{
    int position;
    int standardLetterSize = standardLetters.size();

    position = standardLetters.find(c);

    if (position != string::npos) // standard
    {
        return position+1;
    }
    else if (nonStandardOption == AsStandard || nonStandardOption == ConsiderGapInCutoff) // behave like standard ones
    {
        position = nonStandardLetters.find(c);

        if (position != string::npos)
        {
            return position + standardLetterSize + 1; // non-standard
        }
    }
    return 0; // condier as gap
}

/// @brief remove gappy positions from sequences
/// @param sequences 
/// @param gapCutoff 
void removeGappyPositions(vector<vector<int>>& sequences, float gapCutoff)
{
    vector<int> querySequence = sequences[0];
    int length = querySequence.size();
    int depth = sequences.size();
    int gapCutoffNo = depth * gapCutoff;
    vector<int> removingPositions;
    int i, j;

    // find gappy positions
  	for(i = length-1; i >= 0; i--)
    {
  		int gapCount = 0;
  		for(j = 0; j < depth; j++)
        {
            if(sequences[j][i] == 0)
            {
                gapCount++;
            }
        }
        if(gapCount >= gapCutoffNo)
        {
  	        removingPositions.push_back(i);
  		}
    }
    //remove gappy positions from all sequences
    for (auto& sequence : sequences)
    {
        for (int pos : removingPositions)
        {
            sequence.erase(sequence.begin() + pos);
        }
    }
}

/// @brief map chars to digits and prepare sequences based values of omitGapsInQuery on gapCutoff
/// @param sequences 
/// @param omitGapsInQuery 
/// @param alphabet 
/// @param nonStandardOption 
/// @param gapCutoff 
/// @return 
vector<vector<int>> processSequences(FlagHandler flagHandler, vector<Sequence> sequences, bool omitGapsInQuery,
                                                    string standardLetters, string nonStandardLetters, NonStandardHandler nonStandardOption)
{
    vector<vector<int>> sequences2num;
    if(sequences.size() == 0)
    {
        return sequences2num;
    }

    string querySequence = sequences[0].sequence;

    if (omitGapsInQuery &&  querySequence.find('-') != string::npos) // if query sequence contains any gaps and they meant to be omitted
    { 
        keepNonGapPositionsOfQuerySequence(sequences);
        querySequence = sequences[0].sequence;
    }

    // map letters to numbers
    vector<int> sequence2num;
    int length = querySequence.length();

    for (auto sequence : sequences)
    {
        sequence2num = {};
        for (int position = 0; position < length; position++)
        {
            sequence2num.push_back(char2num(sequence.sequence[position], standardLetters,
                                            nonStandardLetters, nonStandardOption));
        }
        sequences2num.push_back(sequence2num);            
    }

    // gap_cutoff
    float gapCutoff = stof(flagHandler.getFlagValue("gap_cutoff"));
    if (gapCutoff <= 0 || gapCutoff > 1)
    {
        throw runtime_error("gap_cutoff should be grater than 0 and equal or less than 1");
    }

    if(gapCutoff < 1)
    {
        removeGappyPositions(sequences2num, gapCutoff);
    }

    return sequences2num;
}

/// @brief compute NEFF base on given options
/// @param sequences 
/// @param threshold 
/// @param norm 
/// @param isSymmetric 
/// @param nonStandardOption 
/// @return 
float computeNEFF(vector<vector<int>> sequences, float threshold, Normalization norm, bool isSymmetric,
                     string standardLetters, NonStandardHandler nonStandardOption)
{
    if(sequences.size() == 0)
    {
        cout << "There is no sequence to compute NEFF for." <<endl;
        return 0;
    }

    vector<int> querySequence = sequences[0];
    int length = querySequence.size();

    int position, i, j; // loop indexes    

    int non_gap_count;
    vector <bool> non_gap_seq; // keeps positions of residues in a sequence
    vector<vector<bool>> non_gap_msa; // keeps positions of non-gap residues in all sequences
    vector<int> cutoff; // kepps max num of mismatches for a sequence to be considered homolog
    non_gap_seq.assign(length, 0);

    // computeing cutoff for each sequence
    // and keeping non-gap positions in non_gap_msa for asymmetric option
    if (isSymmetric)
    {
        cutoff.push_back(length * (1-threshold));
    }
    else
    {
        for (auto sequence : sequences)
        {
            non_gap_count = 0;
            // finding non-gap positions of the sequence
            for (position = 0; position < length; position++)
            {                
                if ((nonStandardOption ==  ConsiderGapInCutoff && (sequence[position] == 0 || sequence[position] > standardLetters.size()))
                    || (nonStandardOption != ConsiderGap && sequence[position] == 0))
                {
                    non_gap_seq[position] = 0;
                }                
                else
                {
                    non_gap_count++;
                    non_gap_seq[position] = 1;
                }
            }
            non_gap_msa.push_back(non_gap_seq);
            cutoff.push_back(non_gap_count * (1-threshold));
        }
    }    
    non_gap_seq.clear();

    int msa_depth = sequences.size();
    int mismatch_i = 0, mismatch_j = 0; // # mismatches in i'th and j'th sequences
    float neff = 0;
    vector<int> sequence_weight(msa_depth, 1); // number of homolog sequences to each sequence

    // iterate through each pair of sequence and compute sequence weights
    for (i = 0; i < msa_depth; i++)
    {
        for (j = i+1; j < msa_depth; j++)
        {
            mismatch_i = mismatch_j = 0;
            for (position = 0; position < length; position++)
            {
                if (sequences[i][position] == sequences[j][position]) // position match
                {
                    continue;
                }
                if (isSymmetric) 
                {
                    // increment both sequences when there is a position mismatch                    
                    mismatch_i++;
                    mismatch_j++;
                    if (mismatch_i > cutoff[0])
                    {
                        // no need to iterate more when already found cutoff mismatches this pair 
                        break;
                    }
                }
                else //asymmetric
                {
                    mismatch_i += non_gap_msa[i][position]; // increment mismatches if this position is non-gap
                    mismatch_j += non_gap_msa[j][position];
                    
                    if ((mismatch_i > cutoff[i]) && (mismatch_j > cutoff[j]))
                    {
                        // no need to iterate more when found unsimilarity threshhold for this pair
                        break;
                    }
                }
            }
            if(isSymmetric)
            {
                sequence_weight[i] += (mismatch_i <= cutoff[0]);
                sequence_weight[j] += (mismatch_j <= cutoff[0]);
            }
            else
            {
                sequence_weight[i] += (mismatch_i <= cutoff[i]);
                sequence_weight[j] += (mismatch_j <= cutoff[j]);
            }
        }
        neff += 1./sequence_weight[i];
    }

    // normalizing Nf
    switch(norm)
    {
        case Sqrt_L:
            return neff/sqrt(length);
        case L:
            return neff/length;
        default:
            return neff;
    }
}

int getNonGapStartPosition(string firstAlignement, int startPos)
{
    if(startPos != 1)
    {
        int nonGapPosition = 0;
        int i;

        for (i = 0; i < firstAlignement.length(); i++)
        {
            if (firstAlignement[i] != '-') {
                nonGapPosition++;
                if (nonGapPosition == startPos)
                {                
                    break;
                }
            }
        }
        return i;
    }
    return 1;
}

//ToDo
int getNonGapEndPosition(string firstAlignement, int startPos, int endPos)
{
    int length = endPos - startPos +1;
    int nonGapPosition = 0;
    int i;

    for (i = startPos; i < firstAlignement.length(); i++)
    {
        if (firstAlignement[i] != '-') {
            nonGapPosition++;
            if (nonGapPosition == length)
            {                
                break;
            }
        }
    }
    return i;
}

/// @brief set desired depth and segment of sequence based on given flag
/// @param sequences 
/// @param flagHandler
void setDepthAndDegment(vector<Sequence>& sequences, FlagHandler flagHandler)
{
    float threshold, gapCutoff;
    int depth, startPos, endPos;
    std::istringstream iss;
    // set depth
    string sdepth = flagHandler.getFlagValue("depth");
    if(sdepth != "inf")
    {
        iss.str(sdepth);
        if (!(iss >> depth) || !iss.eof() || depth <= 0)
        {
            throw runtime_error("\'depth\' should be a positive integer.");
        }
        depth = min(depth, (int)sequences.size()); // if given value is greater than original depth, consider the original depth
        sequences.resize(depth);
    }
    else
    {
        depth = sequences.size();
    }

    string firstAlignment = sequences[0].sequence;
    int lengthOfFirstAlignment = firstAlignment.length();

    // pos_start
    string sstartPos= flagHandler.getFlagValue("pos_start");
    iss.clear();
    iss.str("");
    iss.str(sstartPos);
    if (!(iss >> startPos) || !iss.eof() || startPos <= 0 || startPos >= lengthOfFirstAlignment)
    {
        throw runtime_error("\'pos_start\' should be a positive integer, less than length of query sequnce.");
    }

    // pos_end
    string sendPos = flagHandler.getFlagValue("pos_end");

    if(sendPos != "inf")
    {
        iss.clear();
        iss.str("");
        iss.str(sendPos);
        if (!(iss >> endPos) || !iss.eof() || endPos <= 0 || endPos <= startPos)
        {
            throw runtime_error("\'pos_end\' should be a positive integer, greater than \'pos_start\'");
        }
        endPos = min(endPos, lengthOfFirstAlignment); // if given value is greater than length of query sequnce, it considers length
    }
    else
    {
        endPos = lengthOfFirstAlignment;
    }

    // check this range is in the range of query sequence length
    int coutOfGapPositions = count(firstAlignment.begin(), firstAlignment.end(), '-');
    int lengthOfQuerySeq = lengthOfFirstAlignment - coutOfGapPositions;
    if(endPos - startPos > lengthOfQuerySeq)
    {
        throw runtime_error("\'pos_start\' and \'pos_end\' should be in the range of query sequence length");
    }

    if(startPos != 1 || endPos != lengthOfQuerySeq)
    {
        int nonGapStartPos = startPos;
        int nonGapEndPos = endPos;

        // set non-gap start position and end position if there is any gaps in the first alignment        
        if (coutOfGapPositions > 0)
        {
            nonGapStartPos = getNonGapStartPosition(firstAlignment, startPos);
            if(endPos != lengthOfQuerySeq)
            {
                nonGapEndPos = getNonGapEndPosition(firstAlignment, nonGapStartPos, endPos);         
            }
        }

        // Extract the substrings based on nonGap positions of start and end  AND update sequences, accordingly
        for (auto& sequence : sequences)
        {
            sequence.sequence = sequence.sequence.substr(nonGapStartPos -1 , nonGapEndPos - nonGapStartPos + 1);
        }
    }
}

int main(int argc, char **argv)
{
    /* Handling flags */
    vector<string> args(argv + 1, argv + argc);
    FlagHandler flagHandler(Flags);

    try
    { 
        flagHandler.processFlags(args);
        flagHandler.checkRequiredFlags();
    
        // file
        string file = flagHandler.getFlagValue("file");
        string format = getFormat(file, "file");

        // alphabet
        Alphabet alphabet = static_cast<Alphabet>(stoi(flagHandler.getFlagValue("alphabet")));

        // check_validation
        bool checkValidation = flagHandler.getFlagValue("check_validation") == "true";

        // omit_query_gaps
        bool omitGapsInQuery = flagHandler.getFlagValue("omit_query_gaps") == "true";

        MSAReader* msa_reader;
        if (format == "a2m")
            msa_reader = new MSAReader_a2m(file, alphabet, checkValidation, omitGapsInQuery);
        else if(format == "a3m")
            msa_reader = new MSAReader_a3m(file, alphabet, checkValidation, omitGapsInQuery);
        else if(format == "fasta" || format == "afa")
            msa_reader = new MSAReader_fasta(file, alphabet, checkValidation, omitGapsInQuery);
        else if(format == "sto")
            msa_reader = new MSAReader_sto(file, alphabet, checkValidation, omitGapsInQuery);
        else if(format == "clustal")
            msa_reader = new MSAReader_clustal(file, alphabet, checkValidation, omitGapsInQuery);
        else if (format == "aln")
            msa_reader = new MSAReader_aln(file, alphabet, checkValidation, omitGapsInQuery);
        else if (format == "pfam")
            msa_reader = new MSAReader_pfam(file, alphabet, checkValidation, omitGapsInQuery);
        else
            throw runtime_error("Not supported MSA file format (" + format + ")");

        vector<Sequence> sequences = msa_reader->read();

        setDepthAndDegment(sequences, flagHandler);

        // non_standard_option
        NonStandardHandler nonStandardOption = static_cast<NonStandardHandler>(stoi(flagHandler.getFlagValue("non_standard_option")));

        string standardLetters= getStandardLetters(alphabet);
        string nonStandardLetters= getNonStandardLetters(alphabet);

        vector<vector<int>> sequences2num = processSequences
                                            (flagHandler, sequences, omitGapsInQuery, standardLetters, nonStandardLetters, nonStandardOption);

        // norm
        Normalization norm = static_cast<Normalization>(stoi(flagHandler.getFlagValue("norm")));

        // threshold
        float threshold = stof(flagHandler.getFlagValue("threshold"));
        if (threshold < 0 || threshold > 1)
        {
            throw runtime_error("Threshold should be between 0 and 1");
        }
        
        // is_symmetric
        bool isSymmetric = flagHandler.getFlagValue("is_symmetric") == "true";

        cout << sequences2num[0].size() << endl; //length

        cout << sequences2num.size() << endl; //msa depth

        cout << computeNEFF(sequences2num,  threshold, norm, isSymmetric, standardLetters, nonStandardOption) << endl;
        
        return 0;
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}
