#include <QMainWindow>
#include <QGraphicsView>
#include "mainwindow.h"
#include "vars.h"
#include "displayships.h"
#include "selectship.h"
#include "battleshipgame.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "blast.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_2->setEnabled(false);

    ui->pushButton->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    ui->pushButton_6->hide();

    bsg = new BattleshipGame();
    connect(bsg, SIGNAL(setPhaseText(QString)), this, SLOT(onPhaseTextSet(QString)));
    connect(bsg, SIGNAL(humanAnimationDone(int, int, int)), this, SLOT(whenHumanAnimationDone(int, int, int)));

    ui->graphicsView->setSceneRect(0,0,WID*SQUARE,LEN*SQUARE);
    ui->graphicsView_2->setSceneRect(0,0,WID*SQUARE,LEN*SQUARE);

    thiscolor = new QColor(Qt::gray);//new QColor(127, 127, 127);

    bsg->scene = new QGraphicsScene(this);
    bsg->scene2 = new QGraphicsScene(this);
    bsg->scene3 = new QGraphicsScene(this);
    ui->graphicsView->setScene(bsg->scene);
    ui->graphicsView_2->setScene(bsg->scene2);
    ui->graphicsView_3->setScene(bsg->scene3);
    // ui->graphicsView->setSceneRect(0, 0, (SQUARE * 10), (SQUARE * 10));
    // ui->graphicsView->setSceneRect(0, 0, 0, 0);

    redBrush = new QBrush(Qt::red);
    blueBrush = new QBrush(Qt::blue);
    grayBrush = new QBrush(Qt::lightGray);

    // definedBrush = new QBrush(Qt::lightGray);
    definedBrush = new QBrush(*thiscolor);


    blackpen = new QPen(Qt::black);
    blackpen->setWidth(1);

    timesup = new QTimer(this);
//    connect(timesup, SIGNAL(timeout()), bsg->scene,SLOT(advance()));  //Unnoted because animation will be handled by bsg's advance().
//    connect(timesup, SIGNAL(timeout()), bsg->scene2,SLOT(advance()));
    connect(timesup, SIGNAL(timeout()), bsg,SLOT(advance()));
    timesup->start(500/ANIMEFRAMES);//1000 = normal speed


    DrawLayout();
   // bsg->ship = new displayships(scene);
   // scene->addItem(bsg->ship);


    bsg->humanplayer->selship[0] = new selectship(bsg->scene3, 3);
    bsg->scene3->addItem(bsg->humanplayer->selship[0]);

    connect(bsg->humanplayer->selship[0], SIGNAL(Clicked(int)), this, SLOT(OnClick(int)));

    bsg->humanplayer->selship[1] = new selectship(bsg->scene3, 2);
    bsg->scene3->addItem(bsg->humanplayer->selship[1]);

    connect(bsg->humanplayer->selship[1], SIGNAL(Clicked(int)), this, SLOT(OnClick(int)));

    bsg->humanplayer->selship[2] = new selectship(bsg->scene3, 1);
    bsg->scene3->addItem(bsg->humanplayer->selship[2]);

    connect(bsg->humanplayer->selship[2], SIGNAL(Clicked(int)), this, SLOT(OnClick(int)));

    bsg->humanplayer->selship[3] = new selectship(bsg->scene3, 0);
    bsg->scene3->addItem(bsg->humanplayer->selship[3]);

    connect(bsg->humanplayer->selship[3], SIGNAL(Clicked(int)), this, SLOT(OnClick(int)));


    bsg->cpuship[0] = new cpuships(bsg->scene3, 3);
    bsg->scene3->addItem(bsg->cpuship[0]);

    //connect(this, SIGNAL(Sunk(int)), bsg->cpuship, SLOT(OnSunk(int)));
    connect(bsg, SIGNAL(sinkCPUShip(int)), bsg->cpuship[0], SLOT(OnSunk(int)));

    bsg->cpuship[1] = new cpuships(bsg->scene3, 2);
    bsg->scene3->addItem(bsg->cpuship[1]);

    //connect(this, SIGNAL(Sunk(int)), bsg->cpuship, SLOT(OnSunk(int)));
    connect(bsg, SIGNAL(sinkCPUShip(int)), bsg->cpuship[1], SLOT(OnSunk(int)));

    bsg->cpuship[2] = new cpuships(bsg->scene3, 1);
    bsg->scene3->addItem(bsg->cpuship[2]);

    //connect(this, SIGNAL(Sunk(int)), bsg->cpuship, SLOT(OnSunk(int)));
    connect(bsg, SIGNAL(sinkCPUShip(int)), bsg->cpuship[2], SLOT(OnSunk(int)));

    bsg->cpuship[3] = new cpuships(bsg->scene3, 0);
    bsg->scene3->addItem(bsg->cpuship[3]);

    //connect(this, SIGNAL(Sunk(int)), bsg->cpuship, SLOT(OnSunk(int)));
    connect(bsg, SIGNAL(sinkCPUShip(int)), bsg->cpuship[3], SLOT(OnSunk(int)));

    // TEST CODE
    //emit Sunk(0);
    //emit Sunk(1);
    //emit Sunk(2);
    //emit Sunk(3);

    /*for(short int x = 0; x < WID; ++x)
    {
        for(short int y = 0; y < LEN; ++y)
        {
            if(x > (y*.5) && x < (y*2) && y > (x*.5) && y < (x*2))      //Down-Right
            {
                emit SendDothis(x,y,1);
            }
          //if(x < (y*.5) && x > (y*2) && y < (x*.5) && y > (x*2))      //Up-Left
          //if(x < -(y*.5) && x > -(y*2) && y > -(x*.5) && y < -(x*2))  //Down-Left
          //if(x > -(y*.5) && x < -(y*2) && y < -(x*.5) && y > -(x*2))  //Up-Right
        }
    }*/
    /*
    //Set WID and LEN to 15 for this code to work well
    emit SendDothis(7,7,64);
    for(short int x = -7; x < WID-7; ++x)
    {
        for(short int y = -7; y < LEN-7; ++y)
        {
            if(x > -(y*.5) && x < -(y*2) && y < -(x*.5) && y > -(x*2))
            {
                emit SendDothis(x+7,y+7,1);
            }
        }
    }
    bsg->gamephase = playerturn;
    */
}

