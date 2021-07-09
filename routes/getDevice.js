const express = require('express');
const router = express.Router();
const Device = require('../models/Device');

router.get('/', async (req, res) => {
  try {
    const deviceData = await Device.find();
    res.json(deviceData);
  } catch (err) {
    res.json({
      message: err
    });

  }

});

module.exports = router;