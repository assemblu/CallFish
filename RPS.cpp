// Author: Emirhan Gocturk

#include <thread>
#include "Game.h"
#include "Player.h"
#include "Bot.h"

int main()
{
  Game game;
  Player player(game);
  Bot bot(game);

  game.SetPlayerConditionVariable(&player.GetConditionVariable());
  game.SetBotConditionVariable(&bot.GetConditionVariable());
  game.SetPlayer(&player);
  game.SetBot(&bot);
  player.SetGameConditionVariable(&game.GetConditionVariable());
  bot.SetGameConditionVariable(&game.GetConditionVariable());

  // start game thread
  std::thread game_thread([&]() {
    game.Play();
  });
   
  // start player thread
  std::thread player_thread([&]() {
   player.JoinGame();
  });
  
  // start player thread
  std::thread bot_thread([&]() {
   bot.JoinGame();
  });


  game_thread.join();
  player_thread.join();
  bot_thread.join();

  return 0;
}
