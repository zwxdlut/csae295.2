#include "packer.h"
#include "log.h"

namespace protocol
{
void Packer::unpack(const void *_buf, const size_t _size, Handler &_handler)
{
    if (nullptr == _buf || 0 == _size)
    {
        LOGE(TAG, "unpack: buffer is null or _size is 0!\n");
        return;
    }

    uint8_t *buf = (uint8_t*)_buf;
    if (0xF2 != buf[0])
    {   
        LOGE(TAG, "unpack: No identifier!\n");
        return;
    }
    
    // unpack message
    uint8_t data_type = *(uint8_t*)(buf + 5);

    switch (data_type)
    {
    case VEH2CLOUD_INH:
    {
        Veh2CloudInh msg(buf, _size);
        _handler.on_unpack(msg);
        break;
    }

    case CLOUD2VEH_INH_RES:
    {
        Cloud2VehInhRes msg(buf, _size);
        _handler.on_unpack(msg);
        break;
    }

    case VEH2CLOUD_STATE:
    {
        Veh2CloudState msg(buf, _size);
        _handler.on_unpack(msg);
        break;
    }

    case HEARTBEAT:
    {
        MessageHeader msg(buf, _size);
        _handler.on_unpack(msg);
        break;
    }

    case HEARTBEAT_RES:
    {
        MessageHeader msg(buf, _size);
        _handler.on_unpack(msg);
        break;
    }

    default:
        break;
    }
}
} // namespace protocal


