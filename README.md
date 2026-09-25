# 🔐 RFID Door Lock Access Control System

An RFID-based access control system that replaces traditional keys with contactless cards/tags. When an authorized card is scanned, the electronic door lock is released for a few seconds; unauthorized cards are rejected and logged to the Serial Monitor.

## How It Works
1. The MFRC522 RFID reader scans a card/tag placed near it.
2. The Arduino reads the card's unique ID (UID) and compares it against a list of authorized UIDs.
3. If the UID matches:
   - The relay activates, unlocking the electronic door lock/solenoid.
   - The green LED lights up and a short beep confirms access.
4. If the UID does not match:
   - Access is denied, the red LED lights up, and a warning beep sounds.
5. The door automatically re-locks after a set unlock duration.

## Components
| Component | Purpose |
|---|---|
| Arduino Uno (or ESP32) | Microcontroller running the logic |
| MFRC522 RFID reader module | Scans RFID cards/tags |
| RFID cards/tags | Used as access credentials |
| Relay module | Switches the electronic lock/solenoid |
| Electronic door lock / solenoid | Physically locks/unlocks the door |
| Green LED | Access granted indicator |
| Red LED | Access denied indicator |
| Buzzer (optional) | Audible feedback |

## Wiring (Arduino Uno)
| MFRC522 Pin | Arduino Pin |
|---|---|
| SDA (SS) | 10 |
| SCK | 13 |
| MOSI | 11 |
| MISO | 12 |
| RST | 9 |
| GND | GND |
| 3.3V | 3.3V |

| Other Component | Arduino Pin |
|---|---|
| Relay (IN) | 7 |
| Green LED | 5 |
| Red LED | 6 |
| Buzzer | 8 |

> ⚠️ The MFRC522 module runs on 3.3V logic — do not power it from 5V, as this can damage it.

## Setup Instructions
1. Install the **MFRC522** library: Arduino IDE → Tools → Manage Libraries → search "MFRC522" (by GithubCommunity) → Install.
2. Wire the components as shown above.
3. Upload `rfid_door_lock.ino` to your Arduino.
4. Open the Serial Monitor (9600 baud) and scan each card you want to authorize.
5. Copy the printed UID (e.g., `12 34 56 78`) and add it to the `authorizedUIDs` array in the code.
6. Re-upload the sketch. Only cards in the list will now unlock the door.

## Possible Improvements
- Store authorized UIDs in EEPROM so new cards can be added without reflashing the code.
- Add a keypad for PIN-based backup access.
- Log every access attempt (with timestamp) to an SD card or send it to a cloud database.
- Send a notification (SMS/Telegram/email) when an unauthorized card is scanned.
- Add a mobile app or web dashboard to manage authorized users remotely.
