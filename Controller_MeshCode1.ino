//masterController
#include "painlessMesh.h"
#include <ArduinoJson.h>

#define MESH_PREFIX     "whateverYouLike"
#define MESH_PASSWORD   "somethingSneaky"
#define MESH_PORT       5555


Scheduler userScheduler; // to control your personal task
painlessMesh mesh;
int SW1, SW2, SW3, SW4;

//COM12
  int msg1 = 0;
  int msg2 = 1;
  int msg3 = 1;
  int msg4 = 0;

// Function prototype
void sendMessage();

Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);

void sendMessage() {
DynamicJsonDocument doc2(1024);
  doc2["msg1"] = msg1;
  doc2["msg2"] = msg2;
  doc2["msg3"] = msg3;
  doc2["msg4"] = msg4;

  String jsonString2;
  serializeJson(doc2, jsonString2);
  mesh.sendBroadcast(jsonString2);

  // msg += mesh.getNodeId();
  // mesh.sendBroadcast( msg1 );
  // mesh.sendBroadcast( msg2 );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback(uint32_t from, String &msg) {

 Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, msg);
  // Extract values for each battery
  SW1 = doc["SW1"];
  SW2 = doc["SW2"];
  SW3 = doc["SW3"];
  SW4 = doc["SW4"];

  // Print the extracted values
  // Print the extracted values
  Serial.println("Print the extracted values");
  Serial.println();

  Serial.printf("SW1:", SW1);
  Serial.println(SW1);
  // if(SW1==1){
 // digitalWrite(PIN_RELAY_1,SW1);
 //digitalWrite(msg1,SW1);
      // }
    // else if(SW1==0){
  // digitalWrite(PIN_RELAY_1,LOW);
    // }
  Serial.printf("SW2:", SW2);
  Serial.println(SW2);
  Serial.printf("SW3:", SW3);
  Serial.println(SW3);
 
   
   Serial.printf("SW4:", SW4);
  Serial.println(SW4);
   
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}


void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {
  Serial.begin(115200);


  mesh.setDebugMsgTypes(ERROR | STARTUP);

  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void loop() {
  mesh.update();
  //onoff();
  Serial.println("relay on off");
   
}

