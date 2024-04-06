//slavedriver Mesh Communication
//ArduinoJson by Benoit version 7.0.3 is used 
#include "painlessMesh.h"
#include <Wire.h>

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
#define PIN_RELAY_1  14// The ESP32 pin GPIO14 connected to the IN1 pin of relay module
#define PIN_RELAY_2  12 // The ESP32 pin GPIO4 connected to the IN2 pin of relay module
#define PIN_RELAY_3  0 // The ESP32 pin GPIO0 connected to the IN3 pin of relay module
#define PIN_RELAY_4  2 // The ESP32 pin GPIO2 connected to the IN4 pin of relay module

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

int SW1 = 0;
int SW2 = 0;
int SW3 = 0;
int SW4 = 0;
//COM17
// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  // Create a JSON document
  StaticJsonDocument<200> jsonDocument;

  // Add data to the JSON document
  jsonDocument["SW1"] = SW1;
  jsonDocument["SW2"] = SW2;
  jsonDocument["SW3"] = SW3;
  jsonDocument["SW4"] = SW4;
  // Serialize the JSON document to a string
  String jsonString;
  serializeJson(jsonDocument, jsonString);

  // Send the JSON string
  mesh.sendBroadcast(jsonString);

  taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 5));
  
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {

  // Parse the received JSON string
  StaticJsonDocument<200> jsonDocument;
  DeserializationError error = deserializeJson(jsonDocument, msg);

  if (!error) {
    // Extract the values from the JSON document
    int SW1 = jsonDocument["msg1"];
    int SW2 = jsonDocument["msg2"];
    int SW3 = jsonDocument["msg3"];
    int SW4 = jsonDocument["msg4"];
      // Do something with received data
    Serial.print("Received SW1: ");
    Serial.println(SW1); // Print with 2 decimal places
    if (SW1 == 1) {// Turn on relay 1
    Serial.println(11);
    digitalWrite(PIN_RELAY_1, HIGH);
    } else { // Turn off relay 
      Serial.println(12);
    digitalWrite(PIN_RELAY_1, LOW);
  }
 
   
   
    Serial.print("Received SW2: ");
    Serial.println(SW2); // Print with 2 decimal places
    if (SW2 == 0) {// Turn on relay 2
      Serial.println("hi");
    digitalWrite(PIN_RELAY_2, LOW);
   } else{// Turn off relay 
     Serial.println("hhhh");
    digitalWrite(PIN_RELAY_2, HIGH);
  }

 
    Serial.print("Received SW3: ");
    Serial.println(SW3); // Print with 2 decimal places
    if (SW3 == 1) {// Turn on relay 3
      Serial.println("hello");
    digitalWrite(PIN_RELAY_3, HIGH);
   } else { // Turn off relay 
     Serial.println("bye");
    digitalWrite(PIN_RELAY_3, LOW);
  }

   
   
    Serial.print("Received SW4: ");
    Serial.println(SW4); // Print with 2 decimal places
   if (SW4 == 1) {// Turn on relay 4
     Serial.println(4);
   digitalWrite(PIN_RELAY_4, HIGH);
  } else {//Turn off relay 
    Serial.println(40);
    digitalWrite(PIN_RELAY_4, LOW);
}





  // Serial.printf("startHere: Received from %u B1 = %f\n, B2 = %f\n", from, SW1,2, SW2,2);
  } else {
    Serial.println("Error parsing JSON");
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
} 



void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RELAY_1, OUTPUT);
  pinMode(PIN_RELAY_2, OUTPUT);
  pinMode(PIN_RELAY_3, OUTPUT);
  pinMode(PIN_RELAY_4, OUTPUT);
  Wire.begin();

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  //onoff();

}





/*void onoff() {
  digitalWrite(PIN_RELAY_1, SW1 == 1 ? HIGH : LOW);

  // Example: If SW2 is 1, turn on relay 2, else turn it off
  digitalWrite(PIN_RELAY_2, SW2 == 0 ? HIGH : LOW);

  // Example: If SW3 is 1, turn on relay 3, else turn it off
  digitalWrite(PIN_RELAY_3, SW3 == 1 ? HIGH : LOW);

  // Example: If SW4 is 1, turn on relay 4, else turn it off
  digitalWrite(PIN_RELAY_4, SW4 == 0 ? HIGH : LOW);
  // Check if SW1 is pressed
  //if (digitalRead(SW1) == 0) {
    // Turn on relay 1
    //digitalWrite(PIN_RELAY_1, LOW);
  //}// else {
    // Turn off relay 1
    //digitalWrite(PIN_RELAY_1, 1);
  //}

  // Check if SW2 is pressed
  //if (digitalRead(SW2) == 1) {
    // Turn on relay 2
    //digitalWrite(PIN_RELAY_2, HIGH);
  //}// else {
    // Turn off relay 2
    //digitalWrite(PIN_RELAY_2, LOW);
  //}

  // Check if SW3 is pressed
  //if (digitalRead(SW3) == 0) {
    // Turn on relay 3
   // digitalWrite(PIN_RELAY_3, LOW);
  //} //else {
    // Turn off relay 3
    //digitalWrite(PIN_RELAY_3, LOW);
  //}

  // Check if SW4 is pressed
  //if (digitalRead(SW4) == 1) {
    // Turn on relay 4
    //digitalWrite(PIN_RELAY_4, HIGH);
  //} //else {
    // Turn off relay 4
    //digitalWrite(PIN_RELAY_4, LOW);
  //}
}*/


