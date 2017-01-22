#include "cpu.h"
#include "vars.h"
#include <QThread>
#include "gameships.h"


CPU::CPU()
{
    isCPU = true;
    isDeciding = false;
    for(int w = 0; w < WID; ++w)
    {
        for(int l = 0; l < LEN; ++l)
        {
            CPUGrid[w][l] = new cpupoint;
            CPUGrid[w][l]->pointis = unknown;
            CPUGrid[w][l]->shipis = 0;
        }
    }
    hits = new short int;
    *hits = 0;
    direction = new short int;
    *direction = NoD;
    target = new TARGET_POINT;
    lasttarget = new TARGET_POINT;
    //CPU-specific displayships
}
CPU::~CPU()
{
    for(int w = 0; w < WID; ++w)
    {
        for(int l = 0; l < LEN; ++l)
        {
            delete CPUGrid[w][l];
        }
    }
    delete hits;
    delete direction;
    delete target;
    delete lasttarget;
}


bool CPU::setUpCPUShips()
{
    /*
     * Set up ships in CPU's grid. Algorithm:
     *
     * Loop through each ship, using the index values.
     * Select an orientation at random.
     * Select a position at random.
     * Check other ships to ensure there are no shared points -- you want the ship on a spot of its own.
     * If the ship is on a spot already occupied, return to step 2.
     * Repeat the loop until all ships are accounted for.
     */
    short int count;
    bool ship_now_in_place;
    short int orient, posX, posY;

    for (count = 0; count < 4; ++count)
    {
        ship_now_in_place = false;
        while(!ship_now_in_place)
        {
            orient = randomnum(2);
            if (orient == 0)
            {
                PlayerShips[count]->setDirection(Up);
                posX = randomnum(WID);
                posY = randomnum(LEN - PlayerShips[count]->size);
            }
            else
            {
                PlayerShips[count]->setDirection(Right);
                posX = randomnum(WID - PlayerShips[count]->size);
                posY = randomnum(LEN);
            }
            ship_now_in_place = placeShip(count+1, posX, posY);
        }
    }
    return true;
}

TARGET_POINT CPU::Turn(int, int, MYPOINT* OPGrid[WID][LEN])
{
    return CPU_Turn(OPGrid);
}

