#ifndef __PROTOCOL_MESSAGE_H__
#define __PROTOCOL_MESSAGE_H__

#include <stdint.h>
#include <string.h>
#include <cinttypes>

#include <vector>
#include <iostream>
#include <memory>

#include "converter.h"
#include "log.h"

#define VEH2CLOUD_INH     0x34
#define CLOUD2VEH_INH_RES 0x35
#define VEH2CLOUD_STATE   0x15
#define HEARTBEAT         0x0C
#define HEARTBEAT_RES     0x0D

namespace protocol
{
#pragma pack(1)

/**
 * Message buffer.
 */
struct MessageBuffer
{
    template<typename T>
    inline static void deleter(T *_p)
    {	
        delete [](uint8_t*)_p;
    }

    uint32_t size;
    uint8_t  data[0];
};

/**
 * Message header.
 */
struct MessageHeader
{
    MessageHeader(
        const uint32_t _data_len,
        const uint8_t  _data_type,
        const uint8_t  _version,
        const uint64_t _timestamp,
        const uint8_t  _ctrl): 
            data_len_(_data_len), 
            data_type_(_data_type), 
            version_(_version), 
            timestamp_(_timestamp), 
            ctrl_(_ctrl)
    {
    }

    MessageHeader(const void *_buf, const size_t _size, const bool _big_endian = true) 
    {
        if (nullptr == _buf)
        {
            LOGE(TAG, "MessageHeader: Buffer is null!\n");
            return;
        }

        if (_size < sizeof(MessageHeader))
        {
            LOGE(TAG, "MessageHeader: Invalid buffer size %ld!", _size);
            return;
        }

        size_t offset = 0;
        char *buf = (char*)_buf;

        id_ = *(uint8_t*)(buf + offset);
        offset += sizeof(id_);

        data_len_ = _big_endian ? __builtin_bswap32(*(uint32_t*)(buf + offset)) : *(uint32_t*)(buf + offset);
        offset += sizeof(data_len_);

        data_type_ = *(uint8_t*)(buf + offset);
        offset += sizeof(data_type_);

        version_ = *(uint8_t*)(buf + offset);
        offset += sizeof(version_);

        timestamp_ = _big_endian ? __builtin_bswap64(*(uint64_t*)(buf + offset)) : *(uint64_t*)(buf + offset);
        offset += sizeof(timestamp_);

        ctrl_ = *(uint8_t*)(buf + offset);
        offset += sizeof(ctrl_);
    }

    uint8_t get_header_length() const
    {           
        return sizeof(MessageHeader);
    }

    uint32_t get_data_length() const
    {
        return data_len_;
    }

    void set_data_length(const uint32_t _data_len)
    {
        data_len_ = _data_len;
    }

    size_t to_bytes(void *_buf, const size_t _size, const bool _big_endian = true) const
    {
        if (nullptr == _buf)
        {
            LOGE(TAG, "to_bytes: Buffer is null!\n");
            return 0;
        }

        if (_size < get_header_length())
        {
            LOGE(TAG, "tobytes: Invalid size %ld, header length %d!\n", _size, get_header_length());
            return 0;
        }

        size_t offset = 0;
        uint8_t *buf = (uint8_t*)_buf;

        *(uint8_t*)(buf + offset) = id_;
        offset += sizeof(id_);

        *(uint32_t *)(buf + offset) = _big_endian ? __builtin_bswap32(data_len_) : data_len_;
        offset += sizeof(data_len_);

        *(uint8_t*)(buf + offset) = data_type_;
        offset += sizeof(data_type_);

        *(uint8_t*)(buf + offset) = version_;
        offset += sizeof(version_);

        *(uint64_t *)(buf + offset) = _big_endian ? __builtin_bswap64(timestamp_) : timestamp_;
        offset += sizeof(timestamp_);

        *(uint8_t*)(buf + offset) = ctrl_;
        offset += sizeof(ctrl_);
        
        return offset;
    }

    friend std::ostream& operator<<(std::ostream &os, const MessageHeader &_header)
    {
        uint64_t value = 0;
        size_t offset = 0;
        char ostr[256] = "";

        value = _header.id_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 标识位: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_header.id_);
        os << ostr;

        value = _header.data_len_;
        sprintf(ostr, "%-4ld[%08" PRIX64 "] 数据段长度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_header.data_len_);
        os << ostr;

        value = _header.data_type_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 数据类别: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_header.data_type_);
        os << ostr;

        value = _header.version_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 版本号: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_header.version_);
        os << ostr;

        value = _header.timestamp_;
        sprintf(ostr, "%-4ld[%016" PRIX64 "] 时间戳: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_header.timestamp_);
        os << ostr;

        value = _header.ctrl_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 控制内容: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_header.ctrl_);
        os << ostr;

        return os;
    }

    static constexpr const char *TAG = "protocol::MessageHeader";

    uint8_t  id_ = 0xF2;
    uint32_t data_len_;
    uint8_t  data_type_;
    uint8_t  version_;
    uint64_t timestamp_;
    uint8_t  ctrl_;
};

#pragma pack()
} // namespace protocal

#endif // __PROTOCOL_MESSAGE_H__
