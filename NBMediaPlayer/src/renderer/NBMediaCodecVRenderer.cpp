//
// Created by liu enbao on 24/09/2018.
//

#include "NBMediaCodecVRenderer.h"
#include <NBLog.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libavformat/avformat.h>

#ifdef __cplusplus
}
#endif

#define LOG_TAG "NBMediaCodecVRenderer"

NBMediaCodecVRenderer::NBMediaCodecVRenderer() {

}

NBMediaCodecVRenderer::~NBMediaCodecVRenderer() {

}

nb_status_t NBMediaCodecVRenderer::displayFrameImpl(NBMediaBuffer* mediaBuffer, int tgtWidth, int tgtHeight) {
//    NBLOG_INFO(LOG_TAG, "display frame everyframe");
    AVFrame* frame = (AVFrame*)mediaBuffer->data();

    //set flags to 1 show this frame
    frame->flags = 1;
    return OK;
}

void NBMediaCodecVRenderer::invalidate() {

}

nb_status_t NBMediaCodecVRenderer::start(NBMetaData* metaData) {
    //call base start first
    NBGLVideoRenderer::start(metaData);
    return OK;
}

nb_status_t NBMediaCodecVRenderer::stop() {
    //call base stop first
    NBGLVideoRenderer::stop();
    return OK;
}