/*
 * Copyright (c) 2026 Berat Dizdar
 * Distributed under the zlib License. See LICENSE for details.
 */
#pragma once
#include <stdint.h>

#define MOUSE_BUTTON_LEFT   0
#define MOUSE_BUTTON_RIGHT  1
#define MOUSE_BUTTON_MIDDLE 2

typedef struct Window *IWindow;

IWindow  Window_Create(const char *title, int width, int height);
void     Window_Destroy(IWindow w);
int      Window_CreateVulkanSurface(IWindow w, void *vk_instance, void *out_vk_surface);

int      Window_ShouldClose(IWindow w);
void     Window_PollEvents(IWindow w);
void     Window_SendDestroyMessage(IWindow w);

void     *Window_GetPtr(IWindow w);
void     Window_GetMousePos(IWindow w, double *mx, double *my);
int      Window_MouseDown(int b);
int      Window_MousePressed(int b);
int      Window_MouseReleased(int b);
int      Window_KeyDown(int k);
int      Window_KeyPressed(int k);
int      Window_KeyReleased(int k);

uint64_t Window_OS_GetTime();
void     Window_OS_Sleep(uint32_t ms);
