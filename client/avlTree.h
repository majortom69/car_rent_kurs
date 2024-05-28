#ifndef AVLTREE_CLASS_H
#define AVLTREE_CLASS_H
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct ClientData {
    std::string driverLicense;
    std::string fullName;
    std::string passport;
    std::string address;

    ClientData() = default;

    // Конструктор для инициализации данных из JSON
    ClientData(const json& j) {
        driverLicense = j["DRIVER_LICENSE"];
        fullName = j["FULL_NAME"];
        passport = j["PASSPORT"];
        address = j["ADDRESS"];
    }
};

class AVLTree;

struct AVLNode {
    ClientData data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(ClientData d) : data(d), left(nullptr), right(nullptr), height(1) {}

    // Добавляем дружественное объявление для класса AVLTree
    friend class AVLTree;
};

class AVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);

   
    AVLNode* insertNode(AVLNode* node, ClientData data);
    AVLNode* deleteNode(AVLNode* node, const std::string& dlNumber);

  
    void preOrderTraversal(AVLNode* node, nlohmann::json& jsonArray);
    
    AVLNode* minValueNode(AVLNode* node);
public:
    AVLTree() : root(nullptr) {}

    void insert(const ClientData& data);
    void remove(const std::string& dlNumber);

    void balanceAndInsert(const json& jsonData);

    json preOrder();

    void preOrderTraversal(AVLNode* node, const std::string& searchString, std::vector<ClientData>& results, bool searchByAddress = false) {
        if (node == nullptr) return;

        if (searchByAddress) {
            if (node->data.address.find(searchString) != std::string::npos) {
                results.push_back(node->data);
            }
        }
        else {
            if (node->data.fullName.find(searchString) != std::string::npos) {
                results.push_back(node->data);
            }
        }

        preOrderTraversal(node->left, searchString, results, searchByAddress);
        preOrderTraversal(node->right, searchString, results, searchByAddress);
    }

    json searchClientsByFullNameSubstring(const std::string& searchString) {
        std::vector<ClientData> results;
        preOrderTraversal(root, searchString, results, false);

        json resultJson;
        for (const auto& client : results) {
            resultJson.push_back({
                {"driverLicense", client.driverLicense},
                {"fullName", client.fullName},
                {"passport", client.passport},
                {"address", client.address}
                });
        }

        return resultJson;
    }

    json searchClientsByAddressSubstring(const std::string& searchString) {
        std::vector<ClientData> results;
        preOrderTraversal(root, searchString, results, true);

        json resultJson;
        for (const auto& client : results) {
            resultJson.push_back({
                {"driverLicense", client.driverLicense},
                {"fullName", client.fullName},
                {"passport", client.passport},
                {"address", client.address}
                });
        }

        return resultJson;
    }
    std::string getFullNameByDlNumber(const std::string& dlNumber) {
        AVLNode* current = root;
        while (current != nullptr) {
            if (dlNumber < current->data.driverLicense) {
                current = current->left;
            }
            else if (dlNumber > current->data.driverLicense) {
                current = current->right;
            }
            else {
                return current->data.fullName;
            }
        }
        return ""; // Return an empty string if not found
    }
   

};
#endif


