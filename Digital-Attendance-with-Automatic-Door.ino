#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <SD.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

// Pin definitions
#define SS_RFID 10
#define RST_PIN 9
#define CS_SD 4
#define SERVO_PIN 3
#define RTC_IO 6    // DAT
#define RTC_SCLK 7  // CLK
#define RTC_CE 8    // RST

// Other definitions remain same
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2
#define CLOSED_ANGLE 0
#define OPEN_ANGLE 90

// Initialize objects
MFRC522 rfid(SS_RFID, RST_PIN);
Servo doorServo;
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
ThreeWire myWire(RTC_IO, RTC_SCLK, RTC_CE);
RtcDS1302<ThreeWire> rtc(myWire);
File logFile;
String uidString;

// Add at top with other globals
unsigned long previousMillis = 0;
const long interval = 1000; // Update time every 1 second

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  // Initialize RTC - only read mode
  rtc.Begin();
  
  // Other initializations
  SPI.begin();
  rfid.PCD_Init();
  doorServo.attach(SERVO_PIN);
  doorServo.write(CLOSED_ANGLE);
  
  // Initialize LCD with initial message
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Ready for Card");    // First line
  lcd.setCursor(0, 1);           // Move to second line
  displayDateTime();             // Show time on second line
  
  // Initialize SD Card
  if (!SD.begin(CS_SD)) {
    Serial.println("SD card failed!");
    return;
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // Update time display every second
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    lcd.setCursor(0, 1);  // Move to second line
    displayDateTime();     // Update time
  }

  // Check for RFID card
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    readCard();
    logAccess();
    checkAccess();
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}

void readCard() {
  uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uidString += String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    uidString += String(rfid.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();
}

void displayDateTime() {
  RtcDateTime now = rtc.GetDateTime();
  char datestring[20];
  
  snprintf_P(datestring, 
             countof(datestring),
             PSTR("%02u/%02u %02u:%02u"), 
             now.Month(),
             now.Day(),
             now.Hour(),
             now.Minute());
  lcd.print(datestring);
}

void logAccess() {
  RtcDateTime now = rtc.GetDateTime();
  logFile = SD.open("access.txt", FILE_WRITE);
  
  if (logFile) {
    // Log format: UID, MM/DD/YYYY HH:MM:SS
    logFile.print(uidString);
    logFile.print(", ");
    logFile.print(now.Month());
    logFile.print('/');
    logFile.print(now.Day());
    logFile.print('/');
    logFile.print(now.Year());
    logFile.print(' ');
    logFile.print(now.Hour());
    logFile.print(':');
    logFile.print(now.Minute());
    logFile.print(':');
    logFile.println(now.Second());
    logFile.close();
  } else {
    Serial.println("Error opening log file!");
  }
}

void checkAccess() {
  RtcDateTime now = rtc.GetDateTime();
  char datestring[20];
  
  if (uidString.substring(1) == "49 4A F8 0F") {
    // Format date/time string
    snprintf_P(datestring, 
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"), 
               now.Month(),
               now.Day(),
               now.Year(),
               now.Hour(),
               now.Minute(),
               now.Second());
               
    // LCD Display
    lcd.clear();
    lcd.print("Attendance OK!");
    lcd.setCursor(0, 1);
    lcd.print(datestring);
    
    // Serial Output
    Serial.println("Attendance recorded:");
    Serial.print("ID: ");
    Serial.println(uidString);
    Serial.print("Time: ");
    Serial.println(datestring);
    
    // Door control
    doorServo.write(OPEN_ANGLE);
    delay(3000);
    doorServo.write(CLOSED_ANGLE);
    
    delay(2000);  // Show attendance info longer
    lcd.clear();
    lcd.print("Ready for Card");
    lcd.setCursor(0, 1);
    displayDateTime();
  } else {
    lcd.clear();
    lcd.print("Invalid Card!");
    lcd.setCursor(0, 1);
    lcd.print(uidString);
    
    Serial.println("Invalid card detected:");
    Serial.println(uidString);
    
    delay(1000);
    lcd.clear();
    lcd.print("Ready for Card");
    lcd.setCursor(0, 1);
    displayDateTime();
  }
}
