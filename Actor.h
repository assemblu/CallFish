#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Symbol.h"

class Game;

class Actor
{
public:
  std::atomic<Symbol> _choice;
  std::atomic<bool> _hand;

public:
  Actor(Game& game);
  ~Actor();

  void JoinGame();
  void SetGameConditionVariable(std::condition_variable* cv);
  std::condition_variable& GetConditionVariable();
  bool WaitForGameStart();
  bool WaitForReveal();
  bool WaitForRoundReset();

protected:
  Game& _game;
  std::mutex _mtx;
  std::condition_variable _cv;
  std::condition_variable* _cv_game;

};

