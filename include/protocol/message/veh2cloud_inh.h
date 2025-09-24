#ifndef __PROTOCOL_VEH2CLOUD_INH_H__
#define __PROTOCOL_VEH2CLOUD_INH_H__

#include "message.h"

#define COMM_TYPE_4G      0 
#define COMM_TYPE_5G      1 
#define COMM_TYPE_OTHER   2 
#define COMM_TYPE_DEFAULT 0xFF 

#define TIME_SYNC_UNKNOWN  0
#define TIME_SYNC_PTP      1
#define TIME_SYNC_GNSS     2
#define TIME_SYNC_NTP      3
#define TIME_SYNC_INTERNET 4
#define TIME_SYNC_OTHER    5
#define TIME_SYNC_DEFAULT  0xFF 

#define GNSS_TYPE_GCJ02 0
#define GNSS_TYPE_USER  1

#define CLOUD2VEH_INH_RES_COMFIRM     0
#define CLOUD2VEH_INH_RES_FAIL        1
#define CLOUD2VEH_INH_RES_MSG_ERROR   2 
#define CLOUD2VEH_INH_RES_UNSUPPORTED 3

namespace protocol
{
#pragma pack(1)

/**
 * VEH2CLOUD_INH.
 */
struct Veh2CloudInh: public MessageHeader
{
    Veh2CloudInh(
        const uint8_t  _version,
        const uint64_t _timestamp,
        const uint8_t  _ctrl, 
        const std::string &_vehicle_id,
        const std::string &_sw_ver,
        const std::string &_hw_ver,
        const std::string &_ad_ver,
        const uint8_t _com_type,
        const uint8_t _pos_confidence,
        const uint8_t _time_sync,
        const uint8_t _gnss_type,
        const std::string &_user_data):
            MessageHeader(
                sizeof(Veh2CloudInh) - sizeof(MessageHeader) - sizeof(sw_ver_) - sizeof(hw_ver_) 
                - sizeof(ad_ver_) - sizeof(user_data_), VEH2CLOUD_INH, _version, _timestamp, _ctrl), 
            sw_ver_(_sw_ver), hw_ver_(_hw_ver), ad_ver_(_ad_ver), com_type_(_com_type), pos_confidence_(_pos_confidence), 
            time_sync_(_time_sync), gnss_type_(_gnss_type), user_data_(_user_data)
    {
        strncpy(vehicle_id_, _vehicle_id.c_str(), sizeof(vehicle_id_));
 
        sw_ver_len_ = _sw_ver.length() >= 0xFF ? 0 : _sw_ver.length();
        data_len_ += sw_ver_len_;

        hw_ver_len_ = _hw_ver.length() >= 0xFF ? 0 : _hw_ver.length();
        data_len_ += hw_ver_len_;

        ad_ver_len_ = _ad_ver.length() >= 0xFF ? 0 : _ad_ver.length();
        data_len_ += ad_ver_len_;

        user_data_len_ = _user_data.length() >= 0xFF ? 0 : _user_data.length();
        data_len_ += user_data_len_;
    }

    Veh2CloudInh(const void *_buf, const size_t _size, const bool _big_endian = true): 
        MessageHeader(_buf, _size, _big_endian)
    {
        size_t offset = get_header_length();

        if (_size - offset < get_data_length())
        {
            LOGE(TAG, "Veh2CloudInh: Invalid size %ld, offset %ld, data length %d!\n", _size, offset, get_data_length());
            return;
        }
        
        char *buf = (char*)_buf;

        memcpy(vehicle_id_, buf + offset, sizeof(vehicle_id_));
        offset += sizeof(vehicle_id_);

        sw_ver_len_ = *(uint8_t*)(buf + offset);
        offset += sizeof(sw_ver_len_);
        if (0 != sw_ver_len_ && 0xFF !=  sw_ver_len_)
        {
            sw_ver_ = std::string(buf + offset, sw_ver_len_);
            offset += sw_ver_len_;
        }

        hw_ver_len_ = *(uint8_t*)(buf + offset);
        offset += sizeof(hw_ver_len_);
        if (0 != hw_ver_len_ && 0xFF !=  hw_ver_len_)
        {
            hw_ver_ = std::string(buf + offset, hw_ver_len_);
            offset += hw_ver_len_;
        }

        ad_ver_len_ = *(uint8_t*)(buf + offset);
        offset += sizeof(ad_ver_len_);
        if (0 != ad_ver_len_ && 0xFF !=  ad_ver_len_)
        {
            ad_ver_ = std::string(buf + offset, ad_ver_len_);
            offset += ad_ver_len_;
        }

        com_type_ = *(uint8_t*)(buf + offset);
        offset += sizeof(com_type_);

        pos_confidence_ = *(uint8_t*)(buf + offset);
        offset += sizeof(pos_confidence_);

        time_sync_ = *(uint8_t*)(buf + offset);
        offset += sizeof(time_sync_);

        gnss_type_ = *(uint8_t*)(buf + offset);
        offset += sizeof(gnss_type_);

        user_data_len_ = *(uint8_t*)(buf + offset);
        offset += sizeof(user_data_len_);
        if (0 != user_data_len_ )
        {
            user_data_ = std::string(buf + offset, user_data_len_);
            offset += user_data_len_;
        }
    }

