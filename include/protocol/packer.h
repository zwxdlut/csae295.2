#ifndef __PROTOCOL_PACKER_H__
#define __PROTOCOL_PACKER_H__

#include <string>  
#include <memory>
#include <vector>
#include <type_traits>

#include "veh2cloud_inh.h"
#include "veh2cloud_state.h"

namespace protocol
{
/**
 * Message codec.
 */
class Packer
{
public:
    /**
     * Packer handler.
     */
    class Handler
    {
    public:
        virtual ~Handler() {}

        virtual void on_unpack(const MessageHeader &_msg) {}

        virtual void on_unpack(const Veh2CloudInh &_msg) {}
    
        virtual void on_unpack(const Cloud2VehInhRes &_msg) {}

        virtual void on_unpack(const Veh2CloudState &_msg) {}
    };

    template<typename T , typename std::enable_if<std::is_base_of<MessageHeader, T>::value>::type* = nullptr>
    static std::shared_ptr<MessageBuffer> pack(const T &_t)
    {
        size_t size = _t.get_header_length() + _t.get_data_length();
        std::vector<uint8_t> v;

        // pack message
        uint8_t msg[size];
        size = _t.to_bytes(msg, size);
        v.insert(v.end(), msg, msg + size);

        // copy memory
        size = v.size();
        std::shared_ptr<MessageBuffer> p((MessageBuffer*)new uint8_t[size + sizeof(MessageBuffer)], MessageBuffer::deleter<MessageBuffer>);
        p->size = size;
        memcpy(p->data, v.data(), size);

        return p;
    }

    static void unpack(const void *_buf, const size_t _size, Handler &_handler);

private:
    static constexpr const char *TAG = "protocol::Packer";
};
} // namespace protocal

#endif // __PROTOCOL_PACKER_H__