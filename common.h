/**
 * @file common.h
 * @brief This file contains declaration of common utility functions used in the MSA project.
 */

#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>

const std::string STANDARD_AMINO_ACIDS = "ACDEFGHIKLMNPQRSTVWY";
const std::string NON_STANDARD_AMINO_ACIDS = "XOUBJZ";
const std::string STANDARD_RNA_NUCLEOTIDES = "AUCG";
const std::string STANDARD_DNA_NUCLEOTIDES = "ATCG";
const std::string NON_STANDARD_NUCLEOTIDES = "N";
const std::string GAP = "-.";

// Structure to represent a sequence
struct Sequence
{
    std::string id;       // Sequence identifier
    std::string sequence; // Sequence data
    std::string remarks;  // Remarks associated with the sequence
};

// Enumeration for NEFF normalization options
enum Normalization
{
    Sqrt_L = 0, // Square root of the length
    L,          // Length
    None        // No normalization
};

// Enumeration for MSA alphabet
enum Alphabet
{
    protein = 0,
    RNA,
    DNA
};

// Enumeration for options we have for handling non-standard residues in neff computation
enum NonStandardHandler
{
    AsStandard = 0, // treat the the same as standard residues, default in deepMSA symmetric
    ConsiderGapInCutoff, // consider them as gaps when computing cutoff of sequence (only used in asymmetryc version, default in deepMSA assymetric)
    ConsiderGap // consider them as gaps in computing similarity cutoff and checking position match/mismatch, default in Gremlin
};

/**
 * @brief Get the format from a file path.
 * @param file file path.
 * @param name name of the file (for error messages).
 * @return The format string.
 */
std::string getFormat(std::string file, std::string name);

/**
 * @brief Keep only the positions of non-gap residues in the query sequence for all sequences.
 * @param sequences The vector of sequences to process.
 */
void keepNonGapPositionsOfQuerySequence(std::vector<Sequence>& sequences);

/// @brief gets the list of allowed letters for the provided input alphabet
/// @param alphabet 
/// @return list of allowed letters 
std::string getAllowedLetters(Alphabet alphabet);

/// @brief gets the list of standard letters for the provided input alphabet
/// @param alphabet 
/// @return list of standard letters 
std::string getStandardLetters(Alphabet alphabet);

/// @brief gets the list of non-standard letters for the provided input alphabet
/// @param alphabet 
/// @return list of non-standard letters 
std::string getNonStandardLetters(Alphabet alphabet);
#endif