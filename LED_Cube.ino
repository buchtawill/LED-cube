/*
 *  Copyright 2020 Will Buchta

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 * Render and shift functions taken from Harry Le:
 * https://github.com/itsharryle/LED_CUBE/tree/master/CUBE
 * This code is still a work in progress. Writing animations is not easy.
 * 
 */
#include <SPI.h>
#define POS_X 0
#define NEG_X 1
#define POS_Z 2
#define NEG_Z 3
#define POS_Y 4
#define NEG_Y 5
int cube[8][8];
int susShifts[8][8];
uint8_t characters[38][8] = {
  {0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C}, //0
  {0x10, 0x18, 0x14, 0x10, 0x10, 0x10, 0x10, 0x3C}, //1
  {0x3C, 0x42, 0x40, 0x40, 0x3C, 0x02, 0x02, 0x7E}, //2
  {0x3C, 0x40, 0x40, 0x3C, 0x40, 0x40, 0x42, 0x3C}, //3
  {0x22, 0x22, 0x22, 0x22, 0x7E, 0x20, 0x20, 0x20}, //4
  {0x7E, 0x02, 0x02, 0x3E, 0x40, 0x40, 0x42, 0x3C}, //5
  {0x3C, 0x02, 0x02, 0x3E, 0x42, 0x42, 0x42, 0x3C}, //6
  {0x3C, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40}, //7
  {0x3C, 0x42, 0x42, 0x3C, 0x42, 0x42, 0x42, 0x3C}, //8
  {0x3C, 0x42, 0x42, 0x42, 0x3C, 0x40, 0x40, 0x3C}, //9
  { 0x00, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66}, // 10 A
  { 0x00,  0x7C,  0x66,  0x66,  0x7C,  0x66,  0x66,  0x7C }, //11 B
  { 0x00,  0x3C,  0x66,  0x60,  0x60,  0x60,  0x66,  0x3C }, //12 C
  { 0x00,  0x7C,  0x66,  0x66,  0x66,  0x66,  0x66,  0x7C }, //13 D
  { 0x00,  0x7E,  0x60,  0x60,  0x7C,  0x60,  0x60,  0x7E }, //14 E
  { 0x00,  0x7E,  0x60,  0x60,  0x7C,  0x60,  0x60,  0x60 }, //15 F
  { 0x00,  0x3C,  0x66,  0x60,  0x60,  0x6E,  0x66,  0x3C }, //16 G
  { 0x00,  0x66,  0x66,  0x66,  0x7E,  0x66,  0x66,  0x66 },   // 17 : H
  { 0x00,  0x3C,  0x18,  0x18,  0x18,  0x18,  0x18,  0x3C },   // 18 : I
  { 0x00,  0x1E,  0x0C,  0x0C,  0x0C,  0x6C,  0x6C,  0x38 },    // 19 : J
  { 0x00,  0x66,  0x6C,  0x78,  0x70,  0x78,  0x6C,  0x66 },    // 20 : K
  { 0x00,  0x60,  0x60,  0x60,  0x60,  0x60,  0x60,  0x7E },    // 21 : L
  { 0x00,  0x63,  0x77,  0x7F,  0x6B,  0x63,  0x63,  0x63 },    // 22 : M
  { 0x00,  0x63,  0x73,  0x7B,  0x6F,  0x67,  0x63,  0x63 },    // 23 : N
  { 0x00,  0x3C,  0x66,  0x66,  0x66,  0x66,  0x66,  0x3C },    // 24 : O
  { 0x00,  0x7C,  0x66,  0x66,  0x66,  0x7C,  0x60,  0x60 },    // 24 : P
  { 0x00,  0x3C,  0x66,  0x66,  0x66,  0x6E,  0x3C,  0x06 },    // 26 : Q
  { 0x00,  0x7C,  0x66,  0x66,  0x7C,  0x78,  0x6C,  0x66 },    // 27 : R
  { 0x00,  0x3C,  0x66,  0x60,  0x3C,  0x06,  0x66,  0x3C },    // 28 : S
  { 0x00,  0x7E,  0x5A,  0x18,  0x18,  0x18,  0x18,  0x18 },    // 29 : T
  { 0x00,  0x66,  0x66,  0x66,  0x66,  0x66,  0x66,  0x3E },    // 30 : U
  { 0x00,  0x66,  0x66,  0x66,  0x66,  0x66,  0x3C,  0x18 },    // 31 : V
  { 0x00,  0x63,  0x63,  0x63,  0x6B,  0x7F,  0x77,  0x63 },    // 32 : W
  { 0x00,  0x63,  0x63,  0x36,  0x1C,  0x36,  0x63,  0x63 },    // 33 : X
  { 0x00,  0x66,  0x66,  0x66,  0x3C,  0x18,  0x18,  0x18 },    // 34 : Y
  { 0x00,  0x7E,  0x06,  0x0C,  0x18,  0x30,  0x60,  0x7E },    // 35 : Z
  {0xc3, 0xc3, 0x00, 0x18, 0x18, 0x81, 0xff, 0x7e},         // 36 smiley
  {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}                  //37 space
};
double baud = 200000;
void setup() {
  randomSeed(analogRead(A0));
  SPI.begin();
  SPI.beginTransaction(SPISettings(baud, MSBFIRST, SPI_MODE0));
  Serial.begin(9600);
  while (!Serial);
  //cube[layer from top][row from right] = B00111100
  //cube[1][0]=128;
  //shift(NEG_Z);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      susShifts[i][j] = random(0, 8);
    }
  }
}

