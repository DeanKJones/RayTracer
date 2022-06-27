#pragma once
#include "Application.h"

bool g_ApplicationRunning = true;

int main(int argc, char **argv) {
  while (g_ApplicationRunning) {
    Core::Application *app = Core::CreateApplication(argc, argv);
    app->Run();
    delete app;
  }

  return 0;
}