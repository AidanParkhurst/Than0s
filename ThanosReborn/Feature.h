#pragma once
class Feature {
protected:
	bool toggled;
	bool running;
public:
	Feature() {
		toggled = true;
		running = true;
	}

	virtual void Run() {
	}

	virtual void Toggle() {
		toggled = !toggled;
	}

	virtual void Stop() {
		running = false;
	}
};