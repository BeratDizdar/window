#include "window.h"
#define GLAD_GL_IMPLEMENTATION
#include "depen/gl.h"
#include "depen/glfw/glfw3.h"

#if defined(_WIN32)
    #define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__)
    #define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include "depen/glfw/glfw3native.h"
#include <stdio.h>
#include <stdlib.h>

static uint8_t key_curr[GLFW_KEY_LAST + 1];
static uint8_t key_prev[GLFW_KEY_LAST + 1];
static uint8_t mouse_curr[GLFW_MOUSE_BUTTON_LAST + 1];
static uint8_t mouse_prev[GLFW_MOUSE_BUTTON_LAST + 1];

typedef struct Window {
    GLFWwindow *window;
} Window;

IWindow Window_Create(const char *title, int width, int height) {
    glfwInit();
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    IWindow w = malloc(sizeof(Window));
    w->window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!w->window) {
        fprintf(stderr, "[Window]: glfwCreateWindow!\n");
        glfwTerminate();
        free(w);
        return NULL;
    }

    return w;
}

void *Window_GetPtr(IWindow w) {
#if defined(_WIN32)
    return (void*glfwGetWin32Window(w->window);
#elif defined(__APPLE__)
    return (void*)glfwGetCocoaWindow(w->window);
#else
    return NULL;
#endif
}

int Window_CreateVulkanSurface(IWindow w, void *vk_instance, void *out_vk_surface) {
    return glfwCreateWindowSurface((VkInstance)vk_instance, w->window, NULL, (VkSurfaceKHR*)out_vk_surface) == 0;
}

int Window_ShouldClose(IWindow w) {
    return glfwWindowShouldClose(w->window);
}

void Window_PollEvents(IWindow w) {
    glfwPollEvents();
    memcpy(key_prev, key_curr, sizeof(key_curr));
    for (int k = 0; k <= GLFW_KEY_LAST; ++k) {
        key_curr[k] = (glfwGetKey(w->window, k) == GLFW_PRESS);
    }

    memcpy(mouse_prev, mouse_curr, sizeof(mouse_curr));
    for (int b = 0; b <= GLFW_MOUSE_BUTTON_LAST; ++b) {
        mouse_curr[b] = (glfwGetMouseButton(w->window, b) == GLFW_PRESS);
    }
}

void Window_GetMousePos(IWindow w, double *mx, double *my) {
    glfwGetCursorPos(w->window, mx, my);
}

int Window_MouseDown(int b) {
    if (b < 0 || b > GLFW_MOUSE_BUTTON_LAST) return 0;
    return mouse_curr[b];
}

int Window_MousePressed(int b) {
    if (b < 0 || b > GLFW_MOUSE_BUTTON_LAST) return 0;
    return mouse_curr[b] && !mouse_prev[b];
}

int Window_MouseReleased(int b) {
    if (b < 0 || b > GLFW_MOUSE_BUTTON_LAST) return 0;
    return !mouse_curr[b] && mouse_prev[b];
}

int Window_KeyDown(int k) {
    return key_curr[k];
}

int Window_KeyPressed(int k) {
    return key_curr[k] && !key_prev[k];
}

int Window_KeyReleased(int k) {
    return !key_curr[k] && key_prev[k];
}

void Window_SendDestroyMessage(IWindow w) {
    glfwSetWindowShouldClose(w->window, GLFW_TRUE);
}

void Window_Destroy(IWindow w) {
    glfwDestroyWindow(w->window);
    glfwTerminate();
    free(w);
}

uint64_t Window_OS_GetTime() {
    return (glfwGetTimerValue() * 1000000ULL) / glfwGetTimerFrequency();
}

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
void Window_OS_Sleep(uint32_t ms) {
    Sleep(ms);
}
#else
#include <unistd.h>
void Window_OS_Sleep(uint32_t ms) {
    usleep(ms * 1000);
}
#endif