//WATCH OUT!!! In the functions, x = W and y = L. This may cause a problem if something else happens.
TARGET_POINT CPU::CPU_Turn(MYPOINT* OPGrid[WID][LEN])
{
    static TARGET_POINT* lasttarget = 0;
    if(lasttarget == 0)
    {
        lasttarget = new TARGET_POINT;
    }
    if(target->cpupnt.pointis == hit)
    {
        lasttarget->cpupnt = target->cpupnt;
        lasttarget->x = target->x;
        lasttarget->y = target->y;
    }

    findTarget(CPUGrid, target);

    CPUFire(CPUGrid, OPGrid, hits, target, lasttarget, direction);

    if(target->cpupnt.shipis != 0)
    {
            *hits = 0;
            resetMSPoints(CPUGrid);
            findUnidentifiedShips(CPUGrid, hits, lasttarget);

            return *target;
    }
    else if(target->cpupnt.pointis == hit)
    {

        switch(*hits)
        {
        case 1:
            suspectPoints(CPUGrid, target);

            break;
        case 2:
            cutOffMSPoints(CPUGrid, target, direction);
            suspectPoint(CPUGrid, target, direction);

            break;
        default:
            suspectPoint(CPUGrid, target, direction);

            break;
        }
    }

    if(howmanyMSPoints(CPUGrid) == 0)
    {
        *hits = 0;
        findUnidentifiedShips(CPUGrid, hits, lasttarget);

    }

    return *target;
}

    void findTarget(CPU_POINT* CPUGrid[WID][LEN], TARGET_POINT* target)
    {
        static short int settargets = 0;
        short int mspoints = howmanyMSPoints(CPUGrid);

        if(settargets == 2)
        {
            if(mspoints > 0)
            {
                mspoints = randomnum(mspoints);
                findMSPoint(CPUGrid, target, mspoints);
            }
            else
            {
                do
                {
                    findRandomPoint(CPUGrid, target);
                }while(allPointsAroundAreMisses(CPUGrid, target) == true);
            }
        }
        else
        {
            switch(settargets)
            {
            case 0:
                target->x = 1;
                target->y = 7;
                target->cpupnt.pointis = CPUGrid[7][11]->pointis;
                target->cpupnt.shipis = CPUGrid[7][11]->shipis;
                break;
            case 1:
                target->x = 2;
                target->y = 7;
                target->cpupnt.pointis = CPUGrid[7][11]->pointis;
                target->cpupnt.shipis = CPUGrid[7][11]->shipis;
                break;
            default:
                break;
            }
            ++settargets;
        }
    }

        bool allPointsAroundAreMisses(CPU_POINT* CPUGrid[WID][LEN], TARGET_POINT* target)
        {
            short int points = 0;
            if(target->x-1 > -1)
            {
                if(CPUGrid[target->x-1][target->y]->pointis == miss)
                {
                    ++points;
                }
            }
            if(target->x+1 < WID)
            {
                if(CPUGrid[target->x+1][target->y]->pointis == miss)
                {
                    ++points;
                }
            }
            if(target->y-1 > -1)
            {
                if(CPUGrid[target->x][target->y-1]->pointis == miss)
                {
                    ++points;
                }
            }
            if(target->y+1 < LEN)
            {
                if(CPUGrid[target->x][target->y+1]->pointis == miss)
                {
                    ++points;
                }
            }

            if(points == 4)
            {
                return true;
            }
            return false;
        }

        short int howmanyMSPoints(CPU_POINT* CPUGrid[WID][LEN])
        {
            short int thismany = 0;
            short int w;
            short int l;

            for(w = 0; w < WID; ++w)//WATCH OUT!!! Here, I'm making w and l less than their maximum places. If the grid is not set up like a normal array, this may cause a problem.
            {
                for(l = 0; l < LEN; ++l)
                {
                    if(CPUGrid[w][l]->pointis == maybeship)
                        ++thismany;
                }
            }

            return thismany;
        }
        short int randomnum(short int bound)//WATCH OUT!!! This function can return 0 and a number one less than bound. May cause a bug.
        {
            if(bound == 0 || bound == 1)
                return 0;
            return qrand() % (bound);
        }
        void findMSPoint(CPU_POINT* CPUGrid[WID][LEN], TARGET_POINT* target, short int mspoints)
        {
            short int ourpoint = mspoints;
            short int w;
            short int l;

            for(w = 0; w < WID; ++w)//WATCH OUT!!! Here, I'm making w and l less than their maximum places. If the grid is not set up like a normal array, this may cause a problem.
            {
                for(l = 0; l < LEN; ++l)
                {
                    if(CPUGrid[w][l]->pointis == maybeship)
                    {
                        if(ourpoint == 0)
                        {
                            target->x = w;
                            target->y = l;
                            target->cpupnt.pointis = CPUGrid[w][l]->pointis;
                            target->cpupnt.shipis = CPUGrid[w][l]->shipis;
                            return;
                        }
                        else
                        {
                            --ourpoint;
                            if(ourpoint == 0)
                            {
                                target->x = w;
                                target->y = l;
                                target->cpupnt.pointis = CPUGrid[w][l]->pointis;
                                target->cpupnt.shipis = CPUGrid[w][l]->shipis;
                                return;
                            }
                        }
                    }
                }
            }
        }
        void findRandomPoint(CPU_POINT* CPUGrid[WID][LEN], TARGET_POINT* target)
        {
            short int w;
            short int l;
            short int randpoint = randomnum(howmanyUnknownPoints(CPUGrid));

            for(w = 0; w < WID; ++w)//WATCH OUT!!! Here, I'm making w and l less than their maximum places. If the grid is not set up like a normal array, this may cause a problem.
            {
                for(l = 0; l < LEN; ++l)
                {
                    if(CPUGrid[w][l]->pointis == unknown)
                    {
                        if(randpoint == 0)
                        {
                            target->x = w;
                            target->y = l;
                            target->cpupnt.pointis = CPUGrid[w][l]->pointis;
                            target->cpupnt.shipis = CPUGrid[w][l]->shipis;
                            return;
                        }
                        else
                        {
                            --randpoint;
                            if(randpoint == 0)
                            {
                                target->x = w;
                                target->y = l;
                                target->cpupnt.pointis = CPUGrid[w][l]->pointis;
                                target->cpupnt.shipis = CPUGrid[w][l]->shipis;
                                return;
                            }
                        }
                    }
                }
            }
        }
        short int howmanyUnknownPoints(CPU_POINT* CPUGrid[WID][LEN])
        {
            short int thismany = 0;
            short int w;
            short int l;

            for(w = 0; w < WID; ++w)//WATCH OUT!!! Here, I'm making w and l less than their maximum places. If the grid is not set up like a normal array, this may cause a problem.
            {
                for(l = 0; l < LEN; ++l)
                {
                    if(CPUGrid[w][l]->pointis == unknown)
                        ++thismany;
                }
            }

            return thismany;
        }

    void CPUFire(CPU_POINT* CPUGrid[WID][LEN], MYPOINT* OPGrid[WID][LEN], short int* hits, TARGET_POINT* target, TARGET_POINT* lasttarget, short int* direction)
    {
        OPGrid[target->x][target->y]->hit = true;
        if(OPGrid[target->x][target->y]->shipis > 0)
        {
            ++*hits;
            setDirection(target, lasttarget, direction);
            CPUGrid[target->x][target->y]->pointis = hit;
            if(shipIsSunk(OPGrid, target))
                revealShipToCPUGrid(CPUGrid, OPGrid, target);
        }
        else
            CPUGrid[target->x][target->y]->pointis = miss;

        target->cpupnt = *CPUGrid[target->x][target->y];
    }
        void setDirection(TARGET_POINT* target, TARGET_POINT* lasttarget, short int* direction)
        {//WATCH OUT!!! THIS ALL MAY NEED TO BE EDITED! It is assuming the grid's width is it's x value, it's length is it's y value, and it is numbered from the top-left corner.
            *direction = NoD;
            if(target->y < lasttarget->y)
            {
                *direction = Up;
                return;
            }
            if(target->y > lasttarget->y)
            {
                *direction = Down;
                return;
            }
            if(target->x < lasttarget->x)
            {
                *direction = Left;
                return;
            }
            if(target->x > lasttarget->x)
            {
                *direction = Right;
                return;
            }

        }
        bool shipIsSunk(MYPOINT* OPGrid[WID][LEN], TARGET_POINT* target)
        {
            short int ship = OPGrid[target->x][target->y]->shipis;
            short int w;
            short int l;

            for(w = 0; w < WID; ++w)//WATCH OUT!!! Here, I'm making w and l less than their maximum places. If the grid is not set up like a normal array, this may cause a problem.
            {
                for(l = 0; l < LEN; ++l)
                {
                    if(OPGrid[w][l]->hit == false && OPGrid[w][l]->shipis == ship)
                    {
                        return false;
                    }
                }
            }
            return true;
        }
        void revealShipToCPUGrid(CPU_POINT* CPUGrid[WID][LEN], MYPOINT* OPGrid[WID][LEN], TARGET_POINT* target)
        {
            short int ship = OPGrid[target->x][target->y]->shipis;
            short int w;
            short int l;

            for(w = 0; w < WID; ++w)//WATCH OUT!!! Here, I'm making w and l less than their maximum places. If the grid is not set up like a normal array, this may cause a problem.
            {
                for(l = 0; l < LEN; ++l)
                {
                    if(OPGrid[w][l]->shipis == ship)
                    {
                        CPUGrid[w][l]->shipis = OPGrid[w][l]->shipis;
                    }
                }
            }
        }

    bool hasLost(MYPOINT* OPGrid[WID][LEN])
    {
        short int w;
        short int l;

        for(w = 0; w < WID; ++w)//WATCH OUT!!! Here, I'm making w and l less than their maximum places. If the grid is not set up like a normal array, this may cause a problem.
        {
            for(l = 0; l < LEN; ++l)
            {
                if(OPGrid[w][l]->hit == false && OPGrid[w][l]->shipis > 0)
                {
                    return false;
                }
            }
        }
        return true;
    }

    void resetMSPoints(CPU_POINT* CPUGrid[WID][LEN])
    {
        short int w;
        short int l;

        for(w = 0; w < WID; ++w)
        {
            for(l = 0; l < LEN; ++l)
            {
                if(CPUGrid[w][l]->pointis == maybeship)
                    CPUGrid[w][l]->pointis = unknown;
            }
        }
    }

    void findUnidentifiedShips(CPU_POINT* CPUGrid[WID][LEN], short int* hits, TARGET_POINT* lasttarget)
    {
        short int w;
        short int l;

        for(w = 0; w < WID; ++w)
        {
            for(l = 0; l < LEN; ++l)
            {
                if(CPUGrid[w][l]->shipis == 0 && CPUGrid[w][l]->pointis == hit)
                {
                    changePointsAround(CPUGrid, w, l, unknown, maybeship);
                    lasttarget->cpupnt = *CPUGrid[w][l];
                    lasttarget->x = w;
                    lasttarget->y = l;
                }
            }
        }

        whatShouldHitsBe(CPUGrid, hits);
    }
        void whatShouldHitsBe(CPU_POINT* CPUGrid[WID][LEN], short int* hits)
        {//BUG COULD BE HERE
            short int w;
            short int l;

            for(w = 0; w < WID; ++w)
            {
                for(l = 0; l < LEN; ++l)
                {
                    if(CPUGrid[w][l]->shipis == 0 && CPUGrid[w][l]->pointis == hit)
                    {
                        short int d = NoD;
                        if(*hits < 1)
                            *hits = 1;
                        switch(foundHitsAroundPoint(CPUGrid, w, l, d))
                        {
                        case Up:
                            if(*hits < 2)
                                *hits = 2;
                                d = Up;
                            break;
                        case Down:
                            if(*hits < 2)
                                *hits = 2;
                                d = Down;
                            break;
                        case Left:
                            if(*hits < 2)
                                *hits = 2;
                                d = Left;
                            break;
                        case Right:
                            if(*hits < 2)
                                *hits = 2;
                                d = Right;
                            break;
                        default:
                            break;
                        }
                        if(d != NoD)
                        {
                            if(foundHitsAroundPoint(CPUGrid, w, l, d) == true)
                            {
                                *hits = 3;
                                return;
                            }
                        }
                    }
                }
            }
        }
            short int foundHitsAroundPoint(CPU_POINT* CPUGrid[WID][LEN], short int x, short int y, short int r)
            {
                //WATCH OUT!!! This whole thig here.
                switch(r)
                {
                case NoD:
                    if(x-1 > -1 && CPUGrid[x-1][y]->shipis == 0 && CPUGrid[x-1][y]->pointis == hit)
                        return Left;
                    if(x+1 < WID && CPUGrid[x+1][y]->shipis == 0 && CPUGrid[x+1][y]->pointis == hit)
                        return Right;
                    if(y+1 < LEN && CPUGrid[x][y+1]->shipis == 0 && CPUGrid[x][y+1]->pointis == hit)
                        return Down;
                    if(y-1 > -1 && CPUGrid[x][y-1]->shipis == 0 && CPUGrid[x][y-1]->pointis == hit)
                        return Up;
                    break;
                case Left:
                    if(x-2 > -1 && CPUGrid[x-2][y]->shipis == 0 && CPUGrid[x-2][y]->pointis == hit)
                        return true;
                    break;
                case Right:
                    if(x+2 < WID && CPUGrid[x+2][y]->shipis == 0 && CPUGrid[x+2][y]->pointis == hit)
                        return true;
                    break;
                case Down:
                    if(y+2 < LEN && CPUGrid[x][y+2]->shipis == 0 && CPUGrid[x][y+2]->pointis == hit)
                        return true;
                    break;
                case Up:
                    if(y-2 > -1 && CPUGrid[x][y-2]->shipis == 0 && CPUGrid[x][y-2]->pointis == hit)
                        return true;
                    break;
                }
                return false;//NoD
            }

    void suspectPoints(CPU_POINT* CPUGrid[WID][LEN], TARGET_POINT* target)
    {
        changePointsAround(CPUGrid, target->x, target->y, unknown, maybeship);
    }

    void cutOffMSPoints(CPU_POINT* CPUGrid[WID][LEN], TARGET_POINT* target, short int* direction)
    {//WATCH OUT!!! THIS ALL MAY NEED TO BE EDITED! It is assuming the grid's width is it's x value, it's length is it's y value, and it is numbered from the top-left corner.
        short int x = target->x;
        short int y = target->y;

        switch(*direction)
        {           //NOTE: The coordinates are set in the OPPOSITE direction,
        case Up:    //which is why they're going the opposite way (y should be
            ++y;    //decreasing to go Up).
            break;
        case Down:
            --y;
            break;
        case Left:
            ++x;
            break;
        case Right:
            --x;
            break;
        default:
            break;
        }

        changePointsAround(CPUGrid, x, y, maybeship, unknown);

        switch(*direction)
        {
        case Up:
            ++y;
            break;
        case Down:
            --y;
            break;
        case Left:
            ++x;
            break;
        case Right:
            --x;
            break;
        default:
            break;
        }
        if(x > -1 && x < WID && y > -1 && y < LEN && CPUGrid[x][y]->pointis == unknown)
        {
            CPUGrid[x][y]->pointis = maybeship;
        }
    }

    void suspectPoint(CPU_POINT* CPUGrid[WID][LEN], TARGET_POINT* target, short int* direction)
    {//WATCH OUT!!! THIS ALL MAY NEED TO BE EDITED! It is assuming the grid's width is it's x value, it's length is it's y value, and it is numbered from the top-left corner.
        short int x = target->x;
        short int y = target->y;

        switch(*direction)
        {
        case Up:
            --y;
            break;
        case Down:
            ++y;
            break;
        case Left:
            --x;
            break;
        case Right:
            ++x;
            break;
        default:
            break;
        }
        if(x > -1 && x < WID && y > -1 && y < LEN && CPUGrid[x][y]->pointis == unknown)
        {
            CPUGrid[x][y]->pointis = maybeship;
        }
    }

    void changePointsAround(CPU_POINT* CPUGrid[WID][LEN], short int x, short int y, short int pointiscurrent, short int pointbecomes)
    {
        //WATCH OUT!!! This whole thig here.
        if(x-1 > -1 && CPUGrid[x-1][y]->pointis == pointiscurrent)
            CPUGrid[x-1][y]->pointis = pointbecomes;
        if(x+1 < WID && CPUGrid[x+1][y]->pointis == pointiscurrent)
            CPUGrid[x+1][y]->pointis = pointbecomes;
        if(y+1 < LEN && CPUGrid[x][y+1]->pointis == pointiscurrent)
            CPUGrid[x][y+1]->pointis = pointbecomes;
        if(y-1 > -1 && CPUGrid[x][y-1]->pointis == pointiscurrent)
            CPUGrid[x][y-1]->pointis = pointbecomes;
    }
