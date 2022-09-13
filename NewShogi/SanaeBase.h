#ifndef INCLUDE_GUARD_SANAEBASE_H
#define INCLUDE_GUARD_SANAEBASE_H

#include<stdlib.h>
#include <stdexcept>
#include <exception>

class SANAE_BASE {
public:
	virtual void mem_error(const char* text = "Failed to secure memory."){
		throw std::runtime_error(text);
	}
	virtual void range_error(const char* text = "Passed of value out of range."){
		throw std::out_of_range(text);
	}
	virtual void runtime_error(const char* text = "Runtime Error."){
		throw std::runtime_error(text);
	}
};
template<typename T> class SANAE_ARRAY_BASE {
public:
	virtual void sfree(T* mem) final {
		free(mem);mem = NULL;
	}
	virtual void _delete(T* mem) final {
		delete[] mem;mem = NULL;
	}
	/*Success = true */
	virtual [[nodiscard]] bool _calloc(T** mem,size_t count,bool dofree=false,unsigned int spare_value=10) noexcept {
		if (dofree)sfree(*mem);
		count += spare_value;
		*mem = (T*)calloc(count,sizeof(T*));
		return *mem == NULL ? false : true;
	}
	/*Success = true*/
	virtual [[nodiscard]] bool _new(T** mem, size_t count, bool dodelete = false, unsigned int spare_value = 10) noexcept {
		if (dodelete)delete [] *mem;
		try {
			*mem = new T[count + spare_value];
			return true;
		}
		catch (std::bad_alloc) {
			return false;
		}
	}
	/*Error*/
	virtual void mem_error(const char* text="Failed to secure memory."){
		throw std::runtime_error(text);
	}
	virtual void range_error(const char* text="Passed of value out of range."){
		throw std::out_of_range(text);
	}
	/*Function*/
	virtual void _ARRAY_COPY(T** tomem,T** frommem,size_t count) {
		for (int i = 0; i < count; i++)(*tomem)[i] = (*frommem)[i];
	}
	virtual void _reverse(T** mem,size_t count) {
		T* mem_copy = NULL;
		if(!_calloc(&mem_copy,count,false,10))mem_error();
		_ARRAY_COPY(&mem_copy,mem,count);
		for (int i = count - 1, j = 0; i >= 0; i--, j++)(*mem)[j] = mem_copy[i];
		sfree(mem_copy);
	}
};
#endif