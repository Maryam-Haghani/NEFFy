#ifndef MSA_SPLITTER_H
#define MSA_SPLITTER_H

#include <iostream>
#include <vector>
#include <tuple>

class MSASplitter {
public:
    MSASplitter(int _splitPosition);

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
    int splitPosition; // Split position

    bool isSet2(const std::vector<int>& row) const;
    bool isSet3(const std::vector<int>& row) const;
    std::vector<int> getNonZeroPart(const std::vector<int>& row, int start, int end) const;
};

#endif // MSA_SPLITTER_H
