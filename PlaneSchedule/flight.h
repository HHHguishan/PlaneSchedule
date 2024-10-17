#ifndef __FLIGHT_H__
#define __FLIGHT_H__

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

void head_add_flight(struct flight* F ,int Direction,int NumPassenger,double LeftFuel,double fcpm,int Status,int FlightNumber);
void tail_add_flight(struct flight* F ,int Direction,int NumPassenger,double LeftFuel,double fcpm,int Status,int FlightNumber);


#endif