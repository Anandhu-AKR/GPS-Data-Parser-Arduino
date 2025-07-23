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

// Define the SoftwareSerial object for communication with the GPS module.
// RX pin of Arduino connected to TX of GPS (pin 2), TX pin of Arduino connected to RX of GPS (pin 3).
SoftwareSerial m(2, 3); 

// Global variables to store NMEA sentence parts and raw latitude/longitude.
String s, latRaw, lonRaw;

void setup() {
  // Initialize the SoftwareSerial communication with the GPS module at 9600 baud.
  m.begin(9600);
  // Initialize the hardware Serial communication for debugging and output to the Serial Monitor at 9600 baud.
  Serial.begin(9600);
}

void loop() {
  // Check if data is available from the GPS module.
  if (m.available()) {
    // Look for the start of an NMEA sentence, which is indicated by '$'.
    if (m.read() == '$') {
      // Read the NMEA sentence type (e.g., GPGGA, GPRMC) until the next comma.
      s = m.readStringUntil(',');
      
      // If the sentence type is GPGGA (Global Positioning System Fix Data).
      if (s == "GPGGA") {
        // --- UTC Time Extraction ---
        // The GPGGA sentence format is: $GPGGA,hhmmss.sss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*CS
        // The first field after $GPGGA is the UTC time.
        String utcTimeRaw = m.readStringUntil(','); // Read the UTC time (hhmmss.sss)
        
        // Extract hours, minutes, and seconds from the raw UTC time string.
        int hours = utcTimeRaw.substring(0, 2).toInt();
        int minutes = utcTimeRaw.substring(2, 4).toInt();
        float seconds = utcTimeRaw.substring(4).toFloat(); // Seconds can have decimal places

        // --- IST Conversion (UTC + 5 hours 30 minutes) ---
        // Convert UTC to IST. IST is UTC + 5 hours 30 minutes.
        // Add 5 hours to the UTC hours.
        hours += 5;
        // Add 30 minutes to the UTC minutes.
        minutes += 30;

        // Handle minute overflow (if minutes exceed 59, increment hours and adjust minutes).
        if (minutes >= 60) {
          hours += 1;
          minutes -= 60;
        }
        
        // Handle hour overflow (if hours exceed 23, adjust for next day, though for simple time display,
        // just modulo 24 is sufficient for the hour part).
        hours %= 24; 

        // Print the extracted and converted IST time.
        Serial.print("IST Time: ");
        // Ensure leading zeros for hours and minutes if they are single digits.
        if (hours < 10) Serial.print("0");
        Serial.print(hours);
        Serial.print(":");
        if (minutes < 10) Serial.print("0");
        Serial.print(minutes);
        Serial.print(":");
        // For seconds, we might want to format them to two decimal places if needed.
        // For simplicity, we'll just print the float as is or cast to int if only whole seconds are desired.
        // If you want 2 decimal places: Serial.print(seconds, 2);
        if (seconds < 10) Serial.print("0"); // For seconds less than 10, add a leading zero.
        Serial.println(seconds, 2); // Print seconds with 2 decimal places.

        // --- Latitude and Longitude Extraction ---
        latRaw = m.readStringUntil(',');       // Read raw Latitude (DDMM.MMMM)
        m.readStringUntil(',');                // Skip N/S indicator
        lonRaw = m.readStringUntil(',');       // Read raw Longitude (DDDMM.MMMM)
        m.readStringUntil(',');                // Skip E/W indicator

        // Convert raw latitude and longitude strings to decimal degrees.
        float lat = convertToDecimal(latRaw);
        float lon = convertToDecimal(lonRaw);

        // Print the converted latitude and longitude.
        Serial.print("Latitude : ");
        Serial.println(lat, 6); // Print with 6 decimal places for precision
        Serial.print("Longitude : ");
        Serial.println(lon, 6); // Print with 6 decimal places for precision
      }
    }
  }
}

// Function to convert raw NMEA latitude/longitude format (DDMM.MMMM or DDDMM.MMMM)
// to decimal degrees.
float convertToDecimal(String raw) {
  // Find the index of the decimal point.
  int dotIndex = raw.indexOf('.');
  
  // Determine the length of the degrees part based on the dot index.
  // For latitude (DDMM.MMMM), dotIndex will be 4, so degrees length is 2.
  // For longitude (DDDMM.MMMM), dotIndex will be 5, so degrees length is 3.
  int degreeLength = (dotIndex == 4) ? 2 : 3;
  
  // Extract the degrees part as an integer.
  int degrees = raw.substring(0, degreeLength).toInt();
  
  // Extract the minutes part (including decimals) as a float.
  float minutes = raw.substring(degreeLength).toFloat();
  
  // Calculate the final decimal value: degrees + (minutes / 60.0).
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
