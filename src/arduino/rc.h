#ifndef RC_H
#define RC_H

#include "mode.h"

class RemoteControl : public Mode {
  public:
    RemoteControl();
    void event(uint8_t * data, size_t len) override;
};

#endif