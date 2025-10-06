#include "SuperTimer.h"

#include <map>

std::map<UINT_PTR, SuperTimer*> timer_map;

VOID CALLBACK timer_callback(HWND hwnd, UINT unnamedParam2,
					UINT_PTR id, DWORD unnamedParam4) {
	if (timer_map.count(id)) {
		SuperTimer* timer = timer_map[id];
		timer->execute();
	}
}

SuperTimer::SuperTimer(const std::function<void()>& executor,UINT delay) {
	this->executor = executor;
	this->delay = delay;
	timer = SetTimer(NULL, NULL, delay, timer_callback);
	if (timer != NULL) {
		timer_map[timer] = this;
	}
}

SuperTimer::SuperTimer(const std::function<void()>& executor,UINT delay, bool is_active) {
	this->executor = executor;
	this->delay = delay;
	timer = NULL;
	if (is_active) {
		start();
	}
}

void SuperTimer::execute() {
	if (executor != nullptr) {
		executor();
	}
}

void SuperTimer::stop() {
	if (timer != NULL) {
		timer_map.erase(timer);
		KillTimer(NULL, timer);
		timer = NULL;
	}
}

void SuperTimer::start() {
	stop();
	timer = SetTimer(NULL, NULL, delay, timer_callback);
	if (timer != NULL) {
		timer_map[timer] = this;
	}
}

SuperTimer::~SuperTimer() {
	stop();
}