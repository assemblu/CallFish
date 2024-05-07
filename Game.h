#include <condition_variable>
#include <thread>
#include "Symbol.h"

class Player;
class Bot;

#pragma once
class Game
{
public:
  Game();
  ~Game();

  /*
  * Description: Asks the user for the number of rounds
  * Arguments: None
  */
  bool AskRounds();

  /*
  * Description: Starts the game
  * Arguments: None
  */
  void Play();

  /*
  * Description: Returns the condition variable for the game
  */
  std::condition_variable& GetConditionVariable();

  void SetPlayer(Player* player);
  void SetPlayerConditionVariable(std::condition_variable* cv);
  bool VerifyPlayer();

  void SetBot(Bot* bot);
  void SetBotConditionVariable(std::condition_variable* cv);

  void SetGameRounds(int rounds);
  int GetGameRounds();
  int GetRound();
  void SetRound(int round);

  bool SendStartSignal();
  bool WaitForPlayerChoice();
  bool SendRevealSignal();
  bool CheckResults();
  Result DetermineWinner(Symbol player, Symbol bot);
  bool PrintWinner(Result winner);
  bool ResetRound();

public:
  bool _start, _reveal;
  int _game_rounds;
  int _round;

private:
  std::mutex _mtx;
  std::condition_variable _cv;
  std::condition_variable* _cv_player;
  std::condition_variable* _cv_bot;
  Player* _player;
  Bot* _bot;

};
