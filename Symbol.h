#include <unordered_map>

#pragma once

/*
* Description: Symbol enum
*/
  static enum Symbol
  {
    ROCK = 1,
    PAPER,
    SCISSORS
  };

  /*
  * Description: Result enum
  */
  enum Result {
    DRAW = 1,
    PLAYER_WINS,
    BOT_WINS,
    INVALID
  };

/*
* Description: Symbol map
*/
  static std::unordered_map<Symbol, std::string> symbol_map = {
    {ROCK, "Fist"},
    {PAPER, "Open Hand"},
    {SCISSORS, "Index and Middle Finger"}
  };

