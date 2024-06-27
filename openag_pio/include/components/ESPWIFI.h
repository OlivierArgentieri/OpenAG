#ifndef WM_NETWORK_H
#define WM_NETWORK_H

#include <WString.h>
#include <vector>
#include <functional>
#include <WiFi.h>

#include "../utils/WMLog.h"
#include "../utils/WMEEPROM.h"
#include "../utils/WMUtils.h"
#include "../base/Singleton.h"
#include "../base/BaseComponent.h"
#include "../utils/WMHandlers.h"

#define SSIID "airgradient"

struct WNIPAddress {
  uint first;
  uint second;
  uint third;
  uint fourth;

  String toString() {
    return String(first) + "." + String(second) + "." + String(third) + "." + String(fourth);
  }
};

struct WMNetworkData {
  IPAddress localIP;
  IPAddress mask;
  IPAddress gateway;
  IPAddress firstDNS;
  IPAddress secondaryDNS;
  String ssid;
  String password;
  String rssi;
  String hostname;
  bool open;

  WMNetworkData() {
    localIP = IPAddress(0, 0, 0, 0);
    mask = IPAddress(0, 0, 0, 0);
    gateway = IPAddress(0, 0, 0, 0);
    firstDNS = IPAddress(0, 0, 0, 0);
    secondaryDNS = IPAddress(0, 0, 0, 0);
    ssid = "";
    rssi = "";
    password = "";
    hostname = "";
    open = false;
  }

  WMNetworkData(
    const String local_ip,
    const String mask,
    const String gateway,
    const String first_dns,
    const String second_dns,
    const String ssid,
    const String password,
    const String rssi,
    const String hostname,
    bool open
    ) {
    this->localIP.fromString(local_ip);
    this->mask.fromString(mask);
    this->gateway.fromString(gateway);
    this->firstDNS.fromString(first_dns);
    this->secondaryDNS.fromString(second_dns);
    this->ssid = ssid;
    this->password = password;
    this->rssi = rssi;
    this->hostname = hostname;
    this->open = open;
  }

  // is_static_configured
  bool isStaticConfigured() const {
    return localIP != IPAddress(0, 0, 0, 0) && mask != IPAddress(0, 0, 0, 0) && gateway != IPAddress(0, 0, 0, 0);
  }
};

class ESPWIFI: public BaseComponent, public Singleton<ESPWIFI> {
  friend class Singleton<ESPWIFI>;
  public:
  /**
   * constructor
  */
  ESPWIFI() = default;

public:
  /**
   * Overrided Methods
   */
  void Setup() override;
  void Loop(const float dt) override {};

public:
  /**
   * Public Methods
   */

  /** Scan network method */
  void Scan(bool force = false);

  /** Get list of available networks */
  void GetAvailableNetworks(std::vector<WMNetworkData>& outNetworks);

  /** tryToConnect */
  bool TryToConnect(const WMNetworkData& network);    

  /** IsWifiConnected ? */
  inline bool IsConnected() const { return WiFi.status() == WL_CONNECTED; };
  
  /** Get localIP*/
  inline IPAddress GetLocalIP() { if (IsConnected()) return WiFi.localIP(); else WiFi.softAPIP();};

  public:
  /**
   * Public Handlers
   */
  StringHandler onSuccessfullConnection = nullptr;
  StringHandler onFailedConnection = nullptr;
  
private:
  /**
   * Private Property 
   */
  std::vector<WMNetworkData> networksScanned;

};

/** inline */
inline void ESPWIFI::Setup() {
  WMNetworkData network_data;
  WMEEPROM::Get<WMNetworkData>(network_data, 0);

  LOG_INFO(network_data.password);
  LOG_INFO(network_data.ssid);
  LOG_INFO(network_data.localIP.toString());
  LOG_INFO(network_data.mask.toString());
  LOG_INFO(network_data.gateway.toString());

  if(this->TryToConnect(network_data)) {
      if (onSuccessfullConnection)
        onSuccessfullConnection(WiFi.localIP().toString());
      return;
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(SSIID, "");
  this->Scan();
  
  if (onFailedConnection)
    onFailedConnection(WiFi.softAPIP().toString());

}

inline void ESPWIFI::Scan(bool force) {
  if (!force && networksScanned.size() > 0)
    return;

  networksScanned.clear();

  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    WMNetworkData network;
    network.ssid = WiFi.SSID(i);
    network.rssi = WiFi.RSSI(i);
    network.open = WiFi.encryptionType(i) == WIFI_AUTH_OPEN;
    networksScanned.push_back(network);
  }
}

inline void ESPWIFI::GetAvailableNetworks(std::vector<WMNetworkData>& outNetworks)  {
  outNetworks = networksScanned;
}

inline bool ESPWIFI::TryToConnect(const WMNetworkData& network) {
  if(network.isStaticConfigured()) {
    if (!WiFi.config(network.localIP, network.gateway, network.mask)){
      LOG_ERROR("Failed to configure static IP");
      return false;
    }
  }
  WiFi.mode(WIFI_STA);

  //WiFi.setHostname(network.hostname.c_str());
  WiFi.begin(network.ssid, network.password);

  int timeout = 20;
  while (!IsConnected() && timeout > 0) {
    timeout--;
    DELAY(1000);
    LOG_ERROR("Failed to connect to network, retrying...");
  }

  return IsConnected();
}
#endif
