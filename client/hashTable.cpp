#include "hashTable.h"

HashTable::HashTable() : table(100), currentSize(0) {}


json HashTable::searchByBrand(const std::string& brand) const {
    nlohmann::json result = nlohmann::json::array();
    for (const auto& entry : table) {
        if (entry.has_value() && entry->brand == brand) {
            nlohmann::json carJson = {
                {"GOS_NUMBER", entry->registrationNumber},
                {"BRAND", entry->brand},
                {"COLOR", entry->color},
                {"YEAR", entry->year},
                {"AVAILABLE", entry->available}
            };
            result.push_back(carJson);
        }
    }
    return result;
}

size_t HashTable::hashFunction(const std::string& key) const {
    size_t hash = 0;
    for (char ch : key) {
        hash = hash * 31 + ch;
    }
    return hash % table.size();
}

size_t HashTable::probe(size_t index) const {
    return (index + 1) % table.size();
}

bool HashTable::insert(const CarData& data) {
    size_t index = hashFunction(data.registrationNumber);
    size_t startIndex = index;

    do {
        if (!table[index].has_value()) {
            table[index] = data;
            currentSize++;
            return true;
        }
        index = probe(index);
    } while (index != startIndex);

    return false; // “аблица полна
}

bool HashTable::remove(const std::string& registrationNumber) {
    size_t index = hashFunction(registrationNumber);
    size_t startIndex = index;

    do {
        if (table[index].has_value() && table[index]->registrationNumber == registrationNumber) {
            table[index].reset();
            currentSize--;
            return true;
        }
        index = probe(index);
    } while (index != startIndex);

    return false; // Ёлемент не найден
}

//std::optional<CarData> HashTable::search(const std::string& registrationNumber) const {
//    size_t index = hashFunction(registrationNumber);
//    size_t startIndex = index;
//
//    do {
//        if (table[index].has_value() && table[index]->registrationNumber == registrationNumber) {
//            return table[index];
//        }
//        index = probe(index);
//    } while (index != startIndex);
//
//    return std::nullopt; // Ёлемент не найден
//}
json HashTable::search(const std::string& registrationNumber) const {
    size_t index = hashFunction(registrationNumber);
    size_t startIndex = index;

    do {
        if (table[index].has_value() && table[index]->registrationNumber == registrationNumber) {
            nlohmann::json carJson = {
                {"GOS_NUMBER", table[index]->registrationNumber},
                {"BRAND", table[index]->brand},
                {"COLOR", table[index]->color},
                {"YEAR", table[index]->year},
                {"AVAILABLE", table[index]->available}
            };
            return carJson;
        }
        index = probe(index);
    } while (index != startIndex);

    return nlohmann::json(); // Return an empty JSON object if not found
}

nlohmann::json HashTable::toJson() const {
    nlohmann::json jsonArray = nlohmann::json::array();

    for (size_t i = 0; i < table.size(); ++i) {
        if (table[i].has_value()) {
            jsonArray.push_back({
                {"HASH", i},
                {"GOS_NUMBER", table[i]->registrationNumber},
                {"BRAND", table[i]->brand},
                {"COLOR", table[i]->color},
                {"YEAR", table[i]->year},
                {"AVAILABLE", table[i]->available}
                });
        }
    }

    return jsonArray;
}

void HashTable::printHashTable() const {
    for (size_t i = 0; i < table.size(); ++i) {
        if (table[i].has_value()) {
            qDebug() << "Hash:" << i << "Registration Number:" << QString::fromStdString(table[i]->registrationNumber)
                << "Brand:" << QString::fromStdString(table[i]->brand)
                << "Color:" << QString::fromStdString(table[i]->color)
                << "Year:" << table[i]->year
                << "Available:" << table[i]->available;
        }
    }
}

void HashTable::insertCarsFromJson(const json& carsJson) {
    for (const auto& carJson : carsJson) {
        CarData carData;
        carData.registrationNumber = carJson["GOS_NUMBER"];
        carData.brand = carJson["BRAND"];
        carData.color = carJson["COLOR"];
        carData.year = carJson["YEAR"];
        carData.available = carJson["AVAILABLE"];
        insert(carData);
    }
}