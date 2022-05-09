#pragma once

namespace triton {
    typedef struct {
        int x;
        int y;
    } Vector;

    typedef struct {
        int width;
        int height;
    } Size;

    typedef struct {
        Vector* position;
        Size* size;
    } Rect;
}
