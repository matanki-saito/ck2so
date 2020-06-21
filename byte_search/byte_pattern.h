#pragma once

#include <stddef.h>
#include <string>
#include<vector>
#include<filesystem>
#include <fstream>
#include <sys/mman.h>
#include "unistd.h"
#include <boost/utility/string_view.hpp>
#include <boost/algorithm/string.hpp>

class memory_pointer{


public:
	union {
		void* Pointer;
		std::uintptr_t Address;
	};

	memory_pointer() :Pointer{}{}
	memory_pointer(void* p) : Pointer(p){}
	memory_pointer(std::uintptr_t i): Address(i){}

	std::uintptr_t address(std::ptrdiff_t offset = 0) const{
		return (this->Address + offset);
	}

	template<typename T = void> T* pointer(std::ptrdiff_t offset = 0) const{
		return reinterpret_cast<T*>(this->address(offset));
	}

	template<class T> T* get() const {
		return (T*)this->address();
	}

	operator std::uintptr_t() const{
		return this->address();
	}

	template<class T> operator T* () const {
		return reinterpret_cast<T*>(this->Pointer);
	}
};

class BytePattern{
	// field
	std::vector<std::pair<std::uintptr_t, std::uintptr_t>> _ranges;
	std::vector<std::uint8_t> _pattern;
	std::vector<std::uint8_t> _mask;
	std::vector<memory_pointer> _results;
	std::string _literal;
	std::ptrdiff_t _bmbc[256];

	// log
	static std::ofstream& log_stream();
	void debug_output() const;
	

	// pattern
	void transform_pattern(boost::string_view literal);
	std::pair<uint8_t, uint8_t> parse_sub_pattern(boost::string_view sub);

	// util
	void bm_preprocess();
	template <typename Fn> void for_each_result(Fn Pr) const {
		for (auto& result : this->_results){
			Pr(result);
		}
	}
	#ifndef max
	#define max(a,b)            (((a) > (b)) ? (a) : (b))
	#endif

	#ifdef __x86_64
	#define ADDRLEN 8
	#elif
	#define ADDRLEN 8
	#endif

public:
	// log
	static void StartLog(const char* module_name);
	static void ShutdownLog();
	static void LoggingInfo(const std::string message);

	// start end
	static BytePattern& temp_instance();
	BytePattern();
	BytePattern& setModule();
	BytePattern& setModule(std::string moduleName);
	void getModuleRanges(std::string moduleName);
	void clear();

	// pattern
	BytePattern& find_pattern(boost::string_view pattern_literal);
	BytePattern& set_pattern(boost::string_view pattern_literal);

	// search
	BytePattern& search();
	void bm_search();

	// util
	std::size_t count() const;
	bool has_size(std::size_t expected, std::string version) const;

	// get
	memory_pointer get(std::size_t index) const;
	memory_pointer get_first() const;
	memory_pointer get_second() const;
};