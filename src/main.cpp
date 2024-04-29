
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

struct BLK { 
  uint8_t  ID; 
  uint16_t Position;
};

BLK servos[15]; 
int th = 1000; 
int pos = 500;
int vel = 300;

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
  /*for (int i = 0; i < 10; i++){
    Serial.print("0X");
    Serial.print(buf[i], HEX);
    Serial.print(" ");
    }*/
  //Serial.println();
  delay(Time);
}


void moveServos(BLK servos[], uint8_t Num, uint16_t Time)
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
  delay(Time);
}

void serv16(int pos1,int pos2,int pos3,int pos4,int pos5,int pos6,int pos7,int pos8,int pos9,int pos10,int pos11,int pos12,int pos13,int pos14,int pos15,int pos16,int Time)
{
  servos[0].Position = pos1;
  servos[1].Position = pos2;
  servos[2].Position = pos3;
  servos[3].Position = pos4;
  servos[4].Position = pos5;
  servos[5].Position = pos6;
  servos[6].Position = pos7;
  servos[7].Position = pos8;
  servos[8].Position = pos9;
  servos[9].Position = pos10;
  servos[10].Position = pos11;
  servos[11].Position = pos12;
  servos[12].Position = pos13;
  servos[13].Position = pos14;
  servos[14].Position = pos15;
  servos[15].Position = pos16;
  moveServos(servos,16,Time);
}

void serv16m(int pos1,int pos2,int pos3,int pos4,int pos5,int pos6,int pos7,int pos8,int pos9,int pos10,int pos11,int pos12,int pos13,int pos14,int pos15,int pos16,int Time)
{
  servos[0].Position = th-pos6;
  servos[1].Position = th-pos7;
  servos[2].Position = th-pos8;
  servos[3].Position = th-pos9;
  servos[4].Position = th-pos10;
  servos[5].Position = th-pos1;
  servos[6].Position = th-pos2;
  servos[7].Position = th-pos3;
  servos[8].Position = th-pos4;
  servos[9].Position = th-pos5;
  servos[10].Position = th-pos14;
  servos[11].Position = th-pos15;
  servos[12].Position = th-pos16;
  servos[13].Position = th-pos11;
  servos[14].Position = th-pos12;
  servos[15].Position = th-pos13;
  moveServos(servos,16,Time);
}

void setup() {
  Serial.begin(9600);
  SerialPort.begin(9600, SERIAL_8N1, rx2Pin, tx2Pin);
  //ps5.begin("D0:BC:C1:05:69:31");  mando de renzo
  ps5.begin("4c:b9:9b:3b:f6:6e");
  
  Serial.println("Ready.");
  for (int i = 0; i < 16; i++) {
    servos[i].ID = i+1; 
  }  
  
}

