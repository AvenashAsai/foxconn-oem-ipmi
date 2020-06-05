/*
// Copyright (c) 2020 Foxconn Corporation
//
//
*/

#pragma once
#include <ipmid/api.hpp>
#include <ipmid/utils.hpp>
#include <nlohmann/json.hpp>

enum fxn_sys_cmds
{
    CMD_SYS_HW_CONFIG = 0x00,
    CMD_SYS_GET_SYS_FW_VER = 0x01,
};

const char *versionPath="/etc/foxconn/version.json";