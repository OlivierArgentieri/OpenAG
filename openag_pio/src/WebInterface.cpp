#include "WebInterface.h"
#include <WebServer.h>
#include <ESPAsyncWebServer.h>
#include <vector>
#include "utils/WMEEPROM.h"
#include "utils/WMLog.h"
#include "components/ESPWIFI.h"
#include "SPIFFS.h"
#include <stdlib.h> 

/**
 * Private Methods
 */

String WebInterface::ProcessTemplate(const String& templateContent)
{
  if (templateContent == "WIFI_CONTAINER") {
    std::vector<WMNetworkData> availableNetworks;
    ESPWIFI::GetInstance().GetAvailableNetworks(availableNetworks);
    String content = "";
    for (const WMNetworkData& network: availableNetworks ) {
      content += "<div class='card shadow'>";

      content += "<div class='card-header'>";
      content += "<h2>" + (network.ssid ? network.ssid : "Unknown") + "</h2>";
      content += "</div>";
      
      content += "<div class='card-body'>";
      content += "<p>Signal Strength: " + String(((network.rssi.toInt() * -1))) + "\%</p>";
      content += "<p>Security: " + network.open ? "Open</p>" : "Closed</p>";
      content += "</div>";

      content += "<div class='card-action'>";
      
      String data = "{'ssid_field':'" + network.ssid + "', 'open_field': " + (network.open ? 'true' : 'false') + " }";

      content += "<button onclick=\"openForm(" + data + ")\">Connect</button>";
      content += "</div>";
      content += "</div>";
    }
    return content;
  }
  return "";
}


/**
* Public Methods
*/

void WebInterface::Setup()
{
  if(!SPIFFS.begin(true)){
    return;
  }

  // Scan once
  ESPWIFI::GetInstance().Scan();

  // Serve
  server = new AsyncWebServer(80);
  {
    server->on("/", HTTP_GET, [&](AsyncWebServerRequest *request) 
    {
      request->send(SPIFFS, "/index.html", String(), false, [this](const String& var){
        return this->ProcessTemplate(var);
      });
    });

    server->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/style.css", "text/css");
    });
    
    server->on("/rescan", HTTP_GET, [&](AsyncWebServerRequest *request) 
    {
      if (rescanHandler)
        rescanHandler();
      ESPWIFI::GetInstance().Scan(true);
      request->redirect("/");
    });

    server->on("/reset", HTTP_GET, [&](AsyncWebServerRequest *request) 
    {
      if (resetHandler)
        resetHandler();

      LOG_WARNING("EEPROM is corrupted, saved default network data");
      WMNetworkData network_data;
      WMEEPROM::Save<WMNetworkData>(network_data, 0);
    });

    server->on("/connect", HTTP_POST, [&](AsyncWebServerRequest *request) 
    {
      WMNetworkData network_data{
        request->getParam(2)->value(), // ip
        request->getParam(3)->value(), // mask
        request->getParam(4)->value(), // gateway
        "", // first_dns
        "", // second_dns
        request->getParam(0)->value(), // ssid
        request->getParam(1)->value(), // password
        "", // rssi
        request->getParam(5)->value(), // hostname
        false // open
      };


      // Save data
      WMEEPROM::Save<WMNetworkData>(network_data, 0);
      request->send(200, "text/html", "Password saved, please reboot...");
    });
    
    server->on("/metrics", HTTP_GET, [&](AsyncWebServerRequest *request)
    {
      if (metricsHandler)
      {
        String content = metricsHandler();
        request->send(200, "text/plain", content);
      }
      // TODO
    });
  }


  server->begin(); 
}

/**
* Private Methods
*/

void WebInterface::CreateWebUI(String& outContent)
{
  outContent = "<!DOCTYPE HTML>\r\n<html>Wifi Credentials Update page";
  outContent += "<div>";
  outContent += "<a href='/rescan'>";
  outContent += "<button>Refresh</button>";
  outContent += "</a>";
  outContent += "</div>";

  outContent += "<table>";
  outContent += "<tr>";
  outContent += "<th> SSID </th>";
  outContent += "<th> RSSI </th>";
  outContent += "<th> OPEN </th>";
  outContent += "<th> Action </th>";
  outContent += "</tr>";

  outContent += "</td>";
  
  
  // Get available networks
  std::vector<WMNetworkData> availableNetworks;
  ESPWIFI::GetInstance().GetAvailableNetworks(availableNetworks);

  // Display available networks
  for (const WMNetworkData& network: availableNetworks )
  {
    outContent += "<tr>";

    outContent += "<td>";
    outContent += network.ssid; 
    outContent += "</td>";
    
    outContent += "<td>";
    outContent += network.rssi;
    outContent += "</td>";

    outContent += "<td>";
    outContent += network.open ? "Yes" : "No";
    outContent += "</td>";
    
    outContent += "<td>";
    outContent += "<form action='/connect' method='post'>";
    
    outContent += "<input type='text' name='password' placeholder='password'>";
    outContent += "<input type='text' name='ssid' value='" + network.ssid + "'>";
    outContent += "<input type='text' name='ip' placeholder='ipaddr'>";
    outContent += "<input type='text' name='mask' placeholder='mask'>";
    outContent += "<input type='text' name='gateway' placeholder='gateway'>";
    outContent += "<input type='text' name='hostname' placeholder='hostname'>";
    outContent += "<input type='submit' value='Connect'>";
    outContent += "</form>";

    outContent += "</td>";
    outContent += "</tr>";
  }

  outContent += "</table>";


  // manual connect

  outContent += "<form action='/connect' method='post'>";
  outContent += "<input type='text' name='password' placeholder='password'>";
  outContent += "<input type='text' name='ssid' placeholder='ssid'>";
  outContent += "<input type='text' name='ip' placeholder='ipaddr'>";
  outContent += "<input type='text' name='mask' placeholder='mask'>";
  outContent += "<input type='text' name='gateway' placeholder='gateway'>";
  outContent += "<input type='text' name='hostname' placeholder='hostname'>";
  outContent += "<input type='submit' value='Connect'>";
  outContent += "</form>";

  outContent += "</html>";
}
