#include "qtcontainers.h"

QWidget* CreateContainer::createClientContainer(const QString& dlNumber, 
                                                const QString& fullName, 
                                                const QString& passport, 
                                                const QString& address, 
                                         
                                                QWidget* clientEditFrame, QScrollArea* RentalScrollArea) {
    // Create a new container widget
    QWidget* mainContainer = new QWidget;

    // Set the background color and border for visualization
    mainContainer->setStyleSheet("background-color: white; border: 1px solid gray;");

    // Create the collapsible container layout
    QVBoxLayout* containerLayout = new QVBoxLayout(mainContainer);
    mainContainer->setLayout(containerLayout);

    // Create the main label/button to toggle visibility
    QPushButton* toggleButton = new QPushButton("↓", mainContainer);
    toggleButton->setFixedSize(20, 20); // Set button size
    toggleButton->setCheckable(true); // Make it checkable to toggle visibility

    // Create the main information label (always visible)
    QLabel* mainInfoLabel = new QLabel("Номер водительского удостоверения: " + dlNumber + "<br>Имя: " + fullName, mainContainer);
    mainInfoLabel->setTextFormat(Qt::RichText);
    mainInfoLabel->setAlignment(Qt::AlignTop);

    // Add the main information label and toggle button to the container layout
    containerLayout->addWidget(mainInfoLabel);
    containerLayout->addWidget(toggleButton);

    // Create the additional information widget (initially hidden)
    QWidget* additionalInfoWidget = new QWidget(mainContainer);
    additionalInfoWidget->setStyleSheet("background-color: lightgray; border: 1px solid gray;");
    additionalInfoWidget->setVisible(false); // Initially hide the additional info
    additionalInfoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Allow expansion horizontally

    // Create additional information labels
    QVBoxLayout* additionalInfoLayout = new QVBoxLayout(additionalInfoWidget);
    QLabel* additionalInfoLabel = new QLabel("Пасспорт: " + passport + "<br>Адрес: " + address, additionalInfoWidget);
    additionalInfoLabel->setTextFormat(Qt::RichText);
    additionalInfoLabel->setAlignment(Qt::AlignTop);
    additionalInfoLayout->addWidget(additionalInfoLabel);

    // Add the delete button
    QPushButton* deleteButton = new QPushButton("Снять с обслуживания", additionalInfoWidget);
    QObject::connect(deleteButton, &QPushButton::clicked, [=]() {
        mainContainer->deleteLater();
        removeClientContainerFromScrollArea(RentalScrollArea, dlNumber);
        json formData = {
            {"action", "delete"},
            {"entity", "client"},
            {"driverLicense", dlNumber.toStdString()},
        };
        ws.sendData(formData);
        tree.remove(dlNumber.toStdString());
        /*QMessageBox::information(this, "Успешно!", "Клиент добавлен в авл дерево");*/
       
        
        });
    additionalInfoLayout->addWidget(deleteButton);

    QPushButton* editButton = new QPushButton("Изменить информацию", additionalInfoWidget);
    QObject::connect(editButton, &QPushButton::clicked, [=]() {
      
        QLineEdit* firstNameEdit = clientEditFrame->findChild<QLineEdit*>("firstNameEdit");
        QLineEdit* middleNameEdit = clientEditFrame->findChild<QLineEdit*>("middleNameEdit");
        QLineEdit* lastNameEdit = clientEditFrame->findChild<QLineEdit*>("lastNameEdit");
        QLineEdit* driversLicenseEdit = clientEditFrame->findChild<QLineEdit*>("driversLicenseEdit");
        QLineEdit* passportEdit = clientEditFrame->findChild<QLineEdit*>("passportEdit");
        QLineEdit* addressEdit = clientEditFrame->findChild<QLineEdit*>("addressEdit");

        firstNameEdit->setText(fullName.split(' ')[0]);
        middleNameEdit->setText(fullName.split(' ').size() > 2 ? fullName.split(' ')[1] : "");
        lastNameEdit->setText(fullName.split(' ').size() > 1 ? fullName.split(' ').last() : "");
        driversLicenseEdit->setText(dlNumber);
        passportEdit->setText(passport);
        addressEdit->setText(address);

        clientEditFrame->show();

        
       
        QPushButton* submitClientEdit = clientEditFrame->findChild<QPushButton*>("submitClientEdit");
        QObject::connect(submitClientEdit, &QPushButton::clicked, [=]() {
            
            QString editedDlNumber = driversLicenseEdit->text();
            QString editedFullName = firstNameEdit->text() + " " + middleNameEdit->text() + " " + lastNameEdit->text();
            QString editedPassport = passportEdit->text();
            QString editedAddress = addressEdit->text();

            
            mainInfoLabel->setText("Номер водительского удостоверения: " + editedDlNumber + "<br>Имя: " + editedFullName);

            // Update additionalInfoLabel with edited data
            additionalInfoLabel->setText("Пасспорт: " + editedPassport + "<br>Адресс: " + editedAddress);

            // Hide the client edit frame
            clientEditFrame->hide();

            json formData = {
                {"action", "update"},
                {"entity", "client"},
                {"driverLicense", editedDlNumber.toStdString()},
                {"fullName", editedFullName.toStdString()},
                {"passport", editedPassport.toStdString()},
                {"address", editedAddress.toStdString()},
              
            };
            ws.sendData(formData);
            });
        });
    additionalInfoLayout->addWidget(editButton);

    // Add connections to toggle visibility when the button is clicked
    QObject::connect(toggleButton, &QPushButton::clicked, [=]() {
        additionalInfoWidget->setVisible(toggleButton->isChecked());
        toggleButton->setText(toggleButton->isChecked() ? "↑" : "↓");
        });

    // Add the additional information widget to the container layout
    containerLayout->addWidget(additionalInfoWidget);

    return mainContainer;
}


