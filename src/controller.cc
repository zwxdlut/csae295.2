#include "controller.h"
#include "log.h"
#include "util.h"

Controller::~Controller()
{
    stop();
}

void Controller::set_callback(Callback *_callback)
{
    callback_ = _callback;

    up_sock_.set_connect_state_callback([](const socketlib::ConnectState _state, void *_param)
    {
        Controller *p = (Controller*)_param;

        if (nullptr != p->callback_)
        {
            p->callback_->on_up_connect_state(_state);
        }
    }, this);

    down_sock_.set_connect_state_callback([](const socketlib::ConnectState _state, void *_param)
    {
        Controller *p = (Controller*)_param;

        if (nullptr != p->callback_)
        {
            p->callback_->on_down_connect_state(_state);
        }
    }, this);
}

void Controller::start(const char _up_addr[], const uint32_t _up_port, const char _down_addr[], const uint32_t _down_port)
{
    stopped = false;

    // upstream

    // create socket and connect it to server
    up_sock_.open(_up_addr, _up_port);
    
    // receive thread
    up_recv_thread_ = std::thread([this]()
    {
        this->up_sock_recv_thread();
    });

    // send thread
    up_send_thread_ = std::thread([this]()
    {
        this->up_sock_send_thread();
    });

    // downstream

    // create socket and connect it to server
    down_sock_.open(_down_addr, _down_port);
    
    // receive thread
    down_recv_thread_ = std::thread([this]()
    {
        this->down_sock_recv_thread();
    });

    // send thread
    down_send_thread_ = std::thread([this]()
    {
        this->down_sock_send_thread();
    });
}

void Controller::stop()
{
    if (stopped)
    {
        return;
    }

    stopped = true;

    up_sock_.close();
    up_recv_thread_.join();
    up_send_queue_.notify();
    up_send_thread_.join();

    down_sock_.close();
    down_recv_thread_.join();
    down_send_queue_.notify();
    down_send_thread_.join();
}

// message

void Controller::send(const MessageHeader &_msg)
{
    auto buf = Packer::pack(_msg);
    down_send_queue_.put(buf); 
}

void Controller::send(const Veh2CloudInh &_msg)
{
    auto buf = Packer::pack(_msg);
    up_send_queue_.put(buf); 
}

void Controller::send(const Cloud2VehInhRes &_msg)
{
    auto buf = Packer::pack(_msg);
    up_send_queue_.put(buf); 
}

void Controller::send(const Veh2CloudState &_msg)
{
    auto buf = Packer::pack(_msg);
    up_send_queue_.put(buf); 
}

// virtual methods implementation

void Controller::on_unpack(const MessageHeader &_msg)
{
    std::cout << std::endl << _msg;

    if (nullptr == callback_)
    {
        return;
    }

    switch (_msg.data_type_)
    {
    case HEARTBEAT_RES:
        // TODO: 
        break;
    
    default:
        break;
    }
}

void Controller::on_unpack(const Veh2CloudInh &_msg)
{
    std::cout << std::endl << _msg;
}

void Controller::on_unpack(const Cloud2VehInhRes &_msg)
{
    std::cout << std::endl << _msg;

    if (nullptr != callback_)
    {
        callback_->on_cloud2veh_inh_res(_msg);
    }
}

void Controller::on_unpack(const Veh2CloudState &_msg)
{
    std::cout << std::endl << _msg;
}

// private

// upstream

void Controller::up_sock_recv_thread()
{
    uint8_t buf[4096] = {0};
    ssize_t size = 0;

    while (!stopped)
    {
        size = up_sock_.recv(buf, sizeof(buf));

        if (0 == size)
        {
            LOGW(TAG, "up_sock_recv_thread: remote shutdown, exit!\n");
            return;
        }
        else if (0 > size)
        {
            if (EAGAIN == errno || EWOULDBLOCK == errno || EINTR == errno)
            {
                LOGW(TAG, "up_sock_recv_thread: continue receive!\n");
                continue;
            }
            else
            {
                LOGE(TAG, "up_sock_recv_thread: receive error, exit!\n");
                return;
            }
        }
        
        printf("\n");
        print_buffer("SOCK-RX(UP)", 0, buf, size);
        Packer::unpack(buf, size, *this);
    }
}

void Controller::up_sock_send_thread()
{
    while (!stopped)
    {	
        auto p = up_send_queue_.take();

        if (nullptr == p)
        {
            continue;
        }
        
        printf("\n");
        print_buffer("SOCK-TX(UP)", 0, p->data, p->size);
        up_sock_.send(p->data, p->size);
        up_send_queue_.pull();
    }
}

// downstream

void Controller::down_sock_recv_thread()
{
    uint8_t buf[4096] = {0};
    ssize_t size = 0;

    while (!stopped)
    {
        size = down_sock_.recv(buf, sizeof(buf));

        if (0 == size)
        {
            LOGW(TAG, "down_sock_recv_thread: remote shutdown, exit!\n");
            return;
        }
        else if (0 > size)
        {
            if (EAGAIN == errno || EWOULDBLOCK == errno || EINTR == errno)
            {
                LOGW(TAG, "down_sock_recv_thread: continue receive!\n");
                continue;
            }
            else
            {
                LOGE(TAG, "down_sock_recv_thread: receive error, exit!\n");
                return;
            }
        }
        
        printf("\n");
        print_buffer("SOCK-RX(DOWN)", 0, buf, size);
        Packer::unpack(buf, size, *this);
    }
}

void Controller::down_sock_send_thread()
{
    while (!stopped)
    {	
        auto p = down_send_queue_.take();

        if (nullptr == p)
        {
            continue;
        }
        
        printf("\n");
        print_buffer("SOCK-TX(DOWN)",0, p->data, p->size);
        down_sock_.send(p->data, p->size);
        down_send_queue_.pull();
    }
}
