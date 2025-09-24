#ifndef __PROTOCOL_VEH2CLOUD_STATE_H__
#define __PROTOCOL_VEH2CLOUD_STATE_H__

#include <algorithm>

#include "message.h"

#define BREAK_FLAG_UP      0
#define BREAK_FLAG_DOWN    1
#define BREAK_FLAG_DEFAULT 0xFF

#define DRIVE_MODE_MANUAL           1
#define DRIVE_MODE_AUTO             2
#define DRIVE_MODE_CLOUD_MANUAL     3 
#define DRIVE_MODE_CLOUD_AUTO       4 
#define DRIVE_MODE_NO_DIRVER_MANUAL 5 
#define DRIVE_MODE_DETACH           6
#define DRIVE_MODE_NONE             7
#define DRIVE_MODE_REMOTE           8 
#define DRIVE_MODE_NOT_DEFINED      9 
#define DRIVE_MODE_DEFAULT          0xFF

namespace protocol
{
#pragma pack(1)

struct Position
{
    Position() {}
    Position(const uint32_t _longitude, const uint32_t _latitude, const uint32_t _elevation):
        longitude(_longitude), latitude(_latitude), elevation(_elevation) {}

    uint32_t longitude;
    uint32_t latitude;
    uint32_t elevation;
};

struct Position2D
{
    Position2D() {}
    Position2D(const uint32_t _longitude, const uint32_t _latitude):
        longitude(_longitude), latitude(_latitude) {}

    uint32_t longitude;
    uint32_t latitude;
};

/**
 * VEH2CLOUD_STATE.
 */
struct Veh2CloudState: public MessageHeader
{
    Veh2CloudState(
        const uint8_t  _version,
        const uint64_t _timestamp,
        const uint8_t _ctrl, 
        const std::string &_vehicle_id,
        const std::vector<uint8_t> &_message_id,
        const uint64_t _gnss_timestamp,
        const uint16_t _gnss_velocity,
        const Position &_position,
        const uint32_t _heading,
        const uint8_t  _gear,
        const uint32_t _steering_angle,
        const uint16_t _velocity,
        const uint16_t _acc_lon,
        const uint16_t _acc_lat,
        const uint16_t _acc_ver,
        const uint16_t _yaw_rate,
        const uint16_t _accel_pos,
        const uint16_t _engine_speed,
        const uint32_t _engine_torque,
        const uint8_t  _break_flag,
        const uint16_t _break_pos,
        const uint16_t _break_pressure,
        const uint16_t _fuel_consume,
        const uint8_t _drive_mode,
        const Position2D &_dest_location,
        const std::vector<Position2D> &_pass_pos):
            MessageHeader(sizeof(Veh2CloudState) - sizeof(MessageHeader) - sizeof(pass_pos_) + _pass_pos.size(), VEH2CLOUD_STATE, _version, _timestamp, _ctrl), 
            gnss_timestamp_(_gnss_timestamp), gnss_velocity_(_gnss_velocity), position_(_position), heading_(_heading), gear_(_gear), 
            steering_angle_(_steering_angle), velocity_(_velocity), acc_lon_(_acc_lon), acc_lat_(_acc_lat), acc_ver_(_acc_ver), yaw_rate_(_yaw_rate), accel_pos_(_accel_pos), 
            engine_speed_(_engine_speed), engine_torque_(_engine_torque), break_flag_(_break_flag), break_pos_(_break_pos), break_pressure_(_break_pressure), 
            fuel_consume_(_fuel_consume), drive_mode_(_drive_mode), dest_location_(_dest_location), pass_pos_num_(_pass_pos.size()), pass_pos_(_pass_pos)
    {
        strncpy(vehicle_id_, _vehicle_id.c_str(), sizeof(vehicle_id_));

        size_t n = sizeof(message_id_) <= _message_id.size() ? sizeof(message_id_) : _message_id.size();
        memcpy(message_id_, _message_id.data(), n);
    }