MainWindow::~MainWindow()
{
/*    delete ui;
    delete redBrush;
    delete blueBrush;
    delete grayBrush;
    delete definedBrush;
    delete blackpen;
    delete thiscolor;
    delete bsg->scene;
    delete bsg->scene2;
    delete bsg->scene3;
    delete bsg;*/
}

void MainWindow::DrawLayout()
{
    int n;
    int m;
    ui->graphicsView->centerOn(QPointF(WID*SQUARE/2,LEN*SQUARE/2));

    int x;
    int y;

    int offset = SQUARE;

    for (n = 0, x = 0; n < WID; ++n, x += offset)
    {
        for (m = 0, y = 0; m < LEN; ++m, y += offset)
        {
            bsg->HumanSquares[n][m] = bsg->scene->addRect(x, y, offset, offset, *blackpen, *definedBrush);
            //rectangle[n][m] = scene2->addRect(x, y, offset, offset, *blackpen, *definedBrush);
            bsg->CPUsquares[n][m] = new boardsquare(bsg->scene2, offset, x, y, n, m);

            connect(bsg->CPUsquares[n][m], SIGNAL(Clicked(int, int)), this, SLOT(OnReceiveNM(int, int)));
            connect(this, SIGNAL(SendDothis(int,int,int)), bsg->CPUsquares[n][m], SLOT(OnReceiveOrders(int,int,int)));
        }
    }
}

