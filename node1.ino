//include required libraries
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <DHT.h>

#include "config.h"
#include "SensorManager.h"
#include "network.h"
#include "actuator.h"
#include "telemetry.h"
#include "rpc.h"

static char sharedBuf[200];

//Create object of type ethernet pubsubclient
static EthernetClient ethClient;
static PubSubClient   mqttClient(ethClient);

unsigned long lastTelemetry = 0;

void setup() 
{
  Serial.begin(9600);
  Serial.print("Node1 is booting...");

  // put your setup code here, to run once:
   // initilase sensor 
   sensor_begin();
   //initialise actutors
   actuators_begin();

   telemetry_init(sharedBuf,sizeof(sharedBuf));
   rpc_init(&mqttClient,sharedBuf,sizeof(sharedBuf));  //initializing rpc request
  
   //upon receiving the data which function needs to be called back
   mqttClient.setCallback(rpc_mqttCallback);
   // coonect board to internet and mqtt
   network_begin(&mqttClient);
   mqttClient.setCallback(rpc_mqttCallback);

   digitalWrite(PIN_LED_RED , LOW);

}

void loop() 
{
  // put your main code here, to run repeatedly:
  //keep checking if board is connected to things board
  network_maintain();

  //read data from the sensors 
  SensorData data;
  sensors_read( &data);//humidity,temp,lm35,vib,sensor_error,machine_status

  //publish to the cloud 
  //check if network is available 
  if(network_isConnected())
  {   unsigned long now = millis();
      //publish data to things board every 5 secs
      if(now - lastTelemetry >= TELEMETRY_INTERVAL) //checking 5 sec is over or not
      {
          lastTelemetry = now;
          telemetry_publishTelemetry(&data,actuators_getRelayState);//converting sensor into json and publish
      }
  }
  
  actuators_updateStatusLEDs(network_isConnected(),data.sensorError);      
}
