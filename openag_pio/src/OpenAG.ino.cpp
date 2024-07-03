# 1 "C:\\Users\\Olivier\\AppData\\Local\\Temp\\tmpf630ltkr"
#include <Arduino.h>
# 1 "C:/Users/Olivier/Documents/PullGithub/OpenAG/openag_pio/src/OpenAG.ino"
#include "utils/WMUtils.h"
#include "Airgradient/airgradientesp32.h"
#include "utils/WMLog.h"
int main();
void setup();
void loop();
#line 6 "C:/Users/Olivier/Documents/PullGithub/OpenAG/openag_pio/src/OpenAG.ino"
int main(){
  setup();
  while(true){
    loop();
  }
  return 0;
}

Airgradientesp32* airgradientesp32 = nullptr;
void setup() {
  Serial.begin(115200);

  DELAY(1000);

  airgradientesp32 = new Airgradientesp32();
  airgradientesp32->Setup();
}

void loop() {
  airgradientesp32->Loop();
}