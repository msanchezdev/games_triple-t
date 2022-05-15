#pragma once

namespace triton {
    class Stopwatch {
    private:
        int start_time;
        int end_time;
        int elapsed_time;
        bool running;

    public:
        void Start();
        void Stop();
        void Reset();
        int GetElapsedTime();
    };
}
