// const express = require("express");
const { getDataFromTable, getAllData, getClients, getCars, getRecords, insertRecord, insertClient, insertCar, deleteClient, deleteCar, deleteRecord, editCar, editClient, updateCarAvilability } = require("./database");
// const { createHash } = require('crypto');
// const multer = require('multer');


// function hash(string) {
//   return createHash('sha256').update(string).digest('hex');
// }

// const app = express();
// const PORT = 8000;

// app.use(express.urlencoded({ extended: false }));
// app.use(express.json());

// const storage = multer.diskStorage({
//   destination: function (req, file, cb) {
//       cb(null, 'uploads/');
//   },
//   filename: function (req, file, cb) {
//       const compName = req.body.computerName;
//       const fileExt = compName + ".png"; 
//       cb(null, fileExt);
//   }
// });

// const upload = multer({ storage: storage });

// app.post('/mysqldata', async (req, res) => {
//   const clients = await getClients();
//   // console.log("client hash: " ,req.body.computerName);
//   // // const  clients = await getClients();
//   // // // console.log("server hash: ",JSON.stringify(clients));
//   // // console.log("server hash: ", hash(JSON.stringify(clients)))
//   // // res.send(JSON.stringify(clients));
//   // // if(req.body.computerName == hash(JSON.stringify(clients))) {
//   // //   res.send(JSON.stringify(clients))
//   // // }
//   // res.send(req.body.computerName);
//   res.send(clients);
 
  
  
// })

// app.listen(PORT, () => {
//   console.log("Port is listening on port " + PORT);
// });


const WebSocket = require('ws');
const server = new WebSocket.Server({ port: 8080 });

server.on('connection', async (ws) => {
  console.log('Client connected');

  try {
      
      const clients = await getClients();
      const cars = await getCars();
      const records = await getRecords();


      const data = {
          clients: clients,
          cars: cars,
          records: records
      };
      
      // Отправить данные с БД клиенту
      ws.send(JSON.stringify(data));
  } catch (error) {
      ws.send(JSON.stringify({ error: 'Error fetching data' }));
  }
  ws.on('message', async function incoming(message) {
    console.log('Received message:', message.toString());

    try {
        
        const data = JSON.parse(message);
        console.log(data)

        // Process the received data
        // console.log('Rental Date:', data.rentalDate);
        // console.log('Return Date:', data.returnDate);
        // console.log('Selected Car:', data.selectedCar);
        // console.log('Selected Client:', data.selectedClient);
        // await insertRecord(data.selectedClient, data.selectedCar,data.rentalDate, data.returnDate );

        if (data.action === 'insert') {
          if (data.entity === 'car') {
              await insertCar(data.registrationNumber, data.brand, data.color, data.year, data.available);
          } else if (data.entity === 'client') {
              await insertClient(data.driverLicense, data.fullName, data.passport, data.address);
          } else if (data.entity === 'record') {
              await insertRecord(data.driverLicense, data.gosNumber, data.rentalDate, data.returnDate);
          }
      } else if (data.action === 'update') {
        if (data.entity === 'car') {
          await editCar(data.registrationNumber, data.brand, data.color, data.year, data.available);
        } else if (data.entity === 'client') {
          await editClient(data.driverLicense, data.fullName, data.passport, data.address);
        } else if (data.entity === 'record') {
          // await deleteRecord(data.driverLicense);
        } else if(data.entity === 'caravl') {
          await updateCarAvilability(data.registrationNumber, data.available);
        }
      } else if (data.action === 'delete') {
          if (data.entity === 'car') {
            await deleteCar(data.registrationNumber);
          } else if (data.entity === 'client') {
            await deleteClient(data.driverLicense);
          } else if (data.entity === 'record') {
            await deleteRecord(data.driverLicense);
          }
      } else {
          
      }

        
    } catch (error) {
        console.error('Error parsing JSON:', error);
    }
  });
  
  ws.on('close', () => {
      console.log('Client disconnected');
  });
});

console.log('WebSocket server is running on ws://localhost:8080');