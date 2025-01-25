#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_MSG_LEN 128

enum msg_type{ 
  read_req, edit_req, func_req, yaml_read
};

struct msg_interp{
  int req_type;
  int cur_msg_count;
  int tot_msg_count;
  int msg_length;
  char msg[MAX_MSG_LEN];
  int cumsum = 0; // Check type
};

uint8_t* str_to_byte_msg(int req_type, char* msg_str){
  struct msg_interp msg_instance;

  if (strlen(msg_str)>MAX_MSG_LEN){
    Serial.println("The message is too long");
    return NULL;
  }

  msg_instance.req_type = req_type;
  // CHANGE COUNTERS LATER

  msg_instance.cur_msg_count = 1000;  // Adjust later
  msg_instance.tot_msg_count = 1;  // Adjust later
  //msg_instance.msg_length = strlen(msg_str) < MAX_MSG_LEN ? strlen(msg_str) : MAX_MSG_LEN;
  msg_instance.msg_length = strlen(msg_str);

  strncpy(msg_instance.msg, msg_str, MAX_MSG_LEN);

  size_t struct_size = sizeof(struct msg_interp);
  uint8_t* byte_msg = (uint8_t*)malloc(struct_size);
  if (byte_msg == NULL) {
      perror("Failed to allocate memory for MSG");
      return NULL;
  }

  // Copy the struct contents to the byte array
  memcpy(byte_msg, &msg_instance, struct_size);
  
  Serial.print("Byte array: ");
    for (size_t i = 0; i < struct_size; i++) {
      Serial.print(byte_msg[i], HEX);  // Prints each byte as hexadecimal
      Serial.print(" ");
    }
    Serial.println();

  return byte_msg;
}

void print_byte_array(size_t length, uint8_t* pData){
  Serial.print("Byte array: ");
  for (size_t i = 0; i < length; i++) {
    Serial.print(pData[i], HEX);  // Prints each byte as hexadecimal
    Serial.print(" ");
  }
  Serial.println();
}


void print_msg(struct msg_interp* msg){
  Serial.printf("req_type: %d, cur_msg_count: %d, tot_msg_count: %d, msg_length: %d, msg: %s\n",
  msg->req_type, msg->cur_msg_count, msg->tot_msg_count, msg->msg_length, msg->msg);
}

/////////////////////////////////////////////
// Simple communication usage example      //
/////////////////////////////////////////////

// void return_BLE(){

//   uint8_t* msg_bytes = str_to_byte_msg(0,"Hi! How are you today?! Here is a dot . ");
//   uint16_t len = sizeof(struct msg_interp);
//   struct msg_interp* try_to_read = (struct msg_interp*)msg_bytes;
//   Serial.printf("req_type: %d, cur_msg_count: %d, tot_msg_count: %d, msg_length: %d, msg: %s",
//    try_to_read->req_type, try_to_read->cur_msg_count, try_to_read->tot_msg_count, try_to_read->msg_length, try_to_read->msg);


//   pCharacteristic->setValue(msg_bytes, len);
//   pCharacteristic->notify();
// }
