#include <stdio.h>

#include "controller.h"
#include "util.h"

#define UP_SERVER_ADDRESS   "60.16.59.129"
#define UP_SERVER_PORT      50011
#define DOWN_SERVER_ADDRESS "60.16.59.129"
#define DOWN_SERVER_PORT    50012

Controller g_controller;
Timer g_hb_timer;
Timer g_state_timer;
Timer g_inh_timer;
bool g_inh_res = false;
size_t g_inh_count = 0;

class ControllerCallback: public Controller::Callback
{
public:
    void on_up_connect_state(const socketlib::ConnectState _state)
    {
        LOGD(TAG, "on_up_connect_state: state %d\n", _state);

        if (socketlib::ConnectState::CONNECTED == _state)
        {
            LOGD(TAG, "on_up_connect_state: The connection is established.\n");

            g_inh_timer.start(1000, [](void *_param)
            {
                 if (3 <= g_inh_count)
                {
                    LOGE("veh2cloud_inh", "connection exception! VEH2CLOUD_INH count %zu", g_inh_count);
                    g_inh_timer.stop();
                    return;
                }

                Veh2CloudInh msg(
                    0x01, get_utc_timestamp_ms(), 0xFC, "Q1001", "sw_v1.0", "hw_v1.0", "ad_v1.0", 
                    COMM_TYPE_4G, 15, TIME_SYNC_GNSS, GNSS_TYPE_GCJ02, "VEH2CLOUD_INH");
                g_controller.send(msg);
                g_inh_count++;
            }, nullptr);

            g_state_timer.start(200, [](void *_param)
            {
                Veh2CloudState msg(
                    0x01, get_utc_timestamp_ms(), 0xFC, "Q1001", std::vector<uint8_t>{1}, get_utc_timestamp_ms(), 
                    4000, Position(90, 90, 700), 100000, 31, 200000, 4100, 500, 400, 300, 200, 500, 3000, 50000, 
                    1, 500, 20000, 1000, 1, Position2D(80, 80), std::vector<Position2D>());
                g_controller.send(msg);
            }, nullptr);
        }
        else
        {
            LOGW(TAG, "on_up_connect_state: The connection is lost, and a reconnection is scheduled in 5 seconds.\n");

            g_hb_timer.stop();
            g_inh_timer.stop();
            g_state_timer.stop();
            g_inh_res = false;
            g_inh_count = 0;
            g_controller.stop();

            std::thread t([]()
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                g_controller.start(UP_SERVER_ADDRESS, UP_SERVER_PORT, DOWN_SERVER_ADDRESS, DOWN_SERVER_PORT);
            });
            t.detach();
        }
    }

    void on_down_connect_state(const socketlib::ConnectState _state)
    {
        LOGD(TAG, "on_down_connect_state: state %d\n", _state);

        if (socketlib::ConnectState::CONNECTED == _state)
        {
            LOGD(TAG, "on_down_connect_state: The connection is established.\n");

            g_hb_timer.start(60000, [](void *_param)
            {
                MessageHeader msg(0, HEARTBEAT, 0x01, get_utc_timestamp_ms(), 0xFC);
                g_controller.send(msg);
            }, nullptr);
        }
        else
        {
            LOGW(TAG, "on_down_connect_state: The connection is lost, and a reconnection is scheduled in 5 seconds.\n");

            g_hb_timer.stop();
            g_state_timer.stop();
            g_inh_timer.stop();
            g_inh_res = false;
            g_inh_count = 0;
            g_controller.stop();

            std::thread t([]()
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                g_controller.start(UP_SERVER_ADDRESS, UP_SERVER_PORT, DOWN_SERVER_ADDRESS, DOWN_SERVER_PORT);
            });
            t.detach();
        }
    }

    void on_cloud2veh_inh_res(const Cloud2VehInhRes &_msg) 
    {
        LOGD(TAG, "on_cloud2veh_inh_res\n");

        g_inh_timer.stop();
        g_inh_res = true;
    }

private:
    static constexpr const char *TAG = "ControllerCallback";
};

int main(int argc, char *argv[])
{
    ControllerCallback ccallback;

    g_controller.set_callback(&ccallback);
    g_controller.start(UP_SERVER_ADDRESS, UP_SERVER_PORT, DOWN_SERVER_ADDRESS, DOWN_SERVER_PORT);

    while (1) {}
    
    // never reach
    g_hb_timer.stop();
    g_state_timer.stop();
    g_inh_timer.stop();
    g_inh_res = false;
    g_inh_count = 0;
    g_controller.stop();

    return 0;
}