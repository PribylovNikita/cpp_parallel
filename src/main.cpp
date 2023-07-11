#include <iostream>
#include <thread>
#include <atomic>
#include "EventQueue.hpp"

const auto kTimeout = std::chrono::seconds(5);
const auto kHelpString = "usage: multithreading OR multithreading <test_type> <limit>\n"
                         "<test_type>:\n"
                         "\t1 to limit the number of responses of device A,\n"
                         "\t2 to limit the number of responses of device B,\n"
                         "\t3 to limit the number of responses of both devices A amd B.\n"
                         "<limit>:\n"
                         "\tThe number of times a device will respond.\n\tIf 0, no limit is applied.\n"
                         "e.g.: multithreading 1 10 (device A will respond 10 times, device B will not stop responding).\n";


void readA(std::shared_ptr<DeviceA>& deviceA, EventQueue& eventQueue) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    eventQueue.push(std::make_shared<DataEvent>(deviceA));
}

void readB(std::shared_ptr<DeviceB>& deviceB, EventQueue& eventQueue) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    eventQueue.push(std::make_shared<DataEvent>(deviceB));
}


void readFromDeviceA(EventQueue& eventQueue, std::atomic<bool>& stopFlag, int limit) {
    std::shared_ptr<DeviceA> device = std::make_shared<DeviceA>();
    eventQueue.push(std::make_shared<StartedEvent>(device));
    int count = 0;
    while (!stopFlag) {
        if (limit != 0 && count++ >= limit) {
            std::cerr << device->getName() << " stopped responding\n";
            break;
        }
        readA(device, eventQueue);
    }
    eventQueue.push(std::make_shared<WorkDoneEvent>(device));
}

void readFromDeviceB(EventQueue& eventQueue, std::atomic<bool>& stopFlag, int limit) {
    std::shared_ptr<DeviceB> device = std::make_shared<DeviceB>();
    eventQueue.push(std::make_shared<StartedEvent>(device));
    int count = 0;
    while (!stopFlag) {
        if (limit != 0 && count++ >= limit) {
            std::cerr << device->getName() << " stopped responding\n";
            break;
        }
        readB(device, eventQueue);
    }
    eventQueue.push(std::make_shared<WorkDoneEvent>(device));
}


void processEvents(EventQueue& eventQueue, std::atomic<bool>& stopFlag) {
    while (!stopFlag) {
        auto event = eventQueue.pop(kTimeout);
        if (event) {
            std::cout << event->toString() << std::endl;
        } else {
            std::cout << "Timeout: No events received for " << kTimeout.count()  << " seconds. Terminating.\n";
            stopFlag = true;
        }
    }
}


int main(int argc, char* argv[]) {

    if (argc != 1 && argc != 3) {
        std::cerr << kHelpString;
        return -1;
    }

    int limitA = 0;
    int limitB = 0;

    if (argc == 3) {
        int type = std::atoi(argv[1]);
        int limit = std::atoi(argv[2]);
        switch (type) {
            case 1:
                limitA = limit;
                break;
            case 2:
                limitB = limit;
                break;
            case 3:
                limitA = limit;
                limitB = limit;
                break;
            default:
                std::cerr << kHelpString;
                return -1;
        }
    }

    EventQueue eventQueue;
    std::atomic<bool> stopFlag(false);

    // Create threads for device reading and event processing
    std::thread threadA(readFromDeviceA, std::ref(eventQueue), std::ref(stopFlag), limitA);
    std::thread threadB(readFromDeviceB, std::ref(eventQueue), std::ref(stopFlag), limitB);
    processEvents(eventQueue, stopFlag);

    threadA.join();
    threadB.join();

    return 0;
}
