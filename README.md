# 🛰️ GPS Data Extraction and Conversion using Arduino

This project demonstrates how to extract and convert raw GPS data from a `$GPGGA` NMEA sentence using an Arduino and a GPS module.

### ✅ Features

- Reads GPS data from a serial connection  
- Extracts **latitude**, **longitude**, and **time**  
- Converts coordinates from NMEA format (`DDMM.MMMM` / `DDDMM.MMMM`) to **Decimal Degrees**  
- Prints the results to the **Serial Monitor**

---

## 📦 Components Used

- Arduino UNO / Nano  
- GPS Module (e.g., NEO-6M)  
- Jumper Wires  
- USB Cable  
- Optional: Breadboard  

---

## ⚙️ Hardware Setup

| GPS Pin | Arduino Pin |
|---------|-------------|
| VCC     | 5V          |
| GND     | GND         |
| TX      | Pin 2 (RX)  |
| RX      | Pin 3 (TX)  |

> Using `SoftwareSerial` to handle communication on pins **2** and **3**.

---

## 🧪 Sample Raw NMEA Sentence

$GPGGA,055649.000,0959.6422,N,07617.4754,E,1,08,0.9,545.4,M,46.9,M,,*47

yaml
Copy
Edit

---

## 🧭 Extracted Fields

| Field     | Value       | Meaning                   |
|-----------|-------------|---------------------------|
| Time      | 055649.000  | 05:56:49 UTC              |
| Latitude  | 0959.6422   | 09° 59.6422' North        |
| Longitude | 07617.4754  | 76° 17.4754' East         |
| Fix       | 1           | Valid GPS Fix             |
| Satellites | 08         | 8 satellites used         |

---

## 📐 Latitude & Longitude Conversion

### 🧮 Conversion Formula:

Decimal Degrees = Degrees + (Minutes / 60)


### ▶️ Latitude: `0959.6422`

- Degrees: `09`  
- Minutes: `59.6422`  
- Calculation: `9 + (59.6422 / 60)`  
- **Result:** `9.994037° N`

### ▶️ Longitude: `07617.4754`

- Degrees: `076`  
- Minutes: `17.4754`  
- Calculation: `76 + (17.4754 / 60)`  
- **Result:** `76.291257° E`

---

## 📍 Final Position (Decimal Degrees)

Latitude = 9.994037° N
Longitude = 76.291257° E

pgsql
Copy
Edit

🔗 [View on Google Maps](https://www.google.com/maps?q=9.994037,76.291257)

---

## ⏰ Time Conversion

- **Raw Time**: `055649.000`  
- **Parsed**: `05:56:49 UTC`

You can convert UTC to your local timezone manually or programmatically.

---

## 🧑‍💻 Arduino Code Overview

```cpp
#include <SoftwareSerial.h>

SoftwareSerial m(2, 3); // RX=2, TX=3

void setup() {
  m.begin(9600);
  Serial.begin(9600);
}

String s, latRaw, lonRaw;

void loop() {
  if (m.available()) {
    if (m.read() == '$') {
      s = m.readStringUntil(',');

      if (s == "GPGGA") {
        m.readStringUntil(',');          // Skip time
        latRaw = m.readStringUntil(','); // Latitude
        m.readStringUntil(',');          // N/S
        lonRaw = m.readStringUntil(','); // Longitude
        m.readStringUntil(',');          // E/W

        float lat = convertToDecimal(latRaw);
        float lon = convertToDecimal(lonRaw);

        Serial.print("Latitude : ");
        Serial.println(lat, 6);
        Serial.print("Longitude : ");
        Serial.println(lon, 6);
      }
    }
  }
}

float convertToDecimal(String raw) {
  int dotIndex = raw.indexOf('.');
  int degreeLength = (dotIndex == 4) ? 2 : 3;

  int degrees = raw.substring(0, degreeLength).toInt();
  float minutes = raw.substring(degreeLength).toFloat();

  return degrees + (minutes / 60.0);
}

```
## 📎 Notes

Only $GPGGA is parsed. Extend to handle $GPRMC, $GPGLL, etc.

Negative coordinates (South and West) are not handled in this snippet. You can check direction fields (N/S, E/W) and apply - accordingly.

Time is in UTC; apply your timezone offset for local time.

## 📜 License

This project is open-source under the MIT License.

Feel free to use and modify it for your own projects.

## 🤝 Contributing

Pull requests are welcome!
Want to add:

$GPRMC support?

Timestamp parsing?

Firebase/IoT logging?
