## soundsense-backend
<br>
<br>

 # Inhaltsangabe
 <br>

 + [Inbetriebnahme](#Inbetriebnahme)
 + [Funktionsflow](#Funktionsflow)
 + [Technologien](#Technologien)
 + [Todos/Future Work](#future-work)
 + [Links](#Links)

 


<br>



# Inbetriebnahme
<br>

Voraussetzung zur Inbetriebnahme des Backends ist node.js.
<br>
Nachdem dann das Repo heruntergladen wurde wird es mit 
```
npm start
```
gestartet und läuft dann über die lokale rechner IP. Allerdings wird hierzu noch eine "dotenv" Datei benötigt, die hier nicht in das Repo mitgepusht wurde, in der die Umgebungsvariablen definiert sind. Hier sind die Datenbank-keys und die Verbindungsdaten für TTN gespeichert. Die TTN Daten sind in dieser Version noch ungenutzt, können aber bei LoRa Implementierung genutzt werden. Lediglich der Datenbank-key muss hier ersetzt werden.
Eine gehostete Version kann allerdings schon unter `soundsensefe.ds.ava.hfg.design` aufgerufen und getestet werden. Hier kann über einen REST Client wie ARC oder Postman auf das Backend zugegriffen werden.

<br>
<br>
<br>

## dotenv Datei Aufbau
```
DB_CONNECTION= <your DB key>
TTN_KEY= <Your TTN key>
TTN_URL=mqtt://eu.thethings.network:1883
TTN_CLI= <TTN device id>
TTN_USR= <TTN user id>

```

<br>
<br>
<br>

## Sensor Einstellungen
Um den Sensor in Betrieb zu nehmen muss man in der soundmodule_V2.ino nur ein paar Variablen anpassen.
In Zeile 33 und 34 müssen die Wifi Zugangsdaten eingetragen werden und in Zeile 159 das MQTT topic, damit die Sensordaten nicht an die öffentliche Anwendung geschickt werden.

<br>
<br>
<br>
<br>

# Funktionsflow
<br>

Hier sieht man den Funktionsflow des Systems und welche Funktionen vom Konzept vorgesehen sind.

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

Hier sieht man die Prototypen der Sensoren. Diese Basieren auf einem ESP8266 Mikrocontroller und einem Max 9814 Mikrofonmodul. Diese Schicken alle 30 Sekunden einen Messwert über WiFi und MQTT an das Backend.
Im Konzept sind allerdings Heltec WiFi LoRa 32 Module, die per TTN ihre Daten an das Backend übermitteln, vorgesehen. Zur besseren Prototypisierung eines Sensors wurde allerdings hier an dieser Stelle auf WiFi zurückgegriffen. Gut zu sehen ist hier auch noch, dass die Sensoren noch per USB Kabel mit Energie versorgt werden. Im finalen Produkt wird die Energieversorgung dann über ein Solarmodul in Kombination mit einem Lipo Akku, der über eine Ladeelektronik geladen wird, gewährleistet. Dies erlaubt es, dass die Sensoren autark und ohne externe Stromversorgung laufen können. Um dann noch zusätzlich Energie einsparen zu können kann der Sensor zwischen jedem Messzyklus in einen deep sleep Modus versetzt werden. Dies bedeutet, dass er alle 30 Sekunden "aufwacht", einen Messwert übermittelt und danach wieder in den sleep Modus fällt. Dies sollte auf längere Sicht deutlich Energie einsparen und somit zu einer längeren Laufzeit des Systems beitragen.

![alt text](sensoren.jpg "sensoren")

<br>
<br>
<br>
<br>

## Techtree

Hier sieht man den Techtree des Systems. Hier sind die Micorservices aufgelistet, die von technischer Seite aus das Soundsense Gesamtsystem darstellen und in ihrer Summe den Fullstack ergeben.

![alt text](fullstack.jpg "fullstack")

<br>
<br>
<br>



## Benutzte Technologien 

* WiFi + MQTT 
* MongoDB (No SQL Datenbank) 
* REST API
* VUE (im Frontend)

<br>
<br>
<br>

## Speicherbedarf der Datenbank und Skalierung
<br>
Dadurch, dass von jedem Sensor alle 30 Sekunden ein Messwert ausgeht, ist der Speicherbedarf der Datenbank sehr hoch. Bereits ein Sensor hat nach einer Laufzeit von etwas mehr als drei Tagen ein Megabyte an Daten generiert. Wenn das System auf mehrere Sensoren hochskaliert wird, skalieren die Datenmengen proportional mit hoch. Dementsprechend müsste für eine tatsächliche Anwendung darauf geachtet werden dass die Datenbank mitskalieren kann, heißt dass das Speicherangebot ausreichend sein sollte um die Datenmengen die durch das System anfallen speichern zu können. Aufgrund der hohen Datenmengen wurde auch der Kompromiss getroffen einen Messzyklus von 30 Sekunden durchzuführen. Ein kürzerer Messzyklus würde dafür sorgen dass zwar die Genauigkeit der Messung steigt aber die Datenmengen sich bei einer Verkürzung auf z.B. 15 Sekunden verdoppeln und bei 10 Sekunden sogar verdreifachen. Ein längerer Messzyklus führt im gleichen Sinne auch dazu dass zwar die Datenbankauslastung sinkt aber ebenso auch die Messgenauigkeit.

<br>
<br>
<br>
<br>

<br>



## REST API Nutzung

<br>

Eine GET Anfrage auf `/getSoundData` gibt die Sounddaten Collection aus in der alle Messwertdatensätze gelistet sind.


<br>
<br>

Mit einer POST Anfrage auf `/addSensor` mit dem Inhalt deviceId, long, lat und adress (nachzusehen in der route addSensor.js und im beigefügten Model) können Sensoren mit den dazugehörigen Metadaten hinzugefügt werden.
Alternativ exisitert hier auch eine Frontend Seite die den Registrierprozess simuliert. In diesem Prozess soll mithilfe eines QR Codes, auf dem die MAC Adresse gespeichert ist, und über die longitude und latitude des mobile devices, auf dem die Inbetriebnahme durchgeführt wird, der Sensor zum System hinzugefügt werden. Zusätzlich kann hier auch noch eine Adresse mitgeschickt werden die man manuell eingibt. Um dies zu Simulieren wurde eine schlichte html Seite erstellt, die diese Aufgabe übernimmt und über eine manuelle Eingabe die Daten in die Datenbank schreibt. Allerdings müsste beim manuellen Hinzufügen eines Sensors noch ein SoundData Objekt angelegt werden mit der dazugehörigen id, sodass das Frontend keinen Fehler wirft. Bei einem realen Sensor passiert dies allerdings nicht, da er sobald er eingeschaltet wird, automatisch Messwerte in die Datenbank schreibt.

<br>
<br>

Aufrufen kann man diese dann unter `https://soundsensefe.ds.ava.hfg.design/#/reg`

<br>

Das öffentliche Backend ist erreichbar unter `soundsenseiotbe.ds.ava.hfg.design`

<br>
<br>



Eine GET Anfrage auf `/getDevice` gibt die device Daten Collection aus. In dieser sind alle registrierten Sensoren gelistet.

<br>
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
<br>
<br>
<br>
<br>
<br>

<a name="future-work"></a>
# Todos/Future Work

<br>

Um das System aus einem Entwicklunsstand in einen finalen Stand zu heben gibt es noch einige Punkte die erfüllt werden müssen.

+  Im Frontend müssen noch für den vorgesehenen Betrieb die replay Funktion, Graphen und eine Heatmap implementiert werden (siehe Frontend Repo)
+ Ein Nutzeraccount System muss noch implementiert werden um zwischen den Installateuren, die die Sensoren verteilen und in Betrieb nehmen und den Nutzern die sich die Daten ansehen, unterschieden werden kann.
+ Es muss darauf geachtet werden, dass die Datenbank mit den Datenmengen mitskaliert. Ein ensprechender Datenbankanbieter oder eine dementsprechend große Datenbank sollte benutzt werden.
+ Die Sensoren müssen noch mit Solarmodulen, Akkus und einem Deep Sleep des Sensors kombiniert werden um einen Langzeitbetrieb gewährleisten zu können.
+ Sicherheitsfeatures müssen hierbei auch bedacht werden um sich vor Hackerangriffen und Botnetzen wie z.B. mirai zu schützen. Beim auftreten von Sicherheitslücken sollten diese Umgehend geschlossen werden. Besser ist es jedoch im System vor dem Finalen Zustand alle bekannten IOT Sicherheitslücken zu schließen.

<br>
<br>
<br>
<br>
<br>
<br>
<br>



<br>
<br>
<br>
<br>

# Links
Frontend Repository [Klick here](https://github.com/IoTSteve/soundsense-frontend)

<br>
<br>

### HfG Online Exhibition Page
<!--See [Configuration Reference](https://cli.vuejs.org/config/).-->
link will follow prior to the exhibition weekend
