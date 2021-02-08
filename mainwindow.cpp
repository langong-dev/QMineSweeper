#include "mainwindow.h"
#include "ui_mainwindow.h"

static int paint_flag = 0;
QPainter * paintmywindow;
MainWindow * app;
int ROW =9;
int COL =9;
int BOOMNUM = 10;
block * mine = new block(ROW,COL,BOOMNUM);
int click_x,click_y,end = 0,timeend=1;
int LENGTH=mine->getcol()*21;
int HEIGHT=mine->getrow()*21 + TITILE_HEIGH;
static bool showimage = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    app = this;
    ui->setupUi(this);
    this->setFixedSize(LENGTH, HEIGHT);
    runtime = new QTimer(this);
    connect(runtime,SIGNAL(timeout()),this,SLOT(on_secondadd()));
    connect(runtime,SIGNAL(timeout()),this,SLOT(update()));

    this->setWindowTitle("QMineSweeper");
    this->setWindowIcon(QIcon(":/image/image/langong.png"));

    QMenuBar *menubar = new QMenuBar(this);
    this->setMenuBar(menubar);
    QMenu *gamemenu = new QMenu("Game",menubar);
    QAction *pgame = new QAction("Easy");
    QAction *mgame = new QAction("Middle");
    QAction *sgame = new QAction("Hard");
    QAction *ugame = new QAction("Customize");
    QAction *quit = new QAction("Quit");

    connect(pgame, SIGNAL(triggered()), this, SLOT(Easy_triggered()));
    connect(mgame, SIGNAL(triggered()), this, SLOT(Middle_triggered()));
    connect(sgame, SIGNAL(triggered()), this, SLOT(Hard_triggered()));
    connect(ugame, SIGNAL(triggered()), this, SLOT(Customize_triggered()));
    connect(quit, SIGNAL(triggered()), this, SLOT(Exit_triggered()));

    gamemenu->addAction(pgame);
    gamemenu->addAction(mgame);
    gamemenu->addAction(sgame);
    gamemenu->addAction(ugame);
    gamemenu->addAction(quit);

    QMenu *aboutmenu = new QMenu("About", menubar);
    QAction *aboutact = new QAction("About");
    QAction *aboutqtact = new QAction("About Qt");

    connect(aboutact, SIGNAL(triggered()), this, SLOT(About_triggered()));
    connect(aboutqtact, SIGNAL(triggered()), this, SLOT(AboutQT_triggered()));

    aboutmenu->addAction(aboutact);
    aboutmenu->addAction(aboutqtact);

    menubar->addMenu(gamemenu);
    menubar->addMenu(aboutmenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
   // Mouse Click
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    click_x = event->position().x();
    click_y = event->position().y();
#else
     click_x = event->x();
     click_y = event->y();
#endif
    if(event->buttons()==(Qt::LeftButton)&&(click_x>(int)LENGTH*100/200-11)&&
         (click_x<(int)LENGTH*100/200+11 +EXPRESSION_LENGHT)&&(click_y>7+TTILE_BEGIN)&&
         (click_y<7+TTILE_BEGIN+EXPRESSION_HEIGH))
    {
         restart(mine->getrow(),mine->getcol(),mine->getboomNum());
         paint_flag = 0;
         end = 0;
         timeend = 1;
         runtime->stop();
         update();
         return;
    }
    else if( click_y<TITILE_HEIGH ) return;

    if(end==1)
    return ;
   // Check positon
    click_x = click_x/BLOCK_LENGTH;
    click_y = (click_y-TITILE_HEIGH)/BLOCK_HEIGH;
    if(event->buttons()==(Qt::LeftButton))
     {
         if(timeend==1)
         {
             runtime->start(1000);
         }
         if(mine->p[click_y][click_x]==99)
         {
            // Clicked boom!
             paint_flag = 4;
             end = 1;
             timeend=1;
             runtime->stop();
             update();

         }
         else if((mine->p[click_y][click_x]<10))
         {
             // DFS
             showblock(click_y,click_x);
             paint_flag = 2;
             if (iswin() == 1)
             {
                 end = 1;
             }
             else
             {
                 end = 0;
             }
             if (end)
//             if(end = iswin()==1)
             {
                 paint_flag = 5;
                 timeend=1;
                 runtime->stop();
             }
             update();
             if(end)
         {
             if(showimage)
              {
                  QMessageBox message(QMessageBox::NoIcon, "You Win!", "Well done! I really appreciate you!");
                  message.setIconPixmap(QPixmap(":/image/image/win.png"));
                  showimage = !showimage;
                  message.exec();
              }
             else
             {
                  QMessageBox message(QMessageBox::NoIcon, "You Win!", "I will give you a red flower!");
                  message.setIconPixmap(QPixmap(":/image/image/good.png"));
                  showimage = !showimage;
                  message.exec();
             }

         }
         }
          else return;

     }
    else if(event->buttons()==(Qt::RightButton))
     {
         // Right Click
         if((mine->p[click_y][click_x]<10)||(mine->p[click_y][click_x]==99))
         {
             mine->p[click_y][click_x] = mine->p[click_y][click_x] +50;
             mine->minusredFlagNum();
         }
         else if((mine->p[click_y][click_x]>=50&&mine->p[click_y][click_x]<60)||(mine->p[click_y][click_x]>110))
         {
             mine->p[click_y][click_x] = mine->p[click_y][click_x] -50;
             mine->addredFlagNum();
         }
         paint_flag = 3;
         if (iswin() == 1)
         {
             end = 1;
         }
         else
         {
             end = 0;
         }
         if (end)
//         if(end = iswin()==1)
         {
             paint_flag = 5;
             timeend=1;
             runtime->stop();
         }
         update();
         if(end)
         {
             if(showimage)
              {
                  QMessageBox message(QMessageBox::NoIcon, "You Win!", "Well done! I really appreciate you!");
                  message.setIconPixmap(QPixmap(":/image/image/win.png"));
                  showimage = !showimage;
                  message.exec();
              }
             else
             {
                  QMessageBox message(QMessageBox::NoIcon, "You Win!", "I will give you a red flower!");
                  message.setIconPixmap(QPixmap(":/image/image/good.png"));
                  showimage = !showimage;
                  message.exec();
             }
         }
    }
}
//绘图事件
void MainWindow::paintEvent(QPaintEvent *event)
{
     QPainter painter(this);
     paintmywindow = &painter;
     // Load images
     QPixmap mine_title(":/image/image/mine_title.bmp");
     QPixmap block_close(":/image/image/block_close.bmp");
     QPixmap block_open(":/image/image/block_open.bmp");
     QPixmap red_flag(":/image/image/redflag.bmp");
     QPixmap keil_boom(":/image/image/keilboom.bmp");
     QPixmap click_boom(":/image/image/clickboom.bmp");
     QPixmap boom(":/image/image/boom.bmp");
     QPixmap qq_smile(":/image/image/smile.bmp");
     QPixmap qq_lose(":/image/image/lose.bmp");
     QPixmap qq_pround(":/image/image/proud.bmp");
     QPixmap ele_0(":/image/image/ele0.bmp");
     QPixmap ele_1(":/image/image/ele1.bmp");
     QPixmap ele_2(":/image/image/ele2.bmp");
     QPixmap ele_3(":/image/image/ele3.bmp");
     QPixmap ele_4(":/image/image/ele4.bmp");
     QPixmap ele_5(":/image/image/ele5.bmp");
     QPixmap ele_6(":/image/image/ele6.bmp");
     QPixmap ele_7(":/image/image/ele7.bmp");
     QPixmap ele_8(":/image/image/ele8.bmp");
     QPixmap ele_9(":/image/image/ele9.bmp");
     QPixmap blk_1(":/image/image/blk1.bmp");
     QPixmap blk_2(":/image/image/blk2.bmp");
     QPixmap blk_3(":/image/image/blk3.bmp");
     QPixmap blk_4(":/image/image/blk4.bmp");
     QPixmap blk_5(":/image/image/blk5.bmp");
     QPixmap blk_6(":/image/image/blk6.bmp");
     QPixmap blk_7(":/image/image/blk7.bmp");
     QPixmap blk_8(":/image/image/blk8.bmp");


    // Init
    if(paint_flag==0)
    {
        painttitle(paintmywindow);
        for(int i = 0;i<mine->getcol()*BLOCK_LENGTH;i=i+BLOCK_LENGTH)
            for(int j =TITILE_HEIGH;j<TITILE_HEIGH+mine->getrow()*BLOCK_HEIGH;j=j+BLOCK_HEIGH)
              painter.drawPixmap(i,j,block_close,0,0,BLOCK_LENGTH,BLOCK_LENGTH);
    }
    else if (paint_flag == 4)
    {
        painttitle(paintmywindow);
        for(int i = 0;i<mine->getrow();i++)
        {
            for(int j = 0;j<mine->getcol();j++ )
               {
                if((mine->p[i][j]<10))
                {
                    painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,block_close,0,0,BLOCK_LENGTH,BLOCK_HEIGH);
                }
                else if((mine->p[i][j]==99))
                {
                    painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,boom,0,0,BLOCK_LENGTH,BLOCK_HEIGH);
                }
                else if (mine->p[i][j]>110)
                {
                    painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,red_flag,0,0,BLOCK_LENGTH,BLOCK_HEIGH);
                }
                else if ((mine->p[i][j]<60)&&(mine->p[i][j]>=50))
                {
                    painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,keil_boom,0,0,BLOCK_LENGTH,BLOCK_HEIGH);
                }
                else if ((mine->p[i][j])>99&&(mine->p[i][j]<110))
                {
                   int blk_num = mine->p[i][j] - 100;
                   switch (blk_num)
                   {
                    case 0: painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,block_open,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                    case 1: painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_1,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                    case 2: painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_2,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                    case 3: painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_3,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                    case 4: painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_4,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                    case 5: painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_5,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                    case 6: painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_6,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                    case 7: painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_7,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                    case 8: painter.drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_8,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                   }
                }
               }
            }
        painter.drawPixmap(click_x*BLOCK_LENGTH,click_y*BLOCK_HEIGH+TITILE_HEIGH,click_boom,0,0,BLOCK_LENGTH,BLOCK_HEIGH);

     }
    else if (paint_flag == 2 )
    {
       painttitle(paintmywindow);
       paintboom(paintmywindow);
    }
    else if(paint_flag ==3)
    {

        painttitle(paintmywindow);
        paintboom(paintmywindow);
    }
    else if(paint_flag == 5)
    {
        painttitle(paintmywindow);
        paintboom(paintmywindow);
    }
}

