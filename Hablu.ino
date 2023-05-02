#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t flag = 0;

//Defining BLE characteristics
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_NAME "Flag Animation"
#define CHARACTERISTIC_DESC_UP "Flag up animation"
#define CHARACTERISTIC_DESC_DOWN "Flag down animation"
#define CHARACTERISTIC_DESC_OFF "No animation"

//Locating sensors and connected pins
const int WHEEL_SENSOR = 2; // Wheel sensor input pin
const int ROPE_SENSOR = 3; // Rope sensor input pin
const int TOP_FLAG_SENSOR = 4; // Flag up input pin
const int BOTTOM_FLAG_SENSOR = 5; // Flag down input pin
const int BALL_SENSOR = 6; // Cannonball sensor input pin
const int FLAME_SENSOR = 7; // Flame sensor input pin
const int X_MARKS_BUTTON = 8; // X marks the spot input pin
const int CHEST_SENSOR_PIN = 9; // Treasure chest input pin

void setup() {
  pinMode(TOP_FLAG_SENSOR, INPUT_PULLUP);
  pinMode(BOTTOM_FLAG_SENSOR, INPUT_PULLUP);
  pinMode(WHEEL_SENSOR, INPUT_PULLUP);
  pinMode(ROPE_SENSOR, INPUT_PULLUP);
  pinMode(BALL_SENSOR, INPUT_PULLUP);
  pinMode(FLAME_SENSOR, INPUT_PULLUP);
  pinMode(X_MARKS_BUTTON, INPUT_PULLUP);
  pinMode(CHEST_SENSOR, INPUT_PULLUP);
  Serial.begin(9600);

  BLEDevice::init("Flag Animation");
  pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );
  pCharacteristic->setValue(&flag, 1);
  
  // Add descriptors to the characteristic
  BLEDescriptor* desc_up = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
  desc_up->setValue(CHARACTERISTIC_DESC_UP);
  pCharacteristic->addDescriptor(desc_up);
  
  BLEDescriptor* desc_down = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
  desc_down->setValue(CHARACTERISTIC_DESC_DOWN);
  pCharacteristic->addDescriptor(desc_down);
  
  BLEDescriptor* desc_off = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
  desc_off->setValue(CHARACTERISTIC_DESC_OFF);
  pCharacteristic->addDescriptor(desc_off);
  
  pService->start();
  BLEAdvertising* pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  int top_reed_state = digitalRead(TOP_FLAG_SENSOR);
  int bottom_reed_state = digitalRead(BOTTOM_FLAG_SENSOR);
  int wheel_sensor_state = digitalRead(WHEEL_SENSOR);
  int rope_sensor_state = digitalRead(ROPE_SENSOR);
  int ball_sensor_state = digitalRead(BALL_SENSOR);
  int flame_sensor_state = digitalRead(FLAME_SENSOR);
  int x_marks_state = digitalRead(X_MARKS);
  static int previous_state = HIGH;
  int current_state = digitalRead(CHEST_SENSOR);

//FLAG ANIMATION
  if (top_reed_state == LOW) {
    digitalWrite(FLAG_UP_PIN, HIGH); //Flag detected at top point
    digitalWrite(FLAG_DOWN_PIN, LOW);
    Serial.println("Flag up");
    Serial.println("Activating flag up animation...");
    flag = 1;
    pCharacteristic->setValue(&flag, 1);}
    
    else if (bottom_reed_state == LOW) {
    digitalWrite(FLAG_UP_PIN, LOW);
    digitalWrite(FLAG_DOWN_PIN, HIGH); //Flag detected at bottom point
    Serial.println("Flag down");
    Serial.println("Activating flag down animation...");
    flag = 2;
    pCharacteristic->setValue(&flag, 1);}

    else {
    digitalWrite(FLAG_UP_PIN, LOW);
    digitalWrite(FLAG_DOWN_PIN, LOW);
    Serial.println("No flag movement detected");
    Serial.println("Stopping any animation...");
    flag = 0;
    pCharacteristic->setValue(&flag, 1);
  }

//WHEEL ANIMATION
  if (wheel_sensor_state == LOW) //Wheel is moving 
  {
    Serial.println("Magnet moving!");
    Serial.println("Activating wheel spin animation...");
    // Add code to trigger wheel spin animation on app here
  } else {
    Serial.println("Magnet still"); //Wheel is still
    Serial.println("Stopping any animation...");
    // Add code to stop any animation on app here
  }

  //ROPE SPOOL ANIMATION
  if (rope_sensor_state == LOW) //Rope is moving
  {
    Serial.println("Rope detected moving!");
    Serial.println("Activating rope animation...");
    // Add code to trigger rope animation on app here
  } else //Rope is still
  {
    Serial.println("No rope movement detected");
    Serial.println("Stopping any animation...");
    // Add code to stop any animation on app here
  }

//CANNON ANIMATION
 if (ball_sensor_state == LOW) //Cannonball loaded
 {
    Serial.println("Ball sensor pressed!");
    Serial.println("Activating loading animation...");
    // Add code to trigger loading animation on app here
  } else if (flame_sensor_state == LOW) //Cannon fired
  {
    Serial.println("Flame sensor pressed!");
    Serial.println("Activating firing animation...");
    // Add code to trigger firing animation on app here
  } else {
    Serial.println("No sensor detected");
    Serial.println("Stopping any animation...");
    // Add code to stop any animation on app here
  }

//X MARKS THE SPOT ANIMATION
  if (x_marks_state == LOW) //X marks button pressed
  {
    Serial.println("X marks button pressed");
    Serial.println("Activating x marks animation...");
    // Add code to trigger X marks animation on app here
  }

  //TREASURE CHEST ANIMATION
if (current_state == LOW && previous_state == HIGH) //The chest has been shut (magnet has come close to sensor)
{
    Serial.println("Magnet detected near chest sensor - closing chest animation");
    Serial.println("Activating treasure chest animation...");
    // Add code to trigger closing chest animation on app here
 
  } else if (current_state == HIGH && previous_state == LOW) //The chest has been open (magnet has moved from sensor)
  {
    Serial.println("Magnet movement detected near chest sensor - opening chest animation");
    // Add code to trigger opening chest animation on app here
    triggerAnimation(OPENING_CHEST_ANIMATION);
  }

delay(500); // wait for a short period before reading the switches again
}
