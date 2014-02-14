#include "stdafx.h"
#include "GameTimer.h"

GameTimer::GameTimer()
{
  // Get how many ticks occur each second.
  long long ticksPerSecond;
  QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond);

  mSecondsPerTick = 1.0f / ticksPerSecond;
}

GameTimer::~GameTimer()
{
}

void GameTimer::Tick()
{
  if(!mIsStopped && !mIsPaused)
  {
    long long currentTime;
    QueryPerformanceCounter((LARGE_INTEGER *)&currentTime);

    mCurrentTime = currentTime;
    mDeltaTime = ( mCurrentTime - mPreviousTime ) * mSecondsPerTick;

    mPreviousTime = mCurrentTime;
  }
}

float GameTimer::GetDeltaTime()
{
  return mDeltaTime;
}

void GameTimer::Start()
{
  mIsStopped = false;
  mIsPaused = false;

  Reset();
}

bool GameTimer::IsStopped()
{
  return mIsStopped;
}

bool GameTimer::IsPaused()
{
  return mIsPaused;
}

void GameTimer::Stop()
{
  mIsStopped = true;
}

void GameTimer::Pause()
{
  mIsPaused = true;
}

void GameTimer::Reset()
{
  long long currentTime;
  QueryPerformanceCounter((LARGE_INTEGER *)&currentTime);

  mCurrentTime = currentTime;
  mPreviousTime = currentTime;
}