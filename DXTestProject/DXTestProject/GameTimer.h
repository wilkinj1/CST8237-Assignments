#pragma once

class GameTimer
{
public:
  GameTimer();
  ~GameTimer();

  float GetDeltaTime();

  void Start();
  void Stop();
  void Pause();
  void Reset();

  void Tick();

  bool IsStopped();
  bool IsPaused();

protected:
  long long mCurrentTime;
  long long mPreviousTime;

  float mSecondsPerTick;
  float mDeltaTime;

  bool mIsStopped;
  bool mIsPaused;
};