int timer = 0, whichsine = 1;
boolean done = false;
int frame = 0, sineBounceFrame = 0, sendFrame = 0, bounce = 0;
float rad = 3 * PI / 2, inc = PI / 8;
int sinTime = 10, squareTime = 20, sawTime = 20, triTime = 20, RISE_TIME = 10, boxTime = 25;
int sendTime = 1, squareMoveTime = 20, suspendTime = 5;
int bx = 0, by = 0, bz = 0; //initial bottom right corner location of box
boolean sendsw = true, initSend = true, sineBouncing, sending;
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
//<>////////////////////////////////////////////////////////////////////////////////////////<>
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
char changer; int dummy = 3;
void loop() {
  //  if (Serial.available()) {
  //    changer = Serial.read();
  //  }
  //  testKeyboard(changer);
  //  changer='z';
  //  if(frame<50)squarewave(B01111110);
  //  else if(frame<200)boxmove(5);
  //  else if(frame<300)sinewave(255,1);
  //  else if(frame<400)sinewave(255,2);
  //  else if(frame<500)sinewave(255,3);
  //  else if(frame<600)sinewave(255,4);
  //  else if(frame<700)sinewave(255,5);
  //  else if(frame<800)sinewave(255,6);
  //  else if(frame<900)sinewave(255,7);
  //  else if(frame<1000)sinewave(255,8);
  //  else if(frame<1100)weirdsine();
  //  else if(frame<1400)squid();
  //  else frame=0;
  //squid();
  //sinewave(255,2);
  //sineBouncing=true;
  //sineBounce();
  //boxmove(7);
  //sineTest();
  //sendHelper();
  //suspendHelper();
  //squareMove();
  //sending=true;
  //sendHelper();
  //sineTest();
    if(frame<100){
      sineBouncing=true;
      sineBounce();
    }
    else if(frame<150){
      sineBouncing=false;
      sineBounceFrame=0;
      boxmove(4);
    }
    else if(frame<1942){
      sending=true;
      sendHelper();
    }
    else {
      frame=0;
      initSend=true;
    }

  render();
  if (timer == 0) {
    if (sineBouncing)sineBounceFrame++;
    if (sending)sendFrame++;
    frame++;
  }
  timer++;

}
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
//<>////////////////////////////////////////////////////////////////////////////////////////<>
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
int kx = 0, ky = 0, kz = 0;
void testKeyboard(char kdir) {
  clearCube();
  setVoxel(kx, ky, kz);
  if (kdir == 'w')ky++;
  else if (kdir == 's')ky--;
  else if (kdir == 'a')kz--;
  else if (kdir == 'd')kz++;
  else if (kdir == 'i')kx++;
  else if (kdir == 'k')kx--;
  //setbounds and loop
  if (kx < 0)kx = 7;
  if (kx > 7)kx = 0;
  if (ky > 7)ky = 0;
  if (ky < 0)ky = 7;
  if (kz > 7)kz = 0;
  if (kz < 0)kz = 7;
}
void sineTest() {
  if (timer > sinTime) {
    clearCube();
    //shift(POS_X);
    //shift(NEG_Z);
    timer = 0;
    for (int i = 0; i < 16; i++) {
      int sPos = 3.4 * sin(rad + inc * i) + 4;
      int sOut = round(sPos);
      if (i < 8)
        setVoxel(0, sOut, i);
      //setVoxel(7,7-sOut,i);
      else
        setVoxel(i-8, sOut, 7);
      //setVoxel(i,sOut,0);
    }
    rad += inc;
  }
}
void fillSheet(int sheet) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      cube[i][j] = 1 << sheet;
    }
  }
}
void suspendHelper() {
  if (frame == 0 && initSend) {
    fillSheet(0);
    initSend = false;
  }
  if (frame == 448) {
    if (sendsw)fillSheet(7);
    else fillSheet(0);
    sendsw = !sendsw;
    frame = 0;
    cubeDelay(100);
  }
  suspend(sendsw);//64*7=448 is total steps to move
}
void sendHelper() {
  if (initSend) {
    fillSheet(0);
    initSend = false;
  }
  if (sendFrame == 448) {
    if (sendsw)fillSheet(7);
    else fillSheet(0);
    sendsw = !sendsw;
    sendFrame = 0;
    cubeDelay(100);
  }
  sendv(sendsw);//64*7=448 is total steps to move
}
int sqstrt = true;
void squareMove() {
  if (timer > squareMoveTime) {
    timer = 0;
    if (sqstrt) {
      sqstrt = false;
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          cube[i][j] = 1;
        }
      }
    }
  }
}
int sines[4];
void squid() {
  if (timer > sinTime) {
    clearCube();
    timer = 0;
    for (int i = 0; i < 4; i++) {
      int sPos = 3.4 * sin(rad + inc * i) + 4;
      int sOut = round(sPos);
      sines[i] = sOut;
    }
    for (int i = 0; i < 4; i++) {
      setVoxel(3, sines[i], i);
      setVoxel(3, sines[3 - i], i + 4);
      setVoxel(4, sines[i], i);
      setVoxel(4, sines[3 - i], i + 4);

      setVoxel(i, sines[i], 3);
      setVoxel(i, sines[i], 4);
      setVoxel(i + 4, sines[3 - i], 3);
      setVoxel(i + 4, sines[3 - i], 4);
    }
    rad += inc;
  }
}
int susx = 0, susy = 0;
//void suspend(){
//  for(int i = 0; i<8; i++){
//    for(int j = 0; j<8; j++){
//      if(randbool2){
//        while(cube[susy][susx]==128){
//          susx = random(0,8);
//          susy = random(0,8);
//        }
//      }
//      else{
//        while(cube[susy][susx]==1){
//          susx = random(0,8);
//          susy = random(0,8);
//        }
//      }
//      if(randbool2){
//        for(int k=0; k<susShifts[susy][susx]; k++){
//          cube[susy][susx]=cube[susy][susx]<<1;
//          cubeDelay(100);
//        }
//      }
//      else if(!randbool2){
//        for(int k=0; k<8-susShifts[susy][susx]; k++){
//          cube[susy][susx]=cube[susy][susx]>>1;
//          cubeDelay(100);
//        }
//      }
//    }
//  }
//}
void suspend(boolean randbool2) {
  if (timer > suspendTime) {
    timer = 0;
    if (randbool2) {
      while (cube[susy][susx] == 128) {
        susx = random(0, 8);
        susy = random(0, 8);
      }
    }
    else {
      while (cube[susy][susx] == 1) {
        susx = random(0, 8);
        susy = random(0, 8);
      }
    }
    if (randbool2 && cube[susy][susx] < susShifts[susy][susx])cube[susy][susx] = cube[susy][susx] << 1;
    else if (!randbool2 && cube[susy][susx] > susShifts[susy][susx])cube[susy][susx] = cube[susy][susx] >> 1;
  }
}
int sendy = 0, sendx = 0;
void sendv(boolean randbool) {
  if (timer > sendTime) {
    timer = 0;
    if (randbool) {
      while (cube[sendy][sendx] == 128) {
        sendx = random(0, 8);
        sendy = random(0, 8);
      }
    }
    else {
      while (cube[sendy][sendx] == 1) {
        sendx = random(0, 8);
        sendy = random(0, 8);
      }
    }
    if (randbool)cube[sendy][sendx] = cube[sendy][sendx] << 1;
    else if (!randbool)cube[sendy][sendx] = cube[sendy][sendx] >> 1;

  }
}
boolean boxcont = false;
int lastdir = 0;
void boxmove(int sz) {
  if (timer > boxTime) {
    clearCube();
    timer = 0;
    boxcont = false;
    int dir;
    while (!boxcont) {
      dir = random(0, 6);
      if (bx == 0   && dir == NEG_X);
      else if (by == 0 && dir == NEG_Y);
      else if (bz == 0 && dir == NEG_Z);
      else if (bx + sz > 7 && dir == POS_X);
      else if (by + sz > 7 && dir == POS_Y);
      else if (bz + sz > 7 && dir == POS_Z);
      else if (dir == lastdir + 1 | dir == lastdir - 1);
      else {
        boxcont = true;
        lastdir = dir;
      }
    }
    switch (dir) {
      case NEG_X: bx--; break;
      case POS_X: bx++; break;
      case NEG_Y: by--; break;
      case POS_Y: by++; break;
      case NEG_Z: bz--; break;
      case POS_Z: bz++;
    }
    //}
    box(bx, by, bz, sz);
  }
}
void setVoxel(int x, int y, int z) {
  cube[7 - y][7 - z] |= 1 << x;
}
//void rmVoxel(int x, int y, int z){
//  cube[7-y][7-z]-=1<<x;
//}
void sphere() {
  for (int i = 2; i <= 5; i++) {
    setVoxel(2, 2, i);
    setVoxel(2, 5, i);
    setVoxel(2, i, 2);
    setVoxel(2, i, 5);
  }
  setVoxel(1, 3, 3);
  setVoxel(1, 4, 3);
  setVoxel(1, 3, 4);
  setVoxel(1, 4, 4);
  setVoxel(2, 4, 4);
}
void box(int x, int y, int z, int sz) {
  if (x + sz <= 8 && y + sz <= 8 && z + sz <= 8) {
    for (int j = 0; j < sz; j++) {
      for (int i = 0; i < sz; i++) {
        setVoxel(x + i, j + y, z);
        setVoxel(x, i + y, j + z);
        setVoxel(j + x, y, i + z);
        setVoxel(x + i, j + y, z + sz - 1);
        setVoxel(x + sz - 1, i + y, j + z);
        setVoxel(j + x, y + sz - 1, i + z);
      }
    }
  }
}
void sineBounce() {
  specialSinewave(whichsine);
  if (sineBounceFrame > 24) {
    rad = 3 * PI / 2;
    whichsine += 2;
    sineBounceFrame = 0;
    if (whichsine > 8)whichsine = 1;
  }
}
float sPos = 0, cPos = 0, tPos = 0, sPos2;
int flushCount = 0, curdir = 0, maxsin = 1;
void sinewave(int depth, int dir) {
  //set one side of cube as value of sin, then shift
  if (timer > sinTime) {
    timer = 0;
    sPos = 3 * sin(rad) + 4;
    sPos2 = 3 * sin(rad + PI) + 4;
    cPos = 3 * cos(rad) + 4;
    tPos = 3 * tan(rad) + 4;
    int sOut = round(sPos);
    int sOut2 = round(sPos2);
    int cOut = round(cPos);
    int tOut = round(tPos);
    //if(out>7)out=7;
    //cube[tOut][0]=1;
    if (dir == 1) {
      curdir = NEG_Z;
      shift(NEG_Z);
      cube[sOut][0] = depth;
    }
    else if (dir == 2) {
      curdir = NEG_Z;
      shift(NEG_Z);
      for (int i = 0; i < 8; i++) {
        sOut = round(3 * sin(rad + i * inc) + 4);
        cube[sOut][0] += 1 << i;
      }
    }
    else if (dir == 3) {
      curdir = NEG_X;
      shift(NEG_X);
      if (sOut > 7)sOut = 7;
      if (sOut < 1)sOut = 1;
      for (int i = 0; i < 8; i++) {
        cube[sOut][i] += 128;
      }
    }
    else if (dir == 4) {
      curdir = POS_Z;
      shift(POS_Z);
      for (int i = 0; i < 8; i++) {
        sOut = round(3 * sin(rad + i * inc) + 4);
        cube[sOut][7] += 1 << i;
      }
    }
    else if (dir == 5) {
      curdir = POS_Z;
      shift(POS_Z);
      cube[sOut][7] = depth;
    }
    else if (dir == 6) {
      curdir = POS_Z;
      shift(POS_X);
      //shift(POS_Z);
      for (int i = 0; i < 8; i++) {
        sOut = round(3 * sin(rad + i * inc) + 4);
        cube[sOut][i] += 1;
      }
      if (maxsin < 8)maxsin++;
    }
    else if (dir == 7) {
      curdir = POS_X;
      shift(POS_X);
      for (int i = 0; i < 8; i++) {
        cube[sOut][i] += 1;
      }
    }
    else if (dir == 8) {
      shift(NEG_Z);
      for (int i = 0; i < 8; i++) {
        sOut = round(3 * sin(rad + i * inc) + 4);
        cube[sOut][0] += 128 >> i;
      }
    }
    //cube[cOut][0]=128;
    //cube[sOut2][0]+=240;

    rad += inc;
    if (rad > 2 * PI - 0.001 && rad < 2 * PI + 0.001)rad = 0;
    //if(rad>4.71&&rad<4.72)done=true;
  }
}
void flushCube(int dir) {
  if (dir == NEG_Z) {
    shift(dir);
    cube[7][0] = 255;
  }
  else if (dir == POS_Z) {
    shift(dir);
    cube[7][7] = 255;
  }
  else if (dir == NEG_X) {
    shift(dir);
    for (int i = 0; i < 8; i++) {
      cube[7][i] += 128;
    }
  }
  else if (dir == POS_X) {
    shift(POS_X);
    for (int i = 0; i < 8; i++) {
      cube[7][i] += 1;
    }
  }
}
void specialSinewave(int dir) {
  //set one side of cube as value of sin, then shift
  if (timer > sinTime) {
    timer = 0;
    if (!done) {
      sPos = -3 * sin(rad) + 4;
      sPos2 = -3 * sin(rad + PI) + 4;
      cPos = -3 * cos(rad) + 4;
      tPos = -3 * tan(rad) + 4;
      int sOut = round(sPos);
      int sOut2 = round(sPos2);
      int cOut = round(cPos);
      int tOut = round(tPos);
      //if(out>7)out=7;
      //cube[tOut][0]=1;
      if (dir == 1) {
        curdir = NEG_Z;
        shift(NEG_Z);
        cube[sOut][0] = 255;
      }
      else if (dir == 2) {
        curdir = NEG_Z;
        shift(NEG_Z);
        for (int i = 0; i < 8; i++) {
          sOut = round(-3 * sin(rad + i * inc) + 4);
          cube[sOut][0] += 1 << i;
        }
      }
      else if (dir == 3) {
        curdir = NEG_X;
        shift(NEG_X);
        if (sOut > 7)sOut = 7;
        if (sOut < 1)sOut = 1;
        for (int i = 0; i < 8; i++) {
          cube[sOut][i] += 128;
        }
      }
      else if (dir == 4) {
        curdir = POS_Z;
        shift(POS_Z);
        for (int i = 0; i < 8; i++) {
          sOut = round(-3 * sin(rad + i * inc) + 4);
          cube[sOut][7] += 1 << i;
        }
      }
      else if (dir == 5) {
        curdir = POS_Z;
        shift(POS_Z);
        cube[sOut][7] = 255;
      }
      else if (dir == 6) {
        curdir = POS_Z;
        shift(POS_Z);
        for (int i = 0; i < 8; i++) {
          sOut = round(-3 * sin(rad + i * inc) + 4);
          cube[sOut][7] += 128 >> i;
        }
      }
      else if (dir == 7) {
        curdir = POS_X;
        shift(POS_X);
        for (int i = 0; i < 8; i++) {
          cube[sOut][i] += 1;
        }
      }
      else if (dir == 8) {
        shift(NEG_Z);
        for (int i = 0; i < 8; i++) {
          sOut = round(-3 * sin(rad + i * inc) + 4);
          cube[sOut][0] += 128 >> i;
        }
      }
      //cube[cOut][0]=128;
      //cube[sOut2][0]+=240;

      rad += inc;
      if (rad > 2 * PI - 0.001 && rad < 2 * PI + 0.001)rad = 0;
      if (rad > 4.71 && rad < 4.72)done = true;
    }
    else if (done) {
      flushCube(curdir);
      flushCount++;
      if (flushCount > 7) {
        done = false;
        flushCount = 0;
      }
    }
  }
}
int sqPos = 1, sqSw = 1, sqout = 1;
void squarewave(int depth) {
  if (timer > squareTime) {
    timer = 0;
    shift(NEG_Z);
    cube[sqout][0] = depth;
    if (sqSw == 1) {
      sqPos++;
      if (sqPos == 6) {
        sqSw = -1;
        for (int i = 1; i <= 6; i++) {
          cube[i][0] = depth;
        }
        sqout = 6;
      }
    }
    else if (sqSw == -1) {
      sqPos--;
      if (sqPos == 1) {
        sqSw = 1;
        for (int i = 1; i <= 6; i++) {
          cube[i][0] = depth;
        }
        sqout = 1;
      }
    }
  }
}
int sawpos = 1;
void sawwave(int depth) {
  if (timer > sawTime) {
    timer = 0;
    shift(NEG_Z);
    cube[sawpos][0] = depth;
    sawpos++;
    if (sawpos == 8) {
      for (int i = 0; i <= 7; i++) {
        cube[i][0] = depth;
      }
      sawpos = 1;
    }
  }
}

