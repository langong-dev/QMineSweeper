#include "block.h"

block::block(int row ,int col,int boomNum )
{
    p = new int *[row];
    for(int i = 0;i<row;i++)
        p[i] = new int [col];
    for(int i =0;i<row;i++)
        for(int j = 0;j<col;j++)
            p[i][j] = 0;
    srand((unsigned)time(0));
    for(int i =1;i<=boomNum;i++)
    {
         int num = rand()%(row*col)-1;
         int rowBlock = num / col;
         int colBlock = num % col;
         if( p[rowBlock][colBlock] == 99)
         {
          i--;
          continue;
         }
         else
         p[rowBlock][colBlock] = 99;

    }
    for(int i =0;i<row;i++)
        for(int j = 0;j<col;j++)
        {
            if(p[i][j]!=99)
            {
            char blockBoomNum = 0;
            for(int blockRow = i -1;blockRow<=i +1 ;blockRow++)
            {
                for(int blockCol = j -1;blockCol<=j + 1;blockCol++)
                {
                    if(blockRow<0||blockCol<0||blockRow>=row||blockCol>=col)
                    continue;
                    if(p[blockRow][blockCol]==99)
                      blockBoomNum ++;
                }
            }
            p[i][j]=blockBoomNum;
            }

        }
    this->row = row;
    this->col = col;
    this->boomNum = boomNum;
    this->timeNum = 0;
    this->redFlagNum = boomNum;
}

block::~block()
{
   for(int i =0;i<row;i++)
   {
    delete[]  p[i];
   }
   delete [] p;
}
int block::getcol()
{
    return this->col;
}
int block::getrow()
{
    return this->row;
}
int block::getboomNum()
{
    return boomNum;
}
int block::gettimeNum()
{
    return timeNum;
}
int block::getredFlagNum()
{
    return redFlagNum;
}
void block::minusredFlagNum()
{
    redFlagNum = redFlagNum -1;
}
void block::addtimeNum()
{
    timeNum = timeNum + 1;
}
void block::addredFlagNum()
{
     redFlagNum = redFlagNum + 1;
}
