//#include "rentalList.h"
//
//void RentalList::add(const RentalData& rental) {
//    dataMap[rental.registrationNumber] = rental;
//    shakerSort();
//}
//
//
//
//void RentalList::print() {
//    for (const auto& entry : dataMap) {
//        std::cout << "Registration Number: " << entry.first << std::endl;
//        std::cout << "Driver's License Number: " << entry.second.driverLicenseNumber << std::endl;
//        std::cout << "Rental Date: " << entry.second.rentalDate << std::endl;
//        std::cout << "Return Date: " << entry.second.returnDate << std::endl;
//        std::cout << std::endl;
//    }
//}