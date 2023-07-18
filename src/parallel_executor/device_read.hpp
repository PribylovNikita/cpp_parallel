#ifndef MULTITHREADING_DEVICE_READ_HPP
#define MULTITHREADING_DEVICE_READ_HPP

#include <thread>
#include <chrono>
#include "event.hpp"


std::shared_ptr<DataEvent> readA(const std::shared_ptr<DeviceA>& deviceA) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return std::make_shared<DataEvent>(deviceA);
}

std::shared_ptr<DataEvent> readB(const std::shared_ptr<DeviceB>& deviceB) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    return std::make_shared<DataEvent>(deviceB);
}

#endif //MULTITHREADING_DEVICE_READ_HPP
