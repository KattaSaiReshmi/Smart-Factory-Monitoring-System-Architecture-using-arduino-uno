# Smart-Factory-Monitoring-System-Architecture-using-arduino-uno
Project Overview

The Smart Factory Monitoring System is an IoT-based industrial monitoring solution developed using two Arduino nodes simulated in PICSimLab. The system monitors production line and warehouse conditions in real time and transmits sensor data using the MQTT protocol to the ThingsBoard IoT platform for visualization and remote monitoring.

##**System Architecture**
                    ┌─────────────────┐
                    │   ThingsBoard   │
                    │ IoT Dashboard   │
                    └────────▲────────┘
                             │ MQTT
                             │
                ┌────────────┴────────────┐
                │                         │
        ┌───────┴────────┐       ┌───────┴────────┐
        │    Node 1      │       │    Node 2      │
        │ Production Line│       │ Warehouse      │
        │   Monitoring   │       │   Monitoring   │
        └────────────────┘       └────────────────┘

###**Node 1 – Production Line Monitoring**

This node monitors the operating conditions of machines and the production environment.

##**Parameters Monitored**

Temperature
LM35 Temperature Sensor Value
Humidity
Machine Status
Vibration Level
Relay State
Sensor Error Status

**Purpose**
Machine condition monitoring
Fault detection
Environmental monitoring
Production safety

##**Node 2 – Warehouse Monitoring**

This node monitors warehouse security and environmental conditions.

**Parameters Monitored**

Temperature
Humidity
Door Open Status
Motion Detection
LDR Value
Darkness Status (isDark)
Relay State
Sensor Error Status

**Purpose**
Warehouse security monitoring
Intrusion detection
Environmental control
Smart lighting automation

**Communication Protocol**
Protocol: MQTT
Broker: ThingsBoard MQTT Server
Data Format: JSON
Real-time telemetry transmission from both Arduino nodes
