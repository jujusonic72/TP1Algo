#pragma once

struct Position
{
    int x = 0;
    int y = 0;
};
bool operator==(const Position& lpos, const Position& rpos){ return lpos.x == rpos.x && lpos.y == rpos.y; }
