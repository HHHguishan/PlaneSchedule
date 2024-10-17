#include<time.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>

/*private function*/
//以时间为种子
void initRandomSeed()
{
    static int initialized=0;
    if(!initialized)
    {
        srand((unsigned)time(NULL));
        initialized=1;
    }
}

/*
 * Function: randomChance
 * 该函数功能为p的概率返回
*/
double randomChance(double p)
{
    void initRandomSeed();
    double r =rand()/(RAND_MAX+1.0f);
    return r<p;
}

/*
 * Function :randomInteger
 * 该函数随机产生一个low-high的整数，闭区间
*/
int randomInteger(int low,int high)
{
    initRandomSeed();
    double d=rand()/((double)RAND_MAX+1);
    double s=d*((double)high-low+1);
    int res=(int)(low+s);
    return res;
}

/*
 * Function :randomReal
 * 该函数随机产生一个low-high的实数，闭区间
*/
double randomReal(int low,int high)
{
    initRandomSeed();
    double d=rand()/((double)RAND_MAX+1);
    double s=d*((double)high-low);
    return s+low;
}