QWidget* CreateContainer::createCarContainer(const QString& registrationNumber,
    const QString& brand,
    const QString& color,
    int year,
    bool available,
    QString fullName,
    QString dlNumber,// Add availability parameter
    QWidget* carEditFrame,
    QScrollArea* RentalScrollArea, QScrollArea* carPickerScrollArea) {
    QWidget* mainContainer = new QWidget;
    mainContainer->setStyleSheet("background-color: white; border: 1px solid gray;");

    QVBoxLayout* containerLayout = new QVBoxLayout(mainContainer);
    mainContainer->setLayout(containerLayout);

    QPushButton* toggleButton = new QPushButton("↓", mainContainer);
    toggleButton->setFixedSize(20, 20);
    toggleButton->setCheckable(true);

    QLabel* mainInfoLabel = new QLabel("Регистрационный номер: " + registrationNumber + "<br>Марка: " + brand, mainContainer);
    mainInfoLabel->setTextFormat(Qt::RichText);
    mainInfoLabel->setAlignment(Qt::AlignTop);

    containerLayout->addWidget(mainInfoLabel);
    containerLayout->addWidget(toggleButton);

    QWidget* additionalInfoWidget = new QWidget(mainContainer);
    additionalInfoWidget->setStyleSheet("background-color: lightgray; border: 1px solid gray;");
    additionalInfoWidget->setVisible(false);
    additionalInfoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout* additionalInfoLayout = new QVBoxLayout(additionalInfoWidget);
    QLabel* additionalInfoLabel = new QLabel("Цвет: " + color + "<br>Год выпуска: " + QString::number(year), additionalInfoWidget);
    additionalInfoLabel->setTextFormat(Qt::RichText);
    additionalInfoLabel->setAlignment(Qt::AlignTop);
    additionalInfoLayout->addWidget(additionalInfoLabel);

    // Availability Button
    QPushButton* availabilityButton = new QPushButton(available ? "Available" : "Unavailable", additionalInfoWidget);
    availabilityButton->setStyleSheet(available ? "background-color: green;" : "background-color: red;");
    QObject::connect(availabilityButton, &QPushButton::clicked, [=]() mutable {
        available = !available;
        json formData = {
            {"action", "update"},
            {"entity", "caravl"},
            {"registrationNumber", registrationNumber.toStdString()},
            {"available", available}
        };
        ws.sendData(formData);

        if (available) {
            addSimpleCarContainerToScrollArea(carPickerScrollArea, registrationNumber, brand);
        }
        else {
            removeCarContainerFromScrollArea(carPickerScrollArea, registrationNumber);
        }

        availabilityButton->setText(available ? "Available" : "Unavailable");
        availabilityButton->setStyleSheet(available ? "background-color: green;" : "background-color: red;");
        // Emit a signal or call a function to update availability status on the server or elsewhere
        });
    additionalInfoLayout->addWidget(availabilityButton);

    QPushButton* deleteButton = new QPushButton("Удалить", additionalInfoWidget);
    QObject::connect(deleteButton, &QPushButton::clicked, [=]() {
        mainContainer->deleteLater(); // Remove the container when the delete button is clicked
        removeCarContainerFromScrollArea(RentalScrollArea, registrationNumber);
        json formData = {
            {"action", "delete"},
            {"entity", "car"},
            {"registrationNumber", registrationNumber.toStdString()},
        };
        ws.sendData(formData);
        table.remove(registrationNumber.toStdString());
        });
    additionalInfoLayout->addWidget(deleteButton);
    if (!available)
    {
        QLabel* nameLabel = new QLabel("Полное имя: " + fullName, additionalInfoWidget);
        additionalInfoLayout->addWidget(nameLabel);
        QLabel* dlLabel = new QLabel("Номер водительского удостоверения: " + dlNumber, additionalInfoWidget);
        additionalInfoLayout->addWidget(dlLabel);
    }
    QPushButton* editButton = new QPushButton("Изменить", additionalInfoWidget);
    QObject::connect(editButton, &QPushButton::clicked, [=]() {
        // Show the car edit frame and fill its fields with car data
        QComboBox* brandEdit = carEditFrame->findChild<QComboBox*>("brandEdit");
        QLineEdit* gosNumberEdit = carEditFrame->findChild<QLineEdit*>("gosNumberEdit");
        QComboBox* colorEdit = carEditFrame->findChild<QComboBox*>("colorEdit");
        QDateEdit* yearofIssueEdit = carEditFrame->findChild<QDateEdit*>("yearofIssueEdit");

        gosNumberEdit->setText(registrationNumber);
        brandEdit->setCurrentText(brand);
        colorEdit->setCurrentText(color);
        yearofIssueEdit->setDate(QDate(year, 1, 1));

        carEditFrame->show();

        
        QPushButton* submitCarEdit = carEditFrame->findChild<QPushButton*>("sumbitCarEdit");
        QObject::connect(submitCarEdit, &QPushButton::clicked, [=]() {
            
            QString editedRegistrationNumber = gosNumberEdit->text();
            QString editedBrand = brandEdit->currentText();
            QString editedColor = colorEdit->currentText();
            int editedYear = yearofIssueEdit->date().year();

            
            mainInfoLabel->setText("Регистрационный номер: " + editedRegistrationNumber + "<br>Марка: " + editedBrand);


            additionalInfoLabel->setText("Цвет: " + editedColor + "<br>Год выпуска: " + QString::number(editedYear));

            json formData = {
            {"action", "update"},
            {"entity", "car"},
            {"registrationNumber", editedRegistrationNumber.toStdString()},
            {"brand", editedBrand.toStdString()},
            {"color", editedColor.toStdString()},
            {"year", editedYear},
            {"available", 1}
            };
            ws.sendData(formData);
      
            carEditFrame->hide();
            });
        });
    additionalInfoLayout->addWidget(editButton);

    QObject::connect(toggleButton, &QPushButton::clicked, [=]() {
        additionalInfoWidget->setVisible(toggleButton->isChecked());
        toggleButton->setText(toggleButton->isChecked() ? "↑" : "↓");
        });

    containerLayout->addWidget(additionalInfoWidget);

    return mainContainer;
}

