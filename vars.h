#ifndef VARS_H
#define VARS_H

#define WID 12 //CHANGE THESE TO VARIABLES LATER
#define LEN 12
#define SQUARE 30
#define ANIMEFRAMES 20

enum gamephases
{
//    duringanimation,
//    gamebegin,
    placeships,
    playerturn,
    CPUturn,
    gameover
};

struct point
{
    short int shipis;
    bool hit;
};
typedef struct point MYPOINT;

struct cpupoint
{
    short int shipis;
    short int pointis;
};
typedef struct cpupoint CPU_POINT;

struct targetpoint
{
    short int x;
    short int y;
    CPU_POINT cpupnt;
};
typedef struct targetpoint TARGET_POINT;

enum direct
{
    NoD,
    Up,
    Right,
    Down,
    Left
};

enum cpu_point_is
{               //Fired/Not     Tense/Calm
    unknown,    // N             C
    miss,       // F             C
    hit,        // F             T
    maybeship   // N             T
};


#endif // VARS_H
