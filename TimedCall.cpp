﻿#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> f, uint32_t time)
{ 
	std::function<void(void)> f = f;
	time_ = time;
}

void TimedCall::Update()
{ 
	if (isComp)
	{
		return;
	}
	time_--;
	if (time_ <= 0)
	{
		isComp = true;
		// コールバック関数呼び出し
		f();
	}
};