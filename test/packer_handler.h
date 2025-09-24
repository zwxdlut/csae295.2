#ifndef __PROTOCOL_PACKER_HANDLER_H__
#define __PROTOCOL_PACKER_HANDLER_H__

#include "packer.h"

namespace protocol
{
class PackerHandler : public Packer::Handler
{
public:
    void on_unpack(const MessageHeader &_msg) override
    {
        std::cout << std::endl << _msg;
    }

    void on_unpack(const Veh2CloudInh &_msg) override
    {
        std::cout << std::endl << _msg;
    }

    void on_unpack(const Cloud2VehInhRes &_msg) override
    {
        std::cout << std::endl << _msg;
    }

    void on_unpack(const Veh2CloudState &_msg) override
    {
        std::cout << std::endl << _msg;
    }
};
} // namespace protocal

#endif // __PROTOCOL_PACKER_HANDLER_H__
