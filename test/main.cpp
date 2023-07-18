#include <iostream>
#include <thread>
#include <atomic>
#include "device_read.hpp"
#include "EventQueue.hpp"


class Main {
private:
    constexpr static std::chrono::seconds kTimeout = std::chrono::seconds(5);
    constexpr static char kHelpString[] = "usage: cpp_parallel OR cpp_parallel <test_type> <limit>\n"
                             "<test_type>:\n"
                             "\t1 to limit the number of responses of device A,\n"
                             "\t2 to limit the number of responses of device B,\n"
                             "\t3 to limit the number of responses of both devices A amd B.\n"
                             "<limit>:\n"
                             "\tThe number of times a device will respond.\n\tIf 0, no limit is applied.\n"
                             "e.g.: cpp_parallel 1 10 (device A will respond 10 times, device B will not stop responding).\n";


    template<typename DeviceType>
    static void readFromDevice(const std::shared_ptr<DeviceType>& device, EventQueue& eventQueue, std::atomic<bool>& stopFlag, int limit) {
        eventQueue.push(std::make_shared<StartedEvent>(device));
        int count = 0;
        while (!stopFlag) {
            if (limit != 0 && count++ >= limit) {
                std::cerr << device->getName() << " stopped responding\n";
                break;
            }
            if constexpr (std::is_same_v<DeviceType, DeviceA>) {
                eventQueue.push(readA(device));
            } else if constexpr (std::is_same_v<DeviceType, DeviceB>) {
                eventQueue.push(readB(device));
            }
        }
        eventQueue.push(std::make_shared<WorkDoneEvent>(device));
    }


    static void processEvents(EventQueue &eventQueue, std::atomic<bool> &stopFlag) {
        while (!stopFlag) {
            auto event = eventQueue.pop(kTimeout);
            if (event) {
                std::cout << event->toString() << std::endl;
            } else {
                std::cout << "Timeout: No events received for " << kTimeout.count() << " seconds. Terminating.\n";
                stopFlag = true;
            }
        }
    }


public:
    static int main(int argc, char* argv[]) {
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

        std::shared_ptr<DeviceA> deviceA = std::make_shared<DeviceA>();
        std::shared_ptr<DeviceB> deviceB = std::make_shared<DeviceB>();
        EventQueue eventQueue;
        std::atomic<bool> stopFlag(false);

        std::thread threadA(readFromDevice<DeviceA>, std::ref(deviceA), std::ref(eventQueue), std::ref(stopFlag), limitA);
        std::thread threadB(readFromDevice<DeviceB>, std::ref(deviceB), std::ref(eventQueue), std::ref(stopFlag), limitB);
        processEvents(eventQueue, stopFlag);

        threadA.join();
        threadB.join();

        return 0;
    }

};


int main(int argc, char* argv[]) {
    Main::main(argc, argv);
    return 0;
}
