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
 *   --file=<input_file>               Path to the input MSA file (required)\n"
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
 *   --mask_enabled=<true/false>       Enable random sequence masking for NEFF calculation (default: false)\n"
 *   --mask_frac=<value>               Fraction of sequences to be masked in each masking iteration (default: 0)\n"
 *   --mask_count=<value>              Frequency of masking (default: 0)\n"
 *   --multimer_MSA=<true/false>       Compute NEFF for both paired MSA and individual monomer MSAs when MSA is in the form of multimer MSA and composed of 3 parts (default: false)\n"
 *   --monomer_length=<list of values> Length of the monomers, used to obtain NEFF for paired MSA and individual monomer MSAs (default: 0)\n"
 *   --column_neff=<true/false>        Compute Column-wise NEFF (default: false)

 * In symmetric version, threshold for considering a pair of sequence as homolog simply depends on length of alignment, and it would be equal for all sequences and we see a symmetry in similarities
 * While in asymmetric version, threshold depends on number of non-gap residues; therefore, cutoff tends to be different for sequences)
 *
 * NEFF is computed based on the given options.
 */

const char* docstr = R"(

./neff --file=<input_file> [options]

This program computes the Number of Effective Sequences (NEFF) for a multiple sequence alignment (MSA) file.
NEFF is a measure of the effective sequence number that accounts for the redundancy and similarity of sequences in the MSA.

Options:
    --file=<input_file>               Path to the input MSA file (required)
    --alphabet=<value>                Valid alphabet of MSA; alphabet option (0: Protein, 1: RNA, 2: DNA) (default: 0)
    --check_validation=<true/false>   Perform validation on sequences (default: false)
    --threshold=<value>               Threshold value of considering two sequences similar (default: 0.8)
    --norm=<value>                    NEFF normalization option (0: sqrt(Length of alignment), 1: Length of alignment, 2: No normalization) (default: 0)
    --omit_query_gaps=<true/false>    Omit gap positions of query sequence from all sequences for NEFF computation (default: true)
    --is_symmetric=<true/false>       Consider gaps in similarity cutoff computation (asymmetric) or not (symmetric) (default: true)
    --non_standard_option=<value>     Handling non-standard letters in the given alphabet (0: AsStandard, 1: ConsiderGapInCutoff, 2: ConsiderGap)
    --depth=<value>                   Depth of MSA to be cosidered in computation (default: depth of given MSA)
    --gap_cutoff=<value>              Cutoff value for removing gappy positions, when #gaps in position >= gap_cutoff (default=1 : does not remove anything)
    --pos_start=<value>               Start position of each sequence to be considered in neff (inclusive (default: 1))
    --pos_end=<value>                 Last position of each sequence to be considered in neff (inclusive (default: length of sequence in the MSA))
    --only_weights=<true/false>       Return only sequence weights, as # similar sequence, rather than the final NEFF (default: false)
    --mask_enabled=<true/false>       Enable random sequence masking for NEFF calculation (default: false)
    --mask_frac=<value>               Fraction of sequences to be masked in each masking iteration (default: 0)
    --mask_count=<value>              Frequency of masking (default: 0)
    --multimer_MSA=<true/false>       Compute NEFF for both paired MSA and individual monomer MSAs when MSA is in the form of multimer MSA and composed of 3 parts (default: false)
    --monomer_length=<list of values> Length of the monomers, used to obtain NEFF for paired MSA and individual monomer MSAs (default: 0)
    --column_neff=<true/false>        Compute Column-wise NEFF (default: false)

 Example:
    * Compute NEFF for protein MSA:
        ./neff --file=example.a3m --threshold=0.7 --norm=2
)";

#include "flagHandler.h"
#include "msaReader.h"
#include "msaWriter.h"
#include "msaSplitter.h"
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

using namespace std;

