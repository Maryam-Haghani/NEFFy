/**
 * @file flagHandler.cpp
 * @brief This file contains the implementation of the FlagHandler class.
 */

#include "flagHandler.h"

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