QString CreateContainer::extractRegistrationNumberFromLabelText(const QString& labelText) {
    
    int startPos = labelText.indexOf("Регистрационный номер: ") + QString("Регистрационный номер: ").length();
    int endPos = labelText.indexOf("<br>");
    return labelText.mid(startPos, endPos - startPos);
}
void CreateContainer::removeCarContainerByRegistrationNumber(QScrollArea* scrollArea, const QString& registrationNumber) {
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollArea->widget()->layout());
    if (!layout) {
        qDebug() << "No layout found in the scroll area.";
        return;
    }

    for (int i = 0; i < layout->count(); ++i) {
        QWidget* container = layout->itemAt(i)->widget();
        if (!container) continue;

        QLabel* mainInfoLabel = container->findChild<QLabel*>();
        if (mainInfoLabel) {
            QString mainLabelText = mainInfoLabel->text();
            QString extractedRegistrationNumber = extractRegistrationNumberFromLabelText(mainLabelText);
            if (extractedRegistrationNumber == registrationNumber) {
                qDebug() << "Removing container with registration number:" << registrationNumber;
                layout->removeWidget(container);
                delete container;
                return;
            }
        }
    }

    qDebug() << "No container found with registration number:" << registrationNumber;
}

void CreateContainer::addClientContainerToScrollArea(QScrollArea* scrollArea, QWidget* clientEditFrame,  QScrollArea* carPickerScrollArea, const QString& dlNumber, const QString& fullName, const QString& passport, const QString& address) {
    QWidget* container = createClientContainer(dlNumber, fullName, passport, address, clientEditFrame,  carPickerScrollArea);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollArea->widget()->layout());

    if (!layout) {
        layout = new QVBoxLayout(scrollArea->widget());
        layout->setAlignment(Qt::AlignTop);
    }
    layout->addWidget(container);
}

