#ifndef QTCONTAINERS_CLASS_H
#define QTCONTAINERS_CLASS_H
#include "websocket.h"
#include <QtWidgets>

#include <nlohmann/json.hpp>
#include "avlTree.h"
#include "hashTable.h"
#include "rentalList.h"

using json = nlohmann::json;

class CreateContainer
{
public:
	CreateContainer(Websocket& ws_, HashTable& table_, AVLTree& tree_, RentalList& list_) : ws(ws_) , table(table_), tree(tree_), list(list_){};
	~CreateContainer() {};

	void addClientContainerToScrollArea(QScrollArea* scrollArea,
										QWidget* clientEditFrame, QScrollArea* RentalScrollArea,
										const QString& dlNumber, 
										const QString& fullName, 
										const QString& passport, 
										const QString& address);


	void addCarContainerToScrollArea(QScrollArea* scrollArea,
									 QWidget* carEditFrame,
									 QScrollArea* RentalScrollArea, QScrollArea* carPickerScrollArea,
									 const QString& registrationNumber, 
									 const QString& brand, 
									 const QString& color, 
									 int year, bool available, QString fullName,
		QString dlNumber );




	// ScrollArea для выбора машины при вадачи на прокат
	void addSimpleCarContainerToScrollArea(QScrollArea* scrollArea, 
										   const QString& registrationNumber, 
										   const QString& brand);


	void removeCarContainerFromScrollArea(QScrollArea* scrollArea, 
										  const QString& 
										  registrationNumber);

	void removeCarContainerByRegistrationNumber(QScrollArea* scrollArea, const QString& registrationNumber);
	QString extractRegistrationNumberFromLabelText(const QString& labelText);

	void addSimpleClientContainerToScrollArea(QScrollArea* scrollArea,
		const QString& dlNumber,
		const QString& fullName);

	void removeClientContainerFromScrollArea(QScrollArea* scrollArea,
											const QString& dlNumber);

	QString getSelectedRegistrationNumber(QScrollArea* scrollArea);
	QString getSelectedDlNumber(QScrollArea* scrollArea);


	void addRentalContainerToScrollArea(QScrollArea* scrollArea,
		const QString& fullName,
		const QString& brand,
		const QString& returnDate,
		const QString& dlNumber,
		const QString& registrationNumber,
		const QString& rentalDate, QScrollArea* carPickerScrollArea,
		QScrollArea* clientPickerScrollArea);

private:
	Websocket& ws;
	HashTable& table;
	AVLTree& tree;
	RentalList& list;
	QWidget* createClientContainer(const QString& dlNumber,
		const QString& fullName,
		const QString& passport,
		const QString& address,
		QWidget* clientEditFrame,
		QScrollArea* RentalScrollArea);

	QWidget* createCarContainer(const QString& registrationNumber, 
								const QString& brand, 
								const QString& color, 
								int year, 
								bool available,
		QString fullName,
		QString dlNumber,
								QWidget* carEditFrame, 
								QScrollArea* RentalScrollArea, QScrollArea* carPickerScrollArea);

	//выдача  на прокат
	QWidget* createCarContainerForRental(const QString& registrationNumber, 
										 const QString& brand, 
										 QButtonGroup* buttonGroup);


	QButtonGroup* ensureButtonGroup(QScrollArea* scrollArea);




	QWidget* createClientContainerForRental(const QString& dlNumber,
											const QString& fullName,
											QButtonGroup* buttonGroup);

	QButtonGroup* ensureClientButtonGroup(QScrollArea* scrollArea);




	QWidget* createRentalContainer(const QString& fullName,
		const QString& brand,
		const QString& returnDate,
		const QString& dlNumber,
		const QString& registrationNumber,
		const QString& rentalDate,
		QScrollArea* rentalScrollArea,
		QScrollArea* carPickerScrollArea,
		QScrollArea* clientPickerScrollArea);

};

#endif