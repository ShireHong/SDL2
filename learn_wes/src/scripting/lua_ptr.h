
#pragma once

#include <memory>
template<typename T>
class lua_ptr;

template<typename T>
class enable_lua_ptr
{
public:
	enable_lua_ptr(T* tp) : self_(std::make_shared<T*>(tp)) {}
private:
	friend class lua_ptr<T>;
	std::shared_ptr<T*> self_;
};

/** Tmust inherit enable_lua_ptr<T> */
template<typename T>
class lua_ptr
{
public:
	lua_ptr(enable_lua_ptr<T>& o) : self_(o.self_) { }
	T* get_ptr()
	{
		if(auto pp = self_.lock()) {
			return *pp;
		}
		return nullptr;
	}
	std::weak_ptr<T*> self_;
};
