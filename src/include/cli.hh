/* cli.hh - Command line handling and parsing */
#pragma once
#if !defined(__SNS_CLI_HH__)
#define __SNS_CLI_HH__

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

/* Gotta forward declare this because I'm stupid and this is all tangled */
template<typename T> struct arg;

// template<>
// struct arg<bool> final { };
// template<>
// struct arg<std::string> final { };
// template<>
// struct arg<char> final { };
// template<>
// struct arg<uint64_t> final { };
// template<>
// struct arg<int64_t> final { };


using arg_bool_ref_t = std::reference_wrapper<arg<bool>>;
using arg_string_ref_t = std::reference_wrapper<arg<std::string>>;
using arg_char_ref_t = std::reference_wrapper<arg<char>>;
using arg_uint_ref_t = std::reference_wrapper<arg<uint64_t>>;
using arg_int_ref_t = std::reference_wrapper<arg<int64_t>>;

using arg_variant = std::variant<
	arg_bool_ref_t,
	arg_string_ref_t,
	arg_char_ref_t,
	arg_uint_ref_t,
	arg_int_ref_t
>;

extern std::vector<arg_variant> _args;

template<typename T>
struct arg final {
public:

private:
	/* The argument to look for */
	std::string _short;
	std::string _long;
	std::string _desc;
	/* Settings for this arg */
	bool _optional;
	T _default;
	std::function<T(const std::string)>  _parser;
	std::function<bool()> _validate;
	/* Actual Value */
	T _value;
	/* control value */
	bool _was_set;
public:
	constexpr arg() noexcept :
		_short{}, _long{}, _desc{}, _optional{}, _default{},
		_value{}, _was_set{false} { /* NOP */ }

	arg(std::string short_name) noexcept :
		_short{short_name}, _long{}, _desc{}, _optional{}, _default{},
		_value{}, _was_set{false} { _args.emplace_back(std::ref(*this)); }

	arg(std::string short_name, std::string long_name) noexcept :
		_short{short_name}, _long{long_name}, _desc{}, _optional{}, _default{},
		_value{}, _was_set{false} { _args.emplace_back(std::ref(*this)); }

	arg(std::string short_name, std::string long_name, std::string desc) noexcept :
		_short{short_name}, _long{long_name}, _desc{desc}, _optional{}, _default{},
		_value{}, _was_set{false} { _args.emplace_back(std::ref(*this)); }

	arg(std::string short_name, std::string long_name, std::string desc,
		bool optional) noexcept :
		_short{short_name}, _long{long_name}, _desc{desc}, _optional{optional},
		_default{}, _value{}, _was_set{false} { _args.emplace_back(std::ref(*this)); }

	arg(std::string short_name, std::string long_name, std::string desc,
		bool optional, T default_value) noexcept :
		_short{short_name}, _long{long_name}, _desc{desc}, _optional{optional},
		_default{default_value}, _value{}, _was_set{false} {
			_args.emplace_back(std::ref(*this));
		}

	arg(std::string short_name, std::string long_name, std::string desc,
		bool optional, T default_value, std::function<T(const std::string)>  parser) noexcept :
		_short{short_name}, _long{long_name}, _desc{desc}, _optional{optional},
		_default{default_value}, _parser{parser}, _value{}, _was_set{false} {
			_args.emplace_back(std::ref(*this));
		}
};


uint64_t ParseCli(int argc, const char** argv, std::ostream& output = std::cout);


#endif /* __SNS_CLI_HH__ */
