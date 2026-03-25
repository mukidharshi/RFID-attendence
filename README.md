# ESP32 RFID Attendance System

## **Description**

This project is an IoT-based attendance system using **ESP32**, **MFRC522 RFID reader**, and **OLED display**.
It allows students to mark attendance by scanning their RFID cards. The data is sent to a **Flask server** via HTTP POST, and the system handles duplicate marking and invalid cards.

---

## **Features**

* Reads RFID cards using MFRC522
* Displays card UID and status on OLED
* Sends attendance data (name, UID, date, time) to Flask server
* Handles **duplicate marking**
* Shows **Invalid Card** messages
* Fully wireless via WiFi

---

## **Hardware Required**

* ESP32 Development Board
* MFRC522 RFID Module
* OLED Display (128x64)
* Jumper Wires
* RFID Cards/Tags

---

## **Software Required**

* Arduino IDE
* Libraries:

  * `MFRC522`
  * `Adafruit_SSD1306`
  * `Adafruit_GFX`
  * `WiFi`
  * `HTTPClient`

---

## **Wiring**

| ESP32 Pin | Component |
| --------- | --------- |
| 13        | RFID SS   |
| 14        | RFID RST  |
| 18        | RFID SCK  |
| 19        | RFID MISO |
| 23        | RFID MOSI |
| 21        | OLED SDA  |
| 22        | OLED SCL  |

---

## **Setup Instructions**

1. Install required libraries in Arduino IDE.
2. Connect ESP32 to WiFi by editing `ssid` and `password` in the code.
3. Upload the `AttendanceSystem.ino` code to ESP32.
4. Start your Flask server at the URL defined in `serverName`.
5. Scan RFID cards to mark attendance.

---

## **Usage**

* On scanning a valid card, OLED displays:

  * **Name**
  * **Attendance Marked**
* On scanning an already marked card:

  * Displays **Already Marked**
* On scanning an invalid card:

  * Displays **Invalid Card**

---

## **Example JSON Sent to Server**

```json
{
  "name": "Dharshika",
  "uid": "13 98 09 DA",
  "date": "2026-03-25",
  "time": "16:30:15"
}
```

---

## **Future Improvements**

* Store attendance data locally if WiFi is down
* Add a web dashboard to view attendance
* Support multiple classes and sessions

---

 
