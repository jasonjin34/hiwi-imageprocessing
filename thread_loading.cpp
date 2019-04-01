#include "thread_loading.h"
#include <QtCore>
#include <QDebug>

Thread_loading::Thread_loading()
{

}

Thread_loading::Thread_loading(Image_stitching* gui)
{
    this->imageStitching_gui = gui;
}

void Thread_loading::run()
{
    QMutex mutex;
    mutex.lock();
    imageStitching_gui->transfer_processing_function();
    mutex.unlock();

}
