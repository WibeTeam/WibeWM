#pragma once

#include "Types.hpp"

class Tag {
public:
	enum class State {
		None,
		Selected,
		Urgent
	};

	Tag(std::string&& name)
		:_name(name), _state(State::None)
	{ }

private:
	std::string _name;
	State       _state;
};


class Client {
public:
	Client()
	{ }

	void SetName(const std::string& name) {
		_name = name;
	}

	std::string GetName() const {
		return _name;
	}

	void SetFocus(bool focus) {
		_hasFocus = focus;
	}

	bool HasFocus() const {
		return _hasFocus;
	}

	void SetFullScreen(bool fullscreen) {
		_isFullScreen = fullscreen;
	}

	bool isFullScreen() const {
		return _isFullScreen;
	}

	void setUrgent(bool urgent) {
		_isUrgent = urgent;
	}

	bool isUrgent() const {
		return _isUrgent;
	}

	void AssignToTag(shared_ptr<Tag> tag) {
		_tag = tag;
	}

	shared_ptr<Tag> GetTag() const {
		return _tag;
	}

private:
	std::string      _name;

	Rectangle        _currentRect;
	Rectangle        _previousRect;

	bool             _hasFocus;
	bool             _isFloating;
	bool             _isUrgent;
	bool             _isFullScreen;

	shared_ptr<Tag>   _tag;
};