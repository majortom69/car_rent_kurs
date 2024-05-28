const dotenv = require('dotenv');
const mysql = require("mysql2");

dotenv.config()
// client
// const pool = mysql.createPool({
//     host: process.env.MYSQL_HOST,
//     user: process.env.MYSQL_USER,
//     password: process.env.MYSQL_PASSWORD,
//     database: process.env.MYSQL_DATABASE,
// });

const pool = mysql.createPool({
    host: process.env.MYSQL_HOST,
    user: process.env.MYSQL_USER,
    database: process.env.MYSQL_DATABASE,
    password: process.env.MYSQL_PASSWORD,
    port: process.env.MYSQL_PORT
});

const promisePool = pool.promise();

async function getDataFromTable(query) {
    let connection;
    try {
        connection = await mysql.createConnection(dbConfig);
        const [rows] = await connection.execute(query);
        return rows;
    } catch (error) {
        console.error("Error fetching data from database:", error);
        throw error;
    } finally {
        if (connection) {
            await connection.end();
        }
    }
}


async function getClients() {
    try {
        const [rows] = await promisePool.query('SELECT * FROM CLIENTS');
        return rows;
    } catch (error) {
        console.error("Error fetcing clients");
        throw error;
    }
}

async function getCars() {
    try {
        const [rows] = await promisePool.query('SELECT * FROM CARS');
        return rows;
    } catch (error) {
        console.error("Error fetcing cars");
        throw error;
    }
}

async function getRecords() {
    try {
        const [rows] = await promisePool.query('SELECT * FROM RECORDS');
        return rows;
    } catch (error) {
        console.error("Error fetcing Records");
        throw error;
    }
}


async function getAllData() {
    const clients = await getDataFromTable('SELECT * FROM CLIENTS');
    const cars = await getDataFromTable('SELECT * FROM CARS');
    const records = await getDataFromTable('SELECT * FROM RECORDS');
    
    return { clients, cars, records };
}

async function insertRecord(driverLicense, gosNumber, issueDate, returnDate) {
    try {
        const [result] = await promisePool.query(
            'INSERT INTO RECORDS (DRIVER_LICENSE, GOS_NUMBER, ISSUE_DATE, RETURN_DATE) VALUES (?, ?, ?, ?)',
            [driverLicense, gosNumber, issueDate, returnDate]
        );

     
        console.log('Record inserted successfully:', result)
    } catch (error) {
        console.error("Error fetcing Records");
        throw error;
    }
}

async function insertClient(driverLicense, fullName, passport, address) {
    try {
        const [result] = await promisePool.query(
            'INSERT INTO CLIENTS (DRIVER_LICENSE, FULL_NAME, PASSPORT, ADDRESS) VALUES (?, ?, ?, ?)',
            [driverLicense, fullName, passport, address]
        );

     
        console.log('CLient inserted successfully:', result)
    } catch (error) {
        console.error("Error fetcing Records");
        throw error;
    }
}

async function insertCar(gosNumber, brand, color, year, available) {
    try {
        const [result] = await promisePool.execute(
            'INSERT INTO CARS (GOS_NUMBER, BRAND, COLOR, YEAR, AVAILABLE) VALUES (?, ?, ?, ?, ?)',
            [gosNumber, brand, color, year, available]
        );

      
        console.log('Record inserted successfully:', result)
    } catch (error) {
        console.error("Error fetcing Records");
        throw error;
    }
}

async function deleteClient(dlNumber) {
    try {
        const [rows] = await promisePool.query('DELETE FROM CLIENTS WHERE DRIVER_LICENSE = ?',
        [dlNumber]);
        return rows;
    } catch (error) {
        console.error("Error fetcing clients");
        throw error;
    }
}

async function deleteCar(gosNumber) {
    try {
        const [rows] = await promisePool.query('DELETE FROM CARS WHERE GOS_NUMBER = ?',
        [gosNumber]);
        return rows;
    } catch (error) {
        console.error("Error fetcing cars");
        throw error;
    }
}

async function deleteRecord(dlNumber) {
    try {
        const [rows] = await promisePool.query('DELETE FROM RECORDS WHERE DRIVER_LICENSE = ?',
        [dlNumber]);
        return rows;
    } catch (error) {
        console.error("Error fetcing records");
        throw error;
    }
}


async function editCar(gosNumber,  newBrand, newColor, newYear, newAvailable) {
    try {
        console.log("chaning car info")
        // const [rows] = await promisePool.query(
        //     'SELECT AVAILABLE FROM CARS WHERE GOS_NUMBER = ?',
        //     [driverLicense]
        // );

        const [rows] = await promisePool.query(
            'UPDATE CARS SET BRAND = ?, COLOR = ?, YEAR = ?, AVAILABLE = ? WHERE GOS_NUMBER = ?', [newBrand, newColor, newYear, newAvailable, gosNumber]
        );
        console.log(rows);
    } catch (error) {
        console.error("Error fetcing records");
        throw error;
    }
}

async function editClient(dlNumber, fullName, passport, address) {
    try {
     

        const [rows] = await promisePool.query(
            'UPDATE CLIENTS SET FULL_NAME = ?, PASSPORT = ?, ADDRESS = ? WHERE DRIVER_LICENSE = ?', [fullName, passport, address, dlNumber]
        );
        console.log(rows);
    } catch (error) {
        console.error("Error fetcing clients");
        throw error;
    }
}

async function updateCarAvilability(newAvailable, gosNumber) {
    try {
     

        const [rows] = await promisePool.query(
            'UPDATE CARS SET AVAILABLE = ? WHERE GOS_NUMBER = ?', [gosNumber, newAvailable]
        );
        console.log(rows);
    } catch (error) {
        console.error("Error fetcing clients");
        throw error;
    }
}



module.exports = {
    getDataFromTable,getAllData,
    getClients, getCars, getRecords, 
    insertRecord, insertClient, insertCar,
    deleteClient, deleteCar, deleteRecord,
    editCar,editClient, updateCarAvilability
}