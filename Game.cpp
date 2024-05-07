#include "Game.h"
#include "Player.h"
#include "Bot.h"
#include "Printer.h"

/*
* Description: ChoiceKey struct, was working on it to hash the win/lose conditions
*/
struct ChoiceKey
{
  Symbol player_choice;
  Symbol bot_choice;

  ChoiceKey() = default;

  ChoiceKey(Symbol player, Symbol bot) : player_choice(player), bot_choice(bot) {}

  bool operator==(const ChoiceKey& other) const
  {
    return player_choice == other.player_choice && bot_choice == other.bot_choice;
  }
};

// see above
namespace std
{
  template <>
  struct hash<ChoiceKey>
  {
    std::size_t operator()(const ChoiceKey& k) const noexcept
    {
      return static_cast<std::size_t>(k.player_choice) ^ static_cast<std::size_t>(k.bot_choice);
    }
  };
}

// see above
std::unordered_map<ChoiceKey, Result> _results;

/*
* Description: Game constructor
*/
Game::Game() :
  _start(false),
  _reveal(false),
  _player(nullptr),
  _cv_player(nullptr)
{
  // ...
  _results[{ROCK, ROCK}] = DRAW;
  _results[{PAPER, ROCK}] = PLAYER_WINS;
  _results[{SCISSORS, PAPER}] = PLAYER_WINS;
  _results[{ROCK, SCISSORS}] = PLAYER_WINS;
  _results[{PAPER, SCISSORS}] = BOT_WINS;
  _results[{SCISSORS, ROCK}] = BOT_WINS;
}

/*
* Description: Game destructor
*/
Game::~Game()
{
  // ...
}

/* 
* Description: Ask user for number of rounds
*/
bool Game::AskRounds(void)
{
  // get number of rounds
  int game_rounds{ 0 };
  do {
    std::string user_input;
    Printer{} << "Enter number of rounds [N>0]: ";
    std::getline(std::cin, user_input);
    try {
      game_rounds = std::stoi(user_input);
    }
    catch (std::exception& err) {
      Printer{} << "[ERROR] Please input a numerical value.\n";
      game_rounds = 0;
    }
  } while (!game_rounds);
  
  // set game rounds
  _game_rounds = game_rounds;

  return true;
}

/*
* Description: Play the game
*/
void Game::Play()
{
  // check player
  this->VerifyPlayer();

  // ask for rounds from user
  while (!this->AskRounds());
  
  Printer{} << "Game started\n";

  // send start signal
  this->SendStartSignal();

  for (_round = 0; _round < _game_rounds; ++_round) {
    // send round start signal
    this->SendStartSignal();

    // wait until player makes a choice 
    this->WaitForPlayerChoice();
    
    // send reveal signal
    this->SendRevealSignal();
    
    // calculate results
    this->CheckResults();
    
    // reset choices
    this->ResetRound();
 }
  _cv_player->notify_one();
  _cv_bot->notify_one();  
}

/*
* Description: Set the game condition variable
*/
std::condition_variable& Game::GetConditionVariable()
{
  return _cv;
}

/*
* Description: Set the player
*/
void Game::SetPlayer(Player* player)
{
  _player = player;
}

/*
* Description: Set the player condition variable
*/
void Game::SetPlayerConditionVariable(std::condition_variable* cv)
{
  _cv_player = cv;
}

/*
* Description: Verify player and player CV
*/
bool Game::VerifyPlayer()
{
  if (!_player || !_cv_player) {
    Printer{} << "Player or player CV not set\n";
    throw std::runtime_error("Player or player CV not set");
    return false;
  }

  return true;
}

/*
* Description: Set the bot
*/
void Game::SetBot(Bot* bot)
{
  _bot = bot;
}

/*
* Description: Set the bot condition variable
*/
void Game::SetBotConditionVariable(std::condition_variable* cv)
{
  _cv_bot = cv;
}

/*
* Description: Verify bot and bot CV
*/
void Game::SetGameRounds(int rounds)
{
  _game_rounds = rounds;
}

/*
* Description: Get the game rounds
*/
int Game::GetGameRounds()
{
  return _game_rounds;
}

/*
* Description: Get the round
*/
int Game::GetRound()
{
  return _round;
}

/*
* Description: Set the round
*/
void Game::SetRound(int round)
{
  _round = round;
}

/*
* Description: Get the player
*/
bool Game::SendStartSignal()
{
  _start = true;
  _cv_player->notify_one();
  _cv_bot->notify_one();

  return false;
}

/*
* Description: Wait for player choice
*/
bool Game::WaitForPlayerChoice()
{
  std::unique_lock<std::mutex>lock(_mtx);

  _cv.wait(lock, [this] { return _player->_choice != Symbol() && _bot->_choice != Symbol(); });
  _start = false;

  return true;
}

/*
* Description: Send reveal signal
*/
bool Game::SendRevealSignal()
{
  std::unique_lock<std::mutex>lock(_mtx);

  _reveal = true;
  _cv_player->notify_one();
  _cv_bot->notify_one();
  _cv.wait(lock, [this] { return _player->_hand && _bot->_hand; });
  _reveal = false;

  return false;
}

/*
* Description: Check results
*/
bool Game::CheckResults()
{
  // todo: hash
  if (_player->_choice == _bot->_choice) {
    Printer{} << "Draw!\n";
  } else if (_player->_choice == ROCK && _bot->_choice == SCISSORS) {
    Printer{} << "Player wins!\n";
  } else if (_player->_choice == PAPER && _bot->_choice == ROCK) {
    Printer{} << "Player wins!\n";
  } else if (_player->_choice == SCISSORS && _bot->_choice == PAPER) {
    Printer{} << "Player wins!\n";
  } else {
    Printer{} << "Bot wins!\n";
  }

  return false;
}

/*
* Description: Determine winner
*/
Result Game::DetermineWinner(Symbol player, Symbol bot)
{
  // create key
  ChoiceKey key(player, bot);

  // look up result in the map
  auto resultIterator = _results.find(key);
  if (resultIterator != _results.end())
  {
    return resultIterator->second;
  }
  else
  {
    // no result
    return INVALID;
  }
}

/*
* Description: Print winner
*/
bool Game::PrintWinner(Result winner)
{
    switch (winner)
    {
    case DRAW:
      Printer{} << "Draw!\n";
      break;
    case PLAYER_WINS:
      Printer{} << "Player wins!\n";
      break;
    case BOT_WINS:
      Printer{} << "Bot wins!\n";
      break;
    case INVALID:
      Printer{} << "Invalid result!\n";
      break;
    }

  return true;
}

/*
* Description: Reset round
*/
bool Game::ResetRound()
{
  std::unique_lock<std::mutex>lock(_mtx);

  _player->_choice = Symbol();
  _bot->_choice = Symbol();
  _cv_player->notify_one();
  _cv_bot->notify_one();
  _cv.wait(lock, [this] { return !_player->_hand && !_bot->_hand; });

  return true;
}

