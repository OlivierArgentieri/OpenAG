#ifndef WMEEPROM_H
#define WMEEPROM_H

#include <EEPROM.h>

class WMEEPROM {
  public:
    /**
     * Static Public Methods
     */

    /** Save object to EEPROM */
    template<typename T>
    static void Save(const T& object, int address)
    {
      // Initialize EEPROM
      EEPROM.begin(512);

      // Write object to EEPROM
      EEPROM.put(address, object);
      EEPROM.commit();

      // End EEPROM
      EEPROM.end();
    }

    /** Get object from EEPROM */
    template<typename T>
    static void Get(T& object, int address)
    {
      // Initialize EEPROM
      EEPROM.begin(512);
      
      // Read object from EEPROM
      EEPROM.get(address, object);

      // End EEPROM
      EEPROM.end();
    }

    /** Check corrupted data */
    template<typename T>
    static bool IsCorrupted(int address)
    {
      // Initialize EEPROM
      EEPROM.begin(512);

      // Read object from EEPROM
      T object;
      EEPROM.get(address, object);

      // End EEPROM
      EEPROM.end();

      // check if object is corrupted based on its size
      
      return sizeof(object) != sizeof(T);
    }

  private:
    /**
     * Private Methods
     */

    /** Clear EEPROM */
    static void Clear();
};

/** Inline */
inline static void Clear() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

#endif
