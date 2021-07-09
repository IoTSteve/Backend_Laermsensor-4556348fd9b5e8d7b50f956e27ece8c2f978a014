const express = require('express');
const router = express.Router();
const SoundData = require('../models/SoundData');

const agg = [
  {
    $sort: {
      deviceId: 1,
      ts: -1,
    },
  },
  {
    $group: { 
      _id: "$deviceId",
      ts: {$first: "$ts"},
      db: {$first: "$db"},
    
    }
  }
];

router.get('/', async (req, res) => {
  try {
    const soundData = await SoundData.aggregate(agg);
    res.json(soundData);
  } catch (err) {
    res.json({
      message: err
    });

  }

});

module.exports = router;