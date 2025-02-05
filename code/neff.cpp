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
 *   --file=<input_file>               Input files (comma-separated, no spaces) containing multiple sequence alignments (required)\n"
 *   --format=<input_format>           Input file formats (comma-separated, no spaces) containing formats of multiple sequence alignments (optional)\n"
 *   --alphabet=<value>                Valid alphabet of MSA; alphabet option (0: Protein, 1: RNA, 2: DNA) (default: 0)\n"
 *   --check_validation=<true/false>   Perform validation on sequences (default: false)\n"
 *   --threshold=<value>               Threshold value of considering two sequences similar (default: 0.8)\n"
 *   --norm=<value>                    NEFF normalization option (0: sqrt(Length of alignment), 1: Length of alignment, 2: No normalization) (default: 0)\n"
 *   --omit_query_gaps=<true/false>    Omit gap positions of query sequence from all sequences for NEFF computation (default: true)\n"
 *   --is_symmetric=<true/false>       Consider gaps in similarity cutoff computation (asymmetric) or not (symmetric) (default: true)\n"
 *   --non_standard_option=<value>     Handling non-standard letters in the given alphabet (0: AsStandard, 1: ConsiderGapInCutoff, 2: ConsiderGap)\n"
 *   --depth=<value>                   Depth of MSA to be cosidered in computation (default: depth of given MSA)\n"
 *   --gap_cutoff=<value>              Cutoff value for removing gappy positions, when #gaps in position >= gap_cutoff (default=1 : does not remove anything)\n"
 *   --pos_start=<value>               Start position of each sequence to be considered in neff (inclusive (default: 1))\n"
 *   --pos_end=<value>                 Last position of each sequence to be considered in neff (inclusive (default: length of MSA sequence))\n"
 *   --only_weights=<true/false>       Return only sequence weights, as # similar sequence, rather than the final NEFF (default: false)\n"
 *   --multimer_MSA=<true/false>       Compute NEFF for a multimer MSA (default: false)\n"
 *   --stoichiom=<value>               Multimer stoichiometry (default: empty)
 *   --chain_length=<list of values>   Length of the chains in heteromer multimer (default: 0)\n"
 *   --residue_neff=<true/false>       Compute per-resiue (column-wise) NEFF (default: false)
 *
 * For more comprehensive instructions, please refer to the documentation at https://maryam-haghani.github.io/NEFFy.
 */

//const char* docstr = R"(
//This program computes the Number of Effective Sequences (NEFF) for a multiple sequence alignment (MSA) file.
//NEFF is a measure of the effective sequence number that accounts for the redundancy and similarity of sequences in the MSA.
//
//Usage:
//./neff --file=<input_file> [options]
//
//For detailed instructions, please refer to the documentation at https://maryam-haghani.github.io/NEFFy.
//)";

const char* docstr = R"(
NEFF - Number of Effective Sequences Computation Tool

Description:
  This program computes the Number of Effective Sequences (NEFF) for a given multiple sequence alignment (MSA).
  NEFF is used to assess the diversity of sequences by accounting for redundancy and sequence similarity.

Usage:
  ./neff --file=<input_file> [options]

