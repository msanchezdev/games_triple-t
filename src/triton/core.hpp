#pragma once

#include "app.hpp"
#include "resources.hpp"

void TRT_Initialize();
void TRT_InitializeVideo();
void TRT_InitializeGameWindow();
void TRT_InitializeImages();
void TRT_RenderLoop();
void TRT_EventLoop();
void TRT_Shutdown();
void TRT_Debug_PrintInitializedSubsystems();
