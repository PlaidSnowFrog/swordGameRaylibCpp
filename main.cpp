#include "raylib.h"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <sys/types.h>

using std::string;

struct Rect {
  int x, y;
  uint speed;
  bool sword;
  uint swordCountdown;
  bool shield;
  uint shieldCowntdown;
  uint life;
  bool collided;
};

bool areNear(const Rect rect1, const Rect rect2, double distanceThreshold);
bool checkCollision(const Rect rect1, const Rect rect2);

int main(void) {
  Rect player{0, 0, 10, false, 0, false, 0, 10, false};
  Rect enemy{0, 0, 3, false, 0, false, 0, 10, false};

  bool isGameOver = false;
  bool hasPlayerWon = false;

  // Initialize window and game loop
  InitWindow(800, 450, "Sword");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    if (!isGameOver) {
      DrawRectangle(player.x, player.y, 50, 50, BLUE);
      DrawRectangle(enemy.x, enemy.y, 50, 50, RED);

      DrawText(TextFormat("Player Life: %02i      Enemy Life: %02i",
                          player.life, enemy.life),
               10, 10, 20, BLACK);

      // Player Movement
      if (IsKeyDown(KEY_LEFT_SHIFT)) {
        player.speed = 15;
      } else {
        player.speed = 10;
      }

      if (IsKeyDown(KEY_W)) {
        player.y -= player.speed;
      }
      if (IsKeyDown(KEY_S)) {
        player.y += player.speed;
      }
      if (IsKeyDown(KEY_A)) {
        player.x -= player.speed;
      }
      if (IsKeyDown(KEY_D)) {
        player.x += player.speed;
      }

      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !player.sword) {
        player.shield = false;
        player.sword = true;
        player.swordCountdown = 60;
      } else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && player.sword) {
        player.sword = false;
        player.swordCountdown = 0;
      }

      if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && !player.shield) {
        player.sword = false;
        player.shield = true;
        player.shieldCowntdown = 120;
      } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && player.shield) {
        player.shield = false;
        player.shieldCowntdown = 0;
      }

      // Enemy Movement
      if (areNear(enemy, player, 100)) {
        enemy.speed = 7;
        enemy.sword = true;
      } else {
        enemy.speed = 3;
        enemy.sword = false;
      }

      if (enemy.x > player.x) {
        enemy.x -= enemy.speed;
      } else if (enemy.x < player.x) {
        enemy.x += enemy.speed;
      }
      if (enemy.y > player.y) {
        enemy.y -= enemy.speed;
      } else if (enemy.y < player.y) {
        enemy.y += enemy.speed;
      }

      if (player.sword) {
        player.swordCountdown -= 3;
      }
      if (player.swordCountdown <= 0) {
        player.sword = false;
      }

      if (player.shield) {
        player.shieldCowntdown -= 2;
      }
      if (player.shieldCowntdown <= 0) {
        player.shield = false;
      }

      // Sword and shield drawing
      if (player.sword) {
        DrawRectangle(player.x + 10, player.y + 12, 25, 25, DARKPURPLE);
      }
      if (player.shield) {
        DrawRectangle(player.x - 5, player.y - 5, 60, 60, DARKPURPLE);
      }

      // Check collisions
      if (!player.shield && checkCollision(player, enemy)) {
        if (!player.collided) {
          player.life--;
          player.collided = true;
        }
      } else {
        player.collided = false;
      }
      if (player.sword && checkCollision(player, enemy)) {
        if (!enemy.collided) {
          enemy.life--;
          enemy.collided = true;
        }
      } else {
        enemy.collided = false;
      }

      if (player.life < 1 || player.life > 10) {
        isGameOver = true;
      }
      if (enemy.life < 1 || enemy.life > 10) {
        isGameOver = true;
        hasPlayerWon = true;
      }
    } else if (hasPlayerWon) {
      DrawText("You Won!", 480, 270, 20, BLACK);
    } else if (!hasPlayerWon) {
      DrawText("Game Over!", 480, 270, 20, BLACK);
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

bool checkCollision(const Rect rect1, const Rect rect2) {
  // Check if rect1 is to the left of rect2
  if (rect1.x + 50 < rect2.x) {
    return false;
  }

  // Check if rect1 is to the right of rect2
  if (rect1.x > rect2.x + 50) {
    return false;
  }

  // Check if rect1 is above rect2
  if (rect1.y + 50 < rect2.y) {
    return false;
  }

  // Check if rect1 is below rect2
  if (rect1.y > rect2.y + 50) {
    return false;
  }

  // If none of the above conditions are true, rectangles overlap and there is a
  // collision
  return true;
}

bool areNear(const Rect rect1, const Rect rect2, double distanceThreshold) {
  double center1X = static_cast<double>(rect1.x) + 25;
  double center1Y = static_cast<double>(rect1.y) + 25;
  double center2X = static_cast<double>(rect2.x) + 25;
  double center2Y = static_cast<double>(rect2.y) + 25;

  double distance = std::sqrt(std::pow(center2X - center1X, 2) +
                              std::pow(center2Y - center1Y, 2));

  return distance <= distanceThreshold;
}