void CreateContainer::addCarContainerToScrollArea(QScrollArea* scrollArea, 
                                                    QWidget* carEditFrame, 
                                                    QScrollArea* RentalScrollArea, QScrollArea* carPickerScrollArea,
                                                    const QString& registrationNumber, 
                                                    const QString& brand, 
                                                    const QString& color, 
                                                    int year, bool available, QString fullName,
                                                    QString dlNumber) {
    QWidget* container = createCarContainer(registrationNumber, brand, color, year, available, fullName, dlNumber, carEditFrame, RentalScrollArea, carPickerScrollArea);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollArea->widget()->layout());

    if (!layout) {
        layout = new QVBoxLayout(scrollArea->widget());
        layout->setAlignment(Qt::AlignTop);
    }
    layout->addWidget(container);
}








QWidget* CreateContainer::createCarContainerForRental(const QString& registrationNumber, const QString& brand, QButtonGroup* buttonGroup) {
    QWidget* mainContainer = new QWidget;
    mainContainer->setStyleSheet("background-color: white; border: 1px solid gray;");

    QVBoxLayout* containerLayout = new QVBoxLayout(mainContainer);
    mainContainer->setLayout(containerLayout);

    QString displayText = "" + registrationNumber + " : " + brand;
    QRadioButton* radioButton = new QRadioButton(displayText, mainContainer);
    /*radioButton->setTextFormat(Qt::RichText);*/
    radioButton->setChecked(false); // Ensure radio buttons are unchecked by default
    radioButton->setProperty("registrationNumber", registrationNumber); // Store registration number as a property

    buttonGroup->addButton(radioButton);
    containerLayout->addWidget(radioButton);

    return mainContainer;
}

QButtonGroup* CreateContainer::ensureButtonGroup(QScrollArea* scrollArea) {
    static QButtonGroup* buttonGroup = nullptr;
    if (!buttonGroup) {
        buttonGroup = new QButtonGroup(scrollArea);
        buttonGroup->setExclusive(true); // Ensure only one radio button can be selected at a time
    }
    return buttonGroup;
}

void CreateContainer::addSimpleCarContainerToScrollArea(QScrollArea* scrollArea, const QString& registrationNumber, const QString& brand) {
    QButtonGroup* buttonGroup = ensureButtonGroup(scrollArea);
    QWidget* container = createCarContainerForRental(registrationNumber, brand, buttonGroup);

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollArea->widget()->layout());

    if (!layout) {
        QWidget* viewport = new QWidget;
        layout = new QVBoxLayout(viewport);
        layout->setAlignment(Qt::AlignTop);
        scrollArea->setWidget(viewport);
        scrollArea->setWidgetResizable(true);
    }
    layout->addWidget(container);
}

const char* REGISTRATION_NUMBER_PROPERTY = "registrationNumber";

void CreateContainer::removeCarContainerFromScrollArea(QScrollArea* scrollArea, const QString& registrationNumber) {
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollArea->widget()->layout());
    if (!layout) {
        qDebug() << "No layout found in the scroll area.";
        return;
    }

    for (int i = 0; i < layout->count(); ++i) {
        QWidget* container = layout->itemAt(i)->widget();
        if (!container) continue;

        QRadioButton* radioButton = container->findChild<QRadioButton*>();
        if (radioButton) {
            QVariant regNumberProp = radioButton->property("registrationNumber");
            if (regNumberProp.isValid()) {
                QString storedRegistrationNumber = regNumberProp.toString();
                qDebug() << "Checking container with registration number:" << storedRegistrationNumber;
                if (storedRegistrationNumber == registrationNumber) {
                    qDebug() << "Removing container with registration number:" << registrationNumber;
                    layout->removeWidget(container);
                    delete container;
                    return;
                }
            }
            else {
                qDebug() << "Radio button does not have a valid registration number property.";
            }
        }
    }

    qDebug() << "No container found with registration number:" << registrationNumber;
}





