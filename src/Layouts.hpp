#pragma once

#include <vector>
#include <algorithm>
#include "Window.hpp"
#include "Types.hpp"

struct ILayout {
	virtual ~ILayout() {}

	virtual void AddWindow(Window window) = 0;

	virtual void RemoveWindow(Window window) = 0;
};

struct TilingLayout : public ILayout {
private:
	std::vector<Window> windowVec;
	int                 width;
	int                 height;

	void EvalPlaces(){
		printf("Eval positions window\nvec size: %lu\n", windowVec.size());
		int windWidth = width / windowVec.size();
		for (size_t i = 0; i < windowVec.size(); ++i) {
			windowVec[i].Resize(windWidth, height);
			windowVec[i].Move(windWidth*i, 0);
			windowVec[i].Show();
		}
	}
public:
	TilingLayout(int w, int h) :
		width(w), height(h)
	{ }

	virtual ~TilingLayout() {}

	void AddWindow(Window window) {
		printf("Adding window\n");
		windowVec.push_back(window);
		EvalPlaces();
	}

	void RemoveWindow(Window window) {
		printf("Removing window\n");
		auto it = std::find_if(windowVec.begin(), windowVec.end(), [&](Window a){return a.Get() == window.Get();});
		if (it != windowVec.end()) {
			windowVec.erase(it);
			EvalPlaces();
		}
	}
};
