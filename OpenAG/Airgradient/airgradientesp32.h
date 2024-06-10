#ifndef AIRGRADIENTESP32_H
#define AIRGRADIENTESP32_H

#include <WString.h>
#include "../components/ESPWIFI.h"
#include "../components/SSD1306.h"
#include "../components/SGP30.h"
#include "../components/SHT4X.h"
#include "../components/S8Sensair.h"
#include "../WebInterface.h"
#include "../utils/WMLog.h"
#include "../base/BaseComponent.h"
#include "../base/WMBaseComponent.h"

class Airgradientesp32: public WMBaseComponent<Airgradientesp32> {
  friend class WMBaseComponent<Airgradientesp32>;
  public:
    Airgradientesp32();

  private:
    Airgradientesp32(const Airgradientesp32&) = delete;
    Airgradientesp32& operator=(const Airgradientesp32&) = delete;

  SSD1306 oled;
  SGP30 sgp30;
  SHT4X sht4x;
  S8Sensair s8sensair;
};

inline Airgradientesp32::Airgradientesp32() {
  // init components
  this->components = { 
    &oled,
    &s8sensair,
    &sht4x,
    &ESPWIFI::GetInstance(),
    &WebInterface::GetInstance(),
  };

  ESPWIFI::GetInstance().onFailedConnection = [this](String ipaddress) {
    //this->oled.DisplayMessage("Failed to connect \n"  + ipaddress);
    oled.pages.push_back([this, ipaddress](SSD1306& SSD1306) {
      SSD1306.Clear()
      .DrawIcon7x7(
        ICON_MOBILE_NETWORK_0,
        120,
        0
      )
      .DisplayMessage(ipaddress, true, 1, true, 0, 2)
      .DisplayMessage("hotspot: airgradient", true, 1, true, 0, 4)
      .Update();
    });
  };
  
  ESPWIFI::GetInstance().onSuccessfullConnection = [this](String ipaddress) {
    LOG_INFO("Connected to network");
    LOG_INFO(ipaddress);
    oled.pages.push_back([this, ipaddress](SSD1306& SSD1306) {
      SSD1306.Clear()
      .DrawIcon7x7(
        ICON_MOBILE_NETWORK_100,
        120,
        0
      )
      .DisplayMessage(ipaddress, true, 1, true, 0, 2)
      .Update();
    });
  };

  WebInterface::GetInstance().metricsHandler = [this]() -> String {
    String aaa = "";
    //aaa += this->sgp30.GetPrometheusData();
    aaa += this->sht4x.GetPrometheusData();
    aaa += this->s8sensair.GetPrometheusData();
    return aaa;
  };

  oled.pages.push_back(
    [this](SSD1306& SSD1306) {
      SSD1306.Clear()
      .DisplayMessage("Tvoc:" + this->sgp30.GetTVOC(), false, 1, false)
      .DisplayMessage("Co2:" + this->s8sensair.GetCO2(), false, 1, false, 64, 0)
      .DisplayMessage("T:" + this->sht4x.GetTemperature(), false, 1, false, 0, 2)
      .DisplayMessage("Hum:" + this->sht4x.GetHumidity() + "%" , false, 1, false, 64, 2)
      .Update();
  });
}


// inline void Airgradientesp32::setup() {
//   SSD1306::GetInstance().Setup();

//   ESPWIFI::GetInstance().Setup();
//   SGP30::GetInstance().Setup();
//   WebInterface::GetInstance().Setup();
//   SHT4X::GetInstance().Setup();
//   S8Sensair::GetInstance().Setup();
//   SSD1306::GetInstance().DisplayMessage(ESPWIFI::GetInstance().GetLocalIP().toString());
// }

// inline void Airgradientesp32::loop() {
//   SGP30::GetInstance().Loop();
//   SHT4X::GetInstance().Loop();
//   S8Sensair::GetInstance().Loop();
//   WebInterface::GetInstance().Loop();

//   DELAY(1000);
// }

#endif