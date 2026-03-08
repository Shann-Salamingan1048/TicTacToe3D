#pragma once


namespace ShannUtilities
{
    struct Time
    {
        static inline float deltaTime{0.0f};
        static inline float lastFrame{0.0f};
        static inline float currentFrame{0.0f};
    };
}