#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// Define the pins for software serial communication
SoftwareSerial mySerial(2, 3);

// Initialize the fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Define the pin for the relay
#define RELAY_PIN 4

// Define the delay time (in milliseconds) for access
#define ACCESS_DELAY 5000

void setup() {
  // Start serial communication with the fingerprint sensor at 57600 baud
  finger.begin(57600);
  delay(5);

  // Verify the password connection to the fingerprint sensor
  if (finger.verifyPassword()) {
    // Password verified, proceed
  } else {
    // Password verification failed, halt execution
    while (1) {
      delay(1);
    }
  }

  // Set the relay pin as an output
  pinMode(RELAY_PIN, OUTPUT);
  // Set the relay to its default state (HIGH)
  digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  // Check for a valid fingerprint
  if (getFingerPrint() != -1) {
    // If a valid fingerprint is found, activate the relay
    digitalWrite(RELAY_PIN, LOW);
    // Keep the relay activated for the defined access delay
    delay(ACCESS_DELAY);
    // Deactivate the relay
    digitalWrite(RELAY_PIN, HIGH);
  }
  
  // Add a small delay before the next fingerprint scan
  delay(50);
}

// Function to get a fingerprint ID
// Returns -1 if the fingerprint is not recognized, otherwise returns the fingerprint ID
int getFingerPrint() {
  int p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  // Found a match, return the fingerprint ID
  return finger.fingerID;
}
