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

float FlagHandler::getFloatValue(const string& name) const
{
    float value;
    try {
        value = stof(getFlagValue(name));
        if (value <= 0.0 || value > 1.0) {
            throw runtime_error("Invalid '" + name + "' value. It should be a number between 0 and 1.");
        }
    } catch (const exception& e) {
        throw runtime_error("Invalid '" + name + "' value. It should be a number between 0 and 1.");
    }
    return value;
}

vector<string> FlagHandler::getFileArrayValue(const string& name) const
{
    vector<string> values;
    string svalue = getFlagValue(name);

    stringstream ss(svalue);
    string item;
    while (getline(ss, item, ','))
    {
        // check file format to be valid
        if (!filesystem::exists(item))
        {
            throw runtime_error("Error: File '" + item + "' does not exist.");
        }
        string format = getFormat(item, "file");
        bool isValidFormat = std::find(VALID_FORMATS.begin(), VALID_FORMATS.end(), format) != VALID_FORMATS.end();
        if (!isValidFormat) {
            throw runtime_error("Error: Unsupported file format '" + format + "'.");
        }
        values.push_back(item);
    }

    if (values.empty()) {
        throw runtime_error("Invalid '" + name + "' value. It should contain at least one item");
    }
    return values;
}

vector<int> FlagHandler::getIntArrayValue(const string& name) const
{
    vector<int> values;
    try {
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
    } catch (const exception& e) {
        throw runtime_error("Invalid '" + name + "' value. Each value should be an array of positive numbers");
    }
    return values;
}

int FlagHandler::getIntValue(const string& name) const
{
    int value;
    try {
        string svalue = getFlagValue(name);

        if (svalue == "inf")
            return INT_MAX;

        value = stoi(svalue);

        if (value <= 0) {
            throw runtime_error("Invalid '" + name + "' value. It should be a positive number");
        }
    } catch (const exception& e) {
        throw runtime_error("Invalid '" + name + "' value. It should be a positive number");
    }
    return value;
}