    Veh2CloudState(const void *_buf, const size_t _size, const bool _big_endian = true): 
        MessageHeader(_buf, _size, _big_endian)
    {
        size_t offset = get_header_length();

        if (_size - offset < get_data_length())
        {
            LOGE(TAG, "Veh2CloudState: Invalid size %ld, offset %ld, data length %d!\n", _size, offset, get_data_length());
            return;
        }
        
        char *buf = (char*)_buf;

        memcpy(vehicle_id_, buf + offset, sizeof(vehicle_id_));
        offset += sizeof(vehicle_id_);

        std::vector<uint8_t> v(buf + offset, buf + offset + sizeof(message_id_));
        if (_big_endian)
        {
            std::reverse(v.begin(), v.end());
        }
        memcpy(message_id_, v.data(), sizeof(message_id_));
        offset += sizeof(message_id_);

        gnss_timestamp_ = _big_endian ? __builtin_bswap64(*(uint64_t*)(buf + offset)) : *(uint64_t*)(buf + offset);
        offset += sizeof(gnss_timestamp_);

        gnss_velocity_ = _big_endian ? __builtin_bswap16(*(uint16_t*)(buf + offset)) : *(uint16_t*)(buf + offset);
        offset += sizeof(gnss_velocity_);

        position_.longitude = _big_endian ? __builtin_bswap32(*(uint32_t*)(buf + offset)) : *(uint32_t*)(buf + offset);
        offset += sizeof(position_.longitude);

        position_.latitude = _big_endian ? __builtin_bswap32(*(uint32_t*)(buf + offset)) : *(uint32_t*)(buf + offset);
        offset += sizeof(position_.latitude);

        position_.elevation = _big_endian ? __builtin_bswap32(*(uint32_t*)(buf + offset)) : *(uint32_t*)(buf + offset);
        offset += sizeof(position_.elevation);

        heading_ = _big_endian ? __builtin_bswap32(*(uint32_t*)(buf + offset)) : *(uint32_t*)(buf + offset);
        offset += sizeof(heading_);

        gear_ = *(uint8_t*)(buf + offset);
        offset += sizeof(gear_);

        steering_angle_ = _big_endian ? __builtin_bswap32(*(uint32_t*)(buf + offset)) : *(uint32_t*)(buf + offset);
        offset += sizeof(steering_angle_);

        velocity_ = _big_endian ? __builtin_bswap16(*(uint16_t*)(buf + offset)) : *(uint16_t*)(buf + offset);
        offset += sizeof(velocity_);

        acc_lon_ = _big_endian ? __builtin_bswap16(*(uint16_t*)(buf + offset)) : *(uint16_t*)(buf + offset);
        offset += sizeof(acc_lon_);

        acc_lat_ = _big_endian ? __builtin_bswap16(*(uint16_t*)(buf + offset)) : *(uint16_t*)(buf + offset);
        offset += sizeof(acc_lat_);

        acc_ver_ = _big_endian ? __builtin_bswap16(*(uint16_t*)(buf + offset)) : *(uint16_t*)(buf + offset);
        offset += sizeof(acc_ver_);
    
        yaw_rate_ = _big_endian ? __builtin_bswap16(*(uint16_t*)(buf + offset)) : *(uint16_t*)(buf + offset);
        offset += sizeof(yaw_rate_);

        accel_pos_ = _big_endian ? __builtin_bswap16(*(uint16_t*)(buf + offset)) : *(uint16_t*)(buf + offset);
        offset += sizeof(accel_pos_);

        engine_speed_ = _big_endian ? __builtin_bswap16(*(uint16_t*)(buf + offset)) : *(uint16_t*)(buf + offset);
        offset += sizeof(engine_speed_);

        engine_torque_ = _big_endian ? __builtin_bswap32(*(uint32_t*)(buf + offset)) : *(uint32_t*)(buf + offset);
        offset += sizeof(engine_torque_);

        break_flag_ = *(uint8_t*)(buf + offset);
        offset += sizeof(break_flag_);

        break_pos_ = _big_endian ? __builtin_bswap16(*(uint16_t*)(buf + offset)) : *(uint16_t*)(buf + offset);
        offset += sizeof(break_pos_);

        break_pressure_ = _big_endian ? __builtin_bswap16(*(uint16_t*)(buf + offset)) : *(uint16_t*)(buf + offset);
        offset += sizeof(break_pressure_);

        fuel_consume_ = _big_endian ? __builtin_bswap16(*(uint16_t*)(buf + offset)) : *(uint16_t*)(buf + offset);
        offset += sizeof(fuel_consume_);

        drive_mode_ = *(uint8_t*)(buf + offset);
        offset += sizeof(drive_mode_);

        dest_location_.longitude = _big_endian ? __builtin_bswap32(*(uint32_t*)(buf + offset)) : *(uint32_t*)(buf + offset);
        offset += sizeof(dest_location_.longitude);

        dest_location_.latitude = _big_endian ? __builtin_bswap32(*(uint32_t*)(buf + offset)) : *(uint32_t*)(buf + offset);
        offset += sizeof(dest_location_.latitude);

        pass_pos_num_ = *(uint8_t*)(buf + offset);
        offset += sizeof(pass_pos_num_);

        for (size_t i = 0; i < pass_pos_num_; i++)
        {
            Position2D pos;

            pos.longitude = _big_endian ? __builtin_bswap32(*(uint32_t*)(buf + offset)) : *(uint32_t*)(buf + offset);
            offset += sizeof(pos.longitude);

            pos.latitude = _big_endian ? __builtin_bswap32(*(uint32_t*)(buf + offset)) : *(uint32_t*)(buf + offset);
            offset += sizeof(pos.latitude);

            pass_pos_.emplace_back(pos);
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

        std::vector<uint8_t> v(message_id_, message_id_ + sizeof(message_id_));
        if (_big_endian)
        {
            std::reverse(v.begin(), v.end());
        }
        memcpy(buf + offset, v.data(), sizeof(message_id_));
        offset += sizeof(message_id_);

        *(uint64_t *)(buf + offset) = _big_endian ? __builtin_bswap64(gnss_timestamp_) : gnss_timestamp_;
        offset += sizeof(gnss_timestamp_);

        *(uint16_t *)(buf + offset) = _big_endian ? __builtin_bswap16(gnss_velocity_) : gnss_velocity_;
        offset += sizeof(gnss_velocity_);

        *(uint32_t *)(buf + offset) = _big_endian ? __builtin_bswap32(position_.longitude) : position_.longitude;
        offset += sizeof(position_.longitude); 

        *(uint32_t *)(buf + offset) = _big_endian ? __builtin_bswap32(position_.latitude) : position_.latitude;
        offset += sizeof(position_.latitude);

        *(uint32_t *)(buf + offset) = _big_endian ? __builtin_bswap32(position_.elevation) : position_.elevation;
        offset += sizeof(position_.elevation); 

        *(uint32_t *)(buf + offset) = _big_endian ? __builtin_bswap32(heading_) : heading_;
        offset += sizeof(heading_); 

        *(uint8_t *)(buf + offset) = gear_;
        offset += sizeof(gear_);

        *(uint32_t *)(buf + offset) = _big_endian ? __builtin_bswap32(steering_angle_) : steering_angle_;
        offset += sizeof(steering_angle_);

        *(uint16_t *)(buf + offset) = _big_endian ? __builtin_bswap16(velocity_) : velocity_;
        offset += sizeof(velocity_);

        *(uint16_t *)(buf + offset) = _big_endian ? __builtin_bswap16(acc_lon_) : acc_lon_;
        offset += sizeof(acc_lon_);

        *(uint16_t *)(buf + offset) = _big_endian ? __builtin_bswap16(acc_lat_) : acc_lat_;
        offset += sizeof(acc_lat_);

        *(uint16_t *)(buf + offset) = _big_endian ? __builtin_bswap16(acc_ver_) : acc_ver_;
        offset += sizeof(acc_ver_);

        *(uint16_t *)(buf + offset) = _big_endian ? __builtin_bswap16(yaw_rate_) : yaw_rate_;
        offset += sizeof(yaw_rate_);

        *(uint16_t *)(buf + offset) = _big_endian ? __builtin_bswap16(accel_pos_) : accel_pos_;
        offset += sizeof(accel_pos_);

        *(uint16_t *)(buf + offset) = _big_endian ? __builtin_bswap16(engine_speed_) : engine_speed_;
        offset += sizeof(engine_speed_);

        *(uint32_t *)(buf + offset) = _big_endian ? __builtin_bswap32(engine_torque_) : engine_torque_;
        offset += sizeof(engine_torque_);

        *(uint8_t *)(buf + offset) = break_flag_;
        offset += sizeof(break_flag_);

        *(uint16_t *)(buf + offset) = _big_endian ? __builtin_bswap16(break_pos_) : break_pos_;
        offset += sizeof(break_pos_);

        *(uint16_t *)(buf + offset) = _big_endian ? __builtin_bswap16(break_pressure_) : break_pressure_;
        offset += sizeof(break_pressure_);

        *(uint16_t *)(buf + offset) = _big_endian ? __builtin_bswap16(fuel_consume_) : fuel_consume_;
        offset += sizeof(fuel_consume_);

        *(uint8_t *)(buf + offset) = drive_mode_;
        offset += sizeof(drive_mode_);

        *(uint32_t *)(buf + offset) = _big_endian ? __builtin_bswap32(dest_location_.longitude) : dest_location_.longitude;
        offset += sizeof(dest_location_.longitude); 

        *(uint32_t *)(buf + offset) = _big_endian ? __builtin_bswap32(dest_location_.latitude) : dest_location_.latitude;
        offset += sizeof(dest_location_.latitude);

        *(uint8_t *)(buf + offset) = pass_pos_num_;
        offset += sizeof(pass_pos_num_);

        for (size_t i = 0; i < pass_pos_num_; i++)
        {
            *(uint32_t *)(buf + offset) = _big_endian ? __builtin_bswap32(pass_pos_[i].longitude) : pass_pos_[i].longitude;
            offset += sizeof(pass_pos_[i].longitude); 

            *(uint32_t *)(buf + offset) = _big_endian ? __builtin_bswap32(pass_pos_[i].latitude) : pass_pos_[i].latitude;
            offset += sizeof(pass_pos_[i].latitude);
        }

        return offset;
    }  

    friend std::ostream& operator<<(std::ostream &os, const Veh2CloudState &_msg)
    {
        os << (const MessageHeader&)_msg;

        uint64_t value = 0;
        size_t offset = _msg.get_header_length();
        size_t len = 0;
        char str[256] = "";
        char ostr[1024] = "";

        len = sizeof(_msg.vehicle_id_);
        bytes_to_string(_msg.vehicle_id_, len, str);
        sprintf(ostr, "%-4ld[%s] 车辆编号: %s\n", offset, str, std::string(_msg.vehicle_id_, len).c_str());
        offset += len;
        os << ostr;

        len = sizeof(_msg.message_id_);
        bytes_to_string(_msg.message_id_, len, str);
        std::string tmp(str);
        std::reverse(tmp.begin(), tmp.end());
        sprintf(ostr, "%-4ld[%s] 消息编号: %s\n", offset, tmp.c_str(), tmp.c_str());
        offset += len;
        os << ostr;

        value = _msg.gnss_timestamp_;
        sprintf(ostr, "%-4ld[%016" PRIX64 "] GNSS时间戳: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.gnss_timestamp_);
        os << ostr;

        value = _msg.gnss_velocity_;
        sprintf(ostr, "%-4ld[%04" PRIX64 "] GNSS速度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.gnss_velocity_);
        os << ostr;

        value = _msg.position_.longitude;
        sprintf(ostr, "%-4ld[%08" PRIX64 "] 位置-经度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.position_.longitude);
        os << ostr;

        value = _msg.position_.latitude;
        sprintf(ostr, "%-4ld[%08" PRIX64 "] 位置-纬度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.position_.latitude);
        os << ostr;

        value = _msg.position_.elevation;
        sprintf(ostr, "%-4ld[%08" PRIX64 "] 位置-高程: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.position_.elevation);
        os << ostr;

        value = _msg.heading_;
        sprintf(ostr, "%-4ld[%08" PRIX64 "] 航向角: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.heading_);
        os << ostr;

        value = _msg.gear_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 档位: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.gear_);
        os << ostr;

        value = _msg.steering_angle_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 方向盘转角: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.steering_angle_);
        os << ostr;

        value = _msg.velocity_;
        sprintf(ostr, "%-4ld[%04" PRIX64 "] 当前车速: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.velocity_);
        os << ostr;

        value = _msg.acc_lon_;
        sprintf(ostr, "%-4ld[%04" PRIX64 "] 纵向加速度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.acc_lon_);
        os << ostr;

        value = _msg.acc_lat_;
        sprintf(ostr, "%-4ld[%04" PRIX64 "] 横向加速度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.acc_lat_);
        os << ostr;

        value = _msg.acc_ver_;
        sprintf(ostr, "%-4ld[%04" PRIX64 "] 垂向加速度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.acc_ver_);
        os << ostr;

        value = _msg.yaw_rate_;
        sprintf(ostr, "%-4ld[%04" PRIX64 "] 横摆角速度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.yaw_rate_);
        os << ostr;

        value = _msg.accel_pos_;
        sprintf(ostr, "%-4ld[%04" PRIX64 "] 油门开度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.accel_pos_);
        os << ostr;

        value = _msg.engine_speed_;
        sprintf(ostr, "%-4ld[%04" PRIX64 "] 发动机输出转速: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.engine_speed_);
        os << ostr;

        value = _msg.engine_torque_;
        sprintf(ostr, "%-4ld[%08" PRIX64 "] 发动机扭矩: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.engine_torque_);
        os << ostr;

        value = _msg.break_flag_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 制动踏板开关: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.break_flag_);
        os << ostr;

        value = _msg.break_pos_;
        sprintf(ostr, "%-4ld[%04" PRIX64 "] 制动踏板开度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.break_pos_);
        os << ostr;

        value = _msg.break_pressure_;
        sprintf(ostr, "%-4ld[%04" PRIX64 "] 制动主缸压力: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.break_pressure_);
        os << ostr;

        value = _msg.fuel_consume_;
        sprintf(ostr, "%-4ld[%04" PRIX64 "] 油耗: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.fuel_consume_);
        os << ostr;

        value = _msg.drive_mode_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 车辆驾驶模式: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.drive_mode_);
        os << ostr;

        value = _msg.dest_location_.longitude;
        sprintf(ostr, "%-4ld[%08" PRIX64 "] 目的地位置-经度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.dest_location_.longitude);
        os << ostr;

        value = _msg.dest_location_.latitude;
        sprintf(ostr, "%-4ld[%08" PRIX64 "] 目的地位置-纬度: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.dest_location_.latitude);
        os << ostr;

        value = _msg.pass_pos_num_;
        sprintf(ostr, "%-4ld[%02" PRIX64 "] 途经点数量: %" PRIu64 "\n", offset, value, value);
        offset += sizeof(_msg.pass_pos_num_);
        os << ostr;

        for (size_t i = 0; i < _msg.pass_pos_num_; i++)
        {
            value = _msg.pass_pos_[i].longitude;
            sprintf(ostr, "%-4ld[%08" PRIX64 "] 途经点%ld-经度: %" PRIu64 "\n", offset, value, i, value);
            offset += sizeof(_msg.pass_pos_[i].longitude);
            os << ostr;

            value = _msg.pass_pos_[i].latitude;
            sprintf(ostr, "%-4ld[%08" PRIX64 "] 途经点%ld-纬度: %" PRIu64 "\n", offset, value, i, value);
            offset += sizeof(_msg.pass_pos_[i].latitude);
            os << ostr;
        }

        return os;
    }

    static constexpr const char *TAG = "protocol::Veh2CloudState";

    char                    vehicle_id_[8] = "";
    uint8_t                 message_id_[8] = {0};
    uint64_t                gnss_timestamp_;
    uint16_t                gnss_velocity_;
    Position                position_;
    uint32_t                heading_;
    uint8_t                 gear_;
    uint32_t                steering_angle_;
    uint16_t                velocity_;
    uint16_t                acc_lon_;
    uint16_t                acc_lat_;
    uint16_t                acc_ver_;
    uint16_t                yaw_rate_;
    uint16_t                accel_pos_;
    uint16_t                engine_speed_;
    uint32_t                engine_torque_;
    uint8_t                 break_flag_;
    uint16_t                break_pos_;
    uint16_t                break_pressure_;
    uint16_t                fuel_consume_;
    uint8_t                 drive_mode_;
    Position2D              dest_location_;
    uint8_t                 pass_pos_num_;
    std::vector<Position2D> pass_pos_;

};
#pragma pack()
} // namespace protocal

#endif // __PROTOCOL_VEH2CLOUD_STATE_H__