//================== CONSTANTS - DON'T CHANGE THIS SECTION ==================
//MOTOR ONE IS RIGHT, MOTOR TWO IS LEFT
int m11 = 9; //motor 1
int m12 = 12;
int m21 = 11; //motor 10
int m22 = 10;
int m1pow = 13;

//FRONT SENSORS
int TFL = 2; //trig front left 
int EFL = A1; //echo front left 
double durFL, cmFL;

int TFR = 3; //trig front right - orange
int EFR = A2; //echo front right - green
double durFR, cmFR;

//RIGHT SENSORS
int TR1 = 4; //trig right 1 - white
int ER1 = A3; //echo right 1 - brown
double durR1, cmR1;

int TR2 = 5; //trig right 2
int ER2 = A4; //echo right 2
double durR2, cmR2;

//LEFT SENSORS
int TL1 = 6; //trig right 1 - purple
int EL1 = A5; //echo right 1 - white
double durL1, cmL1;

int TL2 = 7; //trig right 2
int EL2 = 8; //echo right 2
double durL2, cmL2;

//variables
int s1 = 8; //speed for motor 1 - 1
int s2 = 25; //speed for motor 10 - 6; used to be 14
int s1B = 4;
int s2B = 8; //used to be 7

int rtt = 480; //right turn time; used to be 790
int ltt = 485; //left turn time - 680 bfore

int ft = 600; // forward time assuming speed is 8 & 13; used to be 385
int ftx = 635;

int twft = 1150; //time to move ~20 cm; used to be 1290
int twftx = 655;

// ==================== GLOBAL VAIRABLES - YOU SHOULD MODIFY THIS =================
//these variables are used to calculate how long the robot should stop for after every move
int nr = 3; //number of right turns
int nl = 4; //number of left turns
int nft = 16; //number of times that the robot moves forward towards a wall (turns don't count)
int ntwft = 10; //number of times that the robot moves forward, but not with a wall in front of it

// ==================== DON'T CHANGE THIS FUNCTION ================
void setup(){
  //serial port begin
  Serial.begin(9600);
  //definign inputs and outputs
  pinMode(TFL, OUTPUT);
  pinMode(EFL, INPUT);
  pinMode(TFR, OUTPUT);
  pinMode(EFR, INPUT);
  
  pinMode(TR1, OUTPUT);
  pinMode(ER1, INPUT);
  pinMode(TR2, OUTPUT);
  pinMode(ER2, INPUT);
  
  pinMode(TL1, OUTPUT);
  pinMode(EL1, INPUT);
  pinMode(TL2, OUTPUT);
  pinMode(EL2, INPUT);

  pinMode(m1pow, OUTPUT);

  //setting up motor 1
  pinMode(m11, OUTPUT);
  pinMode(m12, OUTPUT);

  //setting up motor 2
  pinMode(m21, OUTPUT);
  pinMode(m22, OUTPUT);
  digitalWrite(m1pow, HIGH);
}

// ==================== DONT' CHANGE THIS FUNCTION ================
int calculateStopTime(){
  double wtf = ((ft+ftx)*nft) + ((twft+twftx)*ntwft) + (nl*ltt) + (nr*rtt);
  double temp = 72000 - wtf; 
  temp = temp /(nft+ntwft+nr+nl-1);
  
  return temp;
}

int st = 0;

//===================== ROBOT CODE GOES HERE ======================
void loop(){
  /* REFERENCE
  right(); left(); stop(stopTimeInMiliSeconds);
  forward(-1, true); makes the robot sense the closest wall. use this only if the closest wall is in front of the robot and less than 2 squares away (i.e. less than 1 meter away)
  forward(2, true); makes the robot move forward 2 steps (i.e. 50cm) and uses self-correction
  forward(3, true); makes the robot move forward 3 steps (i.e. 75cm) and does not use self-correction
  3 is right side and 2 is left side
  */
  st = calculateStopTime()/2; //INPUT THE TARGET TIME IN MILISECONDS (e.g. target time 60 seconds -> input 60000 seconds)
  stop(1000);
  forward(-1, true, 1);
  left();
  forward(-1, true, 1);
  right();
  forward(2, false, -1);
  right();
  forward(1, false, -1);
  forward(1, true, 2);
  left();
  forward(-1, true, 1);
  left();
  adjustBySide(2);
  forward(3, false, -1);
  forward(1, true, 2);
  left();
  forward(-1, true, 1);
  right();
  forward(2, true, 3); //consider turning it off if smth goes wrong



  stop(60000000);
}

//================= IF THE ROBOT IS WIGGLING TOO MUCH, COME HERE ==================
//the number inside this function (currently 1.3) means that, if the sensors sense 
//a distance with a difference greater than 1.3 cm, the robot will try to adjust its 
//direction. increase the number to make the self-correction less sensitive & decrease
// the wiggeling. 
bool aboutStraight(long distTwo, long distOne){
  if(distTwo > distOne){
    return ((distOne+1) >= distTwo); //used to be 1.2
  }else if (distTwo < distOne){
    return ((distOne-1) <= distTwo);
  }
  return true;
}