void showblock(int x,int y)
{
    for(int blockRow = x - 1; blockRow <=x + 1;blockRow++)
        for(int blockCol = y - 1;blockCol <=y + 1;blockCol++)
        {
            if(blockRow<0||blockCol<0||blockRow>=mine->getrow()||blockCol>=mine->getcol())
               continue;
            if(mine->p[blockRow][blockCol]<10)
            {
                mine->p[blockRow][blockCol] =  mine->p[blockRow][blockCol] + 100;
                if(mine->p[blockRow][blockCol]==100)
                showblock(blockRow,blockCol);
            }
        }
}

int iswin()
{
    int cnt = 0;
    for(int row = 0; row <mine->getrow();row++)
        for(int col =0;col<mine->getcol();col++)
        {
            if((mine->p[row][col]>=100)&&(mine->p[row][col]<110))
                cnt++;
        }
    if(cnt>=((mine->getcol()*mine->getrow())-mine->getboomNum()))
        return 1;
    else
        return 0;
}

void restart(int row,int col,int boolnum)
{
    mine = new block (row,col,boolnum);
}

void painttitle(QPainter  * painter)
{
    QPixmap mine_title(":/image/image/mine_title.bmp");
    QPixmap block_close(":/image/image/block_close.bmp");
    QPixmap block_open(":/image/image/block_open.bmp");
    QPixmap red_flag(":/image/image/redflag.bmp");
    QPixmap keil_boom(":/image/image/keilboom.bmp");
    QPixmap click_boom(":/image/image/clickboom.bmp");
    QPixmap boom(":/image/image/boom.bmp");
    QPixmap qq_smile(":/image/image/smile.bmp");
    QPixmap qq_lose(":/image/image/lose.bmp");
    QPixmap qq_pround(":/image/image/proud.bmp");
    QPixmap ele_0(":/image/image/ele0.bmp");
    QPixmap ele_1(":/image/image/ele1.bmp");
    QPixmap ele_2(":/image/image/ele2.bmp");
    QPixmap ele_3(":/image/image/ele3.bmp");
    QPixmap ele_4(":/image/image/ele4.bmp");
    QPixmap ele_5(":/image/image/ele5.bmp");
    QPixmap ele_6(":/image/image/ele6.bmp");
    QPixmap ele_7(":/image/image/ele7.bmp");
    QPixmap ele_8(":/image/image/ele8.bmp");
    QPixmap ele_9(":/image/image/ele9.bmp");
    QPixmap ele_10(":/image/image/ele10.bmp");
    QPixmap blk_1(":/image/image/blk1.bmp");
    QPixmap blk_2(":/image/image/blk2.bmp");
    QPixmap blk_3(":/image/image/blk3.bmp");
    QPixmap blk_4(":/image/image/blk4.bmp");
    QPixmap blk_5(":/image/image/blk5.bmp");
    QPixmap blk_6(":/image/image/blk6.bmp");
    QPixmap blk_7(":/image/image/blk7.bmp");
    QPixmap blk_8(":/image/image/blk8.bmp");
    mine_title = mine_title.scaled(QSize(LENGTH,40));

    painter->drawPixmap(0,TTILE_BEGIN,mine_title,0,0,1000,1000);
    if(paint_flag == 4)
    painter->drawPixmap((int)LENGTH*100/200-11,7+TTILE_BEGIN,qq_lose,0,0,1000,1000);
    else if (paint_flag == 5)
    painter->drawPixmap((int)LENGTH*100/200-11,7+TTILE_BEGIN,qq_pround,0,0,1000,1000);
    else
    painter->drawPixmap((int)LENGTH*100/200-11,7+TTILE_BEGIN,qq_smile,0,0,1000,1000);

    int redflagnum = mine->getredFlagNum();
    if(redflagnum<0)
    {
        redflagnum = -redflagnum;
        painter->drawPixmap((int)(LENGTH*40)/200-30,5+TTILE_BEGIN,ele_10,0,0,1000,1000);
    }
    else painter->drawPixmap((int)(LENGTH*40)/200-30,5+TTILE_BEGIN,ele_0,0,0,1000,1000);
    switch(redflagnum/10)
    {
    case 0:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_0,0,0,1000,1000);break;
    case 1:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_1,0,0,1000,1000);break;
    case 2:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_2,0,0,1000,1000);break;
    case 3:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_3,0,0,1000,1000);break;
    case 4:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_4,0,0,1000,1000);break;
    case 5:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_5,0,0,1000,1000);break;
    case 6:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_6,0,0,1000,1000);break;
    case 7:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_7,0,0,1000,1000);break;
    case 8:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_8,0,0,1000,1000);break;
    case 9:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_9,0,0,1000,1000);break;
    }
    switch (redflagnum%10)
    {
    case 0:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_0,0,0,1000,1000);break;
    case 1:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_1,0,0,1000,1000);break;
    case 2:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_2,0,0,1000,1000);break;
    case 3:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_3,0,0,1000,1000);break;
    case 4:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_4,0,0,1000,1000);break;
    case 5:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_5,0,0,1000,1000);break;
    case 6:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_6,0,0,1000,1000);break;
    case 7:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_7,0,0,1000,1000);break;
    case 8:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_8,0,0,1000,1000);break;
    case 9:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_9,0,0,1000,1000);break;
    }

    int timenum = mine->gettimeNum();
    switch  (timenum/100)
    {
    case 0:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TTILE_BEGIN,ele_0,0,0,1000,1000);break;
    case 1:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TTILE_BEGIN,ele_1,0,0,1000,1000);break;
    case 2:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TTILE_BEGIN,ele_2,0,0,1000,1000);break;
    case 3:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TTILE_BEGIN,ele_3,0,0,1000,1000);break;
    case 4:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TTILE_BEGIN,ele_4,0,0,1000,1000);break;
    case 5:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TTILE_BEGIN,ele_5,0,0,1000,1000);break;
    case 6:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TTILE_BEGIN,ele_6,0,0,1000,1000);break;
    case 7:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TTILE_BEGIN,ele_7,0,0,1000,1000);break;
    case 8:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TTILE_BEGIN,ele_8,0,0,1000,1000);break;
    case 9:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TTILE_BEGIN,ele_9,0,0,1000,1000);break;
    }
    switch  (timenum/10)
    {
    case 0:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_0,0,0,1000,1000);break;
    case 1:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_1,0,0,1000,1000);break;
    case 2:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_2,0,0,1000,1000);break;
    case 3:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_3,0,0,1000,1000);break;
    case 4:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_4,0,0,1000,1000);break;
    case 5:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_5,0,0,1000,1000);break;
    case 6:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_6,0,0,1000,1000);break;
    case 7:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_7,0,0,1000,1000);break;
    case 8:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_8,0,0,1000,1000);break;
    case 9:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TTILE_BEGIN,ele_9,0,0,1000,1000);break;
    }
    switch  (timenum%10)
    {
    case 0:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_0,0,0,1000,1000);break;
    case 1:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_1,0,0,1000,1000);break;
    case 2:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_2,0,0,1000,1000);break;
    case 3:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_3,0,0,1000,1000);break;
    case 4:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_4,0,0,1000,1000);break;
    case 5:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_5,0,0,1000,1000);break;
    case 6:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_6,0,0,1000,1000);break;
    case 7:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_7,0,0,1000,1000);break;
    case 8:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_8,0,0,1000,1000);break;
    case 9:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TTILE_BEGIN,ele_9,0,0,1000,1000);break;
    }

}

