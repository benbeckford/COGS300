#ifndef MODE_H
#define MODE_H

#include <stddef.h>
#include <stdint.h>

class Mode {
  public:
    virtual void loop() {};
    virtual void event(uint8_t * data, size_t len) {};
};

#endif