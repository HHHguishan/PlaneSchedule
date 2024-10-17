/*
 * File: PlaneSchedule.c
 * 该项目模拟了一个简单的飞机调度系统。
 * 项目首先在主函数内声明一个航班的指针，随后调用init()函数进行初始化其内容，\
 * 最后开始系统模拟的运行。
 */

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "flight.c"
#include "random.c"

/* Constants */
const double SUN_POSSIBILITY = 0.5; // weather
const double RAIN_POSSIBILITY = 0.4;
const double SNOW_POSSIBILITY = 0.1;
const double VERY_URGENT_STATUS = 0.1;
const double URGENT_STATUS = 0.2;
const double NORMAL_STATUS = 0.7;
const double NEW_FLIGHT = 0.1;
const double FLOW_CONTROL = 0.1;
const int SIMULATION_TIME = 25;//循序次数
const int NUM_RUNWAY = 2;
const int NUM_GATE = 80;

/*Global variable*/
int FlightNumber = 10001;
int Total_flow = 0;

/*Function prototypes*/
void init(struct flight *F);
int RandomWeather(double Pa, double Pb, double Pc);
int RandomStatus(double Pa, double Pb, double Pc);
int RandomDirection(); // 生成0到1的随机数
void report(struct flight *F);//打印航班
void print_head();                                  // 打印表头
void print_info(struct flight *F);                  // 打印航班信息
void runsimulation(struct flight *F);               // 开始模拟
int priority(struct flight *f1, struct flight *f2); // 优先
struct flight *schedule(struct flight *F, int control, int min_wait_time);

/*Main program*/
int main()
{
    struct flight *F = (struct flight *)malloc(sizeof(struct flight)); // sentinel
    F->next = NULL;
    init(F);
    runsimulation(F);
}

/*
 * Function : init
 * 函数完成航班的初始化。
 * 该函数首先通过randomInteger()函数获得一个随机的航班数作为初始状态
 * 随后用RandomDirection()、randomInteger()、randomReal()、RandomStatus()
 * 等函数为每一个航班随机初始内容。
 * 其中结合实际，认为出港的航班剩余燃料为100000,入港的航班燃料则在300-50000随机产生。
 * 最后用reprot函数输出初始化情况，并认为此时跑道均未运行。
 */

void init(struct flight *F)
{
    int num_flight = randomInteger(5, 10);
    for (int i = 0; i < num_flight; i++)
    {
        int Direction = RandomDirection();
        int NumPassenger = randomInteger(100, 200);
        Total_flow += NumPassenger;
        double fcpm = randomReal(25, 75);
        int Status = RandomStatus(VERY_URGENT_STATUS, URGENT_STATUS, NORMAL_STATUS);
        double LeftFuel;
        if (Direction == 1)
        {
            LeftFuel = 99999;
        }
        else
        {
            LeftFuel = randomReal(300, 50000);
        }
        head_add_flight(F, Direction, NumPassenger, LeftFuel, fcpm, Status, FlightNumber++);
    }
    printf("航班初始化完成！当前所有航班情况如下：\n\n");
    report(F);
    printf("认为当前机场跑道均未运行\n\n");

    // while(1)
    // {
    //     printf("按下任意键继续 . . . \n\n");
    //     int c=getchar();
    //     if(c!=EOF)
    //     {
    //         getchar();
    //         break;
    //     }
    // }
    system("pause");
}

/*
 * Function :RandomWeather
 * 随机生成航班的状态，认为1为晴天，0为雨天，-1为暴雪
 * 函数功能为Pa的概率返回1,Pb的概率返回0,Pc的概率返回-1
 * 该函数首先生成一个0-1的实数p，根据其范围返回对应的值，满足函数要求。
 */

