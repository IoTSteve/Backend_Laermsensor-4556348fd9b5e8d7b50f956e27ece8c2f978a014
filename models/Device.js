const mongoose = require('mongoose');

const DeviceSchema = mongoose.Schema({
  deviceId: {
    type: String,
    unique: true,
    required: true
  },
  long: String,
  lat: String,
  ts: {
    type: String,
    default: Date.now(),
  },
  adress: String, 
});

module.exports = mongoose.model('Device', DeviceSchema)


