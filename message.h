#ifndef MESSAGE_H
#define MESSAGE_H
#include <QVector>
//message for signal in histMatching GUI

class message
{
public:
    message();
    message(int,int,double);
    bool getReset();
    int getMax();
    int getMin();
    double getMiddle();
    void setReset(bool);
    void setMiddle(int);
    void setMin(int);
    void setMax(int);
    void setAlphaMin(double);
    void setAlphaMax(double);
    void setalphafactor(QVector<double>& );
    double getalpha();
    double getbeta();
    QVector<double> getalphaptr();
    void clearMessage();

private:
    double middle;
    int max;
    int min;
    bool reset;
    double alpha_min;
    double alpha_max;
    QVector<double> alphafactor;
};

#endif // MESSAGE_H
