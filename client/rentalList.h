#ifndef RENTALLIST_CLASS_H
#define RENTALLIST_CLASS_H

#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ��������� ��� ������������� ������ � ������ �� ������ ��� �������� ����������� �� ��������
struct RentalData {
    std::string driverLicenseNumber; // ����� ������������� ������������� �������
    std::string registrationNumber; // ��������������� ��������������� ����� ����������
    std::string rentalDate; // ���� ������
    std::string returnDate; // ���� ��������

    json toJson() const {
        return {
            {"DRIVER_LICENSE", driverLicenseNumber},
            {"GOS_NUMBER", registrationNumber},
            {"ISSUE_DATE", rentalDate},
            {"RETURN_DATE", returnDate}
        };
    }
};

// ��������� ��� �������������� �� ���������������� ���������������� ������
inline bool compareByRegistrationNumber(const RentalData& a, const RentalData& b) {
    return a.registrationNumber < b.registrationNumber;
}

class RentalList {
private:
    std::vector<RentalData> dataList; // �������� ������ ������

public:
    // ����� ��� ��������� ���������� ������ ������
    void shakerSort() {
        bool swapped = true;
        int start = 0;
        int end = dataList.size() - 1;

        while (swapped) {
            swapped = false;

            // ������������ ����� �������
            for (int i = start; i < end; ++i) {
                if (compareByRegistrationNumber(dataList[i + 1], dataList[i])) {
                    std::swap(dataList[i], dataList[i + 1]);
                    swapped = true;
                }
            }

            if (!swapped)
                break;

            swapped = false;
            --end;

            // ������������ ������ ������
            for (int i = end - 1; i >= start; --i) {
                if (compareByRegistrationNumber(dataList[i + 1], dataList[i])) {
                    std::swap(dataList[i], dataList[i + 1]);
                    swapped = true;
                }
            }
            ++start;
        }
    }

    // ����� ��� ���������� ������ � ������ �� ������ ��� �������� ����������
    void add(const RentalData& rental) {
        dataList.push_back(rental);
        shakerSort();
    }

    // ����� ��� ������ ������ ������
    void print() const {
        for (const auto& rental : dataList) {
            std::cout << "Driver License: " << rental.driverLicenseNumber << ", "
                << "Registration Number: " << rental.registrationNumber << ", "
                << "Rental Date: " << rental.rentalDate << ", "
                << "Return Date: " << rental.returnDate << std::endl;
        }
    }

    void fillRentalList(const json& jsonData) {
        for (const auto& entry : jsonData) {
            RentalData rental;
            rental.driverLicenseNumber = entry["DRIVER_LICENSE"];
            rental.registrationNumber = entry["GOS_NUMBER"];
            rental.rentalDate = entry["ISSUE_DATE"];
            rental.returnDate = entry["RETURN_DATE"];
            add(rental);
        }
    }

    json searchByDriverLicenseNumber(const std::string& driverLicenseNumber) const {
        json result = json::array();

        for (const auto& rental : dataList) {
            if (rental.driverLicenseNumber.find(driverLicenseNumber) != std::string::npos) {
                result.push_back(rental.toJson());
            }
        }

        return result;
    }

    std::string getDriversLicenseFromRegistrationNumber(const std::string& registrationNumber) const {
        for (const auto& rental : dataList) {
            if (rental.registrationNumber == registrationNumber) {
                return rental.driverLicenseNumber;
            }
        }
        return "";
    }

    json toJson() const {
        json result = json::array();

        for (const auto& rental : dataList) {
            result.push_back(rental.toJson());
        }

        return result;
    }

    void removeByRegistrationNumber(const std::string& registrationNumber) {
        auto it = std::remove_if(dataList.begin(), dataList.end(),
            [&registrationNumber](const RentalData& rental) {
                return rental.registrationNumber == registrationNumber;
            });
        if (it != dataList.end()) {
            dataList.erase(it, dataList.end());
            shakerSort();
        }
    }
};

#endif // RENTALLIST_CLASS_H
