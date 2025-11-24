#pragma once
#include <pch.hpp>

class ScopedTimer
{
public:
    explicit ScopedTimer(const std::string& label)
        : m_Label(label), m_Start(std::chrono::high_resolution_clock::now()) {
    }

    ~ScopedTimer()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto durationMs = std::chrono::duration<float, std::milli>(end - m_Start).count();

        spdlog::info("[{}]: {:.2f} ms", m_Label, durationMs);
    }

private:
    std::string m_Label;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};
