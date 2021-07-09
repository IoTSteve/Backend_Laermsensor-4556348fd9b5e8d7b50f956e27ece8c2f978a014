const mqtt = require('mqtt');
const SoundData = require('./models/SoundData')

function connectmqtt() {

  var client = mqtt.connect("http://mqtt.hfg.design", {
    clientId: "soundbackend"
  });

  client.on("connect", function () {
    console.log("connected to mqtt");
    client.subscribe("/sweavs/steve/soundlevel", function (err) {
      if (err) {
        console.log(err) 
      }
    })
    //client.publish("/soundsense", 'hello mqtt. sent by backend') // testmessage 
  });

  client.on('message', function (topic, message) {
    //console.log(message.toString())
    console.log('regex', message.toString().match(/{(.+)}/)[0]);
    const data = JSON.parse(message.toString().match(/{(.+)}/)[0]);
    const post = new SoundData({
      deviceId: data.MAC,
      db: data.dB,
      ts: Date.now()
    });
    post.save();
    console.log(Date.now());
  })

  return client 

};

module.exports = {
  connectmqtt
};