void MainWindow::NewGame()
{
    delete bsg->cpuplayer;
    bsg->blastanimation->blastframe = 0;
    bsg->animeframes = 0;
    for (int x = 0; x < WID; ++x)
    {
        for (int y = 0; y < LEN; ++y)
        {
            delete bsg->HumanSquares[x][y];
            delete bsg->CPUsquares[x][y];
        }
    }
    for(;bsg->NumberOfHumanGridPointsShot > 0;)
    {
        delete bsg->HumanGridPointsShot[--bsg->NumberOfHumanGridPointsShot];
    }
    if(bsg->gamephase != placeships)
    {
        for(int z = 0; z < 4; ++z)
        {
            delete bsg->cpuship[z];
        }
    }
    for(int z = 0; z < 4; ++z)
    {
        delete bsg->humanplayer->ship[z];
        delete bsg->humanplayer->selship[z];
    }
    for(int x=0;x<WID;++x)
    {
        for(int y=0;y<WID;++y)
        {
            delete bsg->humanplayer->PlayerGrid[x][y];
        }
    }
    for(int bazinga=0;bazinga<4;++bazinga)
    {
        delete bsg->humanplayer->PlayerShips[bazinga];
    }
    bsg->gamephase = placeships;



    ui->pushButton_2->setEnabled(false);
    ui->pushButton_2->setText("Begin Game");
    ui->phaselabel->setText("Place Ships");
    bsg->cpuplayer = new CPU();

    DrawLayout();

    bsg->humanplayer->selship[0] = new selectship(bsg->scene3, 3);
    bsg->scene3->addItem(bsg->humanplayer->selship[0]);
    connect(bsg->humanplayer->selship[0], SIGNAL(Clicked(int)), this, SLOT(OnClick(int)));

    bsg->humanplayer->selship[1] = new selectship(bsg->scene3, 2);
    bsg->scene3->addItem(bsg->humanplayer->selship[1]);
    connect(bsg->humanplayer->selship[1], SIGNAL(Clicked(int)), this, SLOT(OnClick(int)));

    bsg->humanplayer->selship[2] = new selectship(bsg->scene3, 1);
    bsg->scene3->addItem(bsg->humanplayer->selship[2]);
    connect(bsg->humanplayer->selship[2], SIGNAL(Clicked(int)), this, SLOT(OnClick(int)));

    bsg->humanplayer->selship[3] = new selectship(bsg->scene3, 0);
    bsg->scene3->addItem(bsg->humanplayer->selship[3]);
    connect(bsg->humanplayer->selship[3], SIGNAL(Clicked(int)), this, SLOT(OnClick(int)));


    bsg->cpuship[0] = new cpuships(bsg->scene3, 3);
    bsg->scene3->addItem(bsg->cpuship[0]);
    connect(bsg, SIGNAL(sinkCPUShip(int)), bsg->cpuship[0], SLOT(OnSunk(int)));

    bsg->cpuship[1] = new cpuships(bsg->scene3, 2);
    bsg->scene3->addItem(bsg->cpuship[1]);
    connect(bsg, SIGNAL(sinkCPUShip(int)), bsg->cpuship[1], SLOT(OnSunk(int)));

    bsg->cpuship[2] = new cpuships(bsg->scene3, 1);
    bsg->scene3->addItem(bsg->cpuship[2]);
    connect(bsg, SIGNAL(sinkCPUShip(int)), bsg->cpuship[2], SLOT(OnSunk(int)));

    bsg->cpuship[3] = new cpuships(bsg->scene3, 0);
    bsg->scene3->addItem(bsg->cpuship[3]);
    connect(bsg, SIGNAL(sinkCPUShip(int)), bsg->cpuship[3], SLOT(OnSunk(int)));

    for(int x=0;x<WID;++x)
    {
        for(int y=0;y<WID;++y)
        {
            bsg->humanplayer->PlayerGrid[x][y] = new MYPOINT;
            bsg->humanplayer->PlayerGrid[x][y]->hit = false;
            bsg->humanplayer->PlayerGrid[x][y]->shipis = 0;
        }
    }
    for(int bazinga=0;bazinga<4;++bazinga)
    {
        bsg->humanplayer->PlayerShips[bazinga] = new gameships(bazinga+1);
    }
    bsg->humanplayer->targetedPoint.cpupnt.pointis = -1;
    bsg->humanplayer->targetedPoint.cpupnt.shipis = -1;
    bsg->humanplayer->targetedPoint.x = -1;
    bsg->humanplayer->targetedPoint.y = -1;

    displayships::phaseIsSetupShips = true;
    for(int bazinga=0;bazinga<4;++bazinga)
    {
        displayships::allocated[bazinga] = false;
        displayships::orientations[bazinga] = 0;
        displayships::x_pos[bazinga] = 0;
        displayships::y_pos[bazinga] = 0;
    }
}

void MainWindow::OnClick(int shipnum)
{
    bsg->humanplayer->ship[shipnum] = new displayships(bsg->scene, shipnum);
    bsg->scene->addItem(bsg->humanplayer->ship[shipnum]);
    displayships::allocated[shipnum] = true;
    connect(bsg->humanplayer->ship[shipnum], SIGNAL(displayship_clicked(int)), this, SLOT(OnDisplayshipClicked(int)));
    OnDisplayshipClicked(shipnum);

    return;
}
void MainWindow::OnDisplayshipClicked(int)
{
    if(bsg->allHumanShipsArePlaced())
    {
        ui->pushButton_2->setEnabled(true);
    }
    else
    {
        ui->pushButton_2->setEnabled(false);
    }
}

