/*
  RFID Door Lock Access Control System
  --------------------------------------
  Reads RFID cards/tags using an RC522 reader. If the scanned card's UID
  matches an authorized card in the list, the electronic door lock is
  unlocked for a few seconds. Unauthorized cards are rejected and logged.

  Components:
    - Arduino Uno (or ESP32)
    - MFRC522 RFID reader module
    - RFID cards/tags
    - Relay module (controls the electronic door lock/solenoid)
    - Green LED (access granted)
    - Red LED (access denied)
    - Buzzer (optional, audible feedback)

  Library required:
    - MFRC522 by GithubCommunity (install via Arduino IDE Library Manager)

  Author: Maimoona Jahanzeb Khattak
*/

#include <SPI.h>
#include <MFRC522.h>

// ---------- Pin setup ----------
#define SS_PIN    10   // RFID SDA/SS pin
#define RST_PIN   9    // RFID reset pin
#define RELAY_PIN 7    // Controls the door lock/solenoid
#define GREEN_LED 5    // Access granted indicator
#define RED_LED   6    // Access denied indicator
#define BUZZER_PIN 8   // Optional buzzer

MFRC522 rfid(SS_PIN, RST_PIN);

const unsigned long UNLOCK_TIME = 5000; // How long the door stays unlocked (ms)

// ---------- Authorized cards ----------
// Add the UIDs of your own cards here (printed to Serial Monitor when scanned).
// Format: each UID is a byte array.
byte authorizedUIDs[][4] = {
  {0x12, 0x34, 0x56, 0x78},   // Example UID #1 - replace with your real card UID
  {0xAB, 0xCD, 0xEF, 0x01}    // Example UID #2 - replace with your real card UID
};
const int numAuthorizedCards = sizeof(authorizedUIDs) / sizeof(authorizedUIDs[0]);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  Serial.println("RFID Door Lock Access Control System ready.");
  Serial.println("Scan a card...");
}

void loop() {
  // Look for a new card
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Card UID: ");
  printUID(rfid.uid.uidByte, rfid.uid.size);

  if (isAuthorized(rfid.uid.uidByte, rfid.uid.size)) {
    grantAccess();
  } else {
    denyAccess();
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

// Print the scanned UID to Serial Monitor (helpful for registering new cards)
void printUID(byte *uid, byte uidSize) {
  for (byte i = 0; i < uidSize; i++) {
    Serial.print(uid[i] < 0x10 ? " 0" : " ");
    Serial.print(uid[i], HEX);
  }
  Serial.println();
}

// Check the scanned UID against the authorized list
bool isAuthorized(byte *scannedUID, byte uidSize) {
  for (int i = 0; i < numAuthorizedCards; i++) {
    bool match = true;
    for (byte j = 0; j < uidSize; j++) {
      if (scannedUID[j] != authorizedUIDs[i][j]) {
        match = false;
        break;
      }
    }
    if (match) return true;
  }
  return false;
}

void grantAccess() {
  Serial.println("Access GRANTED - Unlocking door.");
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RELAY_PIN, HIGH);   // Unlock
  tone(BUZZER_PIN, 1000, 200);     // Short beep

  delay(UNLOCK_TIME);

  digitalWrite(RELAY_PIN, LOW);    // Re-lock
  digitalWrite(GREEN_LED, LOW);
  Serial.println("Door locked again.");
}

void denyAccess() {
  Serial.println("Access DENIED - Unauthorized card.");
  digitalWrite(RED_LED, HIGH);
  tone(BUZZER_PIN, 300, 500);      // Low warning beep

  delay(1000);
  digitalWrite(RED_LED, LOW);
}
