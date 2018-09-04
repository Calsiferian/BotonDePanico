// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <TinyGPS++.h>                                  // Tiny GPS Plus Library
#include <SoftwareSerial.h>
/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int botonPin = 3;
int Indi = 2;
String mess;
// set up the 'counter' feed
AdafruitIO_Feed *boton = io.feed("boton");
//gps
static const int RXPin = 0, TXPin = 2;                // Ublox 6m GPS module to pins 12 and 13
static const uint32_t GPSBaud = 9600;                   // Ublox GPS default Baud Rate is 9600
TinyGPSPlus gps;                                        // Create an Instance of the TinyGPS++ object called gps
SoftwareSerial ss(RXPin, TXPin);                        // The serial connection to the GPS device

void setup() {

  pinMode(Indi, OUTPUT);
  pinMode(botonPin, INPUT);
  digitalWrite(Indi, HIGH);
  // start the serial connection

  ss.begin(GPSBaud);                                    // Set Software Serial Comm Speed to 9600
  // wait for serial monitor to open



  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {

    delay(500);
  }

  // we are connected



}

void loop() {
  String Latitud = floatToString(gps.location.lat(), 8, 5, false);
  String Longitud = floatToString(gps.location.lng(), 8, 5, false);

  mess = Latitud + " " + Longitud + " Emergencia " + (String)random(0, 100);
  delay(200);

  smartDelay(500);                                      // Run Procedure smartDelay


    // io.run(); is required for all sketches.
    // it should always be present at the top of your loop
    // function. it keeps the client connected to
    // io.adafruit.com, and processes any incoming data.
    io.run();
  digitalWrite(Indi, LOW);


  if (digitalRead(botonPin) == 1) {
    digitalWrite(Indi, HIGH);

    boton->save(mess);
    delay(200);
  }






}
// Convierte un float en una cadena.
// n -> número a convertir.
// l -> longitud total de la cadena, por defecto 8.
// d -> decimales, por defecto 2.
// z -> si se desea rellenar con ceros a la izquierda, por defecto true.
String floatToString( float n, int l, int d, boolean z) {
  char c[l + 1];
  String s;

  dtostrf(n, l, d, c);
  s = String(c);

  if (z) {
    s.replace(" ", "0");
  }

  return s;
}
static void smartDelay(unsigned long ms)                // This custom version of delay() ensures that the gps object is being "fed".
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
