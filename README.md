\# 🛰️ GPS Data Extraction and Conversion using Arduino



This project demonstrates how to extract and convert raw GPS data from a `$GPGGA` NMEA sentence using an Arduino and a GPS module.



It:

\- Reads GPS data from a serial connection

\- Extracts \*\*latitude\*\*, \*\*longitude\*\*, and \*\*time\*\*

\- Converts the coordinates from \*\*NMEA format (DDMM.MMMM / DDDMM.MMMM)\*\* to \*\*Decimal Degrees\*\*

\- Prints the results to the serial monitor



---



\## 📦 Components Used



\- Arduino UNO/Nano

\- GPS Module (e.g., NEO-6M)

\- Jumper wires

\- USB cable

\- Optional: Breadboard



---



\## ⚙️ Hardware Setup



| GPS Pin | Arduino Pin |

|---------|-------------|

| VCC     | 5V          |

| GND     | GND         |

| TX      | Pin 2 (RX)  |

| RX      | Pin 3 (TX)  |



> Using `SoftwareSerial` to handle communication on pins 2 and 3.



---



\## 🧪 Sample Raw NMEA Sentence



$GPGGA,055649.000,0959.6422,N,07617.4754,E,1,08,0.9,545.4,M,46.9,M,,\*47



---



\## 🧭 Extracted Fields



| Field        | Value       | Meaning                      |

|--------------|-------------|------------------------------|

| Time         | `055649.000`| 05:56:49 UTC                 |

| Latitude     | `0959.6422` | 09° 59.6422' North           |

| Longitude    | `07617.4754`| 76° 17.4754' East            |

| Fix Quality  | `1`         | Valid Fix                    |

| Satellites   | `08`        | 8 satellites used            |



---



\## 📐 Latitude \& Longitude Conversion



\### 🧮 Conversion Formula:

Decimal Degrees = Degrees + (Minutes / 60)



---



\### ▶️ Latitude: `0959.6422`

\- Degrees: `09`

\- Minutes: `59.6422`

\- Decimal = `9 + (59.6422 / 60)` = `9.994037° N`



---



\### ▶️ Longitude: `07617.4754`

\- Degrees: `076`

\- Minutes: `17.4754`

\- Decimal = `76 + (17.4754 / 60)` = `76.291257° E`



---



\## 📍 Final Position (Decimal Degrees)



```text

Latitude  = 9.994037° N  

Longitude = 76.291257° E

```

🔗 View on Google Maps



\## ⏰ Time Conversion



Raw: 055649.000 → 05:56:49 UTC

Use this format to convert UTC to local time if needed.



\## 🧑‍💻 Arduino Code Overview



```c



\#include <SoftwareSerial.h>

SoftwareSerial m(2, 3); // RX=2, TX=3



void setup() {

&nbsp; m.begin(9600);

&nbsp; Serial.begin(9600);

}



String s, latRaw, lonRaw;



void loop() {

&nbsp; if (m.available()) {

&nbsp;   if (m.read() == '$') {

&nbsp;     s = m.readStringUntil(',');



&nbsp;     if (s == "GPGGA") {

&nbsp;       m.readStringUntil(',');            // Skip time

&nbsp;       latRaw = m.readStringUntil(',');   // Latitude

&nbsp;       m.readStringUntil(',');            // N/S

&nbsp;       lonRaw = m.readStringUntil(',');   // Longitude

&nbsp;       m.readStringUntil(',');            // E/W



&nbsp;       float lat = convertToDecimal(latRaw);

&nbsp;       float lon = convertToDecimal(lonRaw);



&nbsp;       Serial.print("Latitude : ");

&nbsp;       Serial.println(lat, 6);

&nbsp;       Serial.print("Longitude : ");

&nbsp;       Serial.println(lon, 6);

&nbsp;     }

&nbsp;   }

&nbsp; }

}



float convertToDecimal(String raw) {

&nbsp; int dotIndex = raw.indexOf('.');

&nbsp; int degreeLength = (dotIndex == 4) ? 2 : 3;



&nbsp; int degrees = raw.substring(0, degreeLength).toInt();

&nbsp; float minutes = raw.substring(degreeLength).toFloat();



&nbsp; return degrees + (minutes / 60.0);

}



```



\## 📎 Notes



Only $GPGGA is used. You can extend the code to parse $GPRMC, $GPGLL for more info.



Negative values are not handled yet (for S or W). You can add direction checks for full accuracy.



Time is in UTC; convert it based on your timezone as needed.



\## 📜 License



This project is open-source under the MIT License.

Feel free to use and modify it for your own projects.



\## 🤝 Contributing



Pull requests are welcome! If you'd like to add support for $GPRMC, timestamp parsing, or Firebase integration, feel free to open an issue or fork the repo.

