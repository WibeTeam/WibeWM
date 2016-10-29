#pragma once

#include "Types.hpp"

struct ILayout {
	virtual ~ILayout() {}

	virtual void HandleExpose() = 0;
};

class TilingLayout : public ILayout {
public:
	virtual ~TilingLayout() {}

	virtual void HandleExpose() override {

	}
};
