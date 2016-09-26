#include <NewPing.h>

#define TRIGGER_PIN  6 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     3  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define FRONT_THRESH 6
#define TURN_THRESH 60

#define RIGHT 1
#define LEFT -1
#define FORWARD 0
#define RIGHT_0 2
#define LEFT_0 -2
#define TURN_THRESH2 3

NewPing sonarFront(4, 18, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarLeft(5, 2, MAX_DISTANCE);
NewPing sonarRight(6, 3, MAX_DISTANCE);

int fDist = 0;
int rDist = 0;
int lDist = 0;


void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  analogWrite(8, 150);
  analogWrite(9, 150);
}
void dirCorrect(int dir){
  if(dir==1){
    Serial.println( "Right" );
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
  }
  else if(dir==2){
    Serial.println( "Right Zero" );
   digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(10, HIGH); 
    digitalWrite(11, LOW);
  }
  else if (dir==-1){
    Serial.println( "Left" );
    digitalWrite(11, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
  }
  else if (dir==-2){
    Serial.println( "Left Zero" );
    digitalWrite(11, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
  }
    else if (dir==3){
    Serial.println( "STALLED" );
    digitalWrite(11, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
  }
  else{
    Serial.println("Forward");
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(10, LOW);
  }
}
void getDist(void){
    delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    fDist = sonarFront.ping_cm();
    rDist = sonarRight.ping_cm();
    lDist = sonarLeft.ping_cm();
}
void printD(void){
  Serial.print(lDist );
  Serial.print("  ");
  Serial.print(fDist);
  Serial.print("  ");
  Serial.print(rDist);
  Serial.println("  ");
}
int LANE_THRESH;
void loop() {
  getDist();
  printD();
  while(fDist > FRONT_THRESH || fDist==0){    //fDist>10 cm
    // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    getDist();
    printD();
    LANE_THRESH = (rDist + lDist)/2;
    if(rDist > (LANE_THRESH + 2)&& rDist < TURN_THRESH)
      dirCorrect(RIGHT);
    else if( lDist > (LANE_THRESH + 2) && lDist < TURN_THRESH)
      dirCorrect(LEFT);
    else
      dirCorrect(FORWARD);
    Serial.print("Fdist: \t");
    Serial.println(fDist);
    Serial.println("IN LANE FOLLOW LOOP");
  }
  dirCorrect(LEFT);
delay(100);

getDist();

int leftDistTurn=lDist;
int rightDistTurn=rDist;
int frontDistTurn=fDist;
if(lDist==0 || lDist>TURN_THRESH )
{
  while(fDist!=0 && fDist<100)
  {
    dirCorrect(LEFT_0);
    getDist();
  }
  dirCorrect(FORWARD);
  delay(500);
}
if(rDist==0 || rDist>TURN_THRESH )
{
  while(fDist<100 && fDist!=0)
  {
    dirCorrect(RIGHT_0);
    getDist();
    Serial.println(fDist);
  }
  dirCorrect(FORWARD);
  delay(500);
} 



