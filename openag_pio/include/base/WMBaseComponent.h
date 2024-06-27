#ifndef WM_BASECOMPONENT_H
#define WM_BASECOMPONENT_H

#include <vector>

#include "Singleton.h"
#include "../utils/WMHandlers.h"

class BaseComponent;

template<typename T>
class WMBaseComponent : public Singleton<T> {
  public:
  /**
   * Public methods
  */
  virtual void Setup();
  virtual void Loop();

  protected:
    /** Component array */
    std::vector<BaseComponent*> components = {};

    /** Handlers */
    VoidHandler afterSetupHandler = nullptr;
    VoidHandler beforeSetupHandler = nullptr;
    VoidHandler beforeUpdateHandler = nullptr;
    VoidHandler afterUpdateHandler = nullptr;

  private:
  /**
   * Private properties
  */
  
  /** time */
  float deltaTime = 0.0f;

  /** current time */
  unsigned long currentTime = 0;
};

template<typename T>
inline void WMBaseComponent<T>::Setup() {
  if (beforeSetupHandler) {
    beforeSetupHandler();
  }
  for (int i = 0; i < components.size(); i++) {
    components[i]->Setup();
  }
  if (afterSetupHandler)
    afterSetupHandler();

}

template<typename T>
inline void WMBaseComponent<T>::Loop() {
  // calculate delta time
  unsigned long newTime = millis();
  deltaTime = (newTime - currentTime);

  // update current time

  if (beforeUpdateHandler) {
    beforeUpdateHandler();
  }

  for (int i = 0; i < components.size(); i++) {
    components[i]->Loop(deltaTime);
  }
  if (afterUpdateHandler) {
    beforeUpdateHandler();
  }

  currentTime = newTime;
}

#endif
