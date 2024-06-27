#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#include <WString.h>
#include "./base/BaseComponent.h"
#include "./base/Singleton.h"
#include "./utils/WMHandlers.h"

class AsyncWebServer;


class WebInterface: public BaseComponent, public Singleton<WebInterface> {
  friend class Singleton<WebInterface>;
  enum HandlerAction : int
  {
    NONE = 0,
    LISTNETWORKS = 1,
    CONNECTREQUEST = 2,
  };
  public:
    /**
     * Public Methods
     */

    /** Server Setup */
    void Setup()override;
    void Loop(const float dt)override;
    /** Handle Route Behaviour */

  private:
    /**
     * Private Methods
     */
    
    /** Construct web interface */
    void CreateWebUI(String& outContent);

    /** Create routes */
    void CreateRoutes();
  
  public:
    /**
     * Public Handlers
     */
    VoidHandler resetHandler = nullptr;
    VoidHandler rescanHandler = nullptr;
    RtStringHandler metricsHandler = nullptr;
  private:
    /**
    * private property 
    */
    static inline WebInterface* instance = nullptr;
    static inline AsyncWebServer* server = nullptr;
    HandlerAction currentStatuts = HandlerAction::NONE;
};
#endif
