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
#include <filefwcommands.hpp>

namespace ipmi
{
    static void registerFILEFunctions() __attribute__((constructor));
 
    /**
     * Command implementations go here.
     *
    */


    /* ipmi::RspType<> {HANDLER_FUNCTION}
    {

        //Statements of function.
        return CC_INVALID;

    }   */

void registerFILEFunctions()
{

    /*    registerHandler({PRIORITY_VAL}, {NETFN}}, {CMD_VAL}, {PRIVILEGE_LVL},
                    {HANDLER_FUNCTION});
    */
 
    return;
}

}