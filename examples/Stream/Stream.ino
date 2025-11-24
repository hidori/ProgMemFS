// Stream - Example sketch for ProgMemFS library

#include <Arduino.h>
#include "ProgMemStream.h"

// Define some data in program memory
const char message[] PROGMEM = "Hello, ProgMemStream!\n";

// Define a ProgMemStream with some data
ProgMemStream stream(message, strlen(message));

// Buffer for reading data
size_t length;
char buf[256];

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  while (!Serial < 1000);

  Serial.println("ProgMemFS FileSystem Example");

  // Read first 4 bytes
  length = stream.readBytes(buf, 4);
  buf[4] = 0;
  Serial.println(String("First 4 bytes: ") + buf);

  // Read next 4 bytes
  length = stream.readBytes(buf, 4);
  buf[4] = 0;
  Serial.println(String("Next 4 bytes: ") + buf);

  // Get current position
  Serial.println(String("Current position: ") + stream.position());

  // Seek to position 0
  stream.seek(0);
  Serial.println("Seeked to position 0.");

  // Read from the ProgMemStream and print to Serial
  while (stream.available()) {
    char c = stream.read();
    Serial.print(c);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
