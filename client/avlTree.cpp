#include <avlTree.h>


void AVLTree::insert(const ClientData& data) {
    root = insertNode(root, data);
}

void AVLTree::remove(const std::string& dlNumber) {
    root = deleteNode(root, dlNumber);
}
void AVLTree:: preOrderTraversal(AVLNode* node, nlohmann::json& jsonArray) {
    if (node == nullptr) return;

    // Create a JSON object for the current node
    nlohmann::json nodeJson = {
        {"DRIVER_LICENSE", node->data.driverLicense},
        {"FULL_NAME", node->data.fullName},
        {"PASSPORT", node->data.passport},
        {"ADDRESS", node->data.address}
    };

    // Add the JSON object to the JSON array
    jsonArray.push_back(nodeJson);

    // Recurse on the left and right children
    preOrderTraversal(node->left, jsonArray);
    preOrderTraversal(node->right, jsonArray);
}
json AVLTree::preOrder() {
    nlohmann::json result = nlohmann::json::array();
    preOrderTraversal(root, result);
    return result;
}


int AVLTree::getHeight(AVLNode* node) {
    if (node == nullptr) return 0;
    return node->height;
}

int AVLTree::getBalance(AVLNode* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // ��������� �������
    x->right = y;
    y->left = T2;

    // ��������� ������ �����
    y->height = (std::max)(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = (std::max)(getHeight(x->left), getHeight(x->right)) + 1;

    // ���������� ����� ������
    return x;
}

AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // ��������� �������
    y->left = x;
    x->right = T2;

    // ��������� ������ �����
    x->height = (std::max)(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = (std::max)(getHeight(y->left), getHeight(y->right)) + 1;

    // ���������� ����� ������
    return y;
}



AVLNode* AVLTree::insertNode(AVLNode* node, ClientData data) {
    // 1. ���������� ����������� ������� ��� BST
    if (node == nullptr) return new AVLNode(data);

    if (data.driverLicense < node->data.driverLicense)
        node->left = insertNode(node->left, data);
    else if (data.driverLicense > node->data.driverLicense)
        node->right = insertNode(node->right, data);
    else // ��������� �� �����������
        return node;

    // 2. ��������� ������ �������� ����
    node->height = 1 + (std::max)(getHeight(node->left), getHeight(node->right));

    // 3. �������� ������ ������� ����, ����� ���������, �� ����� �� ������ ������������������
    int balance = getBalance(node);

    // ���� ���� ���� ������������������, �� ��������� ���� �� 4-� ��������� ���������

    // ����� ������� (LL)
    if (balance > 1 && data.driverLicense < node->left->data.driverLicense)
        return rotateRight(node);

    // ������ ������� (RR)
    if (balance < -1 && data.driverLicense > node->right->data.driverLicense)
        return rotateLeft(node);

    // ����-������ ������� (LR)
    if (balance > 1 && data.driverLicense > node->left->data.driverLicense) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // �����-����� ������� (RL)
    if (balance < -1 && data.driverLicense < node->right->data.driverLicense) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // ���� ���� �������� ����������������, ������ ���������� ���
    return node;
}

AVLNode* AVLTree::deleteNode(AVLNode* node, const std::string& dlNumber) {
    // 1. ���������� ����������� �������� ��� BST
    if (node == nullptr) return node;

    if (dlNumber < node->data.driverLicense)
        node->left = deleteNode(node->left, dlNumber);
    else if (dlNumber > node->data.driverLicense)
        node->right = deleteNode(node->right, dlNumber);
    else {
        // ���� � ����� ��� ��� �����
        if (node->left == nullptr || node->right == nullptr) {
            AVLNode* temp = node->left ? node->left : node->right;

            // ���� � ���� ��� �����
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            }
            else // ���� � ���� ���� ���� �������
                *node = *temp; // �������� ���������� ������� � ������ ����

            delete temp;
        }
        else {
            // ���� � ����� ������: ������� ���������� ���� � ������ ���������
            AVLNode* temp = minValueNode(node->right);

            // �������� ���������� ���������� ���� � ������ ����
            node->data = temp->data;

            // ������� ���������� ���� � ������ ���������
            node->right = deleteNode(node->right, temp->data.driverLicense);
        }
    }

    // ���� ������ ������� ������ �� ������ ����, ������ ���������� ���
    if (node == nullptr) return node;

    // 2. ��������� ������ �������� ����
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    // 3. �������� ������ ������� ����, ����� ���������, �� ����� �� ������ ������������������
    int balance = getBalance(node);

    // ���� ���� ���� ������������������, �� ��������� ���� �� 4-� ��������� ���������

    // ����� ������� (LL)
    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);

    // ������ ������� (RR)
    if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node);

    // ����-������ ������� (LR)
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // �����-����� ������� (RL)
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // ���� ���� �������� ����������������, ������ ���������� ���
    return node;
}

AVLNode* AVLTree::minValueNode(AVLNode* node) {
    AVLNode* current = node;

    // ���� ����� �� ������ ������ ����
    while (current->left != nullptr)
        current = current->left;

    return current;
}
void AVLTree::balanceAndInsert(const json& jsonData) {
    for (const auto& entry : jsonData) {
        ClientData client(entry);
        root = insertNode(root, client);
    }
}