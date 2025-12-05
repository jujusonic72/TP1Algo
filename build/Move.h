// #pragma once
//
// #include "Command.h"
// #include <cmath>
// #include <cstdio>
// class Move :: Command {
// private:
//     float startX, startY;
//     float targetX, targetY;
//     float currentX, currentY;
//     float speed;
//     bool finished;
//     bool forceFinish;
//
// public:
//     Move(float startX, float startY, float targetX, float targetY, float speed = 100.0f)
//         : startX(startX), startY(startY), targetX(targetX), targetY(targetY),
//           currentX(startX), currentY(startY), speed(speed), 
//           finished(false), forceFinish(false) {}
//
//     // Appelé chaque frame avec le temps écoulé
//     void execute(float deltaTime) {
//         if (finished || forceFinish) return;
//
//         float dx = targetX - currentX;
//         float dy = targetY - currentY;
//         float distance = sqrt(dx*dx + dy*dy);
//
//         if (distance < 0.5f) {
//             // Arrivé à destination
//             currentX = targetX;
//             currentY = targetY;
//             finished = true;
//             postExecute();
//             return;
//         }
//
//         // Déplacer progressivement
//         float dirX = dx / distance;
//         float dirY = dy / distance;
//         float moveDistance = speed * deltaTime;
//
//         if (moveDistance >= distance) {
//             currentX = targetX;
//             currentY = targetY;
//             finished = true;
//             postExecute();
//         } else {
//             currentX += dirX * moveDistance;
//             currentY += dirY * moveDistance;
//         }
//     }
//
//     void postExecute() {
//         // Cleanup ou callback ici
//         printf("Move completed!\n");
//     }
//
//     void cancel() { forceFinish = true; }
//     bool isFinished() const { return finished || forceFinish; }
//     float getX() const { return currentX; }
//     float getY() const { return currentY; }
// };
