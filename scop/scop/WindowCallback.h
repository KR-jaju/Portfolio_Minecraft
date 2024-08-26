#pragma once

#include "framework.h"

class WindowCallback {
public:
	virtual	~WindowCallback() = default;
	virtual LRESULT	onEvent(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) = 0;
};
