#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <dht.h>
#include <SPI.h>
#include <SD.h>

#define DHT11_PIN D0

dht DHT;
const int chipSelect = D8;
File myFile;


char ssid[] = "Rushad";
char pass[] = "12345678";

unsigned long myChannelNumber = 601832;
const char * myWriteAPIKey = "SO3C9HCKU22NPDF5";

WiFiClient  client;

void setup()
{

  Serial.begin(9600);
  while (!Serial) {

  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect))
  {
    Serial.println("Initialization failed!");
    return;
  }
  Serial.println("Initialization done.");

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");


  }
  ThingSpeak.begin(client);
}
void loop() {


  myFile = SD.open("dht11.csv", FILE_WRITE);
  int data = DHT.read11(DHT11_PIN);
  int temp = DHT.temperature;
  int hum = DHT.humidity;


  if (isnan(hum) || isnan(temp))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (myFile)
  {
    Serial.println("Data written to SD card");
    myFile.print(hum);
    myFile.print(",");
    myFile.println(temp);

    myFile.close();
    Serial.println("Temperature : ");
    Serial.println(temp);
    Serial.println("Humidity : ");
    Serial.println(hum);
  }
  else
  {

    Serial.println("Error opening dht11.csv");
  }

  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  Serial.print("Data written to webserver");

}
