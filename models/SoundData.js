const mongoose = require('mongoose');

const SoundDataSchema = mongoose.Schema({
  deviceId: {
    type: String,
    required: true
  },
  
  db: Number,
  ts: {
    type: String,
    default: Date.now(),
  },
});

module.exports = mongoose.model('SoundData', SoundDataSchema)


//collections auftrennen. fixe daten in eigene collections. x
//collection devices und collection measurements x
//in beide collections entweder device id oder object id. x
//device daten von der installation auch in die statische device collection x

//für das streamen. daten in chunks runterladen. stundenweise blöcke.

//TO DO. marker mit den gps daten auf der map placen. und die messdaten in der sidebar anzeigen lassen