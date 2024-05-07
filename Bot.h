#pragma once
#include "Actor.h"

class Bot : public Actor
{
  public:
  Bot(Game& game);
  ~Bot();

  void JoinGame();
};
