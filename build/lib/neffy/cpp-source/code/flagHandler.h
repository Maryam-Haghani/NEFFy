/**
 * @file flagHandler.h
 * @brief This file contains the declaration of the FlagHandler class.
 */

#ifndef FLAGHANDLER_H
#define FLAGHANDLER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>

struct FlagInfo
{    
    bool required;
    std::string value;
};

class FlagHandler
{
private:
    std::unordered_map<std::string, FlagInfo> flags;

public:

    /**
     * @brief Constructor for FlagHandler.
     * @param _flags The map of flags to be handled.
     */
    FlagHandler(const std::unordered_map<std::string, FlagInfo>& _flags);

    /**
     * @brief Processes the command-line arguments and updates the flag values.
     * @param args The command-line arguments.
     * @throws std::runtime_error if a flag is unknown, missing a value, or if a required flag is missing.
     */
    void processFlags(const std::vector<std::string>& args);

    /**
     * @brief Retrieves the value of a flag.
     * @param flagName The name of the flag.
     * @return The value of the flag.
     * @throws std::runtime_error if the flag is unknown.
     */
    std::string getFlagValue(const std::string& flagName) const;

    /**
     * @brief Checks if all required flags have values.
     * @throws std::runtime_error if a required flag is missing a value.
     */
    void checkRequiredFlags() const;

    /// @brief Get given float option by user
    /// @param flagHandler 
    /// @param name 
    /// @return 
    float getFloatValue(const std::string& name) const;

    /// @brief Get list of files
    /// @param name
    /// @return 
    std::vector<std::string> getFileArrayValue(const std::string& name) const;

    /// @brief Get given int option by user
    /// @param flagHandler 
    /// @param name
    /// @return 
    std::vector<int> getIntArrayValue(const std::string& name) const;

    /// @brief Get given int option by user
    /// @param name 
    /// @return 
    int getIntValue(const std::string& name) const;
};

#endif