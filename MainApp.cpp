// MainApp.cpp
#include "MainApp.h"

bool MainApp::OnInit() {
    auto* frame = new MainFrame();
    frame->Show(true);
    return true;
}