unordered_map<string, FlagInfo> Flags =
{
    {"file", {true, ""}},                   // MSA file path
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
    {"mask_enabled", {false, "false"}},     // Enable random  sequence masking for NEFF calculation
    {"mask_frac", {false, "0"}},            // Fraction of sequences to be masked in each masking iteration
    {"mask_count", {false, "0"}},           // Frequency of masking
    {"multimer_MSA", {false, "false"}},     // Compute NEFF for both paired MSA and individual monomer MSAs when MSA is in the form of multimer MSA and composed of 3 parts
    {"monomer_length", {false, "0"}},       // Length of the monomers, used to obtain NEFF for paired MSA and individual monomer MSAs.
    {"column_neff", {false, "false"}}       // Compute Column-wise NEFF
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
    vector<int> querySequence = sequences[0];
    int length = querySequence.size();

    if(msa_depth == 0 || length ==0)
    {
        cerr << "There is no sequence to compute weights for." << endl;
        exit(0);
    }

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

/// @brief Randomly mask sequences in the MSA (except the first sequence)
/// @param sequences 
/// @param maskFrac 
/// @return unmasked sequences, masked indices
tuple<vector<vector<int>>, set<int>> maskSequences(vector<vector<int>>& sequences, double maskFrac) {
    int totalSequences = sequences.size();
    int numToMask = static_cast<int>(maskFrac * totalSequences);

    // Create a vector of indices, starting from 1 to exclude the first sequence
    vector<int> indices(totalSequences - 1);
    // for (int i = 1; i < totalSequences; ++i) {
    //     indices[i - 1] = i;
    // }
    iota(indices.begin(), indices.end(), 1);

    // Randomly shuffle the indices
    random_device rd;
    mt19937 gen(rd());
    shuffle(indices.begin(), indices.end(), gen);

    // Select the first numToMask indices to be masked
    set<int> maskIndices(indices.begin(), indices.begin() + numToMask);

    // Add the first sequence to the selected sequences
    vector<vector<int>> selectedSequences;
    selectedSequences.push_back(sequences[0]);

    // Add the remaining sequences that are not masked
    for (int i = 1; i < totalSequences; ++i) {
        if (maskIndices.find(i) == maskIndices.end()) {
            selectedSequences.push_back(sequences[i]);
        }
    }

    return make_tuple(selectedSequences, maskIndices);
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

/// @brief Get given normalization option by user
/// @param flagHandler 
/// @return 
Normalization getNormalization(FlagHandler& flagHandler) {
    Normalization norm;
    try {
        norm = static_cast<Normalization>(stoi(flagHandler.getFlagValue("norm")));
        if (norm < Normalization::Sqrt_L || norm > Normalization::None) {
            throw runtime_error("");
        } 
    } catch (const exception& e) {
        throw runtime_error("Invalid 'norm' value. It is outside the valid enum range.");
    }
    return norm;
}

/// @brief Get given non_standard_option option by user
/// @param flagHandler 
/// @return 
NonStandardHandler getNonStandardOption(FlagHandler& flagHandler) {
    NonStandardHandler nonStandardOption;
    try {
        nonStandardOption = static_cast<NonStandardHandler>(stoi(flagHandler.getFlagValue("non_standard_option")));
        if (nonStandardOption < NonStandardHandler::AsStandard || nonStandardOption > NonStandardHandler::ConsiderGap) {
            throw runtime_error("");
        }
    } catch (const exception& e) {
        throw runtime_error("Invalid 'non_standard_option' value. It is outside the valid enum range.");
    }
    return nonStandardOption;
}

/// @brief Check flags     
/// @param flagHandler 
void checkFlags(FlagHandler& flagHandler)
{
    // Only one of mask_enabled, only_weights, or multimer_MSA can be true at a time.
    int trueCount = 0;
    if (flagHandler.getFlagValue("mask_enabled") == "true") trueCount++;
    if (flagHandler.getFlagValue("only_weights") == "true") trueCount++;
    if (flagHandler.getFlagValue("multimer_MSA") == "true") trueCount++;
    if (flagHandler.getFlagValue("column_neff") == "true") trueCount++;
    if (trueCount > 1)
    {
        throw runtime_error(
            "Only one of 'mask_enabled', 'only_weights', 'column_neff', or 'multimer_MSA' can be true at a time.");
    }
    if (flagHandler.getFlagValue("mask_enabled") == "true")
    {
        int maskCount;
        try
        {
            maskCount = flagHandler.getIntValue("mask_count");
            if(maskCount == 0)
            {
                throw runtime_error("");
            }
        }
        catch (const exception& e)
        {
            throw runtime_error("When 'mask_enabled' is true, 'mask_count' must be a positive number.");
        }
        
        float maskFrac;
        try
        {
            maskFrac = flagHandler.getFloatValue("mask_frac");
        }
        catch (const exception& e)
        {
            throw runtime_error("When 'mask_enabled' is true, 'mask_frac' should be a number between 0 and 1");
        }
    }
    if (flagHandler.getFlagValue("multimer_MSA") == "true")
    {
        vector<int> monomerLength;
        try
        {
            monomerLength = flagHandler.getIntArrayValue("monomer_length");
        }
        catch (const exception& e)
        {
            throw runtime_error
            ("When 'multimer_MSA' is true, 'monomer_length' should be a list of positive numbers");
        }
        
        if (!((flagHandler.getFlagValue("omit_query_gaps") == "true")
        && (flagHandler.getFlagValue("gap_cutoff") == "1")
        && (flagHandler.getFlagValue("pos_start") == "1")
        && (flagHandler.getFlagValue("pos_end") == "inf")))
        {
            throw runtime_error
            ("When multimer_MSA is true, 'omit_query_gaps', 'pos_start', and 'pos_end' should remain at their default parameters.");
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
    startPos = flagHandler.getIntValue("pos_start");

    if (startPos >= lengthOfFirstAlignment)
    {
        throw runtime_error("'pos_start' should be less than length of query sequnce.");
    }

    // pos_end
    endPos = flagHandler.getIntValue("pos_end");

    if (endPos <= startPos)
    {
        throw runtime_error("'pos_end' should be greater than 'pos_start'");
    }
    // condider the last position if given value is greater than length of query sequnce
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

/// @brief Compute column-wise NEFF
/// @param sequences 
/// @param sequenceWeights
/// @param norm 
/// @return 
std::vector<double> computeColumnwiseNEFF
(const vector<vector<int>>& sequences, const vector<int>& sequenceWeights, Normalization norm) {
    int numSequences = sequences.size();
    if (numSequences == 0) {
        return {};
    }
    int sequenceLength = sequences[0].size();
    
    vector<double> columnNEFF(sequenceLength, 0.0);
    
    for (int col = 0; col < sequenceLength; ++col) {
        double sumWeights = 0.0;
        for (int row = 0; row < numSequences; ++row) {
            // include sequence weight of the current seqeunce in the column NEFF, if column is not corresponding to a gap position
            if (sequences[row][col] != 0)
            {
                sumWeights += 1./ sequenceWeights[row];
            }
        }

        switch(norm) // normalizing Nf
        {
            case Sqrt_L:
                columnNEFF[col] = sumWeights/sqrt(sequenceLength);
                break;
            case L:
                columnNEFF[col] = sumWeights/sequenceLength;
                break;
            default:
                columnNEFF[col] = sumWeights;
                break;
        }
    }
    
    return columnNEFF;
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

    vector<string> files;
    string file, format;
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
                
        // alphabet
        alphabet = getAlphabet(flagHandler);

        // check_validation
        checkValidation = flagHandler.getFlagValue("check_validation") == "true";

        // omit_query_gaps
        omitGapsInQuery = flagHandler.getFlagValue("omit_query_gaps") == "true";

        //depth    
        depth = flagHandler.getIntValue("depth");

        // file
        files = flagHandler.getFileArrayValue("file");

        MSAReader* msaReader;

        for(int f=0; f<files.size(); f++)
        {
            file = files[f];

            format = getFormat(file, "file");

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

            // integrate unique sequences
            integrateUniqueSequences(integratedSequences, sequences);

            // no need to continue if the depth of sequences so far is more than the given depth
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
        isSymmetric = flagHandler.getFlagValue("is_symmetric") == "true";

        float neff = 0.0;

        if (flagHandler.getFlagValue("multimer_MSA") == "true")
        {
            vector<int> splitPosition = flagHandler.getIntArrayValue("monomer_length");
            MSASplitter splitter(splitPosition);

            if (splitter.hasBlockForm(sequences2num))
            {
                auto [pairedMSA, msa1, msa2] = splitter.returnSets();

                // pairedMSA
                sequenceWeights = computeWeights(pairedMSA, threshold, isSymmetric, standardLetters, nonStandardOption);
                neff = computeNeff(sequenceWeights, norm, pairedMSA[0].size());
                cout << "NEFF of Paired MSA:" << neff << endl;

                // MSA 1
                sequenceWeights = computeWeights(msa1, threshold, isSymmetric, standardLetters, nonStandardOption);
                neff = computeNeff(sequenceWeights, norm, msa1[0].size());
                cout << "NEFF of first monomer:" << neff << endl;

                // MSA 2
                sequenceWeights = computeWeights(msa2, threshold, isSymmetric, standardLetters, nonStandardOption);
                neff = computeNeff(sequenceWeights, norm, msa2[0].size());
                cout << "NEFF of second monomer:" << neff << endl;
                return 0;
            }
            else
            {
                cerr << "Provided MSA is not in the form of a multimer MSA composed of "
                     << splitPosition.size()+1 << " monomers" << endl;
                return 1;
            }
        }

        int length = sequences2num[0].size();
        
        if(flagHandler.getFlagValue("mask_enabled") == "true")
        {
            vector<float> neffValues;
            set<int> maskedIndices;
            float highestNeff = 0.0;
            
            // Mask fraction of the sequences masked
            float maskFrac = flagHandler.getFloatValue("mask_frac");
            int maskCount = flagHandler.getIntValue("mask_count");

            for (int i = 0; i < maskCount; ++i)
            {
                auto [maskedSequences2num, currentmaskedIndices] = maskSequences(sequences2num, maskFrac);

                sequenceWeights = computeWeights(maskedSequences2num, threshold, isSymmetric, standardLetters, nonStandardOption);
                neff = computeNeff(sequenceWeights, norm, length);
                neffValues.push_back(neff);
                if(neff > highestNeff)
                {
                    highestNeff = neff;
                    maskedIndices = currentmaskedIndices;
                }
            }

            string neffFile = "neff_values.txt";
            ofstream outputFile(neffFile);
            // write NEFF values corresponding to each mask in a file
            if (outputFile.is_open())
            {
                outputFile << "NEFF Values for each mask iteration:\n"; 
                for (int maskNo = 0; maskNo < neffValues.size(); ++maskNo)
                {
                    outputFile << "Mask " << maskNo + 1 << ": " << neffValues[maskNo] << "\n";
                }
                outputFile.close();
                cout << "NEFF values for each mask iteration have been saved in " << neffFile << endl;
            }

            // Write the masked MSA with highest NEFF in a file
            string msaFile = "MSA_with_highest_neff.fasta";
            MSAWriter* msaWriter = new MSAWriter_fasta(sequences, msaFile, maskedIndices);
            msaWriter->write();
            cout << "Masked MSA file corresponding to the highest NEFF value among masking iterations has been saved in "<< msaFile << endl;
            return 0;
        }
        
        cout << "MSA sequnce Length: "<< length << endl;
        cout << "MSA depth: " << sequences2num.size() << endl;

        sequenceWeights = computeWeights(sequences2num, threshold, isSymmetric, standardLetters, nonStandardOption);

        if(flagHandler.getFlagValue("only_weights") == "true")
        {
            cout << "Sequence weights:" << endl;
            for (int i=0; i < sequenceWeights.size(); i++)
            {
                cout << 1./sequenceWeights[i] << ' ';
            }
            cout << endl << flush;
            return 0;
        }

        if(flagHandler.getFlagValue("column_neff") == "true")
        {
            vector<double> columnNEFF = computeColumnwiseNEFF(sequences2num, sequenceWeights, norm);
            cout << "Column-wise NEFF:" << endl;
            for (int col=0; col < columnNEFF.size(); col++)
            {
                cout << columnNEFF[col] << ' ';
            }
            // Compute the mean of columnNEFF
            double meanColumnNEFF = accumulate(columnNEFF.begin(), columnNEFF.end(), 0.0) / length;
            cout << "\nAverage of Column-wise NEFF: " << meanColumnNEFF <<endl << flush;
            return 0;
        }

        neff = computeNeff(sequenceWeights, norm, length);
        cout << "NEFF: " << neff << endl;
        
        return 0;
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
        cerr << docstr;
        return 1;
    }
}
