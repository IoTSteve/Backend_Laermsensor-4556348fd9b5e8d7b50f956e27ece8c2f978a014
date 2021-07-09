const express = require('express');
const router = express.Router();
const Device = require('../models/Device');

router.post('/', async (req, res) => {
  const newDevice = Device({
    deviceId: req.body.deviceId,
    long: req.body.long,
    lat: req.body.lat,
    adress: req.body.adress,
  });
  newDevice.save();
});

module.exports = router;