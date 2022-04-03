#include "game/map.h"


void loadMAP(char *name, GAME *game)
{
    name = name;
    game = game;
}

void saveMAP(char *name, GAME *game)
{
    name = name;
    game = game;
}

bool isOverlapping(vec2 l1, vec2 r1, vec2 l2, vec2 r2) {
   if (l1.x > r2.x || l2.x > r1.x)
      return false;
   if (l1.y < r2.y || l2.y < r1.y)
      return false;
   return true;
}