void paintboom(QPainter * painter)
{
    QPixmap mine_title(":/image/image/mine_title.bmp");
    QPixmap block_close(":/image/image/block_close.bmp");
    QPixmap block_open(":/image/image/block_open.bmp");
    QPixmap red_flag(":/image/image/redflag.bmp");
    QPixmap keil_boom(":/image/image/keilboom.bmp");
    QPixmap click_boom(":/image/image/clickboom.bmp");
    QPixmap boom(":/image/image/boom.bmp");
    QPixmap qq_smile(":/image/image/smile.bmp");
    QPixmap qq_lose(":/image/image/lose.bmp");
    QPixmap qq_pround(":/image/image/proud.bmp");
    QPixmap ele_0(":/image/image/ele0.bmp");
    QPixmap ele_1(":/image/image/ele1.bmp");
    QPixmap ele_2(":/image/image/ele2.bmp");
    QPixmap ele_3(":/image/image/ele3.bmp");
    QPixmap ele_4(":/image/image/ele4.bmp");
    QPixmap ele_5(":/image/image/ele5.bmp");
    QPixmap ele_6(":/image/image/ele6.bmp");
    QPixmap ele_7(":/image/image/ele7.bmp");
    QPixmap ele_8(":/image/image/ele8.bmp");
    QPixmap ele_9(":/image/image/ele9.bmp");
    QPixmap blk_1(":/image/image/blk1.bmp");
    QPixmap blk_2(":/image/image/blk2.bmp");
    QPixmap blk_3(":/image/image/blk3.bmp");
    QPixmap blk_4(":/image/image/blk4.bmp");
    QPixmap blk_5(":/image/image/blk5.bmp");
    QPixmap blk_6(":/image/image/blk6.bmp");
    QPixmap blk_7(":/image/image/blk7.bmp");
    QPixmap blk_8(":/image/image/blk8.bmp");

    for(int i = 0;i<mine->getrow();i++)
    {
        for(int j = 0;j<mine->getcol();j++ )
           {

            if((mine->p[i][j]<10)||(mine->p[i][j]==99))
            {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,block_close,0,0,BLOCK_LENGTH,BLOCK_HEIGH);
            }
            else if (((mine->p[i][j]<60)&&(mine->p[i][j]>=50))||(mine->p[i][j]>120))
            {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,red_flag,0,0,BLOCK_LENGTH,BLOCK_HEIGH);
            }
            else if ((mine->p[i][j]>99)&&(mine->p[i][j]<110))
            {
               int blk_num = mine->p[i][j] - 100;
               switch (blk_num)
               {
                case 0: painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,block_open,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                case 1: painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_1,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                case 2: painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_2,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                case 3: painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_3,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                case 4: painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_4,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                case 5: painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_5,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                case 6: painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_6,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                case 7: painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_7,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
                case 8: painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGH+TITILE_HEIGH,blk_8,0,0,BLOCK_LENGTH,BLOCK_HEIGH);break;
               }
            }
           }
     }
}

