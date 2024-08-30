/**
 * @file multimerHandler.h
 * @brief This file contains the declaration of the MultimerHandler class to manage and manipulate MSA of a multimer.
 * 
 * - Verify if the MSA is in the format of the given stoichiometry,
 * When multimer is a heteromer (with more than one distinct chain), MSA starts from pairedMSA, followed by the MSA of the chains in the multimer.
 * When multimer is a homomer, MSA is a concatenation of MSA of the chain.
 * 
 * The implementation includes:
 * - Constructor to initialize the split position.
 * - Method to verify the structure of the MSA with the given stochiometry.
 * - Method to return the MSA sets.
 */

#ifndef MULTIMER_HANDLER
#define MULTIMER_HANDLER

#include <iostream>
#include <vector>
#include <tuple>

class MultimerHandler {
public:
    /// @brief // Constructor to initialize the stoichiometry
    /// @param _stoichiom
    MultimerHandler(std::string _stoichiom);

    /// @brief Map chain index to stoichiometry letter
    /// @param index 
    /// @return stoichiometry letter
    std::string chainIndexToStoichiomLetter(int index);

    /// @brief Verify if the MSA is in the form of a paired MSA (sequences followed by the MSA of each chain)
    /// @param msa 
    /// @return 
    bool getHeteromerMSAs(const std::vector<std::vector<int>>& vectors);

    /// @brief Check if stoichiometry follows the pattern of a heteromer (e.g., A2B2C1)
    // The letters must appear in the correct alphabetical order (e.g., A before B, B before C, etc.).
    // If a letter appears, all preceding letters in the alphabet must also appear.
    // Each letter is followed by a number greater than 0.
    /// @return 
    bool isHeteromerFormat();


    /// @brief Check if the stoichiometry is in homomer format (e.g., A3)
    /// @return 
    bool isHomomerFormat();

    /// @brief Get individual MSA for homomer format
    /// @return individual MSA
    std::vector<std::vector<int>> getHomomerIndividualMSA(const std::vector<std::vector<int>>& sequences2num) ;

    /// @brief Get individual MSA for heteromer format
    /// @param sequences2num 
    /// @param chain_lengths 
    /// @return individual MSAs
    std::vector<std::vector<std::vector<int>>> getHetoromerMSAs(const std::vector<std::vector<int>>& sequences2num, const std::vector<int>& chainLengths);

private:
    //stoichiometry
    std::string stoichiom;

    // Number of repeatition of each chain in the multimer
    std::vector<int> numbers;

    // Number of distinct chains for a heteromer
    int chainsCount;

    // Indexes of start and end of each chain
    std::vector<int> chainIntervals;

    /// @brief // Extract the sequence segment from the row that corresponds to the chain specified by the given chainIndex.
    /// @param row 
    /// @param chainIndex 
    /// @param repeat 
    /// @return 
    std::vector<int> getSequenceSegment(const std::vector<int>& row, int chainIndex, int repeat=1) const;

    /// @brief Verify if all segments of the repeated chains in the given row are identical, and if so, return a single segment.
    /// @param row 
    /// @param rowIndex 
    /// @param chainIndex 
    /// @return chainSegment
    std::vector<int> checkRepeatedChainAndReturnOneSegment(const std::vector<int>& row, int rowIndex, int chainIndex);
    
    /// @brief Check if the row is for the paired MSA, i.e, it includes at least two sequence segements corresponding to two chains with non-gap elements
    /// @param row 
    /// @return 
    bool includesInPairedMSA(const std::vector<int>& row);

    /// @brief // Find index of the chain corrsponding to given row; it should correspond exactly to one chain
    /// @param row 
    /// @param row_index 
    /// @return 
    int checkRowCorrespondsOnlyToOneChainAndReturnChainIndex(const std::vector<int>& row, int row_index);

    /// @brief Check MSA format and return MSA row index intervals corresponding to each chain
    /// @param chainIndexOfRows 
    /// @return 
    std::vector<int> checkMSAFormatAndReturnRowIndexIntervalsCorrespondingToChains(const std::vector<int>& chainIndexOfRows);
};

#endif // MULTIMER_HANDLER