    size_t to_bytes(void *_buf, const size_t _size, const bool _big_endian = true) const
    {
        size_t offset = MessageHeader::to_bytes(_buf, _size);

        if (_size - offset < get_data_length())
        {
            LOGE(TAG, "to_bytes: Invalid size %ld, offset %ld, data length %d!\n", _size, offset, get_data_length());
            return offset;
        }

        uint8_t *buf = (uint8_t*)_buf;

        memcpy(buf + offset, vehicle_id_, sizeof(vehicle_id_));
        offset += sizeof(vehicle_id_);

        *(uint8_t*)(buf + offset) = sw_ver_len_;
        offset += sizeof(sw_ver_len_);
        if (0 != sw_ver_len_ && 0xFF !=  sw_ver_len_)
        {
            memcpy(buf + offset, sw_ver_.c_str(), sw_ver_len_);
            offset += sw_ver_len_;
        }

        *(uint8_t *)(buf + offset) = sw_ver_len_;
        offset += sizeof(hw_ver_len_);
        if (0 != hw_ver_len_ && 0xFF !=  hw_ver_len_)
        {
            memcpy(buf + offset, sw_ver_.c_str(), hw_ver_len_);
            offset += hw_ver_len_;
        }

        *(uint8_t *)(buf + offset) = ad_ver_len_;
        offset += sizeof(ad_ver_len_);
        if (0 != ad_ver_len_ && 0xFF !=  ad_ver_len_)
        {
            memcpy(buf + offset, sw_ver_.c_str(), ad_ver_len_);
            offset += ad_ver_len_;
        }

        *(uint8_t *)(buf + offset) = com_type_;
        offset += sizeof(com_type_);

        *(uint8_t *)(buf + offset) = pos_confidence_;
        offset += sizeof(pos_confidence_);

        *(uint8_t *)(buf + offset) = time_sync_;
        offset += sizeof(time_sync_);

        *(uint8_t *)(buf + offset) = gnss_type_;
        offset += sizeof(gnss_type_);

        *(uint8_t *)(buf + offset) = user_data_len_;
        offset += sizeof(user_data_len_);
        if (0 != user_data_len_)
        {
            memcpy(buf + offset, user_data_.c_str(), user_data_len_);
            offset += user_data_len_;
        }

        return offset;
    }  

    friend std::ostream& operator<<(std::ostream &os, const Veh2CloudInh &_msg)
    {
        os << (const MessageHeader&)_msg;

        uint64_t value = 0;
        size_t offset = _msg.get_header_length();
        size_t len = 0;
        char str[256] = "";
        char ostr[512] = "";

        len = sizeof(_msg.vehicle_id_);
        bytes_to_string(_msg.vehicle_id_, len, str);
        sprintf(ostr, "%-4ld[%s] 车辆编号: %s\n", offset, str, std::string(_msg.vehicle_id_, len).c_str());
        offset += len;
        os << ostr;

        value = _msg.sw_ver_len_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 车载终端设备软件版本号长度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.sw_ver_len_);
        os << ostr;

        if (0 != _msg.sw_ver_len_ && 0xFF !=  _msg.sw_ver_len_)
        {
            len = _msg.sw_ver_len_;
            bytes_to_string(_msg.sw_ver_.c_str(), len, str);
            sprintf(ostr, "%-4ld[%s] 车辆软件版本: %s\n", offset, str, _msg.sw_ver_.c_str());
            offset += len;
            os << ostr;
        }

        value = _msg.hw_ver_len_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 自动驾驶系统硬件版本号长度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.hw_ver_len_);
        os << ostr;

        if (0 != _msg.hw_ver_len_ && 0xFF !=  _msg.hw_ver_len_)
        {
            len = _msg.hw_ver_len_;
            bytes_to_string(_msg.hw_ver_.c_str(), len, str);
            sprintf(ostr, "%-4ld[%s] 自动驾驶系统硬件版本: %s\n", offset, str, _msg.hw_ver_.c_str());
            offset += len;
            os << ostr;
        }

