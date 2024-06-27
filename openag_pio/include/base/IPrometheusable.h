#include <WString.h>

#ifndef PROMETHEUSABLE_H
#define PROMETHEUSABLE_H

class IPrometheusable {
  public:
    virtual String GetPrometheusData() const = 0;
    virtual String UniqueName() const = 0;
    
    virtual String GeneratePrometheusData(const String& metricName, const String& value, const String& unit) const {
      String prometheusData = "";
      String uniqueName = UniqueName() + "_" + metricName;
      prometheusData += "# HELP " + uniqueName + " " + uniqueName + "\n";
      prometheusData += "# TYPE " + uniqueName + " gauge\n";
      prometheusData += uniqueName + "{id=\"" + uniqueName + "\"" + " unit=\"" + unit + "\"} " + value + "\n";
      return prometheusData;
    }
};

#endif