Options:
  --file=<input_file>
      Input file(s) containing the MSA(s). Multiple files can be specified as a comma-separated list (without spaces).
      (Required)

  --format=<input_format>
      Format(s) of the input file(s) (comma-separated, no spaces).
      (Optional)

  --alphabet=<value>
      Specifies the valid alphabet for the sequences:
        0 : Protein (default)
        1 : RNA
        2 : DNA

  --check_validation=<true/false>
      Whether to validate sequences to include only letters from the chosen alphabet.
      (Default: false)

  --threshold=<value>
      Similarity threshold (Θ) used to decide if two sequences are considered similar.
      (Default: 0.8)

  --norm=<value>
      NEFF normalization option:
        0 : Normalize by sqrt(Length of alignment) (default)
        1 : Normalize by Length of alignment
        2 : No normalization

  --omit_query_gaps=<true/false>
      If true, gap positions in the query (first) sequence are omitted from all sequences.
      (Default: true)

  --is_symmetric=<true/false>
      Determines whether gap positions are considered in similarity cutoff computation (symmetric) or not (asymmetric).
      (Default: true, meaning symmetric)

  --non_standard_option=<value>
      Determines how non-standard letters are handled:
        0 : AsStandard (default)
        1 : ConsiderGapInCutoff
        2 : ConsiderGap

  --depth=<value>
      Limits the number of sequences (MSA depth) considered in the computation.
      (Default: use all sequences in the input file)

  --gap_cutoff=<value>
      Removes alignment positions where the fraction of gaps is greater than or equal to this value.
      (Default: 1, meaning no positions are removed)

  --pos_start=<value>
      Start position (inclusive) in the alignment for NEFF computation.
      (Default: 1, meaning start of the sequences)

  --pos_end=<value>
      End position (inclusive) in the alignment for NEFF computation.
      (Default: length of the MSA sequence, meaning end of the sequences)

  --only_weights=<true/false>
      If true, the program returns only the sequence weights instead of the final NEFF value.
      (Default: false)

  --multimer_MSA=<true/false>
      If true, computes NEFF for a multimeric MSA.
      (Default: false)

  --stoichiom=<value>
      Specifies the stoichiometry for a multimeric MSA. This parameter is required when --multimer_MSA=true.

  --chain_length=<list of values>
      For heteromer multimer MSAs, provides the lengths of the individual chains.
      This parameter is required when --multimer_MSA=true and the --stoichiom value consists of multiple letters (indicating a heteromer).
      (Default: 0)

  --residue_neff=<true/false>
      If true, computes per-residue (column-wise) NEFF.
      (Default: false)

Examples:
  Compute the NEFF for a protein MSA:
    ./neff --file=msa.a3m --alphabet=0

  Compute NEFF with custom threshold and no normalization:
    ./neff --file=msa.a3m --threshold=0.7 --norm=2

  Compute per-residue NEFF:
    ./neff --file=msa.a3m --residue_neff=true

  For more comprehensive instructions, please refer to the documentation at https://maryam-haghani.github.io/NEFFy.
)";

#include "flagHandler.h"
#include "msaReader.h"
#include "msaWriter.h"
#include "multimerHandler.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <climits>
#include "common.h"
#include <cmath>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <random>
#include <fstream>
#include <set>
#include <thread>
#include <future>
#include <mutex>
#include <chrono>
#include <iostream>

using namespace std;

unordered_map<string, FlagInfo> Flags =
{
    {"file", {true, ""}},                   // Input files (comma-separated, no spaces) containing multiple sequence alignments
    {"format", {false, ""}},                // Input file formats (comma-separated, no spaces) containing formats of multiple sequence alignments
    {"alphabet", {false, "0"}},             // Alphabet of MSA
    {"check_validation", {false, "false"}}, // Perform validation on sequences to include only alphabet letters
    {"threshold", {false, "0.8"}},          // Threshold value for sequence similarity
    {"norm", {false, "0"}},                 // NEFF normalization options
    {"omit_query_gaps", {false, "true"}},   // Omit gap positions of query sequence from all sequences
    {"is_symmetric", {false, "true"}},      // Consider gaps in similarity cutoff computation (asymmetric) or not (symmetric)
    {"non_standard_option", {false, "0"}},  // Handling non-standard letters in the given alphabet of MSA
    {"depth", {false, "inf"}},              // Depth of MSA to be cosidered in computation
    {"gap_cutoff", {false, "1"}},           // Cutoff value for considering a position as gappy and removing that
    {"pos_start", {false, "1"}},            // Start position of each sequence to be considered in NEFF (inclusive)
    {"pos_end", {false, "inf"}},            // Last position of each sequence to be considered in NEFF (inclusive)
    {"only_weights", {false, "false"}},     // Return sequence weights instead of final NEFF
    {"multimer_MSA", {false, "false"}},     // Compute NEFF for a multimer MSA
    {"stoichiom", {false, ""}},             // Multimer stoichiometry
    {"chain_length", {false, "0"}},         // Length of the chains in heteromer multimer
    {"residue_neff", {false, "false"}}      // Compute per-resiue (column-wise) NEFF
};

