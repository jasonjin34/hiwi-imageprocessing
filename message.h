#ifndef MESSAGE_H
#define MESSAGE_H
//message for signal in histMatching GUI

class message
{
public:
    message();
    message(int,int);
    int getMax();
    int getMin();
    void setMin(int);
    void setMax(int);
private:
    int max;
    int min;
};

#endif // MESSAGE_H
