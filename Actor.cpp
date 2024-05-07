#include "Actor.h"
#include "Game.h"

Actor::Actor(Game& game) : 
  _game(game), _cv_game(nullptr)
{
}

Actor::~Actor()
{
}

/*
* Description: Join the game
* @param: none
*/
void Actor::JoinGame()
{
}

/*
* Description: Set the game condition variable
* @param: cv - the condition variable pointer
*/
void Actor::SetGameConditionVariable(std::condition_variable* cv)
{
  _cv_game = cv;
}

/*
* Description: get condition variable reference of the actor
* @param: none
*/
std::condition_variable& Actor::GetConditionVariable()
{
  return _cv;
}

/*
* Description: wait for game start
* @param: none
*/
bool Actor::WaitForGameStart()
{
  std::unique_lock<std::mutex> lck(_mtx);
  _cv.wait(lck, [&]() { return _game._start; });
  return _game._start;
}

/*
* Description: Wait for reveal
* @param: none
*/
bool Actor::WaitForReveal()
{
  std::unique_lock<std::mutex> lck(_mtx);
  _cv.wait(lck, [&]() { return _game._reveal; });
  return _game._reveal;
}

/*
* Description: Wait for round reset
* @param: none
*/
bool Actor::WaitForRoundReset()
{
  std::unique_lock<std::mutex>lock(_mtx);
  // notify game the hand was shown
  _hand = true;
  _cv_game->notify_one();

  // wait for game to reset hands
  _cv.wait(lock, [this] { return _choice == Symbol(); });

  // ack reset
  _hand = false;
  _cv_game->notify_one();

  return true;
}
