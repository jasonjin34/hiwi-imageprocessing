#ifndef THREAD_LOADING_H
#define THREAD_LOADING_H
#include <QtCore>
#include "image_stitching.h"

class Thread_loading : public QThread
{
public:
    Thread_loading();
    Thread_loading(Image_stitching*);
    void run();
    QString input;
private:
    Image_stitching* imageStitching_gui;
};

#endif // THREAD_LOADING_H
