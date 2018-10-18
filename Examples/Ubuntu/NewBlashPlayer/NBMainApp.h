//
// Created by liuenbao on 18-9-15.
//

#ifndef NBMAINAPP_H
#define NBMAINAPP_H

#include <Xgui>

using namespace Xgui;

class NBMainApp : public XApp {
public:
    NBMainApp();
    ~NBMainApp();

public:
    virtual int initialize(int argc, char *argv[]);
    virtual void terminate();
};

#endif //NBMAINAPP_H
