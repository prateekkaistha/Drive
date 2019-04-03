/* LAST EDITED 21:03:2019 19:34
 *  This code controls the drive consisting of two motors powered by sabertooth
   two friglie (i.e), Small LA for AA box one DC Motor which provides yaw for
   the analog camera
   Controls
   Drive:
   Forward  :w
   Right    :a
   Backword :s
   Left     :d
            D1        D2
   SLA1     q        ,ctrl + q
   SLA2     e        ,ctrl + e
   Cam      t        ,ctrl + t
*/

//SPI variables
#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 37);
unsigned int localPort = 23908;
//int data;
 unsigned char data[UDP_TX_PACKET_MAX_SIZE];


int ss = 53;

int speed = 0;        //speed variable
int filter = 0.3;    //fileter variable
int limit = 80;
int f_map[] = {0, 0};
int pwm=200;
//Decode and find the direction
int bits[] = {0, 0, 0, 0, 0, 0, 0, 0};
int dat;
//Friglie and Camera motor pins
int LA1[] = {25, 27, 8};
int LA2[] = {46, 48, 7};
int CAM[] = {47, 49, 3};
EthernetUDP Udp;
void setup () {
  //SPI
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);

  for (int i = 0; i < 3; i++) {
    pinMode(LA1[i], OUTPUT);
    pinMode(LA2[i], OUTPUT);
    pinMode(CAM[i], OUTPUT);
  }

  Serial.begin(9600);
//  Serial1.begin(9600);
  Serial2.begin(9600);
 // Serial1.begin(9600);
  Serial.println("Setup");
}

void loop () {
  int packetSize = Udp.parsePacket();
  //Serial.println(packetSize);
  if(packetSize)
  {
    Udp.read(data, UDP_TX_PACKET_MAX_SIZE);    
    dat = data[0];
    //Serial.print(dat);
    //Serial.println("   ");
    process(abs(dat));    
  }
  
}

void updateBits(int val) {
  for (int i = 0; i < 8; i++)
    bits[i] = getBit(val, i);
  //  display();
}
int i=0;
float a=0,b=0;
float ca=0,cb=0;
float speedlimit=80;
float Up=0.020;
int speedl=0,speedr=0;

void process(int input)
{
 if(i==0)
 { ca=0; cb=0;}

 updateBits(abs(input));
 //DRIVE PART

 a=(bits[7]-bits[5])-(bits[6]-bits[4]);

 b=(bits[7]-bits[5])+(bits[6]-bits[4]);
 
 
 /*
 a=(-bits[7]+bits[5])-(-bits[6]+bits[4]);
 b=(-bits[7]+bits[5])+(-bits[6]+bits[4]);
// */
// display();
 if(abs(a)>1)
 { a=a/2;
  b=a*0.5; 
  }
 if(abs(b)>1)
 { b=b/2;
   a=b*0.5;
   }
Serial.println(" a : ");
Serial.print(a);
Serial.print("          b :");
Serial.println(b);

 if(a!=ca)
 {
  if(a>ca)
  ca=ca+Up;
  if(a<ca)
  ca=ca-Up;
 }
 
if(b!=cb)
 {
  if(b>cb)
  cb=cb+Up;
  if(b<cb)
  cb=cb-Up;
  }
 
 speedl=limit*ca;
 speedr=limit*cb;
 
 Left(speedl);
 Right(speedr);
 //AA box and camera part
 act(LA1, abs(bits[0] - bits[3]), bits[0], pwm);
 act(LA2, abs(bits[0] - bits[2]), bits[0], pwm);
 act(CAM, abs(bits[0] - bits[1]), bits[0], pwm);

i=5;//NEVER EVER touch this....It will ruin the whole algorithm or this process
}


bool getBit(int n, int pos) {
  return (n >> pos) & 1;
}
void display() {
 Serial.println("                          ");
 Serial.println("                          ");
 
  for (int i = 0; i < 8; i++) {
    
    Serial.print(bits[i]);
    Serial.print(" ");
  }
  
  Serial.println();
}
void Left(int t) {
  int x = map(t, -100, 100, 1, 127); // forward
  if(x>=60 &&x<=66)
  x=64;
  //Serial.print("Left:");
  //Serial.print(x);
  
  //Serial.print(" ");
  //Serial.print(t);
  command(x);
}

void Right(int t) {
  int x = map(t, -100, 100, 129, 256); // backward
  if(x>=189 && x<=194)
  x=192;
 // Serial.print("         Right:");
 // Serial.print(x);
 // Serial.println();
  //Serial.print(" ");
  //Serial.println(t);
  command(x);
}

void act(int arr[], boolean A, boolean B, int pwm) 
{
  digitalWrite(arr[0], A);
  digitalWrite(arr[1], B);
  analogWrite(arr[2], pwm);
}

void command(int x) 
{                  

//  Serial1.write(x);                  
//  delay(5);                           
 Serial2.write(x);                   
//  delay(5);                           
//  Serial1.write(x);                    
  delay(5);                            
}