bool aboutStraightDistress(long distTwo, long distOne){
  if(distTwo > distOne){
    if(min(distTwo, distOne) < 20) {return ((distOne+1.2) >= distTwo);}
    else {return ((distOne+2.2) >= distTwo);}
  }else if (distTwo < distOne){
    if(min(distTwo, distOne) < 20) {return ((distOne-1.2) >= distTwo);}
    return ((distOne-2.2) <= distTwo);
  }
  return true;
}

// ============= HELPER FUNCTIONS - DON'T CHANGE ANYTHING FROM HERE ON WARDS ===============
double readDistance(int trig, int echo){
  //sensor is triggered by a HIGH pulse for >= 10 microseconds
  //give a short LOW pulse beforehand tSo ensure a clean HIGH pulse
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  double dur = pulseIn(echo, HIGH);
  delay(50); //prevent signal interference
  
  return dur/58.2;
}

void updateSensorReading(){
  //front sensors
  cmFL = readDistance(TFL, EFL);
  cmFR = readDistance(TFR, EFR);
  // //left sensors
  // cmL1 = min(readDistance(TL1, EL1), readDistance(TL1, EL1));
  // cmL2 = min(readDistance(TL2, EL2), readDistance(TL2, EL2));
  // //right sensors
  // cmR1 = min(readDistance(TR1, ER1), readDistance(TR1, ER1));
  // cmR2 = min(readDistance(TR2, ER2), readDistance(TR2, ER2));

  //left sensors
  cmL1 = readDistance(TL1, EL1);
  cmL2 = readDistance(TL2, EL2);
  //right sensors
  cmR1 = readDistance(TR1, ER1);
  cmR2 = readDistance(TR2, ER2);
}

double average(double one, double two){
  return (one+two)/2;
}

void adjustBySide(int side){
  int adjT = 105;
  if(side == 0) return;
  else if(side == 1){ //front
    if(cmFL > cmFR){ //one back two forward
      digitalWrite(m11, 0);
      digitalWrite(m12, s1);
      digitalWrite(m21, s2);
      digitalWrite(m22, 0);
      delay(adjT);
      stop(10);
    }else{ //one forward two backward
      digitalWrite(m11, s1);
      digitalWrite(m12, 0);
      digitalWrite(m21, 0);
      digitalWrite(m22, s2);
      delay(adjT);
      stop(10);
    }
  }
  else if(side == 2){ //left
    if(cmL1 < cmL2){ //one back two forward
      digitalWrite(m11, 0);
      digitalWrite(m12, s1);
      digitalWrite(m21, s2);
      digitalWrite(m22, 0);
      delay(adjT);
      stop(10);
    }else{ //one forward two backward
      digitalWrite(m11, s1);
      digitalWrite(m12, 0);
      digitalWrite(m21, 0);
      digitalWrite(m22, s2);
      delay(adjT);
      stop(10);
    }
  }else if(side == 3){ //right
    if(cmR1 < cmR2){ //one back two forward
      Serial.println("detected issue and is doing stuff about it in if");
      digitalWrite(m11, 0);
      digitalWrite(m12, s1);
      digitalWrite(m21, s2);
      digitalWrite(m22, 0);
      delay(adjT);
      stop(10);
    }else{ //one forward two backward
      Serial.println("detected issue and is doing stuff about it in else");
      digitalWrite(m11, s1);
      digitalWrite(m12, 0);
      digitalWrite(m21, 0);
      digitalWrite(m22, s2);
      delay(adjT);
      stop(10);
    }
  }
}

void forwardSmall(){
  digitalWrite(m11, s1);
  digitalWrite(m12, 0);
  digitalWrite(m21, s2);
  digitalWrite(m22, 0);
  delay(850);
  stop(st);
}

void forwardTillDist(int distLeft, int side){
  updateSensorReading();

  bool statOne = true; //front right sensor
  bool statTwo = true; //front left sensor

  if(cmFL <= distLeft) {statTwo = false;} //true = proceed
  if(cmFR <= distLeft) {statOne = false;}

  if(side == 3){
        while(!aboutStraight(cmR1, cmR2)){
          adjustBySide(side);
          updateSensorReading();
        }
      }else if (side == 2){
        while(!aboutStraight(cmL1, cmL2)){
          adjustBySide(side);
          updateSensorReading();
        }
      }else if(side == 1){          
        while(!aboutStraight(cmFL, cmFR)){
          adjustBySide(side);
          updateSensorReading();
        }
      }

  while(statOne == true || statTwo == true){
      digitalWrite(m11, s1B);
      digitalWrite(m12, 0);
      digitalWrite(m21, s2B);
      digitalWrite(m22, 0);
      delay(ft);
      stop(st);

      updateSensorReading();
      if(cmFL <= distLeft) {statTwo = false;}
      else {statTwo = true;}
      if(cmFR <= distLeft) {statOne = false;}
      else {statOne = true;}

      // if((cmFL - 10) >= cmFR || (cmFR - 10) >= cmFL){
      //   distress();
      // }
      updateSensorReading();
      if(side == 3){
        while(!aboutStraight(cmR1, cmR2)){
          adjustBySide(side);
          updateSensorReading();
        }
      }else if (side == 2){
        while(!aboutStraight(cmL1, cmL2)){
          adjustBySide(side);
          updateSensorReading();
        }
      }else if(side == 1){          
        while(!aboutStraight(cmFL, cmFR)){
          adjustBySide(side);
          updateSensorReading();
        }
      }
    }
}

