#pragma once

#define SINGLETON(TYPE) \
	static TYPE* get() { \
		static TYPE s; \
		return &s; \
	}
