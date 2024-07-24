#ifndef SGP30_H
#define SGP30_H

#include <Adafruit_SGP30.h>
#include "../utils/WMLog.h"
#include "../utils/WMUtils.h"
#include "../base/BaseComponent.h"
#include "../base/IPrometheusable.h"
#include "../utils/WMHandlers.h"

Adafruit_SGP30 sgp;

struct SGP30Data {
  uint16_t TVOC;
  uint16_t eCO2;

  SGP30Data() {
    TVOC = 0;
    eCO2 = 0;
  }
};

class SGP30 final: public BaseComponent, public IPrometheusable {
public:
  /**
   * Overrided Methods
   */
  void Setup() override;
  void Loop(const float dt) override;
  String GetPrometheusData() const override;

  public:
  /**
   * Public Methods
   */
  String GetTVOC() const {
    return error ? "0" : String(this->latest_data.TVOC);
  }

  String GetCO2() const {
    return error ? "0": String(this->latest_data.eCO2);
  }

  RtDoubleHandler onRequestUpdateHumidity = nullptr;
public:
  /**
   * constructor
   */
  SGP30() = default;

private:
  /**
   * private field
   */
  SGP30Data latest_data;
  bool error = false;

    /** refresh humidity delay */
  unsigned long refreshDelay = 60000;

  /** refresh humidity counter */
  unsigned long refreshCounter = 0;

private:
  /**
   * overrided methods
  */
  String UniqueName() const override { return "SGP30"; }

  static uint16_t doubleToFixedPoint( double number) {
    int power = 1 << 8;
    double number2 = number * power;
    uint16_t value = floor(number2 + 0.5);
    return value;
  }
};

/** Inline */
inline void SGP30::Setup() {
  if (!sgp.begin()) {
    this->error = true;
    LOG_ERROR("Sensor not found");
    return;
  }

  LOG_INFO("Found SGP30 serial #");
}

inline void SGP30::Loop(const float dt) {
  refreshCounter += dt;

  if (error || !sgp.IAQmeasure()) {
    return;
  }

  sgp.IAQmeasureRaw();
  this->latest_data.TVOC = sgp.TVOC;
  this->latest_data.eCO2 = sgp.eCO2;

  if (refreshCounter > refreshDelay) {
    refreshCounter = 0;
    if (!onRequestUpdateHumidity) return;
    LOG_INFO("Updating humidity");

    sgp.setHumidity(doubleToFixedPoint(onRequestUpdateHumidity()));
  }
}

inline String SGP30::GetPrometheusData() const {
  String values = this->GeneratePrometheusData(
    "TVOC",
    String(this->latest_data.TVOC),
    "ppb"
  );

  values += this->GeneratePrometheusData(
    "eCO2",
    String(this->latest_data.eCO2),
    "ppm"
  );
  return values;
}


#endif
