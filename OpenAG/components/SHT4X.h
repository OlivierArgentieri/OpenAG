#ifndef SHT3X_H
#define SHT3X_H

#include <Wire.h>
#include <WString.h>
#include "Adafruit_SHT4x.h"

#include "../base/BaseComponent.h"
#include "../base/IPrometheusable.h"
#include "../utils/WMLog.h"

class SHT4X: public BaseComponent, public IPrometheusable {
  public:
    void Setup() override;
    void Loop(const float dt) override;
    String GetPrometheusData() const override;
    SHT4X() = default;
    String GetTemperature() {
      return error ? "0" : String(temp.temperature);
    }
    
    String GetHumidity() {
      return error ? "0" : String(humidity.relative_humidity);
    }

    String UniqueName() const override { return "SHT4X"; }
  private:
    SHT4X(const SHT4X&) = delete;
    SHT4X& operator=(const SHT4X&) = delete;
    Adafruit_SHT4x sht4x;

    bool error = false;
    sensors_event_t humidity, temp;

};
inline void SHT4X::Setup() {
  if (!sht4x.begin()){
		LOG_ERROR("Cannot Find SHT4X");
    error = true;
    return;
  }


   // You can have 3 different precisions, higher precision takes longer
  sht4x.setPrecision(SHT4X_MED_PRECISION);
  // switch (sht4.getPrecision()) {
  //    case SHT4X_HIGH_PRECISION: 
  //      Serial.println("High precision");
  //      break;
  //    case SHT4X_MED_PRECISION: 
  //      Serial.println("Med precision");
  //      break;
  //    case SHT4X_LOW_PRECISION: 
  //      Serial.println("Low precision");
  //      break;
    // You can have 6 different heater settings
  // higher heat and longer times uses more power
  // and reads will take longer too!
  sht4x.setHeater(SHT4X_NO_HEATER);
  // switch (sht4.getHeater()) {
  //    case SHT4X_NO_HEATER: 
  //      Serial.println("No heater");
  //      break;
  //    case SHT4X_HIGH_HEATER_1S: 
  //      Serial.println("High heat for 1 second");
  //      break;
  //    case SHT4X_HIGH_HEATER_100MS: 
  //      Serial.println("High heat for 0.1 second");
  //      break;
  //    case SHT4X_MED_HEATER_1S: 
  //      Serial.println("Medium heat for 1 second");
  //      break;
  //    case SHT4X_MED_HEATER_100MS: 
  //      Serial.println("Medium heat for 0.1 second");
  //      break;
  //    case SHT4X_LOW_HEATER_1S: 
  //      Serial.println("Low heat for 1 second");
  //      break;
  //    case SHT4X_LOW_HEATER_100MS: 
  //      Serial.println("Low heat for 0.1 second");
  //      break;
  


}

inline void SHT4X::Loop(const float dt) {
  if (error) {
    return;
  }

  sht4x.getEvent(&humidity, &temp);
}

inline String SHT4X::GetPrometheusData() const {
  if (error) {
    LOG_ERROR("Error exist, aborting prometheus data generation for sht4x.");
    return "";
  }

  String prometheusData = this->GeneratePrometheusData("temperature", String(temp.temperature), "celcius");
  prometheusData += this->GeneratePrometheusData("humidity", String(humidity.relative_humidity), "percent");
  return prometheusData;
}
#endif
