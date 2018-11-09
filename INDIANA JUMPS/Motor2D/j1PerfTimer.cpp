// ----------------------------------------------------
// j1PerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "j1PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 j1PerfTimer::frequency = 0;

// ---------------------------------------------
j1PerfTimer::j1PerfTimer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	Start();
}

// ---------------------------------------------
void j1PerfTimer::Start()
{
	started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
double j1PerfTimer::ReadMs() const
{
	return (double(SDL_GetPerformanceCounter() - started_at) / double(frequency)) * 1000.0;
}

// ---------------------------------------------
uint64 j1PerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}