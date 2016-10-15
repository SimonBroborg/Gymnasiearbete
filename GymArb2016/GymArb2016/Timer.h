#pragma once
#include <sstream>
#include <stdio.h>
#include <SDL\SDL_ttf.h>
#include <string>

class Timer
{
public:
	//Initializes variables
	Timer();
	~Timer();

	//Different timer actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timers time
	Uint32 getTicks();

	//checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when timer is started
	Uint32 _startTicks;

	//The ticks stored when the timer is paused
	Uint32 _pausedTicks;

	//the timer status 
	bool _paused;
	bool _started;
};

