#ifndef MULTITHREADING_EVENT_HPP
#define MULTITHREADING_EVENT_HPP

#include <string>
#include <memory>
#include <utility>
#include "device.hpp"

const std::string kWorkDoneString = "Work Done";
const std::string kStartedString = "Started";

class Event{
public:
    virtual ~Event() = default;
    virtual std::string toString() const = 0;
};


class DeviceEvent : public Event{
public:
    explicit DeviceEvent(std::shared_ptr<Device> device): device(std::move(device)) {}

    std::string deviceName() const {
        return device->getName();
    }

    std::string toString() const override {
        return device->getDataAsString();
    }

private:
    std::shared_ptr<Device> device;
};


class DataEvent : public DeviceEvent{
public:
    explicit DataEvent(std::shared_ptr<Device> device): DeviceEvent(std::move(device)) {}

    std::string toString() const override {
        return DeviceEvent::deviceName() + ": " + DeviceEvent::toString();
    }
};


class WorkDoneEvent : public DeviceEvent{
public:
    explicit WorkDoneEvent(std::shared_ptr<Device> device): DeviceEvent(std::move(device)) {}

    std::string toString() const override {
        return DeviceEvent::deviceName() + ": " + kWorkDoneString;
    }
};


class StartedEvent : public DeviceEvent{
public:
    explicit StartedEvent(std::shared_ptr<Device> device): DeviceEvent(std::move(device)) {}

    std::string toString() const override {
        return DeviceEvent::deviceName() + ": " + kStartedString;
    }
};

#endif //MULTITHREADING_EVENT_HPP
