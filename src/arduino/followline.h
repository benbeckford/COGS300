#ifndef FOLLOWLINE_H
#define FOLLOWLINE_H

#include "mode.h"

class FollowLine : public Mode {
  public:
  FollowLine();
  virtual void loop() override;
  virtual void event(uint8_t * data, size_t len) override;
  private:
  bool running;
  int baseSpeed;
  int correction;
};

#endif
