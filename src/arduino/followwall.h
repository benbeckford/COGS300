#ifndef FOLLOWWALL_H
#define FOLLOWWALL_H

#include "mode.h"

class FollowWall : public Mode {
  public:
    FollowWall();
    virtual void loop() override;
    virtual void event(uint8_t * data, size_t len) override;
  private:
    bool running;
    float target;
};


#endif