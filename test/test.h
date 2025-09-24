#ifndef __PROTOCOL_TEST_H__
#define __PROTOCOL_TEST_H__

#include <chrono>
#include <iostream>

#include "packer.h"
#include "packer_handler.h"
#include "util.h"

#define SPLIT_LINE    (std::string(100, '='))

using namespace protocol;

class Test
{
public:
    template<typename T, typename std::enable_if<std::is_base_of<MessageHeader, T>::value>::type* = nullptr>
    static void run(const T &_t) 
    {
        std::cout << std::endl << SPLIT_LINE << std::endl << std::endl << _t;
        
        // pack
        auto buf = Packer::pack(_t);
        printf("\n");
        print_buffer("CSAE295.2", _t.data_type_, buf->data, buf->size);

        // unpack
        PackerHandler handler;
        Packer::unpack(buf->data, buf->size, handler);
    }

    template<typename T, typename std::enable_if<std::is_base_of<MessageHeader, T>::value>::type* = nullptr>
    static void test() 
    {
        LOGE("Test", "test: T isn't specified!\n");
    }
};

template<>
inline void Test::test<MessageHeader>() 
{
    MessageHeader msg(0, HEARTBEAT, 0x01, get_utc_timestamp_ms(), 0xFC);
    run(msg);

    MessageHeader msg2(0, HEARTBEAT_RES, 0x01, get_utc_timestamp_ms(), 0xFC);
    run(msg2);
}

template<>
inline void Test::test<Veh2CloudInh>() 
{
    Veh2CloudInh msg(
        0x01, get_utc_timestamp_ms(), 0xFC, "Q1001", "sw_v1.0", "hw_v1.0", "ad_v1.0", 
        COMM_TYPE_4G, 15, TIME_SYNC_GNSS, GNSS_TYPE_GCJ02, "VEH2CLOUD_INH");
    run(msg);
}

template<>
inline void Test::test<Cloud2VehInhRes>() 
{
    Cloud2VehInhRes msg(
        0x01, get_utc_timestamp_ms(), 0xFC, "Q1001", CLOUD2VEH_INH_RES_COMFIRM);
    run(msg);
}

template<>
inline void Test::test<Veh2CloudState>() 
{
    Veh2CloudState msg(
        0x01, get_utc_timestamp_ms(), 0xFC, "Q1001", std::vector<uint8_t>{1}, get_utc_timestamp_ms(), 
        4000, Position(90, 90, 700), 100000, 31, 200000, 4100, 500, 400, 300, 200, 500, 3000, 50000, 
        1, 500, 20000, 1000, 1, Position2D(80, 80), std::vector<Position2D>());
    run(msg);
}

#endif // __PROTOCOL_TEST_H__
