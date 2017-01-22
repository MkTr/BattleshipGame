#include "battleshipgame.h"
#include "gameships.h"
#include "blast.h"



BattleshipGame::BattleshipGame()
{
    gamephase = placeships;
    humanplayer = new Human();
    cpuplayer = new CPU();
    NumberOfHumanGridPointsShot = 0;
    animeframes = 0;
    blastanimation = new Blast(0,0);
//    boardsquare *squares[WID][LEN];
}
BattleshipGame::~BattleshipGame()
{
    delete humanplayer;
    delete cpuplayer;
    delete blastanimation;
}

bool BattleshipGame::hasLost(MYPOINT* Grid[WID][LEN])
{
    short int w;
    short int l;

    for(w = 0; w < WID; ++w)//WATCH OUT!!! Here, I'm making w and l less than their maximum places. If the grid is not set up like a normal array, this may cause a problem.
    {
        for(l = 0; l < LEN; ++l)
        {
            if(Grid[w][l]->hit == false && Grid[w][l]->shipis > 0)
            {
                return false;
            }
        }
    }
    return true;
}

bool BattleshipGame::allHumanShipsArePlaced()
{
    return humanplayer->allShipsArePlaced();
}

void BattleshipGame::changetoPlayerTurn()
{
    gamephase = playerturn;
    emit(setPhaseText("Your turn"));
}

void BattleshipGame::changetoCPUTurn()
{
    gamephase = CPUturn;
    cpuplayer->isDeciding = true;
    if(*cpuplayer->hits == 0)
    {
        animeframes = randomnum(ANIMEFRAMES/4) + ANIMEFRAMES/4;
    }
    else if(*cpuplayer->hits == 1)
    {
        animeframes = randomnum(ANIMEFRAMES/2) + ANIMEFRAMES/2;
    }
    else if(*cpuplayer->hits == 2)
    {
        animeframes = randomnum((ANIMEFRAMES*3)/4) + (ANIMEFRAMES*3)/4;
    }
    else if(*cpuplayer->hits >= 3)
    {
        animeframes = randomnum(ANIMEFRAMES/4) + ANIMEFRAMES/4;
    }
    emit(setPhaseText("CPU's turn"));
}

void BattleshipGame::changefromPlayerTurn()
{
    animeframes = 0;
    if(humanplayer->targetedPoint.cpupnt.pointis == hit)
    {
        emit humanAnimationDone(humanplayer->targetedPoint.x, humanplayer->targetedPoint.y, 2);
        if(shipIsSunk(cpuplayer->PlayerGrid,&humanplayer->targetedPoint))
        {
            emit sinkCPUShip(humanplayer->targetedPoint.cpupnt.shipis-1);
        }

        if(hasLost(cpuplayer->PlayerGrid))
        {
            changetoGameOver(true);
        }
    }
    else
    {
        emit humanAnimationDone(humanplayer->targetedPoint.x, humanplayer->targetedPoint.y, 1);
    }
    if(gamephase != gameover) changetoCPUTurn();
}

void BattleshipGame::changefromCPUTurn()
{
    animeframes = 0;
    if(cpuplayer->targetedPoint.cpupnt.pointis == hit)
    {
        QPen blackpen(Qt::black);
        QBrush definedBrush(Qt::red);
        HumanGridPointsShot[NumberOfHumanGridPointsShot] = scene->addRect(cpuplayer->targetedPoint.x*SQUARE, cpuplayer->targetedPoint.y*SQUARE, SQUARE, SQUARE, blackpen, definedBrush);
        ++NumberOfHumanGridPointsShot;
        if(shipIsSunk(humanplayer->PlayerGrid,&cpuplayer->targetedPoint))
        {
            emit sinkPlayerShip(cpuplayer->targetedPoint.cpupnt.shipis-1);
        }

        if(hasLost(humanplayer->PlayerGrid))
        {
            changetoGameOver(false);
        }
    }
    else
    {
        QPen blackpen(Qt::black);
        QBrush definedBrush(Qt::blue);
        HumanGridPointsShot[NumberOfHumanGridPointsShot] = scene->addRect(cpuplayer->targetedPoint.x*SQUARE, cpuplayer->targetedPoint.y*SQUARE, SQUARE, SQUARE, blackpen, definedBrush);
        ++NumberOfHumanGridPointsShot;
    }
    if(gamephase != gameover) changetoPlayerTurn();
}

void BattleshipGame::changetoGameOver(bool humanWon)
{
    gamephase = gameover;
    if(humanWon)
    {
        emit(setPhaseText("You win!"));
    }
    else
    {
        emit(setPhaseText("You lose."));
    }
}


void BattleshipGame::advance()
{
    if(animeframes > 0)
    {
        if(gamephase == playerturn)
        {
            --animeframes;
            blastanimation->advance();
            if(animeframes == 0)
            {
                scene2->removeItem(blastanimation);
                changefromPlayerTurn();
            }
        }
        else if(gamephase == CPUturn)
        {
            if(cpuplayer->isDeciding)
            {
                --animeframes;
                if(animeframes == 0)
                {
                    cpuplayer->fireAtPoint(humanplayer->PlayerGrid,humanplayer->PlayerShips);
                    cpuplayer->isDeciding = false;
                    emit(setPhaseText("CPU fires!"));
                    blastanimation->blastframe = 0;
                    blastanimation->setPos(cpuplayer->targetedPoint.x*SQUARE+(SQUARE/2),cpuplayer->targetedPoint.y*SQUARE+(SQUARE/2));
                    scene->addItem(blastanimation);
                    animeframes = ANIMEFRAMES;
                }
            }
            else
            {
                --animeframes;
                blastanimation->advance();
                if(animeframes == 0)
                {
                    scene->removeItem(blastanimation);
                    changefromCPUTurn();
                }
            }
        }
    }
}
