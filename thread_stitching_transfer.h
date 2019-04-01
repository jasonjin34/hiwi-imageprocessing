#ifndef THREAD_STITCHING_TRANSFER_H
#define THREAD_STITCHING_TRANSFER_H
#include <QtCore>
#include "image_stitching.h"

class Thread_stitching_transfer : public QThread
{
public:
    Thread_stitching_transfer();
    Thread_stitching_transfer(Image_stitching*);
    void run();
private:
    Image_stitching* imageStitching_gui;

};

#endif // THREAD_STITCHING_TRANSFER_H