QButtonGroup* CreateContainer::ensureClientButtonGroup(QScrollArea* scrollArea) {
    static QButtonGroup* buttonGroup = nullptr;
    if (!buttonGroup) {
        buttonGroup = new QButtonGroup(scrollArea);
        buttonGroup->setExclusive(true); // Ensure only one radio button can be selected at a time
    }
    return buttonGroup;
}

QWidget* CreateContainer::createClientContainerForRental(const QString& dlNumber, const QString& fullName, QButtonGroup* buttonGroup) {
    QWidget* mainContainer = new QWidget;
    mainContainer->setStyleSheet("background-color: white; border: 1px solid gray;");

    QVBoxLayout* containerLayout = new QVBoxLayout(mainContainer);
    mainContainer->setLayout(containerLayout);

    QString displayText = "" + dlNumber + " : " + fullName;
    QRadioButton* radioButton = new QRadioButton(displayText, mainContainer);
    /*radioButton->setTextFormat(Qt::RichText);*/
    radioButton->setChecked(false); // Ensure radio buttons are unchecked by default
    radioButton->setProperty("dlNumber", dlNumber); // Store registration number as a property

    buttonGroup->addButton(radioButton);
    containerLayout->addWidget(radioButton);

    return mainContainer;
}

void CreateContainer::addSimpleClientContainerToScrollArea(QScrollArea* scrollArea, const QString& dlNumber, const QString& fullName) {
    QButtonGroup* buttonGroup = ensureClientButtonGroup(scrollArea);
    QWidget* container = createClientContainerForRental(dlNumber, fullName, buttonGroup);

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollArea->widget()->layout());

    if (!layout) {
        QWidget* viewport = new QWidget;
        layout = new QVBoxLayout(viewport);
        layout->setAlignment(Qt::AlignTop);
        scrollArea->setWidget(viewport);
        scrollArea->setWidgetResizable(true);
    }
    layout->addWidget(container);
}

void CreateContainer::removeClientContainerFromScrollArea(QScrollArea* scrollArea, const QString& dlNumber) {
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollArea->widget()->layout());
    if (!layout) {
        qDebug() << "No layout found in the scroll area.";
        return;
    }

    for (int i = 0; i < layout->count(); ++i) {
        QWidget* container = layout->itemAt(i)->widget();
        if (!container) continue;

        QRadioButton* radioButton = container->findChild<QRadioButton*>();
        if (radioButton) {
            QVariant regNumberProp = radioButton->property("dlNumber");
            if (regNumberProp.isValid()) {
                QString storedRegistrationNumber = regNumberProp.toString();
                qDebug() << "Checking container with registration number:" << storedRegistrationNumber;
                if (storedRegistrationNumber == dlNumber) {
                    qDebug() << "Removing container with registration number:" << dlNumber;
                    layout->removeWidget(container);
                    delete container;
                    return;
                }
            }
            else {
                qDebug() << "Radio button does not have a valid registration number property.";
            }
        }
    }

    qDebug() << "No container found with registration number:" << dlNumber;
}


QString CreateContainer::getSelectedRegistrationNumber(QScrollArea* scrollArea) {
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollArea->widget()->layout());
    if (!layout) {
        qDebug() << "No layout found in the scroll area.";
        return QString();
    }

    for (int i = 0; i < layout->count(); ++i) {
        QWidget* container = layout->itemAt(i)->widget();
        if (!container) continue;

        QRadioButton* radioButton = container->findChild<QRadioButton*>();
        if (radioButton && radioButton->isChecked()) {
            QVariant regNumberProp = radioButton->property("registrationNumber");
            if (regNumberProp.isValid()) {
                return regNumberProp.toString();
            }
        }
    }

    qDebug() << "No radio button with a valid registration number property is selected in the scroll area.";
    return QString();
}


QString CreateContainer::getSelectedDlNumber(QScrollArea* scrollArea) {
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollArea->widget()->layout());
    if (!layout) {
        qDebug() << "No layout found in the scroll area.";
        return QString();
    }

    for (int i = 0; i < layout->count(); ++i) {
        QWidget* container = layout->itemAt(i)->widget();
        if (!container) continue;

        QRadioButton* radioButton = container->findChild<QRadioButton*>();
        if (radioButton && radioButton->isChecked()) {
            QVariant regNumberProp = radioButton->property("dlNumber");
            if (regNumberProp.isValid()) {
                return regNumberProp.toString();
            }
        }
    }

    qDebug() << "No radio button with a valid registration number property is selected in the scroll area.";
    return QString();
}



