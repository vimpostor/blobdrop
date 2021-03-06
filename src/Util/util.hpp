#pragma once

#define SINGLETON(TYPE) \
	static TYPE* get() { \
		static TYPE s; \
		return &s; \
	}

#define STRICT_SINGLETON(TYPE) \
	TYPE() = delete; \
	explicit TYPE(bool singleton_constructor) {}; \
	static TYPE* get() { \
		static TYPE s {true}; \
		return &s; \
	}
