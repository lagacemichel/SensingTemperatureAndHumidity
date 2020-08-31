/*
  DHT22 Temperature / Humidity Controller Class Header

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

#if !defined(DHT22_H)
#define DHT22_H

#include "Arduino.h"

class DHT22 {
  public:
    // Orthodox cannonical form
    ~DHT22(); // Destructor

    // Constructor connecting digital I/O port to DHT22 device
    DHT22(int port); // Constructor

    // Get temperature and relative humidity
    float temperature();
    float relativeHumidity();
    
  private:
    // Unusable and hidden orthodox cannonical form
    DHT22();  // Default constructor
    DHT22(const DHT22&); // Copy constructor
    DHT22& operator = (const DHT22&); // Assignment operator

    // Internal methods
    unsigned long timeSinceLastRead() const;
    int waitForState(bool state) const;
    void fetchData();
    
  private:
    int m_port;               // I/O pin connected to sensor
    byte m_data[5];         // Data read from DHT22 sensor
    unsigned long m_lastRead; // Last time data was read in milliseconds since program start
    float m_temperature;      // Temperature in degrees Celsius
    float m_relativeHumidity; // Relative humidity in %
    bool m_firstTime;         // Flag if first time around
};

#endif
