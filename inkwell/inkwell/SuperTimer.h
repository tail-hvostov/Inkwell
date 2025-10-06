#ifndef _SUPER_TIMER_H_
#define _SUPER_TIMER_H_

#include <functional>
#include <Windows.h>

class SuperTimer {
private:
	std::function<void()> executor;
	UINT delay;
	UINT_PTR timer;
public:
	SuperTimer(const std::function<void()>& executor, UINT delay);
	SuperTimer(const std::function<void()>& executor, UINT delay, bool is_active);
	void execute();
	void stop();
	void start();
	~SuperTimer();
};

#endif