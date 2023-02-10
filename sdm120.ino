#include <ModbusMaster.h>
#include <SoftwareSerial.h>

float V=0;
float A=0;
float W=0;
float Wh=0;
float PF=0;
float F=0;

SoftwareSerial SerialMod(15,4);
ModbusMaster node;
#define modbusaddr 1

void preTransmission()
{
  //delay(1);
}

void postTransmission()
{
  //delay(1);
}

void setup() {
  Serial.begin(9600); 
  SerialMod.begin(2400); 
  delay(2000);
  Serial.println("Ready");

  node.begin(modbusaddr, SerialMod);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);   
}

float reform_uint16_2_float32(uint16_t u1, uint16_t u2)
{  
  uint32_t num = ((uint32_t)u1 & 0xFFFF) << 16 | ((uint32_t)u2 & 0xFFFF);
    float numf;
    memcpy(&numf, &num, 4);
    return numf;
}

float getRTU(uint16_t m_startAddress){
  uint8_t m_length =2;
  uint16_t result;
  float x;

  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  result = node.readInputRegisters(m_startAddress, m_length);
  if (result == node.ku8MBSuccess) {
     return reform_uint16_2_float32(node.getResponseBuffer(0),node.getResponseBuffer(1));
  }
}  

void loop() {
  V = getRTU(0x0000);
  Serial.println("Voltage : " + String(V,2));
  delay(1000);
  A = getRTU(0x0006);
  Serial.println("Current : " + String(A,2));
  delay(1000);
  W = getRTU(0x000C);
  Serial.println("Active Power : " + String(W,2));
  delay(1000);
  Wh = getRTU(0x0156);
  Serial.println("Total Active Energy : " + String(Wh,2));
  delay(1000);
  PF = getRTU(0x001E);
  Serial.println("Power Factor : " + String(PF,2));
  delay(1000);
  F = getRTU(0x0046);
  Serial.println("Frequency : " + String(F,2));
  delay(1000);
  Serial.println("==================================");
}
