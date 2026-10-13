#pragma once
#include <godot_cpp/core/memory.hpp>

#define SINGLETON(type)                                                                 \
private:                                                                                \
	static inline type *s_Singleton;                                                    \
                                                                                        \
public:                                                                                 \
	static type *get_singleton() {                                                      \
		return s_Singleton;                                                             \
	}                                                                                   \
                                                                                        \
	static void create_singleton() {                                                    \
		CRASH_COND_MSG(s_Singleton != nullptr, "The singleton was already created!");   \
		UtilityFunctions::print("Created "#type" singleton!"); \
		s_Singleton = memnew(type);                                                     \
	}                                                                                   \
                                                                                        \
	static void destroy_singleton() {                                                   \
		CRASH_COND_MSG(s_Singleton == nullptr, "The singleton was already destroyed!"); \
		UtilityFunctions::print("Destroyed "#type" singleton!"); \
		memdelete(s_Singleton);                                                         \
		s_Singleton = nullptr;                                                          \
	}
