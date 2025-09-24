#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "timer.h"
#include "block_queue.h"
#include "packer.h"
#include "socketlib.h"

using namespace protocol;

/**
 * Controller.
 */
class Controller : public Packer::Handler
{
public:
    /**
     * Controller callback.
     */
    class Callback
    {
    public:
        virtual ~Callback() {}

        virtual void on_up_connect_state(const socketlib::ConnectState _state) {};

        virtual void on_down_connect_state(const socketlib::ConnectState _state) {};

        virtual void on_cloud2veh_inh_res(const Cloud2VehInhRes &_msg) {};

        // message
    };

    ~Controller();

    void set_callback(Callback *_callback);

    void start(const char _addr[], const uint32_t _port, const char _down_addr[], const uint32_t _down_port);
    
    void stop();

    // message

    void send(const MessageHeader &_msg);

    void send(const Veh2CloudInh &_msg);

    void send(const Cloud2VehInhRes &_msg);

    void send(const Veh2CloudState &_msg);

    // virtual methods implementation

    void on_unpack(const MessageHeader &_msg) override;

    void on_unpack(const Veh2CloudInh &_msg) override;

    void on_unpack(const Cloud2VehInhRes &_msg) override;

    void on_unpack(const Veh2CloudState &_msg) override;

private:
    // upstream

    void up_sock_recv_thread();

    void up_sock_send_thread();

    // downstream

    void down_sock_recv_thread();

    void down_sock_send_thread();

    static constexpr const char *TAG = "Controller";

    bool stopped = true;
    uint16_t serial_ = 0;

    Callback *callback_ = nullptr;

    // upstream
    socketlib::Client up_sock_;
    std::thread  up_recv_thread_;
    std::thread  up_send_thread_;
    BlockQueue<std::shared_ptr<protocol::MessageBuffer>> up_send_queue_;

    // downstream
    socketlib::Client down_sock_;
    std::thread down_recv_thread_;
    std::thread down_send_thread_;
    BlockQueue<std::shared_ptr<protocol::MessageBuffer>> down_send_queue_;
};

#endif // __CONTROLLER_H__
