#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
  Player(Game& game);
  ~Player();

  void AskUserChoice(int round);
  void JoinGame();
};
