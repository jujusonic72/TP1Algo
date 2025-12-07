#pragma once

struct Position
{
    float x = 0;
    float y = 0;
};
inline bool operator==(const Position& lpos, const Position& rpos){ return lpos.x == rpos.x && lpos.y == rpos.y; }
