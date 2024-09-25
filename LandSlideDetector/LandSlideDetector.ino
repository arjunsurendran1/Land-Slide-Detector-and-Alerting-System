//Final Code
#include <SoftwareSerial.h>

//Pins for sensors Leds and Buzzers
int vib_pin = 7; // vibration sensor pin
int red_led_pin = 9; // Red Led pin
int green_led_pin = 8; // Green Led pin
int buzzer_pin = 12; // Buzzer Pin
int Soil_moisture_pin = A0; // Analog pin for soil moisture sensor
int threshold = 500; // threshold for soil moisture sensor

SoftwareSerial gsmSerial(10, 11); // RX, TX for GSM module
const char* phoneNumber = "+9181290xxxxx"; // Replace with the recipient's number

void setup() {
  //Assign sensors and Led pins
  pinMode(vib_pin, INPUT);
  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  
  Serial.begin(9600);
  gsmSerial.begin(9600);
  
  // Initialize GSM
  gsmSerial.println("AT");
  delay(100);
  gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(100);
}

void loop() {
  //read sensor values
  int soilMoistureValue = analogRead(Soil_moisture_pin); //soil moisture sensor for read water content in soil.
  int val = digitalRead(vib_pin); //vibration sensor 
  
  Serial.println(soilMoistureValue);
  // if both sensor triggers then it will be danger
  if ((val == 1) && (soilMoistureValue <= threshold)) {
    for (int i = 0; i <= 5; i++) {
      Serial.println("Danger");
      MakeCall();
      sendSMS("Danger detected");
      digitalWrite(green_led_pin, LOW);
      digitalWrite(red_led_pin, HIGH);
      tone(buzzer_pin, 1000);  // Generate a tone of 1000 Hz on the buzzer
      delay(500);
      digitalWrite(red_led_pin, LOW);
      noTone(buzzer_pin);  // Turn off the buzzer
      delay(500);
    }
  } 
  //if one of the sensor triggers it will alert us
  else if ((val == 1) || (soilMoistureValue <= threshold)) {
    // Medium alert
    Serial.println("Not Safe");
    digitalWrite(green_led_pin, LOW);
    digitalWrite(red_led_pin, HIGH);
    tone(buzzer_pin, 1000);
    delay(100);
    digitalWrite(red_led_pin, LOW);
    noTone(buzzer_pin);
    delay(300);
  } else {
    // else it is safe there is no chance of land slide.
    digitalWrite(green_led_pin, HIGH);
    digitalWrite(red_led_pin, LOW);
    digitalWrite(buzzer_pin, LOW);
    Serial.println("Safe");
  }
}

//make call
void MakeCall() {
  Serial.println("Making a call...");
  gsmSerial.print("ATD");
  gsmSerial.print(phoneNumber);
  gsmSerial.println(";");
  delay(2000);
}

//send sms
void sendSMS(String message) {
  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(phoneNumber);
  gsmSerial.println("\"");
  delay(100);
  
  gsmSerial.print(message);
  delay(100);
  
  gsmSerial.write(26); // Send Ctrl+Z to indicate the end of the message
  Serial.println("SMS sent: " + message);
  delay(1000);
}
