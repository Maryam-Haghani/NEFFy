/**
 * @file flagHandler.cpp
 * @brief This file contains the implementation of the FlagHandler class.
 */

#include "flagHandler.h"
#include "common.h"
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <climits>
#include <filesystem>
#include <algorithm>

using namespace std;

FlagHandler::FlagHandler(const std::unordered_map<std::string, FlagInfo>& _flags)
    : flags(_flags) {}

void FlagHandler::processFlags(const vector<string>& args)
{
    for (size_t i = 0; i < args.size(); i++)
    {
        if (args[i].find("--") == 0)
        {
            auto arg = args[i].substr(2);
            int assignPosition = arg.find('=');
            if (assignPosition == std::string::npos) // arg does not contain '='
            {
                throw runtime_error("flag \'" + arg + "\' does not have any value");
            }

            string name = arg.substr(0, assignPosition);
            string value = arg.substr(assignPosition+1);

            auto flag = flags.find(name);            
            if (flag != flags.end())
            {
                if (!value.empty())
                {
                    flag->second.value = value;
                }
                else
                {
                    if (flag->second.required)
                    {
                        throw runtime_error("Missing value for required flag: " + arg);
                    }
                    else
                    {
                        throw runtime_error("Missing value for optional flag: " + arg);
                    }
                }
            }
            else
            {
                throw runtime_error("Unknown flag: " + name);
            }
        }
    }
}

string FlagHandler::getFlagValue(const string& flagName) const
{
    auto flag = flags.find(flagName);

    if (flag != flags.end())
    {
        return flag->second.value;
    }
    else
    {
        throw runtime_error("Unknown flag: " + flagName);
    }
}

void FlagHandler::checkRequiredFlags() const
{
    for (const auto& pair : flags)
    {
        const string& flagName = pair.first;
        const FlagInfo& flagInfo = pair.second;

        if (flagInfo.required && flagInfo.value.empty())
        {
            throw runtime_error("Missing value for required flag: " + flagName);
        }
    }
}

float FlagHandler::getBooleanValue(const string& name) const
{
    string value;
    try {
        value = getFlagValue(name);

        if (value == "true") {
            return true;
        } else if (value == "false") {
            return false;
        } else {
            throw runtime_error("");
        }
    } catch (const exception& e) {
        throw runtime_error("Invalid boolean '" + name + "' value. It should be either 'true' or 'false'.");
    }
}

float FlagHandler::getFloatValue(const string& name) const
{
    float value;
    try {
        value = stof(getFlagValue(name));
        if (value <= 0.0 || value > 1.0) {
            throw runtime_error("");
        }
    } catch (const exception& e) {
        throw runtime_error("Invalid '" + name + "' value. It should be a number between 0 and 1 (excluding 0).");
    }
    return value;
}

vector<string> FlagHandler::getArrayValues(const string& name) const
{
    vector<string> values;
    string svalue = getFlagValue(name);

    stringstream ss(svalue);
    string item;
    while (getline(ss, item, ','))
    {
        values.push_back(item);
    }

    return values;
}

string FlagHandler::getFileValue(const string& name) const
{
    string file = getFlagValue(name);
    
    // Check if file exists
    if (!filesystem::exists(file)) {
        throw runtime_error("Error: File '" + file + "' does not exist.");
    }
    
    return file;
}

vector<string> FlagHandler::getFileArrayValue(const string& name) const
{
    vector<string> files = getArrayValues(name);

    for (const string& file : files) {
        // Check if file exists
        if (!filesystem::exists(file)) {
            throw runtime_error("Error: File '" + file + "' does not exist.");
        }
    }

    if (files.empty()) {
        throw runtime_error("Invalid '" + name + "' value. It should contain at least one item");
    }
    return files;
}


vector<int> FlagHandler::getIntArrayValue(const string& name) const
{
    vector<int> values;

    string svalue = getFlagValue(name);

    stringstream ss(svalue);
    string item;
    while (getline(ss, item, ',')) {
        int value = stoi(item);
        if (value <= 0) {
            throw runtime_error("Invalid '" + name + "' value. Each value should be a positive number");
        }
        values.push_back(value);
    }

    if (values.empty()) {
        throw runtime_error("Invalid '" + name + "' value. It should contain at least one positive number");
    }
    return values;
}

int FlagHandler::getIntValue(const string& name) const
{
    string svalue = getFlagValue(name);

    if (svalue == "inf")
        return INT_MAX;

    size_t pos;
    int value = stoi(svalue, &pos);

    // given value is an integer
    if (pos != svalue.length()) {
        throw invalid_argument("Invalid '" + name + "' value. It must be an integer.");
    }

    // given value is positive
    if (value < 0) {
        throw runtime_error("Invalid '" + name + "' value. It cannot be a negative number");
    }
    return value;
}

int FlagHandler::getNonZeroIntValue(const string& name) const
{
    int value = getIntValue(name);

    if (value == 0) {
        throw runtime_error("Invalid '" + name + "' value. It should be a positive number");
    }
    return value;
}