void sinewave2(int depth) {
  //set one side of cube as value of sin, then shift
  if (timer > sinTime) {
    timer = 0;
    shift(POS_X);
    sPos = -3 * sin(rad) + 4;
    sPos2 = -3 * sin(rad + PI) + 4;
    cPos = -3 * cos(rad) + 4;
    tPos = -3 * tan(rad) + 4;
    int sOut = round(sPos);
    int sOut2 = round(sPos2);
    int cOut = round(cPos);
    int tOut = round(tPos);
    for (int i = 0; i < 8; i++) {
      cube[sOut][i] += 1;
    }
    //cube[tOut][0]=1;
    //cube[sOut][0]=depth;
    //cube[cOut][0]=128;
    //cube[sOut2][0]+=240;

    rad += inc;
  }
}
void weirdsine() {
  //set one side of cube as value of sin, then shift
  if (timer > sinTime) {
    timer = 0;
    shift(NEG_Z);
    sPos = -3 * sin(rad) + 4;
    cPos = -3 * cos(rad) + 4;
    rad += inc;
    int sOut = round(sPos);
    int cOut = round(cPos);
    //if(out>7)out=7;
    cube[sOut][0] = 1;
    cube[0][0] += 1 << sOut;
    cube[7][0] += 1 << sOut;
    cube[sOut][0] += 128;
  }
}
int tpos = 0, sw = 1;
void triwave(int depth) {
  if (timer > triTime) {
    shift(NEG_Z);
    timer = 0;
    cube[tpos][0] = depth;
    if (sw == 1) {
      tpos++;
      if (tpos == 7)sw = 2;
    }
    else if (sw == 2) {
      tpos--;
      if (tpos == 0)sw = 1;
    }
  }
}
int a = 7, b = 0, c = 0, d = 0, exponent = 7;
//a is for layer/row, b is for setting/clearing
//c is for # of times, d is which effect.
void rise() {
  timer++;
  if (timer > RISE_TIME) {
    timer = 0;
    if (b == 0 && d == 0) { //fill bottom to top
      for (int i = 0; i < 8; i++) {
        cube[a][i] = 0xFF;
      }
      a--;
      if (a < 0) {
        b = 1;
        a = 7;
      }
    }
    if (b == 1 && d == 0) { //clear bottom to top
      for (int i = 0; i < 8; i++) {
        cube[a][i] = 0;
      }
      a--;
      if (a < 0) {
        b = 0;
        a = 7;
        c++;
      }
    }
  }
}
int CRAZY_TIME = 20;
void crazy() {
  timer++;
  if (timer > CRAZY_TIME) {
    timer = 0;
    if (CRAZY_TIME > 10) {
      CRAZY_TIME -= 2;
    }
    cube[random(0, 8)][random(0, 8)] = random(1, 0xFF);
  }
}
int rainTime = 50;
void rain() {
  if (timer > rainTime) {
    timer = 0;
    shift(NEG_Y);
    for (int i = 0; i < 8; i++) {
      cube[0][i] = random(0, 129);
    }

  }
}
int current = 1, COUNT_TIME = 75;
void count() {
  timer++;
  if (timer > COUNT_TIME) {
    timer = 0;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        cube[i][j] = current;
      }
    }
    current++;
    if (current > 255) {
      current = 0;

    }
  }
}
void clearCube() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      cube[i][j] = 0;
    }
  }
}
void render() {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SS, LOW);
    SPI.transfer(128 >> i);
    for (uint8_t j = 0; j < 8; j++) {
      SPI.transfer(cube[i][j]);
    }
    digitalWrite(SS, HIGH);
  }
}
//render takes 16591 SPI clocks
//baud rate is clocks/second
//16591/baud = time for render
void cubeDelay(double ms) {
  ms *= 10;
  double renderCycle = 18000 / baud; //time in seconds to render
  renderCycle *= 500; //render time in ms because reasons
  double times = ms / renderCycle;
  for (double i = 0; i < times; i++) {
    render();
  }
}
void shift(uint8_t dir) {
  if (dir == POS_X) {
    for (uint8_t y = 0; y < 8; y++) {
      for (uint8_t z = 0; z < 8; z++) {
        cube[y][z] = cube[y][z] << 1;
      }
    }
  } else if (dir == NEG_X) {
    for (uint8_t y = 0; y < 8; y++) {
      for (uint8_t z = 0; z < 8; z++) {
        cube[y][z] = cube[y][z] >> 1;
      }
    }
  } else if (dir == POS_Y) {
    for (uint8_t y = 1; y < 8; y++) {
      for (uint8_t z = 0; z < 8; z++) {
        cube[y - 1][z] = cube[y][z];
      }
    }
    for (uint8_t i = 0; i < 8; i++) {
      cube[7][i] = 0;
    }
  } else if (dir == NEG_Y) {
    for (uint8_t y = 7; y > 0; y--) {
      for (uint8_t z = 0; z < 8; z++) {
        cube[y][z] = cube[y - 1][z];
      }
    }
    for (uint8_t i = 0; i < 8; i++) {
      cube[0][i] = 0;
    }
  } else if (dir == POS_Z) {
    for (uint8_t y = 0; y < 8; y++) {
      for (uint8_t z = 1; z < 8; z++) {
        cube[y][z - 1] = cube[y][z];
      }
    }
    for (uint8_t i = 0; i < 8; i++) {
      cube[i][7] = 0;
    }
  } else if (dir == NEG_Z) {
    for (uint8_t y = 0; y < 8; y++) {
      for (uint8_t z = 7; z > 0; z--) {
        cube[y][z] = cube[y][z - 1];
      }
    }
    for (uint8_t i = 0; i < 8; i++) {
      cube[i][0] = 0;
    }
  }
}
