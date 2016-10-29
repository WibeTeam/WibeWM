#pragma once

#include "Types.hpp"

struct ILayout {
	virtual ~ILayout() {}

};

class TilingLayout : public ILayout {
public:
	virtual ~TilingLayout() {}

};
