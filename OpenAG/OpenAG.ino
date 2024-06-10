#include "utils/WMUtils.h"
#include "Airgradient/airgradientesp32.h"
#include "utils/WMLog.h"
// Define custom macros to avoid using delay function

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