void MainWindow::on_secondadd()
{
    mine->addtimeNum();
}

void createinterface()
{
    end =0;
    timeend =1;
    mine = new block(ROW,COL,BOOMNUM);
    LENGTH=mine->getcol()*21;
    HEIGHT=mine->getrow()*21 + TITILE_HEIGH;
    app->setMinimumSize(LENGTH,HEIGHT);
    app->setMaximumSize(LENGTH,HEIGHT);
}


void MainWindow::Easy_triggered()
{
    end =0;
    timeend =1;
    mine = new block(9,9,10);
    LENGTH=mine->getcol()*21;
    HEIGHT=mine->getrow()*21 + TITILE_HEIGH;
    this->setMinimumSize(LENGTH,HEIGHT);
    this->setMaximumSize(LENGTH,HEIGHT);
}

void MainWindow::Middle_triggered()
{
    end =0;
    timeend =1;
    mine = new block(16,16,40);
    LENGTH=mine->getcol()*21;
    HEIGHT=mine->getrow()*21 + TITILE_HEIGH;
    this->setMinimumSize(LENGTH,HEIGHT);
    this->setMaximumSize(LENGTH,HEIGHT);
}

void MainWindow::Hard_triggered()
{
    end =0;
    timeend =1;
    mine = new block(16,30,99);
    LENGTH=mine->getcol()*21;
    HEIGHT=mine->getrow()*21 + TITILE_HEIGH;
    this->setMinimumSize(LENGTH,HEIGHT);
    this->setMaximumSize(LENGTH,HEIGHT);
}

