#include "thread_stitching_transfer.h"

Thread_stitching_transfer::Thread_stitching_transfer()
{

}

Thread_stitching_transfer::Thread_stitching_transfer(Image_stitching * gui)
{
    this->imageStitching_gui = gui;
}

void Thread_stitching_transfer::run()
{
    qDebug() << "Stitching processing";
    imageStitching_gui->transfer_function();
}
