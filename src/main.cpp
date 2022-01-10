// HystarHelper 
// Detect GC relay trigger and then trigger additional relay to allow hysatr enough time to register 

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


const int button_pin = 13; //GPIO13 = D7  
const int sda = 4; //GPIO14 = D5
const int scl = 5; //GPIO12 = D6
const int relay = 14; //GPIO = D1


int Button_Counter = 0;
int Current_State = 0;
int Last_State = 0;

void updateButtonPress(int newVal){
  int oldVal = newVal -1;
  display.setTextColor(BLACK);
  display.setCursor(70, 10);
  display.setTextSize(3);
  display.print(oldVal);
  display.display();
  delay(100);
  display.setTextColor(WHITE);
  display.setCursor(70, 10);
  display.print(newVal);
  display.display();
  }

void updateStatus(const char* NewStatus, const char* OldStatus){
  display.setTextColor(BLACK);
  display.setTextSize(3);
  display.setCursor(70, 40);
  display.print(OldStatus);
  display.display();
  delay(100);
  display.setTextColor(WHITE);
  display.setCursor(70, 40);
  display.print(NewStatus);
  display.display();
}  

void setup() {
  Serial.begin(115200);
  digitalWrite(relay, HIGH);
  pinMode(relay, OUTPUT);
  Wire.begin(sda, scl);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("GC Relay"); 
  display.setCursor(0,10);
  display.print("Triggered:");
  display.setCursor(0, 40);
  display.print("STATUS:");
  display.display(); 

pinMode(button_pin, INPUT);  
}

void loop() {
  Current_State = digitalRead(button_pin);

  // compare the buttonState to its previous state
  if (Current_State != Last_State) {
    // if the state has changed, increment the counter
    if (Current_State == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      Button_Counter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(Button_Counter);
      updateButtonPress(Button_Counter);
      updateStatus("ON","OFF");
      digitalWrite(relay, LOW);
      Serial.println("Triggering Relay");
      delay(5000);
      digitalWrite(relay, HIGH);
      Serial.println("Relay Off");
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
      updateStatus("OFF","ON");
      
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  
  
  // save the current state as the last state, for next time through the loop
  Last_State = Current_State;

}


