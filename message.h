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
    void setAlphaMin(double);
    void setAlphaMax(double);
    double getalpha();
    double getbeta();
private:
    int max;
    int min;
    double alpha_min;
    double alpha_max;
};

#endif // MESSAGE_H
