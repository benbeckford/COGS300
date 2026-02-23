#ifndef CALIBRATEFORWARDS_H
#define CALIBRATEFORWARDS_H

#include "mode.h"

class CalibrateForwards : public Mode {
  public:
    CalibrateForwards();
    virtual void event(uint8_t * data, size_t len) override;
};

#endif