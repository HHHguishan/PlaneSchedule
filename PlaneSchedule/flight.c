#include<malloc.h>

/*结构体*/
struct flight
{
    int flight_id;
    int direction;
    int num_passenger;
    double left_fuel;  
    double FCPM;       //fuel consumption per minute 
    int status;        //0 (normal)  -1 (urgent) 1(very urgent)   
    struct flight *next;
};

/*头插法插入一个航班结点*/
void head_add_flight(struct flight* F ,int Direction,int NumPassenger,double LeftFuel,double fcpm,int Status,int FlightNumber)
{
    struct flight* Next=(struct flight*)malloc(sizeof(struct flight));
    Next->flight_id=FlightNumber;
    Next->direction=Direction;
    Next->num_passenger=NumPassenger;
    Next->left_fuel=LeftFuel;
    Next->FCPM=fcpm;
    Next->status=Status;
    Next->next=F->next;
    F->next=Next;

};

/*尾插法插入一个航班结点*/
void tail_add_flight(struct flight* F ,int Direction,int NumPassenger,double LeftFuel,double fcpm,int Status,int FlightNumber)
{
    struct flight* Next=(struct flight*)malloc(sizeof(struct flight));
    Next->flight_id=FlightNumber;
    Next->direction=Direction;
    Next->num_passenger=NumPassenger;
    Next->left_fuel=LeftFuel;
    Next->FCPM=fcpm;
    Next->status=Status;
    Next->next=NULL;
    struct flight* temp=F;
    while(temp->next!=NULL)
    {
        temp=temp->next;
    }
    temp->next=Next;
}

void color(short x)
{
    if (x >= 0 && x <= 15)                                           // 参数在0-15的范围颜色
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x); // 只有一个参数，改变字体颜色
    else                                                             // 默认的颜色白色
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

/*0 = 黑色       8 = 灰色
1 = 蓝色       9 = 淡蓝色
2 = 绿色       A = 淡绿色
3 = 浅绿色     B = 淡浅绿色
4 = 红色       C = 淡红色
5 = 紫色       D = 淡紫色
6 = 黄色       E = 淡黄色
7 = 白色       F = 亮白色
*/