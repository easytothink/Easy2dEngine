#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void Restart() {
		pass_time = 0;
		shotted = false;
	}

	void SetWaitTime(float val) {
		wait_time = val;
	}

	void SetOneShot(bool val) {
		one_shot = val;
	}

	void SetOnTimeout(std::function<void()> on_timeout) {
		this->on_timeout = on_timeout;
	}

	void Pause() {
		paused = true;
	}

	void Resume() {
		paused = false;
	}

	void OnUpdate(float ticks) {
		if (paused) return;

		pass_time += ticks;
		if (pass_time >= wait_time) {
			bool can_shot = (!one_shot || (one_shot && !shotted));
			shotted = true;
			if (can_shot && on_timeout)
				on_timeout();
			pass_time -= wait_time;
		}
	}

private:
	float pass_time = 0;
	float wait_time = 0;
	bool paused = false;
	bool shotted = false;
	bool one_shot = false;
	std::function<void()> on_timeout;
};

#endif // !_TIMER_H_