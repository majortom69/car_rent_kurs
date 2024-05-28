#include "QtWidgetsApplication1.h"
#include <QtWidgets>

#include <string>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <iostream>

#include "hashTable.h"
#include "carwidget.h"





QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent), container(ws, hashTable, avlTree, rentalList)
{
    ui.setupUi(this);
    ui.carFrame->hide();
    ui.clientFrame->hide();
    ui.clientEditFrame->hide();
    ui.carFrameEdit->hide();
    ui.frame->hide();
    ui.gosNumberEdit->setReadOnly(true);
    ui.driversLicenseEdit->setReadOnly(true);

    ui.gosNumber->setMaxLength(9);
    ui.gosNumberEdit->setMaxLength(9);
    ui.driversLicense->setMaxLength(12);
    ui.driversLicenseEdit->setMaxLength(12);

    


    ui.rentalDate->setDate(QDate::currentDate());
    ui.returnDate->setDate(QDate::currentDate().addDays(1));

    // scrollArea для клиентов
    QWidget* clientsViewport = new QWidget;
    ui.clientsScrollArea->setWidget(clientsViewport);
    ui.clientsScrollArea->setWidgetResizable(true);
    QVBoxLayout* clientsLayout = new QVBoxLayout(clientsViewport);
    clientsLayout->setAlignment(Qt::AlignTop);

    // scrollArea для машин
    QWidget* carsViewport = new QWidget;
    ui.carsScrollArea->setWidget(carsViewport);
    ui.carsScrollArea->setWidgetResizable(true);
    QVBoxLayout* carsLayout = new QVBoxLayout(carsViewport);
    carsLayout->setAlignment(Qt::AlignTop);

    
    connect(ui.pushButton, SIGNAL(released()), this, SLOT(showRentForm()));

    connect(ui.addClient, SIGNAL(released()), this, SLOT(showAddClientForm()));
    connect(ui.submitClientForm, SIGNAL(released()), this, SLOT(submitClientForm()));

    connect(ui.addCar, SIGNAL(released()), this, SLOT(showAddCarForm()));
    connect(ui.sumbitCarForm, SIGNAL(released()), this, SLOT(sumbitCarForm()));

    connect(ui.submitRentalFormButton, SIGNAL(released()), this, SLOT(submitRentalForm()));

    connect(ui.searchClient, SIGNAL(released()), this, SLOT(searchClient()));
    connect(ui.searchCar, SIGNAL(released()), this, SLOT(searchCar()));
    connect(ui.searchRental, SIGNAL(released()), this, SLOT(searchRental1()));

    json data = ws.getData();
    qDebug() << "Received JSON data:" << QString::fromStdString(data.dump(4));

    json clients = data["clients"];
    json cars = data["cars"];
    json records = data["records"];

    qDebug() << "Clients:" << QString::fromStdString(clients.dump(4));
    qDebug() << "Cars:" << QString::fromStdString(cars.dump(4));
    qDebug() << "Records:" << QString::fromStdString(records.dump(4));

    avlTree.balanceAndInsert(clients);
    json avltreejson = avlTree.preOrder();
    for (const auto& client : avltreejson) {
        QString dlNumber = QString::fromStdString(client["DRIVER_LICENSE"].get<std::string>());
        QString fullName = QString::fromStdString(client["FULL_NAME"].get<std::string>());
        QString passport = QString::fromStdString(client["PASSPORT"].get<std::string>());
        QString address = QString::fromStdString(client["ADDRESS"].get<std::string>());
        container.addClientContainerToScrollArea(ui.clientsScrollArea, ui.clientEditFrame, ui.clientPickerScrollArea, dlNumber, fullName, passport, address);
        container.addSimpleClientContainerToScrollArea(ui.clientPickerScrollArea, dlNumber, fullName);
       /* QString dlNumber = QString::fromStdString(client["DRIVER_LICENSE"].get<std::string>());
        QString fullName = QString::fromStdString(client["FULL_NAME"].get<std::string>());
        QString passport = QString::fromStdString(client[" PASSPORT"].get<std::string>());
        QString address = QString::fromStdString(client["ADDRESS"].get<std::string>());
        container.addClientContainerToScrollArea(ui.clientsScrollArea, ui.clientEditFrame, ui.clientPickerScrollArea, dlNumber, fullName, passport, address);
        container.addSimpleClientContainerToScrollArea(ui.clientPickerScrollArea, dlNumber, fullName);*/
    }
    //json avlClients = avlTree.preOrder();
    //for (const auto& client : avlClients) {

    //    QString dlNumber = QString::fromStdString(client["DRIVER_LICENSE"].get<std::string>());
    //    QString fullName = QString::fromStdString(client["FULL_NAME"].get<std::string>());
    //    QString passport = QString::fromStdString(client[" PASSPORT"].get<std::string>());
    //    QString address = QString::fromStdString(client["ADDRESS"].get<std::string>());
    //    container.addClientContainerToScrollArea(ui.clientsScrollArea, ui.clientEditFrame, ui.clientPickerScrollArea, dlNumber, fullName, passport, address);
    //    container.addSimpleClientContainerToScrollArea(ui.clientPickerScrollArea, dlNumber, fullName);
    //}

    rentalList.fillRentalList(records);
    json rentallistjson = rentalList.toJson();
    


    hashTable.insertCarsFromJson(cars);
    hashTable.printHashTable();
    json hashtbljson = hashTable.toJson();

    
    for (const auto& car : hashtbljson) {
        QString registrationNumber = QString::fromStdString(car["GOS_NUMBER"].get<std::string>());
        QString brand = QString::fromStdString(car["BRAND"].get<std::string>());
        QString color = QString::fromStdString(car["COLOR"].get<std::string>());
        int year = car["YEAR"];
        int available = car["AVAILABLE"];

        qDebug() << "RG NUMBER" << registrationNumber;
        std::string dlNumber = rentalList.getDriversLicenseFromRegistrationNumber(car["GOS_NUMBER"]);
        std::string fullName = avlTree.getFullNameByDlNumber(dlNumber);
        qDebug() << "DLNUMBER" << dlNumber;
        QString dlNumberQstring = QString::fromStdString(dlNumber);
        QString fullNameQString = QString::fromStdString(fullName);
        container.addCarContainerToScrollArea(ui.carsScrollArea, ui.carFrameEdit, ui.carPickerScrollArea, ui.carPickerScrollArea, registrationNumber, brand, color, year, available, fullNameQString, dlNumberQstring);
        
        bool available_bool = (available != 0);
        if (available_bool) {
            container.addSimpleCarContainerToScrollArea(ui.carPickerScrollArea, registrationNumber, brand);
        }
        
    }

    for (const auto& record : rentallistjson) {
        std::string fullName = avlTree.getFullNameByDlNumber(record["DRIVER_LICENSE"]);
        /*std::string brand = hashTable.getBrandFromRegistrationNumber(record["GOS_NUMBER"]);*/

        QString returnDateString = QString::fromStdString(record["RETURN_DATE"].get<std::string>());
        QString selectedClient = QString::fromStdString(record["DRIVER_LICENSE"].get<std::string>());
        QString selectedCar = QString::fromStdString(record["GOS_NUMBER"].get<std::string>());
        QString rentalDateString = QString::fromStdString(record["ISSUE_DATE"].get<std::string>());
        qDebug() << selectedCar;

        QString brand = QString::fromStdString(hashTable.getBrandFromRegistrationNumber(selectedCar.toStdString()));
        container.addRentalContainerToScrollArea(ui.rentalScrollArea, QString::fromStdString(fullName), brand, returnDateString, selectedClient, selectedCar, rentalDateString, ui.carPickerScrollArea, ui.clientPickerScrollArea);
    }

   

}




