#ifndef MULTITHREADING_DEVICE_HPP
#define MULTITHREADING_DEVICE_HPP

#include <string>
#include <vector>
#include <sstream>
#include <random>


class Device {
public:
    Device() = default;
    virtual ~Device() = default;
    virtual std::string getName() = 0;
    virtual std::string getDataAsString() = 0;
};

class DeviceA : public Device {
public:
    std::string getName() override {
        return "Device A";
    };

    std::string generateData() {
        static const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        const int max_length = 500;

        generator.seed(random_device());
        std::uniform_int_distribution<> char_distribution(0, characters.length() - 1);
        std::uniform_int_distribution<> length_distribution(0, max_length);

        std::string random_string;
        int length = length_distribution(generator);
        random_string.reserve(length);

        for (std::size_t i = 0; i < length ; ++i) {
            random_string.push_back(characters[char_distribution(generator)]);
        }

        return random_string;
    }

    std::string getDataAsString() override {
        return generateData();
    };

private:
    std::random_device random_device;
    std::mt19937 generator;
};

class DeviceB : public Device {
public:
    std::string getName() override {
        return "Device B";
    };

    std::vector<int> generateData() {
        const int length = 3;

        generator.seed(random_device());
        std::uniform_int_distribution<> int_distribution(0, 198);

        std::vector<int> random_vector;
        for (std::size_t i = 0; i < length; ++i) {
            random_vector.push_back(int_distribution(generator));
        }

        return random_vector;
    }

    std::string getDataAsString() override {
        std::vector<int> data(generateData());
        std::ostringstream oss;
        for (const int& num : data) {
            oss << num << " ";
        }
        return oss.str();
    };

private:
    std::random_device random_device;
    std::mt19937 generator;
};

#endif //MULTITHREADING_DEVICE_HPP
