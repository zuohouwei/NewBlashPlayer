//
// Created by liuenbao on 18-9-15.
//

#include "NBMainApp.h"
#include "NBMainWindow.h"

NBMainApp::NBMainApp() {

}

NBMainApp::~NBMainApp() {

}

int NBMainApp::initialize(int argc, char *argv[]) {
    XApp::initialize(argc, argv);
    window = new NBMainWindow("Xgui::Button", Xgui::Rect(10, 10, 640, 480));
    return 0;
}

void NBMainApp::terminate() {
    XApp::terminate();
}