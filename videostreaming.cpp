#include "videostreaming.h"
#include "ui_videostreaming.h"
#include <QLibrary>
#include <Windows.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/stitching.hpp>
#include <QDebug>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}


Videostreaming::Videostreaming(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Videostreaming)
{
    ui->setupUi(this);
    //connect(ui->pushButton,SIGNAL(clicked()),this, SLOT(on_pushButton_clicked()));
}

Videostreaming::~Videostreaming()
{
    delete ui;
}

void Videostreaming::on_pushButton_clicked()
{
    //avformat functions
    typedef void (*av_register_all) (void);
    typedef int (*av_read_frame) (AVFormatContext *s, AVPacket* pkt);
    typedef int (*avformat_open_input) (AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options);
    typedef void (*avformat_close_input) (AVFormatContext **s);
    typedef int (*avformat_find_stream_info) (AVFormatContext *ic, AVDictionary **options);
    typedef void (*avformat_network_init)(void);
    typedef AVFormatContext* (*avformat_alloc_context)(void);
    typedef unsigned (*avformat_version)(void);
    typedef int (*av_read_play)(AVFormatContext*);
    typedef attribute_deprecated int (*avpicture_get_size)(enum AVPixelFormat pix_fmt, int width, int height);
    typedef int (*av_find_best_stream) (AVFormatContext *ic, enum AVMediaType type, int wanted_stream_nb, int related_stream, AVCodec **decoder_ret, int flags);
    typedef void (*avformat_close_input)(AVFormatContext **s);

    //avcodec functions
    typedef const char* (*avcodec_configuration)(void);
    typedef void (*av_init_packet)(AVPacket *pkt);
    typedef AVCodecContext* (*avcodec_alloc_context3)(const AVCodec* codec);
    typedef AVCodec* (*avcodec_find_decoder)(enum AVCodecID id);
    typedef int (*avcodec_get_context_default3)(AVCodecContext*s, const AVCodec* codec);
    typedef attribute_deprecated int (*avcodec_copy_context)(AVCodecContext*dest, const AVCodecContext*src);
    typedef int (*avcodec_open2)(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);
    typedef int (*avpicture_fill)(AVPicture *picture, const uint8_t *ptr, enum AVPixelFormat pix_fmt, int width, int height);
    typedef int (*avcodec_decode_video2)(AVCodecContext* avctx,AVFrame* picture, int* got_picture_ptr, const AVPacket* avpkt);
    typedef attribute_deprecated void (*av_free_packet)(AVPacket* pkt);
    typedef int  (*avcodec_close) (AVCodecContext *avctx);

    //swscale function
    typedef struct SwsContext* (*sws_getContext)(int srcW, int srcH, enum AVPixelFormat srcFormat, int dstW, int dstH, enum AVPixelFormat dstFormat, int flags, SwsFilter *srcFilter, SwsFilter *dstFilter, const double *param);
    typedef int (*sws_scale)(struct SwsContext *c, const uint8_t *const srcSlice[], const int srcStride[], int srcSliceY, int srcSliceH, uint8_t *const dst[], const int dstStride[]);
    typedef struct SwsContext* (*sws_getCachedContext)(	struct SwsContext * 	context,
                                                        int 	srcW,
                                                        int 	srcH,
                                                        enum AVPixelFormat 	srcFormat,
                                                        int 	dstW,
                                                        int 	dstH,
                                                        enum AVPixelFormat 	dstFormat,
                                                        int 	flags,
                                                        SwsFilter * 	srcFilter,
                                                        SwsFilter * 	dstFilter,
                                                        const double * 	param
                                                        );

    //avutil function
    typedef AVFrame* (*av_frame_alloc)(void);
    typedef void (*av_frame_free)(AVFrame** frame);
    typedef double (*av_q2d)(AVRational a);
    typedef int64_t (*av_rescale_q) (int64_t a, AVRational bq, AVRational cq) av_const;
    typedef const char * (*av_get_pix_fmt_name) (enum AVPixelFormat pix_fmt);

    //set my functions'name
    static av_register_all registerAll;
    static av_read_frame readFrame;
    static avformat_open_input openInput;
    static avformat_close_input closeInput;
    static avformat_find_stream_info findStreamInfo;
    static avformat_network_init initStreamNetwork;
    static avpicture_get_size avpictureGetSize;
    static av_find_best_stream avFindBestStream;
    static avformat_close_input avformatClose_input;

    avformat_alloc_context contextAlloc;
    static avcodec_configuration Configuration;
    static avformat_version avformatVersion;
    static av_init_packet initavPacket;
    static av_read_play avReadplay;
    avcodec_alloc_context3 avcodecAlloccontext;
    static avcodec_find_decoder avCodecfindDecoder;
    static avcodec_get_context_default3 avcodeGetContextDefault3;
    static avcodec_copy_context avcodecCopyContext;
    static avcodec_open2 avcodecOpen;
    static avpicture_fill avpictureFill;
    static avcodec_decode_video2 avcodecDecode_video2;
    static av_free_packet avFree_packet;
    static  avcodec_close  avcodecClose;

    static sws_getContext swsGetContext;
    static sws_scale swsScale;
    static sws_getCachedContext swsGetCachedContext;

    static av_frame_alloc avFrameAlloc;
    static av_frame_free avFrame_free;
    static av_q2d avQ2d;
    static av_rescale_q avRescaleq;
    static av_get_pix_fmt_name avGet_pix_fmt_name;

    SetDllDirectory(L"C:/HIWI/VideoStreaming/Videostreaming");

    QLibrary avformat("avformat-58.dll");
    QLibrary avcodec("avcodec-58.dll");
    QLibrary swscale("swscale-5.dll");
    QLibrary avutil("avutil-56.dll");

    bool status_avformat, status_avcodec, status_swscale, status_avutil;
    status_avformat = avformat.load();
    status_avcodec = avcodec.load();
    status_swscale = swscale.load();
    status_avutil = avutil.load();

    if(status_avformat && status_avcodec && status_swscale && status_avutil)
    {
        //assign avformat functions
        registerAll = static_cast<av_register_all>(avformat.resolve("av_register_all"));
        readFrame = (av_read_frame)avformat.resolve("av_read_frame");
        openInput = (avformat_open_input)avformat.resolve("avformat_open_input");
        closeInput = (avformat_close_input)avformat.resolve("avformat_close_input");
        findStreamInfo = (avformat_find_stream_info)avformat.resolve("avformat_find_stream_info");
        initStreamNetwork = (avformat_network_init)avformat.resolve("avformat_network_init");
        contextAlloc =(avformat_alloc_context)avformat.resolve("avformat_alloc_context");
        avformatVersion = (avformat_version)avformat.resolve("avformat_version");
        avReadplay = (av_read_play)avformat.resolve("av_read_play");
        avpictureGetSize = (avpicture_get_size)avcodec.resolve("avpicture_get_size");
        avFindBestStream = (av_find_best_stream)avformat.resolve("av_find_best_stream");
        avformatClose_input =(avformat_close_input)avformat.resolve("avformat_close_input");

        //assign avcodec functions
        Configuration = (avcodec_configuration)avcodec.resolve("avcodec_configuration");
        initavPacket = (av_init_packet)avcodec.resolve("av_init_packet");
        avcodecAlloccontext = (avcodec_alloc_context3)avcodec.resolve("avcodec_alloc_context3");
        avCodecfindDecoder = (avcodec_find_decoder)avcodec.resolve("avcodec_find_decoder");
        avcodeGetContextDefault3 = (avcodec_get_context_default3)avcodec.resolve("avcodec_get_context_default3");
        avcodecCopyContext = (avcodec_copy_context)avcodec.resolve("avcodec_copy_context");
        avcodecOpen = (avcodec_open2)avcodec.resolve("avcodec_open2");
        avpictureFill = (avpicture_fill)avcodec.resolve("avpicture_fill");
        avcodecDecode_video2 = (avcodec_decode_video2)avcodec.resolve("avcodec_decode_video2");
        avFree_packet = (av_free_packet)avcodec.resolve("av_free_packet");
        avcodecClose = (avcodec_close)avcodec.resolve("avcodec_close");

        //assign swscale function
        swsGetContext = (sws_getContext)swscale.resolve("sws_getContext");
        swsScale = (sws_scale)swscale.resolve("sws_scale");
        swsGetCachedContext= (sws_getCachedContext)swscale.resolve("sws_getCachedContext");

        //assign avutil functions
        avFrameAlloc = (av_frame_alloc)avutil.resolve("av_frame_alloc");
        avQ2d = (av_q2d)avutil.resolve("av_q2d");
        avRescaleq = (av_rescale_q)avutil.resolve("av_rescale_q");
        avGet_pix_fmt_name = (av_get_pix_fmt_name)avutil.resolve("av_get_pix_fmt_name");
        avFrame_free = (av_frame_free)avutil.resolve("av_frame_free");


        //check avformat version
        QString formatS = QString("the version of avformat: %1").arg(avformatVersion());
        qDebug() << formatS;

        /*********************************get frame image code**********************************/
        registerAll();
        initStreamNetwork();
        int ret;

        //set the url link
        int option = 2; // 1, local video, 2, rtsp protocal
        const char* url;
        if(option == 1) url = "C:/HIWI/VideoStreaming/Videostreaming/testvideo.mp4";
        else url = "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";

        // open input file context
        AVFormatContext* inctx = nullptr;
        ret = openInput(&inctx, url, nullptr, nullptr);
        if (ret < 0) {
            std::cerr << "fail to avforamt_open_input(\"" << url << "\"): ret=" << ret;
            qDebug() << "failed";
        }
        // retrive input stream information
        ret = findStreamInfo(inctx, nullptr);
        if (ret < 0) {
            std::cerr << "fail to avformat_find_stream_info: ret=" << ret;
             qDebug() << "failed";
        }

        // find primary video stream
        AVCodec* vcodec = nullptr;
        ret = avFindBestStream(inctx, AVMEDIA_TYPE_VIDEO, -1, -1, &vcodec, 0);
        if (ret < 0) {
            std::cerr << "fail to av_find_best_stream: ret=" << ret;
             qDebug() << "failed";
        }
        const int vstrm_idx = ret;
        AVStream* vstrm = inctx->streams[vstrm_idx];

        // open video decoder context
        ret = avcodecOpen(vstrm->codec, vcodec, nullptr);
        if (ret < 0) {
            std::cerr << "fail to avcodec_open2: ret=" << ret;
           qDebug() << "failed";
        }

        // print input video stream informataion
        std::cout
            << "infile: " << url << "\n"
            << "format: " << inctx->iformat->name << "\n"
            << "vcodec: " << vcodec->name << "\n"
            << "size:   " << vstrm->codec->width << 'x' << vstrm->codec->height << "\n"
            << "frame:  " << vstrm->nb_frames << "\n"
            << std::flush;

        // initialize sample scaler
        const int dst_width = vstrm->codec->width;
        const int dst_height = vstrm->codec->height;
        const AVPixelFormat dst_pix_fmt = AV_PIX_FMT_BGR24;
        SwsContext* swsctx = swsGetCachedContext(
            nullptr, vstrm->codec->width, vstrm->codec->height, vstrm->codec->pix_fmt,
            dst_width, dst_height, dst_pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
        if (!swsctx) {
            std::cerr << "fail to sws_getCachedContext";
            qDebug() << "failed";
        }

        std::cout << "output: " << dst_width << 'x' << dst_height << ',' << avGet_pix_fmt_name(dst_pix_fmt) << std::endl;

        // allocate frame buffer for output
        AVFrame* frame = avFrameAlloc();
        std::vector<uint8_t> framebuf(avpictureGetSize(dst_pix_fmt, dst_width, dst_height));
        avpictureFill(reinterpret_cast<AVPicture*>(frame), framebuf.data(), dst_pix_fmt, dst_width, dst_height);


        // decoding loop
        AVFrame* decframe = avFrameAlloc();
        unsigned nb_frames = 0;
        bool end_of_stream = false;
        int got_pic = 0;
        AVPacket pkt;

        do {
            if (!end_of_stream) {
                // read packet from input file
                ret = readFrame(inctx, &pkt);
                if (ret < 0 && ret != AVERROR_EOF) {
                    std::cerr << "fail to av_read_frame: ret=" << ret;
                     qDebug() << "failed";
                }
                if (ret == 0 && pkt.stream_index != vstrm_idx)
                    goto next_packet;
                end_of_stream = (ret == AVERROR_EOF);
            }

            if (end_of_stream) {
                // null packet for bumping process
                initavPacket(&pkt);
                pkt.data = nullptr;
                pkt.size = 0;
            }

            // decode video frame
            avcodecDecode_video2(vstrm->codec, decframe, &got_pic, &pkt);
            if (!got_pic)
                goto next_packet;

            // convert frame to OpenCV matrix
            swsScale(swsctx, decframe->data, decframe->linesize, 0, decframe->height, frame->data, frame->linesize);
            {
            cv::Mat image(dst_height, dst_width, CV_8UC3, framebuf.data(), frame->linesize[0]);
            cv::imshow("press ESC to exit", image);
            if (cv::waitKey(1) == 0x1b)
                break;
            }
            std::cout << nb_frames << '\r' << std::flush;  // dump progress
            ++nb_frames;

    next_packet:
            avFree_packet(&pkt);

        } while (!end_of_stream || got_pic);

        std::cout << nb_frames << " frames decoded" << std::endl;

        avFrame_free(&decframe);
        avFrame_free(&frame);
        avcodecClose(vstrm->codec);
        avformatClose_input(&inctx);

        qDebug() << "pass";
    }
    else {
        qDebug() << "fail load library";
    }
}