int RandomWeather(double Pa, double Pb, double Pc)
{
    double p = randomReal(0, 1);
    if (p < Pa)
    {
        return 1;
    }
    else if (p < Pa + Pb)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/*
 * Function: RandomStatus
 * 随机生成航班的状态，认为1为非常紧急，0为紧急，-1为正常
 * 函数功能为在Pa的概率下返回1,Pb的概率下返回0,Pc的概率下返回-1
 * 过程与RandomWeather函数类似。
 */

int RandomStatus(double Pa, double Pb, double Pc)
{
    double p = randomReal(0, 1);
    if (p < Pa)
    {
        return 1; // very urgent
    }
    else if (p < Pa + Pb)
    {
        return 0; // urgent
    }
    else
    {
        return -1; // normal
    }
}

/*
 * Function : RandomDirection
 * 该函数随机生成一个航班的状态
 * 使用randomInteger函数随机生成0-1的两个整数
 * 认为0为入港，1为出港
 */

int RandomDirection()
{
    return randomInteger(0, 1); // 0 in   1 out
}

/*
 * Function : report
 * 该函数用于打印出所有航班的信息
 * 先用print_head函数打印出表头
 * 遍历每一个航班结点，然后用print_info函数输出
 *
 */

void report(struct flight *F)
{
    struct flight *temp = F->next;
    print_head();
    while (temp != NULL)
    {
        print_info(temp);
        temp = temp->next;
    }
}

/*
 * Function :print_head
 * 打印出一个表头
 *
 */

void print_head()
{
    printf("航班号\t进出方向\t乘客数量\t剩余燃料\t每分钟耗油量\t状态\n\n");
}

/*
 * Function :print_info
 * 该函数功能为在一行打印出一个航班结点的信息
 *
 */
void print_info(struct flight *F)
{
    printf("%d\t", F->flight_id);
    if (F->direction == 0)
    {
        printf("入港\t\t");
    }
    else
    {
        printf("出港\t\t");
    }
    printf("%d\t\t%.1lf\t\t%.1lf\t\t", F->num_passenger, F->left_fuel, F->FCPM);
    if (F->status == 1)
    {
        printf("非常紧急\n\n");
    }
    else if (F->status == 0)
    {
        printf("紧急\n\n");
    }
    else if (F->status == -1)
    {
        printf("正常\n\n");
    }
}

/*
 * Function :runsimulation
 * 该函数运行模拟航班调度的过程。
 * 主要内容为在一个for循环内（认为每一次循环5min）
 * 每一个小时随机改变天气，每次循环随机产生流量控制、随机产生新的航班并打印，
 * 然后根据流量控制情况调用schedule函数，产生优先起降的航班序列，
 * 再根据天气情况，安排飞机起降。
 * 最后根据实际情况减少每一个入港航班的燃料。
 */
void runsimulation(struct flight *F)
{   
    int count=0;
    color(6);
    printf("模拟开始：");
    color(7);
    int runway[NUM_RUNWAY]; // 1(using) 0(unused)
    int gate[NUM_GATE];     // 1(using) 0(unused)
    int weather = 1;        // 1(sun) 0(rain) -1(snow)
    int control = 0;        // 1(controlled)  0(not controlled)
    int control_time;
    int wait_time[NUM_RUNWAY];
    int min_wait_time = 0;
    int Runway_capacity = 5;

    for (int i = 0; i < NUM_RUNWAY; i++)
    {
        runway[i] = 0;
        wait_time[i] = 0;
    }

    for (int i = 0; i < NUM_GATE; i++)
    {
        gate[i] = 0;
    }

    for (int t = 0; t < SIMULATION_TIME; t++) // each  t=5min
    {
        if (t * 5 % 60 == 0) // every 60min change the weather
        {
            weather = RandomWeather(SUN_POSSIBILITY, RAIN_POSSIBILITY, SNOW_POSSIBILITY);
            if (weather == 1)
            {
                printf("现在是晴天\n\n");
                Runway_capacity = 5;
            }
            if (weather == 0)
            {
                printf("现在是雨天\n\n");
                Runway_capacity = 15;
            }
            if (weather == -1)
            {
                printf("现在在下雪,无法进行飞机起降了\n\n");
                Runway_capacity = 9999;
            }
        }

        if (randomChance(FLOW_CONTROL))
        {
            control = 1;
            control_time = randomInteger(10, 20);
            printf("流量控制开始,不能出港，只能进港。控制将持续%d分钟\n\n", control_time);
        }

        if (randomChance(NEW_FLIGHT))
        {
            int num_flight = randomInteger(2, 4);
            color(13);
            printf("新增了%d个航班,信息如下:\n\n", num_flight);
            color(7);
            print_head();
            for (int i = 0; i < num_flight; i++)
            {
                int Direction = RandomDirection();
                int NumPassenger = randomInteger(100, 200);
                Total_flow += NumPassenger;
                double fcpm = randomReal(25, 75);
                int Status = RandomStatus(VERY_URGENT_STATUS, URGENT_STATUS, NORMAL_STATUS);
                double LeftFuel;
                if (Direction == 1)
                {
                    LeftFuel = 99999;
                }
                else
                {
                    LeftFuel = randomReal(300, 50000);
                }
                printf("%d\t", FlightNumber);
                if (Direction == 0)
                {
                    printf("入港\t\t");
                }
                else
                {
                    printf("出港\t\t");
                }
                printf("%d\t\t%.1lf\t\t%.1lf\t\t", NumPassenger, LeftFuel, fcpm);
                if (Status == 1)
                {
                    printf("非常紧急\n\n");
                }
                else if (Status == 0)
                {
                    printf("紧急\n\n");
                }
                else if (Status == -1)
                {
                    printf("正常\n\n");
                }
                head_add_flight(F, Direction, NumPassenger, LeftFuel, fcpm, Status, FlightNumber++);
            }
        }
        F = schedule(F, control, min_wait_time);
        printf("经过调度，优先级排序如下：\n\n");
        report(F);

        if (weather != -1)
        {
            if (wait_time[0] == 0)
            {
                if (F->next == NULL)
                {   color(10);
                    printf("当前无航班计划了。\n\n");
                    color(7);
                }

                else
                {   color (2);
                    printf("当前1号跑道空闲,优先级最高的航班开始起降。\n\n需要%d分钟,他的信息如下：\n\n", Runway_capacity);
                    color(7);
                    // print_head();
                    // printf("%d\t",F->next->flight_id);
                    // // print_head(F->next);
                    // wait_time[0]+=Runway_capacity;
                    // F->next=F->next->next;
                    print_head();
                    print_info(F->next);
                    wait_time[0] += Runway_capacity;
                    F->next = F->next->next;
                }
            }
            else
            {
                wait_time[0] -= 5;
                if (wait_time[0] <= 0)
                {   
                    printf("起降完毕,跑道1空闲了。\n");
                   
                    wait_time[0] = 0;
                }
            }
            if (wait_time[1] == 0)
            {
                if (F->next == NULL)
                {   color(10);
                    printf("当前无航班计划了。\n\n");
                    color(7);
                }
                else
                {   color(2);
                    printf("当前2号跑道空闲,优先级最高的航班开始起降。\n\n需要%d分钟,他的信息如下：\n\n", Runway_capacity);
                    color(7);
                    print_head();
                    print_info(F->next);
                    wait_time[1] += Runway_capacity;
                    F->next = F->next->next;
                }
            }
            else
            {
                wait_time[1] -= 5;
                if (wait_time[1] <= 0)
                {
                    printf("起降完毕,跑道2空闲了。\n");
                    wait_time[1] = 0;
                }
            }
        }
        else
        {
            printf("天气为大暴雪，无法起降\n");
        }

        if (wait_time[0] > wait_time[1])
        {
            min_wait_time = wait_time[1];
        }
        else
        {
            min_wait_time = wait_time[0];
        }
        if (control == 1)
        {
            if (control_time > 0)
            {
                control_time -= 5;
                if (control_time <= 0)
                {
                    printf("流量控制结束了。");
                    control_time = 0;
                    control = 0;
                }
                else
                {
                    printf("流量控制还剩%d分钟。", control_time);
                }
            }
        }
        struct flight *temp = F->next;
        while (temp != NULL)
        {
            if (temp->direction == 0)
            {
                temp->left_fuel -= temp->FCPM * 5;
            }
            temp = temp->next;
        }

        // while(1)
        // {
        //     printf("按下任意键继续 . . . \n\n");
        //     int c=getchar();
        //     if(c!=EOF)
        //     {
        //         getchar();
        //         break;
        //     }
        // }

        system("pause");
        printf("5分钟过去了 . . .\n\n");
        printf("--------------------------------------------------------------------------\n");
        count++;
        if (count==SIMULATION_TIME){
            printf("模拟结束！\n");
        }
    }
}

/*
 * Function :priority
 * 函数功能为若f1的优先级大于f2的优先级，则返回；否则返回0
 * 此为第二层优先级：入港（left_time<2t)>出港>入港(left_time>2t)
 *
 */
int priority(struct flight *f1, struct flight *f2) // if f1>f2 return 1
{
    if (f1->direction == 0 && f1->left_fuel / f1->FCPM < 10)
    {
        if (f2->direction == 1)
        {
            return 1;
        }
        else if (f2->left_fuel / f2->FCPM > f1->left_fuel / f1->FCPM)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    if (f1->direction == 0 && f1->left_fuel / f1->FCPM > 10)
    {
        if (f2->direction == 1)
        {
            return 0;
        }
        else if (f2->left_fuel / f2->FCPM < f1->left_fuel / f1->FCPM)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    if (f1->direction == 1)
    {
        if (f2->direction == 1)
        {
            return 1;
        }
        else if (f2->left_fuel / f2->FCPM > 10)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

/*
 * Function :schedule
 * 函数功能为调度航班，返回一个优先级由高到低的航班序列
 * 该函数首先将飞行时间t<min_wait_time的入港航班调往他处降落，从航班序列中删除
 * 在按照紧急程度初始化三个桶，代表非常紧急、紧急、正常
 * 遍历航班，选择对应的紧急程度的桶，在按照第二层优先级插入排序
 * 最后，若无流量控制，则紧急程度为第一优先级，燃料及方向为第二优先级
 * 依次连接三个桶即为最终优先序列
 * 若有流量控制，第一层优先级为航班方向及燃料，遍历三个桶，先依次插入入港方向航班，
 * 再遍历，依次插入出港航班，即为最终优先序列。
 *
 */
struct flight *schedule(struct flight *F, int control, int min_wait_time)
{
    struct flight *bucket[3];

    for (int i = 0; i < 3; i++)
    {
        bucket[i] = (struct flight *)malloc(sizeof(struct flight));
        bucket[i]->next = NULL;
    }

    struct flight *temp = F;
    while (temp->next != NULL)
    {
        if (temp->next->left_fuel / temp->next->FCPM < min_wait_time && temp->next->direction == 0)
        {
            printf("航班号%d的入港飞机燃料不足,被迫前往其他地方降落。\n", temp->next->flight_id);
            temp->next = temp->next->next;
            continue;
        }
        temp = temp->next;
    }

    temp = F->next;
    while (temp != NULL)
    {
        struct flight *p;
        if (temp->status == 1)
        {
            p = bucket[0];
            while (p->next != NULL)
            {
                if (priority(temp, p->next))
                {
                    struct flight *r = (struct flight *)malloc(sizeof(struct flight));
                    r->direction = temp->direction;
                    r->FCPM = temp->FCPM;
                    r->flight_id = temp->flight_id;
                    r->left_fuel = temp->left_fuel;
                    r->num_passenger = temp->num_passenger;
                    r->status = temp->status;
                    r->next = p->next;
                    p->next = r;
                    break;
                }
                else
                {
                    p = p->next;
                }
            }
            if (p->next == NULL)
            {
                struct flight *r = (struct flight *)malloc(sizeof(struct flight));
                r->direction = temp->direction;
                r->FCPM = temp->FCPM;
                r->flight_id = temp->flight_id;
                r->left_fuel = temp->left_fuel;
                r->num_passenger = temp->num_passenger;
                r->status = temp->status;
                r->next = p->next;
                p->next = r;
            }
        }
        if (temp->status == 0)
        {
            p = bucket[1];
            while (p->next != NULL)
            {
                if (priority(temp, p->next))
                {
                    struct flight *r = (struct flight *)malloc(sizeof(struct flight));
                    r->direction = temp->direction;
                    r->FCPM = temp->FCPM;
                    r->flight_id = temp->flight_id;
                    r->left_fuel = temp->left_fuel;
                    r->num_passenger = temp->num_passenger;
                    r->status = temp->status;
                    r->next = p->next;
                    p->next = r;
                    break;
                }
                else
                {
                    p = p->next;
                }
            }
            if (p->next == NULL)
            {
                struct flight *r = (struct flight *)malloc(sizeof(struct flight));
                r->direction = temp->direction;
                r->FCPM = temp->FCPM;
                r->flight_id = temp->flight_id;
                r->left_fuel = temp->left_fuel;
                r->num_passenger = temp->num_passenger;
                r->status = temp->status;
                r->next = p->next;
                p->next = r;
            }
        }
        if (temp->status == -1)
        {
            p = bucket[2];
            while (p->next != NULL)
            {
                if (priority(temp, p->next))
                {
                    struct flight *r = (struct flight *)malloc(sizeof(struct flight));
                    r->direction = temp->direction;
                    r->FCPM = temp->FCPM;
                    r->flight_id = temp->flight_id;
                    r->left_fuel = temp->left_fuel;
                    r->num_passenger = temp->num_passenger;
                    r->status = temp->status;
                    r->next = p->next;
                    p->next = r;
                    break;
                }
                else
                {
                    p = p->next;
                }
            }
            if (p->next == NULL)
            {
                struct flight *r = (struct flight *)malloc(sizeof(struct flight));
                r->direction = temp->direction;
                r->FCPM = temp->FCPM;
                r->flight_id = temp->flight_id;
                r->left_fuel = temp->left_fuel;
                r->num_passenger = temp->num_passenger;
                r->status = temp->status;
                r->next = p->next;
                p->next = r;
            }
        }
        temp = temp->next;
    }

    struct flight *F2 = (struct flight *)malloc(sizeof(struct flight));
    if (control == 0)
    {
        F2 = bucket[0];
        temp = F2;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = bucket[1]->next;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = bucket[2]->next;
        F = F2;
        return F;
    }
    else
    {
        temp = bucket[0]->next;
        while (temp != NULL)
        {
            if (temp->direction == 0)
            {
                tail_add_flight(F2, temp->direction, temp->num_passenger, temp->left_fuel, temp->FCPM, temp->status, temp->flight_id);
            }
            temp = temp->next;
        }
        temp = bucket[1]->next;
        while (temp != NULL)
        {
            if (temp->direction == 0)
            {
                tail_add_flight(F2, temp->direction, temp->num_passenger, temp->left_fuel, temp->FCPM, temp->status, temp->flight_id);
            }
            temp = temp->next;
        }
        temp = bucket[2]->next;
        while (temp != NULL)
        {
            if (temp->direction == 0)
            {
                tail_add_flight(F2, temp->direction, temp->num_passenger, temp->left_fuel, temp->FCPM, temp->status, temp->flight_id);
            }
            temp = temp->next;
        }
        temp = bucket[0]->next;
        while (temp != NULL)
        {
            if (temp->direction == 1)
            {
                tail_add_flight(F2, temp->direction, temp->num_passenger, temp->left_fuel, temp->FCPM, temp->status, temp->flight_id);
            }
            temp = temp->next;
        }
        temp = bucket[1]->next;
        while (temp != NULL)
        {
            if (temp->direction == 1)
            {
                tail_add_flight(F2, temp->direction, temp->num_passenger, temp->left_fuel, temp->FCPM, temp->status, temp->flight_id);
            }
            temp = temp->next;
        }
        temp = bucket[2]->next;
        while (temp != NULL)
        {
            if (temp->direction == 1)
            {
                tail_add_flight(F2, temp->direction, temp->num_passenger, temp->left_fuel, temp->FCPM, temp->status, temp->flight_id);
            }
            temp = temp->next;
        }
        F = F2;
        return F;
    }
}