/*
// Copyright (c) 2020 Foxconn Corporation
//
//
*/

#include <ipmid/api.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <phosphor-logging/log.hpp>
#include <sdbusplus/message/types.hpp>
#include <systemcommands.hpp>
#include <sys/types.h>
#include <variant>

using json = nlohmann::json;

namespace ipmi
{
    static void registerSYSFunctions() __attribute__((constructor));

    ipmi::RspType<std::vector<uint8_t>> hardwareConfig(uint8_t device, uint8_t mode)
    {
        std::vector<uint8_t> rsp = {0x01};
        
        if(device == 0b01) //NCSI
        { 
            switch(mode)
            {
            case 0b00: //Get the current configuration

                break;
                
            case 0b01: //Onboard interface

                break;

            case 0b10: //OPC3.0

                break;
                
            default: //Error

                return ipmi::responseParmOutOfRange();
                break;
            }
        }

        else if(device == 0b10)//VGA
        { 
            switch(mode)
            {
                case 0b00: //Get current configuration

                    break;
                
                case 0b01: //Rear end VGA port

                    break;

                case 0b10: //Front end VGA port

                    break;
                
                default: //Error

                    return ipmi::responseParmOutOfRange();
                    break;
            }
        }

        else
        {

        }
        return ipmi::responseSuccess(rsp);
    }

    ipmi::RspType<std::vector<uint8_t>> getFirmwareVersion(boost::asio::yield_context yield, uint8_t firmwareType, uint8_t index)
    {
        
        std::vector<uint8_t> rsp;

        switch (firmwareType)
        {
            case 0b1: //BMC firmware version
            {
                rsp.push_back(firmwareType);
                //Read BMC firmware version
                //Read and Transfer the version json
                std::ifstream a(versionPath);
                json versionJS = json::parse(a);

                std::string version = versionJS["firmwareVersion"]; //get BMC version
                version.erase(remove(version.begin(), version.end(), '"'), version.end()); //strip "" from string

                if(version.empty())
                {
                    phosphor::logging::log<phosphor::logging::level::ERR>(
                                "Error firmwareVersion came back null"
                                );
                    ipmi::responseUnspecifiedError();
                }
                else
                {
                    //Convert string to a vector of bytes
                    std::vector<int> bytes;
                    for (size_t i = 0; i < version.size(); i+=2)
                    {
                        std::string byteString = version.substr(i,2);
                        int byte = std::stoi(byteString, NULL, 16);
                        bytes.push_back(byte);
                    }
                    //Add byte vector to response vector
                    rsp.insert(rsp.end(), bytes.begin(), bytes.end());
                }
                break;
            }
            case 0b10: //BIOS firmware version 
            {
                std::cout<<"BIOSFIRMWARE"<<std::endl;
                rsp.push_back(firmwareType);
                //Read BMC firmware version
                break;
            }
            case 0b11: //CPLD firmware version
            {
                std::cout<<"CPLDFIRMWARE"<<std::endl;
                
                if(index = 0b01)
                {
                    //Read CPLD at MB
                    rsp.push_back(firmwareType);
                    //push back version
                }
                
                else if(index=0b10)
                {
                    //Read CPLD at MP
                    rsp.push_back(firmwareType);
                    //push back version
                }

                else
                {
                    return ipmi::responseParmOutOfRange();
                    //IPMIFailure
                }
                break;
            }
            case 0b100: //Motherboard version
            {
                std::cout<<"MOTHERBOARD"<<std::endl;
                rsp.push_back(firmwareType);
                //push back version
                break;
            }
            case 0b101: //PSU firmware version
            {
                std::cout<<"PSUFIRMWARE"<<std::endl;
                //Read PSU Version
                rsp.push_back(firmwareType);
                //push back version
                break;
            }
            case 0b110: //FRU VPD version
            {
                std::cout<<"FRUVPD"<<std::endl;
                //Read FRU version
                rsp.push_back(firmwareType);
                //push back version
                break;    
            }
            default: //Error
            {
                return ipmi::responseParmOutOfRange();
                break;
            }    
        }
        return ipmi::responseSuccess(rsp);
    }

    void registerSYSFunctions()
    {

        /*    registerHandler({PRIORITY_VAL}, {NETFN}}, {CMD_VAL}, {PRIVILEGE_LVL},
                            {HANDLER_FUNCTION});
        */

        ipmi::registerHandler(ipmi::prioOemBase, ipmi::netFnOemThree, CMD_SYS_HW_CONFIG, ipmi::Privilege::User, 
                            hardwareConfig);
        ipmi::registerHandler(ipmi::prioOemBase, ipmi::netFnOemThree, CMD_SYS_GET_SYS_FW_VER, ipmi::Privilege::User, 
                            getFirmwareVersion);
        return;
    }    

}