void MainWindow::Exit_triggered()
{
    QApplication::quit();
}

void MainWindow::Customize_triggered()
{
    InputDialog *cust = new InputDialog(this);
    cust->show();
}

void MainWindow::AboutQT_triggered()
{
    qApp->aboutQt();
}

void MainWindow::About_triggered()
{
    QMessageBox mess(QMessageBox::NoIcon, tr("About"), tr("QMineSweeper is an open source minesweeper game written in Qt by LanGong Dev like the minesweeper software that comes with Windows XP and Windows 3.x."));
    QPushButton *buttonURL = (mess.addButton(tr("View Sources"), QMessageBox::AcceptRole));
    QPushButton *buttonLic = (mess.addButton(tr("View License"), QMessageBox::YesRole));
    QPushButton *buttonOk = (mess.addButton(tr("OK"), QMessageBox::RejectRole));
    buttonOk->setFocus();
    mess.exec();
    if (mess.clickedButton() == buttonURL)
    {
        QString url = "https://github.com/langong-dev/QMineSweeper";
        QDesktopServices::openUrl(QUrl(url.toLatin1()));
    }
    else if (mess.clickedButton() == buttonLic){
        QString url = "https://github.com/langong-dev/QMineSweeper/";
        QDesktopServices::openUrl(QUrl(url.toLatin1()));
    }
    else
    {
        return;
    }
}
