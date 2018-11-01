//
// Created by liu enbao on 24/09/2018.
//

#ifndef NBMEDIACODECVRENDERER_H
#define NBMEDIACODECVRENDERER_H

#include "NBGLVideoRenderer.h"

class NBMediaCodecVRenderer : public NBGLVideoRenderer {
public:
    NBMediaCodecVRenderer();
    ~NBMediaCodecVRenderer();

public:
    virtual nb_status_t displayFrameImpl(NBMediaBuffer* mediaBuffer, int tgtWidth, int tgtHeight);

    virtual void invalidate();

public:
    virtual nb_status_t start(NBMetaData* metaData = NULL);
    virtual nb_status_t stop();
};


#endif //NBMEDIACODECVRENDERER_H