/// @brief Map char residues to digit based on given 'nonStandardOption'
/// @param c input letter
/// @param standardLetters 
/// @param nonStandardLetters 
/// @param nonStandardOption 
/// @return 
int char2num(char c, const string& standardLetters, const string& nonStandardLetters, NonStandardHandler nonStandardOption)
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

/// @brief Remove gappy positions from sequences based on given 'gapCutoff'
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

/// @brief Map chars to digits based on provided 'nonStandardOption' and also remove gappy positions based on given 'gapCutoff'
/// @param omitGapsInQuery 
/// @param alphabet 
/// @param nonStandardOption 
/// @param gapCutoff 
/// @return 
vector<vector<int>> processSequences(vector<Sequence> sequences, string standardLetters,
                                    string nonStandardLetters, NonStandardHandler nonStandardOption, float gapCutoff)
{
    vector<vector<int>> sequences2num;
    if(sequences.size() == 0)
    {
        return sequences2num;
    }

    string querySequence = sequences[0].sequence;


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

    if(gapCutoff < 1)
    {
        removeGappyPositions(sequences2num, gapCutoff);
    }

    return sequences2num;
}

/// @brief Compute sequence weights based on given options
/// @param sequences 
/// @param threshold 
/// @param norm 
/// @param isSymmetric 
/// @param nonStandardOption 
/// @return inverse of sequence weights
vector<int> computeWeights(vector<vector<int>> sequences, float threshold, bool isSymmetric,
                     string standardLetters, NonStandardHandler nonStandardOption)
{
    int msa_depth = sequences.size();

    if(msa_depth == 0)
    {
        cerr << "There is no sequence to compute weights for." << endl;
        exit(0);
    }

    vector<int> querySequence = sequences[0];
    int length = querySequence.size();

    int position, i, j; // loop indexes    
    int non_gap_count;
    vector <bool> non_gap_seq; // keeps positions of residues in a sequence
    vector<vector<bool>> non_gap_msa; // keeps positions of non-gap residues in all sequences
    vector<int> cutoff; // keeps max num of mismatches for a sequence to be considered homolog
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

    int mismatch_i = 0, mismatch_j = 0; // # mismatches in i'th and j'th sequences
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
    }

    return sequence_weight;
}

/// @brief Cumpote NEFF values based on sequence weights and given normalization
/// @param sequenceWeights 
/// @param norm 
/// @param sequenceLength 
/// @return 
float computeNeff(vector<int> sequenceWeights, Normalization norm, int length)
{
    float neff = 0;
    for (int i=0; i < sequenceWeights.size(); i++)
    {
        neff += 1./sequenceWeights[i];
    }
    
    switch(norm) // normalizing Nf
    {
        case Sqrt_L:
            neff = neff/sqrt(length);
            break;
        case L:
            neff = neff/length;
            break;
        default:
            break;
    }
    return neff;
}

/// @brief Get given alphabet by user
/// @param flagHandler 
/// @return 
Alphabet getAlphabet(FlagHandler& flagHandler)
{
    int intValue = flagHandler.getIntValue("alphabet");
    Alphabet alphabet = static_cast<Alphabet>(intValue);

    // value is within the valid range of the enum
    if (alphabet < Alphabet::protein || alphabet > Alphabet::DNA) {
        throw runtime_error("Invalid 'alphabet' value. It is outside the valid enum range.");
    }

    return alphabet;
}


/// @brief Get given normalization option by user
/// @param flagHandler 
/// @return 
Normalization getNormalization(FlagHandler& flagHandler)
{
    int intValue = flagHandler.getIntValue("norm");
    Normalization norm = static_cast<Normalization>(intValue);

    // value is within the valid range of the enum
    if (norm < Normalization::Sqrt_L || norm > Normalization::None) {
        throw runtime_error("Invalid 'norm' value. It is outside the valid enum range.");
    }
    return norm;
}

