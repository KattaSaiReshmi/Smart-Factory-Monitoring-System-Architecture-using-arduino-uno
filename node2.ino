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

static SensorManager   sensors;


void setup() 
{
  Serial.begin(9600);
  Serial.print("Node2 is booting...");

  // put your setup code here, to run once:
   // initilase sensor 
   sensor_begin();
   sensor_init(&sensors);
   //initialise actutors
   actuators_begin();

   telemetry_init(sharedBuf,sizeof(sharedBuf));
   rpc_init(&mqttClient,sharedBuf,sizeof(sharedBuf));  //initializing rpc request
   // coonect board to internet and mqtt
   network_begin(&mqttClient);

  

}

void loop() 
{
  // put your main code here, to run repeatedly:
  //keep checking if board is connected to things board
  network_maintain();

  //read data from the sensors 
  SensorData data;
  sensor_read( &data , &sensors);//humidity,temp,ldr,door_status,motion status

  //if humidity is more than 85 turn on the relay if relay is off
  if(data.humidity >= HUMIDITY_CRIT && !actuators_getRelayState())
  {
    actuators_setRelay(1);
  }

  //publish to the cloud 
  //check if network is available 
  if(network_isConnected())
  {   unsigned long now = millis();
      //publish data to things board every 5 secs
      if(now - lastTelemetry >= TELEMETRY_INTERVAL) //checking 5 sec is over or not
      {
          lastTelemetry = now;
          telemetry_publishTelemetry(&data,actuators_getRelayState());//converting sensor into json and publish
      }
  }
  
  actuators_updateStatusLEDs(network_isConnected(),data.sensorError);      
}
