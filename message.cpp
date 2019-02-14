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

bool message::getReset()
{
    return this->reset;
}

int message::getMin(){
    return this->min;
}

void message::setReset(bool status)
{
    this->reset = status;
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

void message::setalphafactor(QVector<double>& vectorptr)
{
    this->alphafactor = vectorptr;
}

double message::getalpha()
{
    return this->alpha_min;
}

double message::getbeta()
{
    return this->alpha_max;
}

QVector<double> message::getalphaptr()
{
    return this->alphafactor;
}

void message::clearMessage()
{
    this->setMax(0);
    this->setAlphaMax(0.0);
    this->setAlphaMin(0.0);
    this->setMin(0);
    alphafactor.clear();
    this->setReset(false);
}