/// @brief Get given non_standard_option option by user
/// @param flagHandler 
/// @return 
NonStandardHandler getNonStandardOption(FlagHandler& flagHandler)
{
    int intValue = flagHandler.getIntValue("non_standard_option");
    NonStandardHandler nonStandardOption = static_cast<NonStandardHandler>(intValue);

    if (nonStandardOption < NonStandardHandler::AsStandard || nonStandardOption > NonStandardHandler::ConsiderGap) {
        throw runtime_error("Invalid 'non_standard_option' value. It is outside the valid enum range.");
    }
    return nonStandardOption;
}

/// @brief Check flags     
/// @param flagHandler 
void checkFlags(FlagHandler& flagHandler)
{
    // Only one of only_weights, multimer_MSA, or residue_neff can be true at a time.
    int trueCount = 0;
    if (flagHandler.getBooleanValue("only_weights")) trueCount++;
    if (flagHandler.getBooleanValue("multimer_MSA")) trueCount++;
    if (flagHandler.getBooleanValue("residue_neff")) trueCount++;
    if (trueCount > 1)
    {
        throw runtime_error(
            "Only one of 'only_weights', 'residue_neff', or 'multimer_MSA' can be true at a time.");
    }
    if (flagHandler.getBooleanValue("multimer_MSA"))
    {
        string stoichiom = flagHandler.getFlagValue("stoichiom");

        if(stoichiom == "")
        {
            throw runtime_error("When 'multimer_MSA'=true, 'stoichiom' should have a value.");
        }
        MultimerHandler handler(stoichiom);
        bool isHomomer = handler.isHomomerFormat();

        if (!isHomomer)
        {
            bool isHeteromer = handler.isHeteromerFormat();

            if (!isHeteromer)
            {
                throw runtime_error
                ("Stoichiometry: " + stoichiom +  " is not in the correct format 'An' for homomers and 'AnBm...' for heteromers");
            }
            
            vector<int> chainLength;
            try
            {
                chainLength = flagHandler.getIntArrayValue("chain_length");
            }
            catch (const exception& e)
            {
                throw runtime_error
                ("When multimer is heteromer, 'chain_length' should be a list of positive numbers");
            }
        }
        
        if (!((flagHandler.getBooleanValue("omit_query_gaps"))
        && (flagHandler.getFlagValue("gap_cutoff") == "1")
        && (flagHandler.getFlagValue("pos_start") == "1")
        && (flagHandler.getFlagValue("pos_end") == "inf")))
        {
            throw runtime_error
            ("When 'multimer_MSA'=true, 'omit_query_gaps', 'pos_start', and 'pos_end' should remain at their default parameters.");
        }
    }
}

/// @brief Set MSA depth based on 'depth' flag
/// @param sequences 
/// @param flagHandler 
void setDepth(vector<Sequence>& sequences,int depth)
{
    // consider the original depth if the given value is greater than the original depth
    depth = min(depth, (int)sequences.size());
    sequences.resize(depth);   
}

/// @brief Get the index of the given startPos in the original first sequence of MSA despite of gaps in the MSA
/// @param firstAlignement 
/// @param startPos 
/// @return startPos + number of gaps until that position for the first sequence in the MSA
int getNonGapStartPosition(string firstAlignement, int startPos)
{
    if(startPos != 0){
        int nonGapPosition = 0;
        int i;
        for (i = 0; i < firstAlignement.length(); i++)
        {
            if (firstAlignement[i] != '-') {
                nonGapPosition++;
                if (nonGapPosition == startPos) break;
            }
        }
        return i;
    }
    return 0;
}

/// @brief Get the end index in the original sequence after a 'length' number of non-gap positions.
/// @param firstAlignement 
/// @param startPos 
/// @param length 
/// @return The end index in the MSA sequence including gaps.
int getNonGapEndPosition(string firstAlignement, int startPos, int length)
{
    int nonGapPosition = 0;
    int i;

    for (i = startPos; i < firstAlignement.length(); i++)
    {
        if (firstAlignement[i] != '-') {
            nonGapPosition++;
            if (nonGapPosition == length)   break;
        }
    }
    return i;
}

