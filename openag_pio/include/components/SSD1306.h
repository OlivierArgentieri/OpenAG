#ifndef SSD1306_H
#define SSD1306_H

#include <WString.h>
#include <GyverOLED.h>
#include <functional>
#include <vector>

#include "../base/BaseComponent.h"
#include "../utils/WMLog.h"
#include "../utils/WMUtils.h"
#include "../utils/WMHandlers.h"

#define OLED_TYPE_SSD1306_128x32 GyverOLED<SSD1306_128x32, OLED_BUFFER>
#define OLED_TYPE_SSD1306_128x32_NOBUFFER GyverOLED<SSD1306_128x32, OLED_NO_BUFFER>
#define OLED_TYPE_SSD1306_128x64 GyverOLED<SSD1306_128x64, OLED_BUFFER>
#define OLED_TYPE_SSD1306_128x64_NOBUFFER GyverOLED<SSD1306_128x64, OLED_NO_BUFFER>
#define OLED_TYPE_SSD1306_128x64_SPI GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, 8, 7, 6>
#define OLED_TYPE_SSH1106_128x64 GyverOLED<SSH1106_128x64> oled;
#define OLED_TYPE OLED_TYPE_SSD1306_128x32

#define ICON_MOBILE_NETWORK_100 0
#define ICON_MOBILE_NETWORK_75 1
#define ICON_MOBILE_NETWORK_50 2
#define ICON_MOBILE_NETWORK_25 3
#define ICON_MOBILE_NETWORK_0 4
const static uint8_t icons_7x7[][7] PROGMEM = {
  {0x60, 0x00, 0x70, 0x00, 0x7c, 0x00, 0x7f}, //51 mobile network 100%
  {0x60, 0x00, 0x70, 0x00, 0x7c, 0x00, 0x40}, //52 mobile network 75%
  {0x60, 0x00, 0x70, 0x00, 0x40, 0x00, 0x40}, //53 mobile network 50%
  {0x60, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40}, //54 mobile network 25%
  {0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40}, //55 mobile network 0%
};


class SSD1306 final: public BaseComponent {
  typedef std::function<void(SSD1306&)> PageHandler;

public:
  /**
   * Overrided Methods
   */
  void Setup() override;
  void Loop(const float dt) override;

public:
  /**
   * Public Methods
   */

  SSD1306();

  public:
  /**
   * private methods
   */

  SSD1306& DisplayMessage(
    const String& message,
    bool autoPrintln = true,
    int scale = 1,
    bool center = false,
    int x = 0,
    int y = 0
  );
  SSD1306& DrawIcon7x7(byte index, int x, int y);
  SSD1306& Update();
  SSD1306& Clear();

private:
  /**
   * private property 
   */ 
  OLED_TYPE oled;

  /** refresh delay */
  unsigned long refreshDelay = 5000;

  /** refresh counter */
  unsigned long refreshCounter = 0;

  /** current page */
  int currentPage = 0;

  public:
    /**
     * Public properties
    */
  /** page array */
  std::vector<PageHandler> pages = {};
};

inline SSD1306::SSD1306() {
  // #if OLED_TYPE != OLED_TYPE_SSD1306_128x32
  //   pages = {}; // clear pages if not 128x32
  //   return
  // #endif

  /* 
  * First page is for the main screen
  * Second page is for the settings screen with ip address
  * Third page is for metrics values
  */
  pages = {};
}

/** Inline */
inline void SSD1306::Setup() {
  oled.init();
  oled.clear();
  oled.update();
  
 Clear()
 .DisplayMessage("Initializing...", true, 1, true)
 .DisplayMessage("OpenR", true, 1, true, 0, 2)
 .Update();
}

inline SSD1306& SSD1306::DisplayMessage(
  const String& message,
  bool autoPrintln,
  int scale,
  bool center,
  int x,
  int y
  ) {

    if (center) {
      oled.setCursor(64 - (message.length() * 6 * scale) / 2, y);
    } else {
      oled.setCursor(x, y);
    }
   // Show initial text
  oled.autoPrintln(autoPrintln);
  oled.setScale(scale);
  oled.print(message);
  return *this;
}

inline SSD1306& SSD1306::DrawIcon7x7(byte index, int x, int y) {
  oled.setCursor(x, y);
  size_t s = sizeof icons_7x7[index];
  for(unsigned int i = 0; i < s; i++) {
    oled.drawByte(pgm_read_byte(&(icons_7x7[index][i])));
  }
  return *this;
}

inline SSD1306& SSD1306::Update() {
  oled.update();
  return *this;
}

inline SSD1306& SSD1306::Clear() {
  oled.clear();
  return *this;
}

inline void SSD1306::Loop(const float dt) {  
  refreshCounter += dt;
  if (refreshCounter < refreshDelay) {
    return;
  }

  refreshCounter = 0;
  currentPage = (currentPage + 1) % pages.size();
  const PageHandler& page = pages[currentPage];
  page(*this);
}
#endif
