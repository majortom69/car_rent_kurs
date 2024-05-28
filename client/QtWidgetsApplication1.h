#pragma once
#include <string>
#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"

#include "websocket.h"


#include "avlTree.h"
#include "hashTable.h"
#include "rentalList.h"
#include "qtcontainers.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;




class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = nullptr);
    ~QtWidgetsApplication1();

    
    
private:
    Ui::QtWidgetsApplication1Class ui;
    
    Websocket ws;
    RentalList rentalList;
    AVLTree avlTree;
    HashTable hashTable;
    CreateContainer container;
  


private slots:
    void showRentForm();

    // работа с клиентами
    void showAddClientForm();
    void submitClientForm();

    void showAddCarForm();
    void sumbitCarForm();

    void submitRentalForm();

    void searchClient();
    void searchCar();
    void searchRental1();
};


