#ifndef HASHTABLE_CLASS_H
#define HASHTABLE_CLASS_H

#include <iostream>
#include <vector>
#include <string>
#include <optional>

#include <qdebug.h>



#include <nlohmann/json.hpp>
using json = nlohmann::json;


struct CarData {
    std::string registrationNumber; // Государственный регистрационный номер
    std::string brand;              // Марка автомобиля
    std::string color;              // Цвет автомобиля
    int year;                       // Год выпуска
    int available;                  // Доступность
};

// Класс хеш-таблицы
class HashTable {
public:
    HashTable();
    bool insert(const CarData& data);
    bool remove(const std::string& registrationNumber);
    json search(const std::string& registrationNumber) const;
    nlohmann::json toJson() const;
    void printHashTable() const;
    void insertCarsFromJson(const json& carsJson);
    json searchByBrand(const std::string& brand) const;


    std::string getBrandFromRegistrationNumber(const std::string& registrationNumber) const {
        for (const auto& entry : table) {
            if (entry.has_value() && entry->registrationNumber == registrationNumber) {
                return entry->brand;
            }
        }

        // If brand not found, return an empty string
        return "";
    }

private:
    size_t hashFunction(const std::string& key) const;
    size_t probe(size_t index) const;

    std::vector<std::optional<CarData>> table;
    size_t currentSize;
};

#endif // HASHTABLE_CLASS_H