void MainWindow::OnReceiveNM(int n, int m)
{
/*
    //DEBUG MESSAGE BOX:
    QString qStrN = QString::number(n);
    QString qStrM = QString::number(m);

    QMessageBox msgBox;
    msgBox.setText("Show N and M");
    msgBox.setInformativeText("N = " + qStrN + " and M = " + qStrM);
    msgBox.setStandardButtons(QMessageBox::Ok);
    //int ret =
    msgBox.exec();*/

    if(bsg->gamephase==playerturn && bsg->animeframes == 0)
    {
        bsg->humanplayer->targetedPoint.x = n;
        bsg->humanplayer->targetedPoint.y = m;
        bsg->humanplayer->fireAtPoint(bsg->cpuplayer->PlayerGrid,bsg->cpuplayer->PlayerShips);

        onPhaseTextSet("You fire!");
        bsg->blastanimation->blastframe = 0;
        bsg->blastanimation->setPos(n*SQUARE+(SQUARE/2),m*SQUARE+(SQUARE/2));
        bsg->scene2->addItem(bsg->blastanimation);
        bsg->animeframes = ANIMEFRAMES;
    }
}
void MainWindow::whenHumanAnimationDone(int n, int m, int doThis)
{
    emit(SendDothis(n, m, doThis));
}

void MainWindow::onPhaseTextSet(QString newtext)
{
    ui->phaselabel->setText(newtext);
}

void MainWindow::on_pushButton_2_clicked()
{
    if(bsg->gamephase == placeships)
    {
        displayships::phaseIsSetupShips = false;

        for(short int ship = 0; ship < 4; ++ship)
        {
            if(displayships::orientations[ship] == 0)
            {
                bsg->humanplayer->PlayerShips[ship]->direction = Down;
                bsg->humanplayer->placeShip(ship+1,displayships::x_pos[ship]/SQUARE,displayships::y_pos[ship]/SQUARE);
            }
            else
            {
                bsg->humanplayer->PlayerShips[ship]->direction = Left;
                bsg->humanplayer->placeShip(ship+1,(displayships::x_pos[ship]/SQUARE)-(bsg->humanplayer->PlayerShips[ship]->size),displayships::y_pos[ship]/SQUARE);
            }
//DEBUG CODE
/*            QPen apen(Qt::black);
            QBrush abrush(Qt::red);
            for(short int pos = 0; pos < ship+2; ++pos)
                bsg->scene->addRect(bsg->humanplayer->PlayerShips[ship]->points[pos].x()*SQUARE, bsg->humanplayer->PlayerShips[ship]->points[pos].y()*SQUARE, SQUARE, SQUARE, apen, abrush);
*/
        }
        bsg->cpuplayer->setUpCPUShips();

        bsg->humanplayer->selship[0] = new selectship(bsg->scene3, 3);
        bsg->scene3->addItem(bsg->humanplayer->selship[0]);
        connect(bsg, SIGNAL(sinkPlayerShip(int)), bsg->humanplayer->selship[0], SLOT(OnSunk(int)));

        bsg->humanplayer->selship[1] = new selectship(bsg->scene3, 2);
        bsg->scene3->addItem(bsg->humanplayer->selship[1]);
        connect(bsg, SIGNAL(sinkPlayerShip(int)), bsg->humanplayer->selship[1], SLOT(OnSunk(int)));

        bsg->humanplayer->selship[2] = new selectship(bsg->scene3, 1);
        bsg->scene3->addItem(bsg->humanplayer->selship[2]);
        connect(bsg, SIGNAL(sinkPlayerShip(int)), bsg->humanplayer->selship[2], SLOT(OnSunk(int)));

        bsg->humanplayer->selship[3] = new selectship(bsg->scene3, 0);
        bsg->scene3->addItem(bsg->humanplayer->selship[3]);
        connect(bsg, SIGNAL(sinkPlayerShip(int)), bsg->humanplayer->selship[3], SLOT(OnSunk(int)));


        switch(randomnum(2))
        {
        case 0:
            bsg->changetoPlayerTurn();
            break;
        case 1:
            bsg->changetoCPUTurn();
            break;
        default:
            qDebug() << "OUR LOGIC IS ABYSMAL";
            break;
        }

        ui->pushButton_2->setText("New Game");

        // TEST CODE
//        bsg->sinkPlayerShip(2);
  //      bsg->sinkPlayerShip(1);
    }
    else
    {
        NewGame();
    }
}
