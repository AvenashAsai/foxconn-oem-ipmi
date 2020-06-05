/*
// Copyright (c) 2020 Foxconn Corporation
//
//
*/
#pragma once
#include <ipmid/api.hpp>

struct fru_info
{
    uint16_t size;
    uint8_t access;
};

struct fru_header
{
    uint8_t version;
    uint8_t internal;
    uint8_t chassis;
    uint8_t board;
    uint8_t product;
    uint8_t multi;
    uint8_t pad;
    uint8_t checksum;
};

#define ACCESS_MASK 0x1