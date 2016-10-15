#include "Timer.h"



Timer::Timer()
{
	_startTicks = 0;
	_pausedTicks = 0;

	_paused = false;
	_started = false;
}


Timer::~Timer()
{
}


void Timer::start()
{
	//start the timer
	_started = true;

	//unpause the timer
	_paused = false;

	//get the current clock time
	_startTicks = SDL_GetTicks();
	_pausedTicks = 0;
}

void Timer::stop()
{
	//stop the timer
	_started = false;

	//unpause the timer
	_paused = false;

	//Clear the tick variables
	_startTicks = 0;
	_pausedTicks = 0;
}

void Timer::pause()
{
	if (_started && !_paused)
	{
		//pause the timer
		_paused = true;

		//calculate the paused ticks
		_pausedTicks = SDL_GetTicks() - _startTicks;
		_startTicks = 0;
	}
}

void Timer::unpause()
{
	if (_started && _paused)
	{
		//unpause the timer;
		_paused = false;

		//reset the starting ticks
		_startTicks = SDL_GetTicks() - _pausedTicks;

		//reset the paused ticks
		_pausedTicks = 0;
	}
}

Uint32 Timer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

	//if the timer is running
	if (_started)
	{
		if (_paused)
		{
			time = _pausedTicks;
		}
		else
		{
			time = SDL_GetTicks() - _startTicks;
		}
	}

	return time;
}

bool Timer::isPaused()
{
	return _paused && _started;
}

bool Timer::isStarted()
{
	return _started;
}