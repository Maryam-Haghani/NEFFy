/**
 * @file msaSplitter.h
 * @brief This file contains the declaration of the MSASplitter class to manage and manipulate MSAs.
 * 
 * The MSASplitter class provides methods to:
 * - Verify if the MSA is in a paired form, where sequences are followed by the MSA of the first monomer, followed by the MSA of the second monomer.
 * - Split the MSA into three sets: paired MSA, MSA of the first monomer, and MSA of the second monomer.
 * 
 * The class includes:
 * - A constructor to initialize the split position.
 * - Methods to verify the structure of the MSA and return the split sets.
 * - Private helper functions to determine set membership and extract non-zero parts of the MSA rows.
 * 
 * Usage:
 * Create an instance of MSASplitter with the desired split position, then use its methods to process and retrieve MSA sets.
 */

#ifndef MSA_SPLITTER_H
#define MSA_SPLITTER_H

#include <iostream>
#include <vector>
#include <tuple>

class MSASplitter {
public:
    MSASplitter(std::vector<int> _splitPositions);

    /// @brief Verify if the MSA is in the form of a paired MSA (sequences followed by the MSA of the first monomer, followed by the MSA of the second monomer)
    /// @param msa 
    /// @return 
    bool hasBlockForm(const std::vector<std::vector<int>>& vectors);

    /// @brief Return paired MSA, MSA of the first monomer, and MSA of the second monomer
    /// @return 
    std::tuple<std::vector<std::vector<int>>, std::vector<std::vector<int>>, std::vector<std::vector<int>>> returnSets();

private:
    std::vector<std::vector<int>> set1;
    std::vector<std::vector<int>> set2;
    std::vector<std::vector<int>> set3;
    std::vector<int> splitPositions;

    bool isSet2(const std::vector<int>& row) const;
    bool isSet3(const std::vector<int>& row) const;
    std::vector<int> getNonZeroPart(const std::vector<int>& row, int start, int end) const;
};

#endif // MSA_SPLITTER_H
