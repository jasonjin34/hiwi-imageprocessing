#include "message.h"

message::message()
{
    this->min = 0;
    this->max = 255;
}

message::message(int min, int max){
    this->min = min;
    this->max = max;
}

int message::getMin(){
    return this->min;
}

int message::getMax(){
    return this->max;
}

void message::setMin(int min){
    this->min = min;
}

void message::setMax(int max){
    this->max = max;
}

void message::setAlphaMin(double min)
{
    this->alpha_min = min;
}

void message::setAlphaMax(double max)
{
    this->alpha_max = max;
}

double message::getalpha()
{
    return this->alpha_min;
}

double message::getbeta()
{
    return this->alpha_max;
}
