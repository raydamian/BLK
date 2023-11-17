#include <ps5Controller.h>
#include <iostream>
#include <HardwareSerial.h>
#include <Arduino.h>
#include <stdarg.h>
#include <Stream.h>


#define GET_LOW_BYTE(A) (uint8_t)((A))
#define GET_HIGH_BYTE(A) (uint8_t)((A) >> 8)

#define FRAME_HEADER            0x55
#define CMD_SERVO_MOVE          0x03

HardwareSerial SerialPort(2);

const uint8_t rx2Pin = 16;
const uint8_t tx2Pin = 17;

uint16_t count=0;

struct LobotServo { 
  uint8_t  ID; 
  uint16_t Position;
};

LobotServo servos[0];   

int pos = 500;


void moveServo(uint8_t servoID, uint16_t Position, uint16_t Time)
//(servoID,Posicion de 0 a 1000,Tiempo en ms)
{
  uint8_t buf[11];
  if (servoID > 31 || !(Time > 0)) {
    return;
  }
  buf[0] = FRAME_HEADER;             
  buf[1] = FRAME_HEADER;
  buf[2] = 8;                             
  buf[3] = CMD_SERVO_MOVE;              
  buf[4] = 1;                         
  buf[5] = GET_LOW_BYTE(Time);        
  buf[6] = GET_HIGH_BYTE(Time);        
  buf[7] = servoID;             
  buf[8] = GET_LOW_BYTE(Position);        
  buf[9] = GET_HIGH_BYTE(Position);        
  SerialPort.write(buf,10);
  for (int i = 0; i < 10; i++){
    Serial.print("0X");
    Serial.print(buf[i], HEX);
    Serial.print(" ");
    }
  Serial.println();
}


void moveServos(LobotServo servos[], uint8_t Num, uint16_t Time)
{
	uint8_t buf[103]; 
	if (Num < 1 || Num > 32 || !(Time > 0)) {
		return; 
	}
	buf[0] = FRAME_HEADER; 
	buf[1] = FRAME_HEADER;
	buf[2] = Num * 3 + 5; 
	buf[3] = CMD_SERVO_MOVE; 
	buf[4] = Num;  
	buf[5] = GET_LOW_BYTE(Time); 
	buf[6] = GET_HIGH_BYTE(Time);
	uint8_t index = 7;
	for (uint8_t i = 0; i < Num; i++) {
		buf[index++] = servos[i].ID;
		buf[index++] = GET_LOW_BYTE(servos[i].Position);
		buf[index++] = GET_HIGH_BYTE(servos[i].Position);
	}
	SerialPort.write(buf, buf[2] + 2);
}

void setup() {
  Serial.begin(9600);
  SerialPort.begin(9600, SERIAL_8N1, rx2Pin, tx2Pin);
  ps5.begin("D0:BC:C1:05:69:31"); 
  
  Serial.println("Ready.");
  servos[0].ID = 14;       
  servos[0].Position = 500;  
  moveServos(servos, 1, 1000);
  delay(1000);
}

void loop() {
//  while (ps5.isConnected() == false) { // commented out as ps5 controller seems to connect quicker when microcontroller is doing nothing
//    Serial.println("PS5 controller not found");
//    delay(300);
//  }

  while (ps5.isConnected() == true) {
    if (ps5.Right()) {
      Serial.printf("moving right %d", pos);
      pos = pos + 10;
      servos[0].Position = pos;  
      moveServos(servos, 1, 50);
      delay(50);
    };
    if (ps5.Left()) {
      Serial.printf("moving left %d", pos);
      pos = pos - 10;
      servos[0].Position = pos;  
      moveServos(servos, 1, 50);
      delay(50);
    };




    // This delay is to make the output more human readable
    Serial.print("Blacky");
    // Remove it when you're not trying to see the output
  }
}

