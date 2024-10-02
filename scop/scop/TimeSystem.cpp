#include "pch.h"
#include "TimeSystem.h"

TimeSystem::TimeSystem()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&this->frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this->prevCount)); // CPU Å¬·°
}

TimeSystem::~TimeSystem()
{
}

void TimeSystem::Update()
{
	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	this->deltaTime = (currentCount - this->prevCount) / static_cast<float>(this->frequency);
	this->prevCount = currentCount;

	this->frameCount++;
	this->frameTime += this->deltaTime;

	if (this->frameTime > 1.f)
	{
		this->fps = static_cast<uint32>(this->frameCount / this->frameTime);

		this->frameTime = 0.f;
		this->frameCount = 0;
	}
}