/// @brief Set desired positiones to compute NEFF for based on given 'pos_start' and 'pos_end' flags
/// @param sequences 
/// @param flagHandler 
void getPositions(vector<Sequence>& sequences, FlagHandler flagHandler)
{
    int startPos;
    int endPos;

    string firstAlignment = sequences[0].sequence;
    int lengthOfFirstAlignment = firstAlignment.length();

    int coutOfGapPositions = count(firstAlignment.begin(), firstAlignment.end(), '-');
    int lengthOfQuerySeq = lengthOfFirstAlignment - coutOfGapPositions;

    // pos_start
    startPos = flagHandler.getNonZeroIntValue("pos_start");

    if (startPos >= lengthOfFirstAlignment)
    {
        throw runtime_error("'pos_start' should be less than the length of query sequence (" + to_string(lengthOfFirstAlignment) + ").");
    }

    // pos_end
    endPos = flagHandler.getNonZeroIntValue("pos_end");

    if (endPos <= startPos)
    {
        throw runtime_error("'pos_end' should be greater than 'pos_start'");
    }
    // condider the last position if given value is greater than length of query sequence
    endPos = min(endPos, lengthOfFirstAlignment);
    
    // is start and end positions are different from start and end positions of the quesry sequence
    if(startPos != 1 || endPos != lengthOfQuerySeq)
    {
        int nonGapStartPos = startPos-1;
        int nonGapEndPos = endPos-1;

        // set non-gap start position and end position if there is any gaps in the first alignment        
        if (coutOfGapPositions > 0)
        {
            nonGapStartPos = getNonGapStartPosition(firstAlignment, startPos);
            if(endPos != lengthOfQuerySeq)
            {
                nonGapEndPos = getNonGapEndPosition(firstAlignment, nonGapStartPos, endPos-startPos+1);         
            }
        }
        // extract the substrings based on nonGap positions of start and end  AND update sequences, accordingly
        for (auto& sequence : sequences)
        {
            sequence.sequence = sequence.sequence.substr(nonGapStartPos , nonGapEndPos - nonGapStartPos + 1);
        }
    }
}

/// @brief to merge sequences and remove redundant sequences
/// @param integratedSequences 
/// @param sequences 
void integrateUniqueSequences(vector<Sequence>& integratedSequences, const vector<Sequence>& sequences) {

    //length of sequences in all provided files should be the same    
    if(integratedSequences.size() != 0 && sequences[0].sequence.size() != integratedSequences[0].sequence.size())
    {
        throw runtime_error("Length of sequences in provided files are not the same.");
    }
    
    // Iterate over the new sequences
    for (const auto& seq : sequences) {
        // Check if the sequence is not already in integratedSequences
        auto it = std::find_if(integratedSequences.begin(), integratedSequences.end(), 
                               [&](const Sequence& integratedSeq) {
                                   return integratedSeq.sequence == seq.sequence;
                               });
        // If the sequence is not found, add it to integratedSequences
        if (it == integratedSequences.end()) {
            integratedSequences.push_back(seq);
        }
    }
}

/// @brief Compute per-residue (column-wise) NEFF
/// @param sequences 
/// @param sequenceWeights
/// @param norm 
/// @return 
vector<float> computeResidueNEFF
(const vector<vector<int>>& sequences, const vector<int>& sequenceWeights, Normalization norm) {
    int numSequences = sequences.size();
    if (numSequences == 0) {
        return {};
    }
    int sequenceLength = sequences[0].size();
    
    vector<float> residueNEFF(sequenceLength, 0.0);
    
    for (int col = 0; col < sequenceLength; ++col) {
        float sumWeights = 0.0;
        for (int row = 0; row < numSequences; ++row) {
            // include sequence weight of the current seqeunce in the residue NEFF, if residue is not corresponding to a gap position
            if (sequences[row][col] != 0)
            {
                sumWeights += 1./ sequenceWeights[row];
            }
        }

        switch(norm) // normalizing Nf
        {
            case Sqrt_L:
                residueNEFF[col] = sumWeights/sqrt(sequenceLength);
                break;
            case L:
                residueNEFF[col] = sumWeights/sequenceLength;
                break;
            default:
                residueNEFF[col] = sumWeights;
                break;
        }
    }
    
    return residueNEFF;
}


