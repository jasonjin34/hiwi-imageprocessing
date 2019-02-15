#include "message.h"

message::message()
{
    this->min = 0;
    this->max = 255;
    this->middle = 0.5;
}

message::message(int min, int max, double middle){
    this->min = min;
    this->max = max;
    this->middle = middle;
}

bool message::getReset()
{
    return this->reset;
}

int message::getMin(){
    return this->min;
}

double message::getMiddle()
{
    return this->middle;
}

void message::setReset(bool status)
{
    this->reset = status;
}

void message::setMiddle(int middle)
{
    this->middle = static_cast<double>(middle/255.0);
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
