#include "Bot.h"
#include "Game.h"
#include "Printer.h"

/*
* Description: Bot constructor
*/
Bot::Bot(Game& game) :
  Actor(game)
{
}

/*
* Description: Bot destructor
*/
Bot::~Bot()
{
}

/*
* Description: Bot choice
*/
void Bot::JoinGame()
{
  this->WaitForGameStart();

  for (size_t round = 0; round < _game._game_rounds; ++round) {
    this->WaitForGameStart();

    // simulate bot choice
    _choice = static_cast<Symbol>(std::rand() % 3);
    _cv_game->notify_one();
   
    // reveal
    this->WaitForReveal();
    Printer{} << "Bot: " << symbol_map[_choice] << "\n";

    // wait for game to reset choice
    // this->WaitForoundReset();
    this->WaitForRoundReset();
  }
  _cv_game->notify_one();
}