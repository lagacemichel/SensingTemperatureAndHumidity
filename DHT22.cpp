/*
  DHT22 Temperature / Humidity Controller Class Body

  DHT22 temperature and relative humidity sensor handling class. This class
  allows the instanciation of objects that communicate with a DHT22 sensor
  connected to a digital input/output port on an Arduino micro-controller.
  This code is associated with the Serial Communication with a Temperature
  Sensor blog post on https://lagacemichel.com

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

#define CYCLES_PER_COUNT 50
#define TIMEOUT_MICROSECONDS 300
#define TIMEOUT TIMEOUT_MICROSECONDS/CYCLES_PER_COUNT*F_CPU/1000000
#define ONE_SECOND 1000
#define TWO_SECONDS 2000
#define TWO_MILLISECONDS 2

// DH22 constructor, accepts Arduino digital I/O port number
DHT22::DHT22(int port) {
  m_port = port;
  m_data[0] = 0;
  m_data[1] = 0;
  m_data[2] = 0;
  m_data[3] = 0;
  m_data[4] = 0;
  m_lastRead = 0;
  m_firstTime = true;
  m_temperature = 0.0;
  m_relativeHumidity = 0.0;
}

// DH22 destructor
DHT22::~DHT22() {
}

// Return temperature
float DHT22::temperature() {
  // Get data from sensor and return temperature read
  fetchData();
  return m_temperature;
}

// Return relative humidity
float DHT22::relativeHumidity() {
  // Get data from sensor and return relative humidity read
  fetchData();
  return m_relativeHumidity;
}

// Return the number of milliseconds since last time read
unsigned long DHT22::timeSinceLastRead() const {
  // Get current processor time
  unsigned long currentMilliseconds = millis();
  unsigned long timeSince = 0;

  // Check if time wrapped around, if so use two's complement
  if (currentMilliseconds < m_lastRead) {
    timeSince = ~m_lastRead + 1 + currentMilliseconds;
  }

  // Otherwise use difference
  else {
    timeSince = currentMilliseconds - m_lastRead;
  }

  // Return elapsed time
  return timeSince;
}

// Wait for serial line to reach a state and return relative time
int DHT22::waitForState(bool state) const {
  int count = 0;
  while ((bool)digitalRead(m_port) != state) {
    count++;
    if (count >= TIMEOUT) {
      count = 0;
      break;
    }
  }
  return count;
}

// Fetch data from sensor
void DHT22::fetchData() {
  // We will access sensor on the very first time and if more than two
  // seconds have passed since last access.
  if (m_firstTime || (timeSinceLastRead() > TWO_SECONDS)) {

    // First time around, wait at least one second for sensor to settle
    if (m_firstTime) {
      m_firstTime = false;
      pinMode(m_port, INPUT_PULLUP);
      delay(ONE_SECOND);
    }

    // Send request signal to reMad temperature and relative humidity from device.
    pinMode(m_port, OUTPUT);
    digitalWrite(m_port, LOW);
    delay(TWO_MILLISECONDS);
    pinMode(m_port, INPUT_PULLUP);

    // Since timings are critical, prevent interrupts while DHT22 transmits
    noInterrupts();

    // Get Start of Transmission, falling, rising, then falling edges
    // Interval: 80 micro-seconds, no need to check
    int lowCounter = 0;
    int highCounter = 0;
    highCounter = waitForState(LOW);
    if (highCounter > 0) {
      lowCounter = waitForState(HIGH);
      if (lowCounter > 0) {
        highCounter = waitForState(LOW);
      }
    }

    // Get 40 bits of data and store them in the data array
    if (highCounter > 0) {
      for (int bitN = 0; bitN < 40; bitN++) {     // 40 bits, 5 x 8-bit bytes
        lowCounter = waitForState(HIGH);          // Byte 0: Relative Humidity MSB
        if (lowCounter > 0) {                     // Byte 1: Relative Humidity LSB
          highCounter = waitForState(LOW);        // Byte 2: Temperature MSB
          if (highCounter > 0) {                  // Byte 3: Temperature LSB
            int byteN = bitN/8;                   // Byte 4: Checksum
            m_data[byteN] <<= 1;
            if (highCounter > lowCounter) {       // Low 50us, High 25us -> 0
              m_data[byteN] |= 1;                 // Low 50us, High 70us -> 1
            }
          }
        }
      }
    }
    // Reception complete, interrupts are re-enabled
    interrupts();

    // Save the time of last data transfer
    m_lastRead = millis();

    // Compute temperature and relative humidity if data is valid
    byte checksum = m_data[0] + m_data[1] + m_data[2] + m_data[3];
    if (m_data[4] == checksum) {
      m_relativeHumidity = int((m_data[0] << 8) + m_data[1])/10.0;
      m_temperature = int((m_data[2] << 8) + m_data[3])/10.0;
    }
    else {
      m_relativeHumidity = 0.0;
      m_temperature = 0.0;
    }
  }
}