void loop() {
//  while (ps5.isConnected() == false) { // commented out as ps5 controller seems to connect quicker when microcontroller is doing nothing
//    Serial.println("PS5 controller not found");
//    delay(300);
//  }

  while (ps5.isConnected() == true) {

    
    if (ps5.R1()) {//parado
      serv16(499,502,503,501,501,502,499,497,502,497,502,826,498,499,169,498,vel);
    }
    if (ps5.Triangle()) {//agachado
      serv16(499,343,804,681,501,501,653,194,324,498,499,826,498,498,173,498,vel);
    }
    if (ps5.Up()) {//caminar adelante
      
      serv16(457,353,774,621,500,454,645,147,321,497,500,826,499,496,172,499,vel);
      serv16(457,441,811,791,498,455,650,193,319,496,499,827,499,498,172,499,vel);
      serv16(513,449,764,727,529,513,638,190,324,502,499,827,499,498,172,499,vel);
      while (ps5.Up())
      {
        serv16(582,420,795,739,528,589,619,281,386,528,499,826,498,498,172,498,vel);
        serv16(593,358,824,736,528,595,560,197,214,532,499,826,499,497,171,498,vel);
        serv16(501,337,879,749,513,526,530,174,192,503,499,827,498,496,171,498,vel);
        serv16(444,359,770,696,522,443,605,140,193,510,500,828,497,498,172,499,vel);
        serv16(442,468,775,805,520,439,605,155,192,505,500,827,498,498,171,498,vel);
        serv16(513,449,764,727,529,513,638,190,324,502,499,827,499,498,172,499,vel);
      }
      
      serv16(457,441,811,791,498,455,650,193,319,496,499,827,499,498,172,499,vel);
      serv16(457,353,774,621,500,454,645,147,321,497,500,826,499,496,172,499,vel);
      serv16(499,343,804,681,501,501,653,194,324,498,499,826,498,498,173,498,vel);
    }
    
      
  while (ps5.Right())
      {
        serv16(448,366,764,654,500,463,654,184,324,500,499,827,498,498,170,498,vel);
        serv16(446,441,764,737,500,462,653,184,324,500,499,827,498,498,171,498,vel);
        serv16(506,412,831,761,503,521,670,172,324,501,499,827,498,499,171,498,vel);
        serv16(499,343,804,681,501,501,653,194,324,498,499,826,498,498,173,498,vel);     
    }
  
      
  while (ps5.Left())
      {
        serv16(554,334,834,680,503,567,629,232,343,499,499,827,498,498,171,498,vel);
        serv16(554,310,867,677,503,568,561,151,224,498,500,826,498,497,171,498,vel);
        serv16(513,310,867,679,503,520,565,151,224,498,500,827,498,497,171,498,vel);
        serv16(499,343,804,681,501,501,653,194,324,498,499,826,498,498,173,498,vel);
      }
  if (ps5.Square())
      {
        serv16(499,343,804,681,501,501,653,194,324,498,499,826,498,498,173,498,400);
        serv16(497,339,807,684,503,502,657,189,322,496,239,889,312,750,101,650,500);
        serv16(498,385,845,944,503,501,623,140,61,496,237,882,313,748,105,649,500);
        serv16(497,322,970,1000,502,501,684,23,9,496,237,836,313,748,140,649,500);
        serv16(495,207,973,885,503,501,782,21,113,497,236,837,314,748,138,649,3000);
        serv16(497,365,644,596,503,501,653,320,377,497,235,837,312,749,137,650,1000);
        serv16(497,365,644,594,502,500,653,320,378,494,235,837,153,750,137,809,500);
        serv16(498,365,645,595,501,500,653,320,378,497,169,880,448,836,137,519,500);
        serv16(499,364,662,585,498,501,654,322,496,494,168,879,228,840,138,516,800);
        serv16(498,365,663,561,500,500,656,323,500,494,168,879,151,659,136,845,800);
        serv16(499,364,662,585,498,501,654,322,496,494,168,879,228,840,138,516,800);
        serv16(498,365,663,561,500,500,656,323,500,494,168,879,151,659,136,845,800);
        serv16(499,364,662,585,498,501,654,322,496,494,168,879,228,840,138,516,800);
        serv16(498,365,663,561,500,500,656,323,500,494,168,879,151,659,136,845,800);

      }
  if (ps5.Circle())
  {
    serv16(499,343,804,681,501,501,653,194,324,498,499,826,498,498,173,498,400);
    serv16(568,366,827,683,502,581,595,289,398,497,500,826,499,497,171,496,400);
    serv16(597,371,783,646,497,579,643,145,346,538,501,610,498,493,325,497,400);
    serv16(588,469,604,580,486,578,645,142,346,624,578,694,347,725,440,706,400);
    serv16(592,471,609,581,492,579,645,143,346,619,734,652,221,849,440,852,400);
    serv16(554,463,609,581,325,579,645,142,346,619,730,652,222,847,438,847,400);
    serv16(562,459,628,586,283,579,426,381,473,667,259,608,303,505,436,625,400);
    serv16(554,463,609,581,325,579,645,142,346,619,730,652,222,847,438,847,400);
    serv16(562,459,628,586,283,579,426,381,473,667,259,608,303,505,436,625,400);
    serv16(554,463,609,581,325,579,645,142,346,619,730,652,222,847,438,847,400);
    serv16(562,459,628,586,283,579,426,381,473,667,259,608,303,505,436,625,400);
    serv16(554,463,609,581,325,579,645,142,346,619,730,652,222,847,438,847,400);
    serv16(562,459,628,586,283,579,426,381,473,667,259,608,303,505,436,625,400); 
    serv16(592,471,609,581,492,579,645,143,346,619,734,652,221,849,440,852,400); 
    serv16(588,469,604,580,486,578,645,142,346,624,578,694,347,725,440,706,400);
    serv16(568,366,827,683,502,581,595,289,398,497,500,826,499,497,171,496,400);
    serv16(499,343,804,681,501,501,653,194,324,498,499,826,498,498,173,498,400);
  }
  }
  }
      
/*
      serv16(510,463,425,335,494,501,419,667,648,502,551,860,693,701,117,299,vel);
      serv16(542,537,374,392,493,537,483,483,497,499,551,860,693,702,115,300,vel);
      serv16(541,533,373,387,492,534,407,611,555,497,551,860,693,702,115,300,vel);
      serv16(548,558,373,387,499,534,397,694,612,467,441,860,693,563,115,300,vel);
      serv16(544,631,260,340,489,551,475,679,707,477,320,859,693,432,115,300,vel);
      serv16(502,631,261,338,494,499,445,692,686,505,320,859,693,432,115,300,vel);
      serv16(441,593,366,402,493,464,425,716,692,502,319,859,693,431,116,300,vel);
      serv16(442,618,302,368,509,437,424,720,706,505,320,859,693,433,117,300,vel);
      serv16(442,523,304,270,525,433,414,720,706,500,440,859,693,560,115,300,vel);
      serv16(443,557,253,237,515,436,342,777,660,510,506,860,693,700,117,300,vel);
*/

  



    // This delay is to make the output more human readable
    // Remove it when you're not trying to see the output