int main(int argc, char **argv)
{
    /* Handling flags */
    vector<string> args(argv + 1, argv + argc);
    FlagHandler flagHandler(Flags);

    // Check for help flag
    for (const auto& arg : args) {
        if (arg == "-h" || arg == "--help") {
            cout << docstr << endl;
            return 0;
        }
    }

    vector<string> files, formats;
    string file;
    float threshold, gapCutoff;
    bool checkValidation, omitGapsInQuery, isSymmetric;
    int depth;
    Alphabet alphabet;
    NonStandardHandler nonStandardOption;
    Normalization norm;
    string standardLetters, nonStandardLetters;
    vector<Sequence> sequences, integratedSequences;
    vector<vector<int>> sequences2num;
    vector<int> sequenceWeights;

    try
    { 
        flagHandler.processFlags(args);
        flagHandler.checkRequiredFlags();

        checkFlags(flagHandler);

        // file
        files = flagHandler.getFileArrayValue("file");

        //formats
        formats = flagHandler.getArrayValues("format");

        if (!formats.empty() && formats.size() != files.size())
        {
            throw runtime_error("'format' must either be empty or have the same number of elements as 'file'.");
        }
                
        // alphabet
        alphabet = getAlphabet(flagHandler);

        // check_validation
        checkValidation = flagHandler.getBooleanValue("check_validation");

        // omit_query_gaps
        omitGapsInQuery = flagHandler.getBooleanValue("omit_query_gaps");

        //depth    
        depth = flagHandler.getNonZeroIntValue("depth");

        MSAReader* msaReader;

        for(int f=0; f<files.size(); f++)
        {
            file = files[f];

            string format = getFormat(file, !formats.empty()? formats[f] : "", "file");

            if (format == "a2m")
                msaReader = new MSAReader_a2m(file, alphabet, checkValidation, omitGapsInQuery);
            else if(format == "a3m")
                msaReader = new MSAReader_a3m(file, alphabet, checkValidation, omitGapsInQuery);
            else if(format == "sto")
                msaReader = new MSAReader_sto(file, alphabet, checkValidation, omitGapsInQuery);
            else if(format == "clustal")
                msaReader = new MSAReader_clustal(file, alphabet, checkValidation, omitGapsInQuery);
            else if (format == "aln")
                msaReader = new MSAReader_aln(file, alphabet, checkValidation, omitGapsInQuery);
            else if (format == "pfam")
                msaReader = new MSAReader_pfam(file, alphabet, checkValidation, omitGapsInQuery);
            else if (find(FASTA_FORMATS.begin(), FASTA_FORMATS.end(), format) != FASTA_FORMATS.end())
                msaReader = new MSAReader_fasta(file, alphabet, checkValidation, omitGapsInQuery);

            sequences = msaReader->read();

            if(sequences.size() == 0)
            {
                continue;
            }

            /// omit gap positions of query sequence in all sequences if omitGapsInQuery=true
            if (omitGapsInQuery &&  sequences[0].sequence.find('-') != string::npos)
            // if query sequence contains any gaps and they meant to be omitted
            { 
                keepNonGapPositionsOfQuerySequence(sequences);
            }

            
            if (files.size() == 1)
            {
                integratedSequences = sequences;
                break;
            }
            
            // integrate unique sequences from files when more than one file exists
            integrateUniqueSequences(integratedSequences, sequences);         

            // no need to integrate if the depth of sequences so far is more than the given depth
            if (depth < integratedSequences.size())
            {
                break;
            }
        }

        sequences = integratedSequences;

        setDepth(sequences, depth);

        getPositions(sequences, flagHandler);

        // non_standard_option
        nonStandardOption = getNonStandardOption(flagHandler);

        standardLetters = getStandardLetters(alphabet);
        nonStandardLetters = getNonStandardLetters(alphabet);

        // gap_cutoff
        gapCutoff = flagHandler.getFloatValue("gap_cutoff");

        sequences2num = processSequences(sequences, standardLetters, nonStandardLetters, nonStandardOption, gapCutoff);

        // norm
        norm = getNormalization(flagHandler);

        // threshold
        threshold = flagHandler.getFloatValue("threshold");
        
        // is_symmetric
        isSymmetric = flagHandler.getBooleanValue("is_symmetric");

        int length = sequences2num[0].size();

        cout << "MSA sequence length: "<< length << endl;
        cout << "MSA depth:" << sequences2num.size() << endl;

        float neff = 0.0;

        if (flagHandler.getBooleanValue("multimer_MSA"))
        {
            MultimerHandler multimerHandler(flagHandler.getFlagValue("stoichiom"));

            if(multimerHandler.isHomomerFormat())
            {
                // Entire MSA
                sequenceWeights = computeWeights(sequences2num, threshold, isSymmetric, standardLetters, nonStandardOption);
                neff = computeNeff(sequenceWeights, norm, sequences2num[0].size());
                cout << "NEFF of entire MSA:" << neff << endl;

                // Individual MSA
                vector<vector<int>> individualMSA = multimerHandler.getHomomerIndividualMSA(sequences2num);
                sequenceWeights = computeWeights(individualMSA, threshold, isSymmetric, standardLetters, nonStandardOption);
                neff = computeNeff(sequenceWeights, norm, individualMSA[0].size());
                cout << "NEFF of Individual MSA: " << neff << endl;
            }
            else //heteromer format
            {
                bool isHeteromer = multimerHandler.isHeteromerFormat();
                vector<int> chainLengths = flagHandler.getIntArrayValue("chain_length");

                vector<vector<vector<int>>> msas = multimerHandler.getHetoromerMSAs(sequences2num, chainLengths);

                // Entire MSA
                sequenceWeights = computeWeights(sequences2num, threshold, isSymmetric, standardLetters, nonStandardOption);
                neff = computeNeff(sequenceWeights, norm, sequences2num[0].size());
                cout << "NEFF of entire MSA:" << neff << endl;

                // Paired MSA
                sequenceWeights = computeWeights(msas[0], threshold, isSymmetric, standardLetters, nonStandardOption);
                neff = computeNeff(sequenceWeights, norm, msas[0][0].size());
                cout << "NEFF of Paired MSA (depth=" << msas[0].size() << "): " << neff << endl;
                            

                // Individual MSAs
                for (int i=1; i< msas.size(); i++)
                {
                    string chain = multimerHandler.chainIndexToStoichiomLetter(i-1);
                    if(msas[i].size() == 0)
                    {
                        cout << "Chain " + chain + " does not have an individual MSA, skipping NEFF calculation..." << endl;
                    }
                    else
                    {
                        sequenceWeights = computeWeights(msas[i], threshold, isSymmetric, standardLetters, nonStandardOption);
                        neff = computeNeff(sequenceWeights, norm, msas[i][0].size());
                        cout << "NEFF of Individual MSA for Chain " << chain << " (depth=" << msas[i].size()-1 << "): " << neff << endl;
                    }
                }
            }
            return 0;
        }

        sequenceWeights = computeWeights(sequences2num, threshold, isSymmetric, standardLetters, nonStandardOption);

        if(flagHandler.getBooleanValue("only_weights"))
        {
            cout << "Sequence weights:" << endl;
            for (int i=0; i < sequenceWeights.size(); i++)
            {
                cout << 1./sequenceWeights[i] << ' ';
            }
            cout << endl << flush;
            return 0;
        }

        if(flagHandler.getBooleanValue("residue_neff"))
        {
            vector<float> residueNEFF = computeResidueNEFF(sequences2num, sequenceWeights, norm);
            cout << "Per-residue (column-wise) NEFF:" << endl;
            for (int col=0; col < residueNEFF.size(); col++)
            {
                cout << residueNEFF[col] << ' ';
            }
            // Compute the median of residueNEFF
            sort(residueNEFF.begin(), residueNEFF.end());
            float median = residueNEFF.size() % 2 == 0 
                    ? (residueNEFF[residueNEFF.size()/2 - 1] + residueNEFF[residueNEFF.size()/2]) / 2.0 
                    : residueNEFF[residueNEFF.size()/2];

            cout << "\nMedian of per-residue (column-wise) NEFF: " << median <<endl << flush;
            return 0;
        }

        neff = computeNeff(sequenceWeights, norm, length);
        cout << "NEFF: " << neff << endl;
        
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