void forward(int n, bool useCorrection, int side){ //-1 when you use the sensor to decide when to turn. n is the number of steps the robot should take before executing the next command in void loop
  //GET DATA FROM SENSOR & DETERMINE NEXT STEP
  updateSensorReading();

  bool statOne = true; //front right sensor
  bool statTwo = true; //front left sensor

  //MOVE BY STEPS
  if(n != -1){

    for(int i = 0; i < n; i++){
      digitalWrite(m11, s1);
      digitalWrite(m12, 0);
      digitalWrite(m21, s2);
      digitalWrite(m22, 0);
      delay(twft);
      stop(st);
      Serial.println(st);
      Serial.println("  ");
      if(i%2 == 0 && !useCorrection){
        digitalWrite(m21, s2);
        digitalWrite(m22, 0);
        delay(140);
        stop(100);
      }
      
      if(useCorrection == true){
        updateSensorReading();
        if(side == 3){
          while(!aboutStraight(cmR1, cmR2)){
            adjustBySide(side);
            updateSensorReading();
          }
        }else if (side == 2){
          while(!aboutStraight(cmL1, cmL2)){
            adjustBySide(side);
            updateSensorReading();
          }
        }else if(side == 1){
          while(!aboutStraight(cmFL, cmFR)){
            adjustBySide(side);
            updateSensorReading();
          }
        }
      }

      
    }
  }else{ //MOVE BY SENSED WALL DIST
    if(cmFL <= 25) {statTwo = false;} //true = proceed
    if(cmFR <= 25) {statOne = false;}

    while(!aboutStraight(cmFL, cmFR)){
        adjustBySide(1);
        updateSensorReading();
    }

    //MOVING
    while(statOne == true && statTwo == true){
      //Serial.println("MOVING");
      digitalWrite(m11, s1B);
      digitalWrite(m12, 0);
      digitalWrite(m21, s2B);
      digitalWrite(m22, 0);
      delay(ft);
      stop(st);

      updateSensorReading();
      if(cmFL <= 24) {statTwo = false;}
      else {statTwo = true;}
      if(cmFR <= 24) {statOne = false;}
      else {statOne = true;}

      if((cmFL - 10) >= cmFR || (cmFR - 10) >= cmFL){
        distress();
      }

      while(!aboutStraight(cmFL, cmFR)){
        adjustBySide(1);
        updateSensorReading();
      }
    }

    
    updateSensorReading();
  }
}

void stop(int time){ //miliseconds
  digitalWrite(m11, 0);
  digitalWrite(m12, 0);
  digitalWrite(m21, 0); 
  digitalWrite(m22, 0);

  delay(time);
}

void left(){
  digitalWrite(m11, s1); //4
  digitalWrite(m12, 0);
  digitalWrite(m21, 0); //4
  digitalWrite(m22, s2);
  delay(ltt);
  stop(st);
}

void right(){
  digitalWrite(m11, 0); //4
  digitalWrite(m12, s1);
  digitalWrite(m21, s2); //9
  digitalWrite(m22, 0);
  delay(rtt);
  stop(st);
}

void distress(){
  backward(600); 
  stop(200);
  updateSensorReading();
    while(!aboutStraightDistress(cmFL, cmFR)){
      adjustBySide(1);
      updateSensorReading();
      Serial.println(cmFL);
      Serial.println(cmFR);
      Serial.println(" ");
    }
}


void backward(int time){
  digitalWrite(m11, 0);
  digitalWrite(m12, s1B);
  digitalWrite(m21, 0);
  digitalWrite(m22, s2B);
  delay(time);
}

// int determineSide(){ 
//   //return 1 for front, 2 for left, and 3 for right. 0 for don't use it for the next step
//   //use the set of numbers that are the closest to the robot & don't look funky
//   bool front = (abs(cmFL-cmFR) <= 17 && max(cmFL, cmFR) <= 175);
//   bool left = (abs(cmL1-cmL2) <= 17 && max(cmL1, cmL2) <= 175);
//   bool right = (abs(cmR1-cmR2) <= 17 && max(cmR1, cmR2) <= 175);

//   double avgF = average(cmFL, cmFR);
//   double avgL = average(cmL1, cmL2);
//   double avgR = average(cmR1, cmR2);
//   double min = min(avgF, min(avgL, avgR));

//   if(!front && !left && !right) {return 0;}
//   if(front && avgF == min) return 1;
//   if(left && avgL == min) return 2;
//   if(right && avgR == min) return 3;
// }



