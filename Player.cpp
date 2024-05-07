#include "Player.h"
#include "Game.h"
#include "Printer.h"

/*
* Description: Player constructor
*/
Player::Player(Game& game) :
  Actor(game)
{
}

/*
* Description: Player destructor
*/
Player::~Player()
{
} 

/*
* Description: Wait for game start signal
*/
void Player::AskUserChoice(int round)
{
  // get user input
  std::string user_input;
  Printer{} << "\n\n## Round " << round + 1 << 
    "##\n\nEnter your move [1 - Rock | 2 - Paper | 3 - Scissors]: ";
  std::getline(std::cin, user_input);
  int choice = std::stoi(user_input);
  _choice = static_cast<Symbol>(choice);
}

/*
* Description: Wait for game start signal
*/
void Player::JoinGame() 
{
  this->WaitForGameStart();

  for (size_t round = 0; round < _game._game_rounds; ++round) {
    this->WaitForGameStart();

    // get user input
    this->AskUserChoice(round);
    _cv_game->notify_one();

    // wait for reveal signal 
    this->WaitForReveal();
    Printer{} << "Player: " << symbol_map[_choice] << "\n";

    this->WaitForRoundReset();
  }
  _cv_game->notify_one();
}
