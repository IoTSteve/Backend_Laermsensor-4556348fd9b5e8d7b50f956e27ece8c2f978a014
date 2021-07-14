## soundsense-backend
<br>
<br>
inhaltsangabe
<br>



# Inbetriebnahme
<br>

voraussetzung zur inbetriebnahme des backends ist node.js
<br>
Nachdem dann das Repo heruntergladen wurde wird es mit 
```
npm start
```
gestartet und läuft dann über die lokale rechner IP. Allerdings wird hierzu noch eine dotenv datei benötigt die hier nicht in das Repo mitgepusht wurde in der die umgebungsvariablen difiniert sind. Hier sind die Datenbank keys und die verbindungsdaten zu mqtt gespeichert. diese müssten zur lokalen Inbetriebnahme noch angepasst werden. 
Eine deployte version kann allerdings schon unter `soundsensefe.ds.ava.hfg.design` aufgerufen und getestet werden. Hier kann über einen Rest client wie ARc oder Postman auf das backend zugegriffen werden.





<br>
<br>
<br>
<br>
<br>

# Funktionsflow
<br>

Hier sieht man den Funktionsflow des Systems und welche Funktionen vom Konzept vorgesehen sind

![alt text](techflow.jpg "techflow")

<br>
<br>
<br>
<br>
<br>
<br>
<br>

# Technologien
<br>

## Sensor Prototypen

Hier sieht man die Prototypen der Sensoren. Diese Basieren auf einem ESP8266 Mikrocontroller und einem Max 9814 Mikrofonmodul. Diese Schicken alle 30 Sekunden einen Messwert über Wifi und mqtt an das backend.
Im konzept sind allerdings Heltec WiFi LoRa 32 Module die per TTN ihre daten an das Backend übermitteln vorgesehen. Zur besseren prototypisierung eines Sensors wurdie allerdings hier an dieser Stelle auf WiFi zurückgergiffen. Gut zu sehen ist hier auch noch dass die Semsoren noch per USB mit Energie versorgt werden. Im finalen Produkt wird die Energieversorgung dann über ein Solarmodul in Kombination mit einem Lipo akku der über eine Ladeelektronik geladen wird gewährleistet. Dies erlaubt es dass die Sensoren autark und ohne externe Stromversorgung laufen können. Um dann noch zusätzlich Energie einsparen zu können kann der Sensor in einen Deep sleep modus versetzt werden zwischen jedem Messzyklus. Dies bedeutet dass er alle 30 Sekunden "aufwacht" einen Messwert übermittelt und danach wieder in den sleep modus fällt. Dies sollte auf längere sicht deutlich energie einsparen und somit zu einer Längeren Laufzeit des systems beitragen.

![alt text](sensoren.jpg "sensoren")

<br>
<br>
<br>
<br>

## Techtree

Hier sieht man den Techtree des Systems. Hier sind die Micorservices aufgelistet die von technischer Seite aus das Soundsense Gesamtsystem darstellen und in ihrer summe den Fullstack ergeben.

![alt text](fullstack.jpg "fullstack")

<br>
<br>
<br>



## Benutzte Technologien 

* Wifi + mqtt
* MongoDB (No SQL Datenbank) 
* REST API
* VUE (im Frontend)

<br>
<br>
<br>

## Speicherbedarf Datenbank und Skalierung
<br>
Dadurch dass von jedem Sensor alle 30 Sekunden ein Messwert ausgeht ist der Speicherbedarf der Datenbank sehr hoch. Bereits ein sensor hat nach einer laufzeit von etwas mehr als drei Tagen ein Megabyte an Daten generiert. Wenn das system auf mehrere Sensoren hochskaliert wird Skalieren die Datenmengen natürlich Proportional mit hoch. Dementsprechend müsste für eine tatsächliche Anwendung darauf geachtet werden dass die Datenbank mitskalieren kann, heißt dass das Speicherandgebot ausreichend sein sollte um die Datenmengen die Durch das System anfallen gespeichert werden können. 

<br>
<br>
<br>
<br>

<br>



## REST API nutzung

<br>

eine get anfrage auf `/getSoundData` gibt die sound daten collection aus in der alle messwert datensätze gelistet sind

<br>

mit einer post anfrage auf `/addSensor` mit dem inhalt deviceId, long, lat und adress (nachzusehen in der route addSensor.js) können Sensoren mit den dazugehörigen metadaten hinzugefügt werden.
alternativ exisitert hier auch eine frontend seite die den Registrierprozess simuliert. In diesem Prozess soll mithilfe eines QR codes auf dem die MAC adresse gespeichert ist und über die longitude und latitude des mobile devices auf dem die Inbetriebnahme durchgeführt wird der sensor zum system hinzugefügt. Zusätzlich kann hier auch noch eine Adresse mitgeschickt werden die man manuell eingibt. Um dies zu simulieren wurde eine schlichte html seite erstellt die diese aufgabe übernimmt und über eine manuell eingabe die daten in die datenbank schiebt. Allerdings müsste beim manuell hinzufügen eines Sensors noch ein SoundData objekt angelegt werden mit der dazugehörigen id dass das Frontend keinen fehler wirft. Bei einem Realen Sensor passiert dies allerdings nicht da er sobald er eingeschaltet wird automatisch werte in die datenbank postet.

<br>

aufrufen kann man diese dann unter `https://soundsensefe.ds.ava.hfg.design/#/reg`

<br>
<br>


## post model 

<br>

```javascript
router.post('/', async (req, res) => {
  const newDevice = Device({
    deviceId: req.body.deviceId,
    long: req.body.long,
    lat: req.body.lat,
    adress: req.body.adress,
  });
  newDevice.save();
```


<br>
<br>
<br>

eine get anfrage auf `/getDevice` gibt die device daten collection aus. In dieser sind alle registreirten Sensoren gelistet.


<br>



<br>
<br>
<br>
<br>
<br>
<br>
<br>

# Todos/Future Work

<br>

Um das system aus einem Prototypischen Stand in einen Finalen Stand zu heben gibt es noch einige Punkte die erfüllt werden müssen.

+  Im frontend müssten noch für den Vorgesehenen Betrieb noch die Replay Funktion, Graphen und eine Heatmap implementiert werden (siehe Frontend Repo)
+ Ein Nutzeraccount System muss noch implementiert werden um zwischen den nutzern die die Sensoren verteilen und in Betrieb nehmen und den nutzern die sich die Daten ansehen unterschieden werden kann.
+ Es muss darauf geachtet werden dass die Datenbank mit den Datenmengen mitskaliert. Ein ensprechender Datenbankanbieter oder einne Dementsprechend große Datenbank sollte benutzt werden.
+ Die Sensoren müssen noch mit Solarmodulen und Akkus in kombination mit einem Deep Sleep des Sensors kombiniert werden um einen Langzeitbetrieb gewährleisten zu können.
+ Sicherheitsfeatures müssen hierbei auch bedacht werden um sich vor Hackerangriffen und Botnetzen wie z.B mirai zu schützen. Beim auftreten von sicherheitslücken sollten diese Umgehend geschlossen werden. Besser ist es jedoch das system im Finalen zustand alle bekannten IOT sciherheitslücken geschlossen werden.

<br>
<br>
<br>
<br>
<br>
<br>
<br>

Frontend Repository [Klick here](https://github.com/IoTSteve/soundsense-frontend)



### HfG Online Exhibition Page
<!--See [Configuration Reference](https://cli.vuejs.org/config/).-->
link will follow prior to the exhibition weekend