        value = _msg.ad_ver_len_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 自动驾驶系统软件版本号长度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.ad_ver_len_);
        os << ostr;

        if (0 != _msg.ad_ver_len_ && 0xFF !=  _msg.ad_ver_len_)
        {
            len = _msg.ad_ver_len_;
            bytes_to_string(_msg.ad_ver_.c_str(), len, str);
            sprintf(ostr, "%-4ld[%s] 自动驾驶系统软件版本号: %s\n", offset, str, _msg.ad_ver_.c_str());
            offset += len;
            os << ostr;
        }

        value = _msg.com_type_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 无线通讯类型: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.com_type_);
        os << ostr;

        value = _msg.pos_confidence_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 定位精度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.pos_confidence_);
        os << ostr;

        value = _msg.time_sync_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 时间同步方式: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.time_sync_);
        os << ostr;

        value = _msg.gnss_type_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 坐标系类型: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.gnss_type_);
        os << ostr;

        value = _msg.user_data_len_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 自定义字段长度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.user_data_len_);
        os << ostr;

        if (0 != _msg.user_data_len_ && 0xFF !=  _msg.user_data_len_)
        {
            len = _msg.user_data_len_;
            bytes_to_string(_msg.user_data_.c_str(), len, str);
            sprintf(ostr, "%-4ld[%s] 自定义字段内容: %s\n", offset, str, _msg.user_data_.c_str());
            offset += len;
            os << ostr;
        }

        return os;
    }

    static constexpr const char *TAG = "protocol::Veh2CloudInh";

    char        vehicle_id_[8] = "";
    uint8_t     sw_ver_len_;
    std::string sw_ver_;
    uint8_t     hw_ver_len_;
    std::string hw_ver_;
    uint8_t     ad_ver_len_;
    std::string ad_ver_;
    uint8_t     com_type_;
    uint8_t     pos_confidence_;
    uint8_t     time_sync_;
    uint8_t     gnss_type_;
    uint8_t     user_data_len_;
    std::string user_data_;
};

/**
 * CLOUD2VEH_INH_RES.
 */
struct Cloud2VehInhRes: public MessageHeader
{
    Cloud2VehInhRes(
        const uint8_t  _version,
        const uint64_t _timestamp,
        const uint8_t  _ctrl, 
        const std::string &_vehicle_id,
        const uint8_t _res):
            MessageHeader(sizeof(Cloud2VehInhRes) - sizeof(MessageHeader), 
            CLOUD2VEH_INH_RES, _version, _timestamp, _ctrl), res_(_res)
    {
        strncpy(vehicle_id_, _vehicle_id.c_str(), sizeof(vehicle_id_));
    }

    Cloud2VehInhRes(const void *_buf, const size_t _size, const bool _big_endian = true): 
        MessageHeader(_buf, _size, _big_endian)
    {
        size_t offset = get_header_length();

        if (_size - offset < get_data_length())
        {
            LOGE(TAG, "Cloud2VehInhRes: Invalid size %ld, offset %ld, data length %d!\n", _size, offset, get_data_length());
            return;
        }
        
        char *buf = (char*)_buf;

        memcpy(vehicle_id_, buf + offset, sizeof(vehicle_id_));
        offset += sizeof(vehicle_id_);

        res_ = *(uint8_t*)(buf + offset);
        offset += sizeof(res_);
    }

    size_t to_bytes(void *_buf, const size_t _size, const bool _big_endian = true) const
    {
        size_t offset = MessageHeader::to_bytes(_buf, _size);

        if (_size - offset < get_data_length())
        {
            LOGE(TAG, "to_bytes: Invalid size %ld, offset %ld, data length %d!\n", _size, offset, get_data_length());
            return offset;
        }

        uint8_t *buf = (uint8_t*)_buf;

        memcpy(buf + offset, vehicle_id_, sizeof(vehicle_id_));
        offset += sizeof(vehicle_id_);

        *(uint8_t *)(buf + offset) = res_;
        offset += sizeof(res_);

        return offset;
    }  

    friend std::ostream& operator<<(std::ostream &os, const Cloud2VehInhRes &_msg)
    {
        os << (const MessageHeader&)_msg;

        uint64_t value = 0;
        size_t offset = _msg.get_header_length();
        size_t len = 0;
        char str[256] = "";
        char ostr[512] = "";

        len = sizeof(_msg.vehicle_id_);
        bytes_to_string(_msg.vehicle_id_, len, str);
        sprintf(ostr, "%-4ld[%s] 车辆编号: %s\n", offset, str, std::string(_msg.vehicle_id_, len).c_str());
        offset += len;
        os << ostr;

        value = _msg.res_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 执行标志: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.res_);
        os << ostr;

        return os;
    }

    static constexpr const char *TAG = "protocol::Cloud2VehInhRes";

    char        vehicle_id_[8];
    uint8_t     res_;
};
#pragma pack()
} // namespace protocal

#endif // __PROTOCOL_VEH2CLOUD_INH_H__