void clearScrollArea(QScrollArea* scrollArea) {
    QWidget* viewport = scrollArea->widget();
    if (!viewport) {
        qDebug() << "No viewport widget found in the scroll area.";
        return;
    }

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(viewport->layout());
    if (!layout) {
        qDebug() << "No layout found in the viewport widget.";
        return;
    }

    // Remove and delete all widgets from the layout
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void QtWidgetsApplication1::searchRental1() {
    clearScrollArea(ui.rentalScrollArea);
    QString searchStr1 = ui.searchRentalLine->text();
    qDebug() << "Search string: " << searchStr1;

    json result;
    result = rentalList.searchByDriverLicenseNumber(searchStr1.toStdString());
    qDebug() << result.dump(4);
    if (result.empty()) return;

    

    //qDebug() << "Search result size: " << result.size();

    for (const auto& car : result) {
        QString dlNumber = QString::fromStdString(car["DRIVER_LICENSE"]);
        QString registrationNumber = QString::fromStdString(car["GOS_NUMBER"]);
        QString rentalDate = QString::fromStdString(car["ISSUE_DATE"]);
        QString returnDate = QString::fromStdString(car["RETURN_DATE"]);
        
        QString brand = QString::fromStdString(hashTable.getBrandFromRegistrationNumber(registrationNumber.toStdString()));
        qDebug() << brand;
        QString fullName = QString::fromStdString(avlTree.getFullNameByDlNumber(dlNumber.toStdString()));
        container.addRentalContainerToScrollArea(ui.rentalScrollArea, fullName, brand, returnDate, dlNumber, registrationNumber, rentalDate, ui.carPickerScrollArea, ui.clientPickerScrollArea);
    }

    ///*for (const auto& car : result) {
    //    qDebug() << "Record found: " << QString::fromStdString(car.dump(4));

    //    QString registrationNumber = QString::fromStdString(car["GOS_NUMBER"].get<std::string>());
    //    QString brand = QString::fromStdString(car["BRAND"].get<std::string>());
    //    QString color = QString::fromStdString(car["COLOR"].get<std::string>());
    //    int year = car["YEAR"];
    //    int available = car["AVAILABLE"];

    //    std::string dlNumber = rentalList.getDriversLicenseFromRegistrationNumber(car["GOS_NUMBER"]);
    //    std::string fullName = avlTree.getFullNameByDlNumber(dlNumber);
    //    qDebug() << "DLNUMBER" << dlNumber;
    //    QString dlNumberQstring = QString::fromStdString(dlNumber);
    //    QString fullNameQString = QString::fromStdString(fullName);
    //    container.addCarContainerToScrollArea(ui.carsScrollArea, ui.carFrameEdit, ui.carPickerScrollArea, ui.carPickerScrollArea, registrationNumber, brand, color, year, available, fullNameQString, dlNumberQstring);

    //    
    //}*/
}

void QtWidgetsApplication1::searchCar() {

    QString searchStr = ui.carSearchLine->text();
    
    clearScrollArea(ui.carsScrollArea);
    if (searchStr == "") {
        json hashtbljson = hashTable.toJson();


        for (const auto& car : hashtbljson) {
            QString registrationNumber = QString::fromStdString(car["GOS_NUMBER"].get<std::string>());
            QString brand = QString::fromStdString(car["BRAND"].get<std::string>());
            QString color = QString::fromStdString(car["COLOR"].get<std::string>());
            int year = car["YEAR"];
            int available = car["AVAILABLE"];
            std::string dlNumber = rentalList.getDriversLicenseFromRegistrationNumber(car["GOS_NUMBER"]);
            std::string fullName = avlTree.getFullNameByDlNumber(dlNumber);

            QString dlNumberQstring = QString::fromStdString(dlNumber);
            QString fullNameQString = QString::fromStdString(fullName);
            container.addCarContainerToScrollArea(ui.carsScrollArea, ui.carFrameEdit, ui.carPickerScrollArea, ui.carPickerScrollArea, registrationNumber, brand, color, year, available, dlNumberQstring, fullNameQString);

           

        }
        return;
    }
    json result;
    if (ui.carSearchFilter->currentText() == "Марка")
    {
        result = hashTable.searchByBrand(searchStr.toStdString());
        for (const auto& car : result) {
            QString registrationNumber = QString::fromStdString(car["GOS_NUMBER"].get<std::string>());
            QString brand = QString::fromStdString(car["BRAND"].get<std::string>());
            QString color = QString::fromStdString(car["COLOR"].get<std::string>());
            int year = car["YEAR"];
            int available = car["AVAILABLE"];
            std::string dlNumber = rentalList.getDriversLicenseFromRegistrationNumber(car["GOS_NUMBER"]);
            std::string fullName = avlTree.getFullNameByDlNumber(dlNumber);

            QString dlNumberQstring = QString::fromStdString(dlNumber);
            QString fullNameQString = QString::fromStdString(fullName);
            container.addCarContainerToScrollArea(ui.carsScrollArea, ui.carFrameEdit, ui.carPickerScrollArea, ui.carPickerScrollArea, registrationNumber, brand, color, year, available, dlNumberQstring, fullNameQString);



        }
    }
    else {
        result = hashTable.search(searchStr.toStdString());
        if (result.empty()) return;
        qDebug() << "FIND UMH HAST TABLE STRUCT " << QString::fromStdString(result.dump(4));
        QString registrationNumber = QString::fromStdString(result["GOS_NUMBER"].get<std::string>());
        QString brand = QString::fromStdString(result["BRAND"].get<std::string>());
        QString color = QString::fromStdString(result["COLOR"].get<std::string>());
        int year = result["YEAR"];
        int available = result["AVAILABLE"];

        std::string dlNumber = rentalList.getDriversLicenseFromRegistrationNumber(result["GOS_NUMBER"]);
        std::string fullName = avlTree.getFullNameByDlNumber(dlNumber);

        QString dlNumberQstring = QString::fromStdString(dlNumber);
        QString fullNameQString = QString::fromStdString(fullName);
        container.addCarContainerToScrollArea(ui.carsScrollArea, ui.carFrameEdit, ui.carPickerScrollArea, ui.carPickerScrollArea, registrationNumber, brand, color, year, available, dlNumberQstring, fullNameQString);
       /* QString registrationNumber = QString::fromStdString(result["GOS_NUMBER"].get<std::string>());
        QString brand = QString::fromStdString(result["BRAND"].get<std::string>());
        QString color = QString::fromStdString(result["COLOR"].get<std::string>());
        int year = result["YEAR"];
        int available = result["AVAILABLE"];

        std::string dlNumber = rentalList.getDriversLicenseFromRegistrationNumber(result["GOS_NUMBER"]);
        std::string fullName = avlTree.getFullNameByDlNumber(dlNumber);

        QString dlNumberQstring = QString::fromStdString(dlNumber);
        QString fullNameQString = QString::fromStdString(fullName);
        container.addCarContainerToScrollArea(ui.carsScrollArea, ui.carFrameEdit, ui.carPickerScrollArea, ui.carPickerScrollArea, registrationNumber, brand, color, year, available, dlNumberQstring, fullNameQString);*/
    }
    
    
    
    
    
    /*for (const auto& car : result) {
        QString registrationNumber = QString::fromStdString(car["GOS_NUMBER"].get<std::string>());
        QString brand = QString::fromStdString(car["BRAND"].get<std::string>());
        QString color = QString::fromStdString(car["COLOR"].get<std::string>());
        int year = car["YEAR"];
        int available = 1;
        container.addCarContainerToScrollArea(ui.carsScrollArea, ui.carFrameEdit, ui.carPickerScrollArea, ui.carPickerScrollArea, registrationNumber, brand, color, year, available);
    }*/
   
}

void QtWidgetsApplication1::searchClient() {
    clearScrollArea(ui.clientsScrollArea);
    QString searchStr = ui.clientSearchLine->text();
    json result;
  
    if (ui.clientSearchFilter->currentText() == "ФИО") {
        result = avlTree.searchClientsByFullNameSubstring(searchStr.toStdString());
    }
    else {
        result = avlTree.searchClientsByAddressSubstring(searchStr.toStdString());
    }

    if (result.empty()) return;
    qDebug() << "found clients" << result.dump(4);
    
   /* if (searchStr.toStdString() == "") {
        result = avlTree.preOrder();
    }*/
    for (const auto& client : result) {
        QString dlNumber = QString::fromStdString(client["driverLicense"].get<std::string>());
        QString fullName = QString::fromStdString(client["fullName"].get<std::string>());
        QString passport = QString::fromStdString(client["passport"].get<std::string>());
        QString address = QString::fromStdString(client["address"].get<std::string>());
        container.addClientContainerToScrollArea(ui.clientsScrollArea, ui.clientEditFrame, ui.clientPickerScrollArea, dlNumber, fullName, passport, address);
    }
}




void QtWidgetsApplication1::showAddCarForm() {
    if (ui.carFrame->isVisible()) {
        ui.carFrame->hide();
    }
    else {
        ui.carFrame->show();
    }
}

void QtWidgetsApplication1::sumbitCarForm() {
    QString registrationNumber = ui.gosNumber->text();
    QString brand = ui.brand->currentText();
    QString color = ui.color->currentText();
    int year = ui.yearofIssue->date().year();

    std::string ifregnumexitst = hashTable.getBrandFromRegistrationNumber(registrationNumber.toStdString());
    if (ifregnumexitst != "") {
        QMessageBox::warning(this, "Хэш таблица", "Рег. номер не может повторятся");
        return;
    }

    std::string dlNumber = rentalList.getDriversLicenseFromRegistrationNumber(registrationNumber.toStdString());
    std::string fullName = avlTree.getFullNameByDlNumber(dlNumber);

    QString dlNumberQstring = QString::fromStdString(dlNumber);
    QString fullNameQString = QString::fromStdString(fullName);
    container.addCarContainerToScrollArea(ui.carsScrollArea, ui.carFrameEdit, ui.carPickerScrollArea, ui.carPickerScrollArea, registrationNumber, brand, color, year, true, dlNumberQstring, fullNameQString);

    /*addContainerToCarsScrollArea(ui.carsScrollArea, registrationNumber, brand, color, year);*/

    container.addSimpleCarContainerToScrollArea(ui.carPickerScrollArea, registrationNumber, brand);
    
    //CarData carData(registrationNumber.toStdString(), brand.toStdString(), color.toStdString(), year, true); // По умолчанию availability = true
    //hashTable.insert(carData);

    /*addContainerToScrollArea(ui.carsScrollArea, registrationNumber, fullName, passport, address);*/
    json formData = {
        {"action", "insert"},
        {"entity", "car"},
        {"registrationNumber", registrationNumber.toStdString()},
        {"brand", brand.toStdString()},
        {"color", color.toStdString()},
        {"year", year},
        {"available", 1}
    };
    ws.sendData(formData);
    CarData crdta;
    crdta.registrationNumber = registrationNumber.toStdString();
    crdta.brand = brand.toStdString();
    crdta.color = color.toStdString();
    crdta.year = year;
    crdta.available = 1;

    bool notFull = hashTable.insert(crdta);
    if (!notFull) {
        QMessageBox::warning(this, "Невозможно добавить авто", "Хэш таблица переполнена");
        return;
    }
    /*addSingleCollapsibleContainerToScrollArea(ui.carsScrollArea, registrationNumber, brand, color, year);*/
    
    /*addSingleCollapsibleContainerToScrollArea(ui.carsScrollArea);*/
    QMessageBox::information(this, "Успешно!", "Авто добавлено в ХЭШ таблицк");
    ui.carFrame->hide();

}

void QtWidgetsApplication1::showRentForm() 
{
    if (ui.frame->isVisible()) {
        ui.frame->hide();
    }
    else {
        ui.frame->show();
    }
}

void QtWidgetsApplication1::showAddClientForm() {

    if (ui.clientFrame->isVisible()) {
        ui.clientFrame->hide();
    }
    else {
        ui.clientFrame->show();
    }
}

void QtWidgetsApplication1::submitClientForm() {
    QString dlNumber = ui.driversLicense->text();
    QString fullName = ui.firstName->text() + " " + ui.middleName->text() + " " + ui.lastName->text();
    QString passport = ui.passport->text();
    QString address = ui.address->text();

    std::string checkexist = avlTree.getFullNameByDlNumber(dlNumber.toStdString());
    if (checkexist != "") {
        QMessageBox::warning(this, "АВЛ Дерево", "Номер вод. уд повторяется");
        return;
    }
   
    /*addContainerToScrollArea(ui.clientsScrollArea, dlNumber, fullName, passport, address);*/
    container.addClientContainerToScrollArea(ui.clientsScrollArea, ui.clientEditFrame, ui.clientPickerScrollArea, dlNumber, fullName, passport, address);
    container.addSimpleClientContainerToScrollArea(ui.clientPickerScrollArea, dlNumber, fullName);
    /*addClientContainer(ui.clientsScrollArea, dlNumber, fullName, passport, address);
    checkboxarea(ui.scrollArea_3, dlNumber, fullName);*/
    ClientData clientData;
    clientData.driverLicense = dlNumber.toStdString();
    clientData.fullName = fullName.toStdString();
    clientData.passport = passport.toStdString();
    clientData.address = address.toStdString();

    avlTree.insert(clientData);
    json formData = {
        {"action", "insert"},
        {"entity", "client"},
        {"driverLicense", dlNumber.toStdString()},
        {"fullName", fullName.toStdString()},
        {"passport", passport.toStdString()},
        {"address", address.toStdString()}
    };
    ws.sendData(formData);
    
    qDebug() << "Preorder traversal of AVL tree : ";
    std::cout << "Preorder traversal of AVL tree:" << std::endl;
    ui.clientFrame->hide();
    QMessageBox::information(this, "Успешно!", "Клиент добавлен в авл дерево");
  /*  avlTree.preOrder();*/
    
}

void QtWidgetsApplication1::submitRentalForm() {
    /*QString rentalDate = ui.rentalDate->date().toString("dd/MM/yyyy");
    QString returnDate = ui.returnDate->date().toString("dd/MM/yyyy");*/

    QDate rentalDate = ui.rentalDate->date();
    QDate returnDate = ui.returnDate->date();
    QDate currentDate = QDate::currentDate();

    QString selectedCar = container.getSelectedRegistrationNumber(ui.carPickerScrollArea);
    QString selectedClient = container.getSelectedDlNumber(ui.clientPickerScrollArea);
    
    // Check if rental date is not older than today
    if (rentalDate < currentDate) {
        qDebug() << "Rental date cannot be older than today.";
        // Optionally show a message box to inform the user
        QMessageBox::warning(this, "Invalid Date", "Rental date cannot be older than today.");
        return;
    }

    // Check if return date is valid
    if (returnDate <= rentalDate) {
        qDebug() << "Return date must be at least one day after the rental date.";
        // Optionally show a message box to inform the user
        QMessageBox::warning(this, "Invalid Date", "Return date must be at least one day after the rental date.");
        return;
    }

    if (selectedCar == "") {
        QMessageBox::warning(this, "Invalid Car", "Pleae select a car.");
        return;
    }

    if (selectedClient == "") {
        QMessageBox::warning(this, "Invalid Client", "Pleae select a client.");
        return;
    }

    // If dates are valid, proceed with the rest of the submission logic
    QString rentalDateString = rentalDate.toString(Qt::ISODate);
    QString returnDateString = returnDate.toString(Qt::ISODate);


    
    qDebug() << "Rental Date:" << rentalDateString;
    qDebug() << "Return Date:" << returnDateString;
    qDebug() << selectedCar;

    RentalData rentalData;
    rentalData.driverLicenseNumber = selectedClient.toStdString();
    rentalData.registrationNumber = selectedCar.toStdString();
    rentalData.rentalDate = rentalDateString.toStdString();
    rentalData.returnDate = returnDateString.toStdString();

    rentalList.add(rentalData);

    std::string fullName = avlTree.getFullNameByDlNumber(selectedClient.toStdString());
   /* std::string brand = hashTable.getBrandFromRegistrationNumber(selectedCar.toStdString());*/
    container.addRentalContainerToScrollArea(ui.rentalScrollArea, QString::fromStdString(fullName), "Brand", returnDateString, selectedClient, selectedCar, rentalDateString, ui.carPickerScrollArea, ui.clientPickerScrollArea);
    json formData = {
        {"action", "insert"},
        {"entity", "record"},
        {"rentalDate", rentalDateString.toStdString()},
        {"returnDate", returnDateString.toStdString()},
        {"gosNumber", selectedCar.toStdString()},
        {"driverLicense", selectedClient.toStdString()}
    };
    ws.sendData(formData);
    
    formData = {
            {"action", "update"},
            {"entity", "caravl"},
            {"registrationNumber", selectedCar.toStdString()},
            {"available", 0}
    };
    ws.sendData(formData);


    
    
    //Удаляем из виду мишину и лиента, которые были выграны в форме
    
    container.removeCarContainerByRegistrationNumber(ui.carsScrollArea, selectedCar);
    container.removeCarContainerFromScrollArea(ui.carPickerScrollArea, selectedCar);
    /*container.removeClientContainerFromScrollArea(ui.clientPickerScrollArea, selectedClient);*/
    container.removeCarContainerByRegistrationNumber(ui.clientsScrollArea, selectedClient);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}
 