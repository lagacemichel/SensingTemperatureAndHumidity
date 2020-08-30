/*
  DHT22 Temperature / Humidity Serial Decoder Sketch

  Program that requests temperature and humidity from a DHT22 sensor
  and returns the results to the serial port. It is associated with
  the Serial Communication with a Temperature Sensor blog post on
  https://lagacemichel.com

  MIT License

  Copyright (c) 2020, Michel Lagace

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "DHT22.h"

// DHT serial input/output port
#define DHT22_PORT 2

// Create DHT22 device instance
DHT22 dht(DHT22_PORT);

// Setup the board.
void setup() {
  Serial.begin(9600);
}

// This is the main loop. It requests for a transmission, decodes
// the signal and displays decoded temperature and humidity.
void loop() {
  // Get relative humidity and temperature
  float relativeHumidity = dht.relativeHumidity();
  float temperature = dht.temperature();

  // Send results back to PC
  Serial.print("T: ");
  Serial.print(temperature,1);
  Serial.print("C RH: ");
  Serial.print(relativeHumidity,1);
  Serial.println("%");
  delay(5000);
}
