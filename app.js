const express = require('express');
const app = express();
const mongoose = require('mongoose');
const cors = require('cors');
require('dotenv/config');
//const SoundData = require('./models/SoundData');
const msqt = require('./mqttwifi');

//mqtt
let mqttClient = msqt.connectmqtt(); 

//MIDDLEWARE
app.use(cors());
app.use(express.json());

//ROUTES
const getSoundDataRoute = require('./routes/getSoundData');
app.use('/getSoundData', getSoundDataRoute);
const addSensorRoute = require('./routes/addSensor');
app.use('/addSensor', addSensorRoute);
const getDeviceRoute = require('./routes/getDevice');
app.use('/getDevice', getDeviceRoute);


//Backend Landing Page
app.get('/', (req,res) => {
  res.send('Welcome to the dungeon');
});

//connect to db
mongoose.connect(process.env.DB_CONNECTION,
  {
    useUnifiedTopology: false,
    useNewUrlParser: true
  }, () => {
    console.log('connected to db!');
});

//start listening to the server
app.listen(3000);