QWidget* CreateContainer::createRentalContainer(const QString& fullName,
    const QString& brand,
    const QString& returnDate,
    const QString& dlNumber,
    const QString& registrationNumber,
    const QString& rentalDate,
    QScrollArea* rentalScrollArea, QScrollArea* carPickerScrollArea,
    QScrollArea* clientPickerScrollArea) {
    QWidget* mainContainer = new QWidget;
    mainContainer->setStyleSheet("background-color: white; border: 1px solid gray;");

    QVBoxLayout* containerLayout = new QVBoxLayout(mainContainer);
    mainContainer->setLayout(containerLayout);

    QPushButton* toggleButton = new QPushButton("↓", mainContainer);
    toggleButton->setFixedSize(20, 20);
    toggleButton->setCheckable(true);

    QString mainInfoText = QString("%1<br>%2<br>Дата вовзврата: %3").arg(fullName).arg(brand).arg(returnDate);
    QLabel* mainInfoLabel = new QLabel(mainInfoText, mainContainer);
    mainInfoLabel->setTextFormat(Qt::RichText);
    mainInfoLabel->setAlignment(Qt::AlignTop);

    containerLayout->addWidget(mainInfoLabel);
    containerLayout->addWidget(toggleButton);

    QWidget* additionalInfoWidget = new QWidget(mainContainer);
    additionalInfoWidget->setStyleSheet("background-color: lightgray; border: 1px solid gray;");
    additionalInfoWidget->setVisible(false);
    additionalInfoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout* additionalInfoLayout = new QVBoxLayout(additionalInfoWidget);
    QString additionalInfoText = QString("Вод. уд.: %1<br>Регистрационный номер: %2<br>Выдан в аренду: %3")
        .arg(dlNumber)
        .arg(registrationNumber)
        .arg(rentalDate);
    QLabel* additionalInfoLabel = new QLabel(additionalInfoText, additionalInfoWidget);
    additionalInfoLabel->setTextFormat(Qt::RichText);
    additionalInfoLabel->setAlignment(Qt::AlignTop);
    additionalInfoLayout->addWidget(additionalInfoLabel);

    QPushButton* removeButton = new QPushButton("Remove", additionalInfoWidget);
    QObject::connect(removeButton, &QPushButton::clicked, [=]() {
        mainContainer->deleteLater(); // Remove the container when the remove button is clicked
        removeCarContainerFromScrollArea(rentalScrollArea, registrationNumber);
        json formData = {
            {"action", "delete"},
            {"entity", "record"},
            {"driverLicense", dlNumber.toStdString()},
        };
        ws.sendData(formData);

        formData = {
            {"action", "update"},
            {"entity", "caravl"},
            {"registrationNumber", registrationNumber.toStdString()},
            {"available", 1}


        };
        ws.sendData(formData);
        list.removeByRegistrationNumber(registrationNumber.toStdString());
       

        
       
        addSimpleCarContainerToScrollArea(carPickerScrollArea, registrationNumber, brand);
        addSimpleClientContainerToScrollArea(clientPickerScrollArea, dlNumber, fullName);
        });
    additionalInfoLayout->addWidget(removeButton);

    QObject::connect(toggleButton, &QPushButton::clicked, [=]() {
        additionalInfoWidget->setVisible(toggleButton->isChecked());
        toggleButton->setText(toggleButton->isChecked() ? "↑" : "↓");
        });

    containerLayout->addWidget(additionalInfoWidget);

    return mainContainer;
}

void CreateContainer::addRentalContainerToScrollArea(QScrollArea* scrollArea,
    const QString& fullName,
    const QString& brand,
    const QString& returnDate,
    const QString& dlNumber,
    const QString& registrationNumber,
    const QString& rentalDate, QScrollArea* carPickerScrollArea,
    QScrollArea* clientPickerScrollArea) {
    QWidget* container = createRentalContainer(fullName, brand, returnDate, dlNumber, registrationNumber, rentalDate, scrollArea, carPickerScrollArea, clientPickerScrollArea);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollArea->widget()->layout());

    if (!layout) {
        QWidget* viewport = new QWidget;
        layout = new QVBoxLayout(viewport);
        layout->setAlignment(Qt::AlignTop);
        scrollArea->setWidget(viewport);
        scrollArea->setWidgetResizable(true);
    }
    layout->addWidget(container);
}