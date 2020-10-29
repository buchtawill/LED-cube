/*
    Copyright 2020 Will Buchta

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   Render function taken from Harry Le
   https://github.com/itsharryle/LED_CUBE/tree/master/CUBE
*/
#include <SPI.h>
int cube[8][8];
int mitarray[8][32] {
  {0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1,  0, 1, 1,  0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1,  0, 1, 1,  0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1,  0, 1, 1,  0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1,  0, 1, 1,  0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1,  0, 1, 1,  0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1,  0, 0, 0,  0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1,  0, 0, 0,  0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
double baud = 200000;
void setup() {
  randomSeed(analogRead(A0));
  SPI.begin();
  SPI.beginTransaction(SPISettings(baud, MSBFIRST, SPI_MODE0));
}

int timer = 0, mitTime = 30;
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
//<>////////////////////////////////////////////////////////////////////////////////////////<>
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
void loop() {
  mit();
  render();
  timer++;
}
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
//<>////////////////////////////////////////////////////////////////////////////////////////<>
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
int mover = 0;
void mit() {
  if (timer > mitTime) {
    clearCube();
    timer = 0;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        cube[j][7 - i] = 3 * mitarray[j][mover + i];  //not the best way to do this, memory is wasted in the mitarray.
      }                                               //that being said, memory use is not a constraint in this scope
    }
    mover++;
    if (mover == 25)mover = 0;
  }
}
void clearCube() {              //This function could be cut down to one step by using SRCLR. I will need to make changes
  for (int i = 0; i < 8; i++) { //to the board in order to do this
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
