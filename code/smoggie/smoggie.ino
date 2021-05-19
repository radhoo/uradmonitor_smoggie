
/**
 *  File:         smoggie.ino
 *  Version:      22
 *  Date:         2019
 *  License:    GPL v3
 *  Description:  uRADMonitor Smoggie code entry point.
 *  
 *  Project:    SMOGGIE is an ultra-low cost automated air quality monitor with a rain proof enclosure and a simple mount system to make installation easy. 
 *  It features a high quality laser scatering Particulate Matter sensor for PM1, PM2.5 and PM10 and an additional sensor for temperature, pressure and humidity. 
 *  It connects to the internet via Wifi and can be powered by a standard 5V micro-usb cable. Readings are accessed via the uRADMonitor API or decentralized via your local network. This monitor is lab tested for data accuracy.
 *
 *  Copyright 2019 by Magnasci SRL, www.magnasci.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


// FW006 - current
#include "uradmonitor.h"
#include "webserver.h"
#include "data_client.h"

uradmonitor       urad;
Webserver         web;
DataClient        client;

char buffer[BUFFER_SIZE];

void sendFunc() {
  client.sendData();
}

void setup() {
  Serial.begin(9600);

  urad.init(buffer, BUFFER_SIZE, &sendFunc);
  // configure globals
  urad.setDeviceID(((uint32_t)DEV_CLASS << 24) | ((uint32_t)DEFAULT_DEV_NUMBER & 0x00FFFFFF));
  // goes before initnetwork
  urad.loadSettings();
  
  // init sensors
  urad.initSensors();

  urad.initNetwork();  
  
  // init webserver
  web.init(&urad, buffer, BUFFER_SIZE);

  // init client
  client.init(&urad, buffer, BUFFER_SIZE);
}


void loop() {
  urad.loop() ;
  web.loop();
}
