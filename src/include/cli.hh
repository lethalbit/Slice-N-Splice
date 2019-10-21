/* cli.hh - Command line handling and parsing */
#pragma once
#if !defined(__SNS_CLI_HH__)
#define __SNS_CLI_HH__

#include <utility.hh>

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <type_traits>

// /* Gotta forward declare this because I'm stupid and this is all tangled */
// template<typename T> struct arg;

// using arg_variant = std::variant<
// 	arg<int8_t>,
// 	arg<int16_t>,
// 	arg<int32_t>,
// 	arg<int64_t>,
// 	arg<uint8_t>,
// 	arg<uint16_t>,
// 	arg<uint32_t>,
// 	arg<uint64_t>,
// 	arg<bool>,
// 	arg<char>,
// 	arg<std::string>
// >;

// extern std::vector<arg_variant> _args;

// template<typename T>
// struct arg final {
// public:

// private:
// 	/* The argument to look for */
// 	std::string _short;
// 	std::string _long;
// 	std::string _desc;
// 	/* Settings for this arg */
// 	bool _optional;
// 	T _default;
// 	std::function<T(const std::string)>  _parser;
// 	std::function<bool()> _validate;
// 	/* Actual Value */
// 	T _value;
// 	/* control value */
// 	bool _was_set;
// public:
// 	constexpr arg() noexcept :
// 		_short{}, _long{}, _desc{}, _optional{}, _default{},
// 		_value{}, _was_set{false} { /* NOP */ }

// 	/* Non-enum constructors */
// 	// template<typename U = T>
// 	// arg(typename std::enable_if<!std::is_enum<U>::value, std::string>::type short_name,
// 	// 	std::string long_name = {}, std::string desc = {}, bool optional = false,
// 	// 	) noexcept : {

// 	// }

// 	template<typename V, typename U = T, typename = typename std::enable_if<!std::is_same_v<V, U>>::type>
// 	arg(arg<V> &_arg) noexcept : arg{ } { swap(_arg); }

// 	template<typename U = T>
// 	arg(typename std::enable_if<!std::is_enum<U>::value, std::string>::type short_name) noexcept :
// 		_short{std::move(short_name)}, _long{}, _desc{}, _optional{false}, _default{},
// 		_value{}, _was_set{false} {
// 			_args.emplace_back(std::move(*this));
// 		}

// 	template<typename U = T>
// 	arg(typename std::enable_if<!std::is_enum<U>::value, std::string>::type short_name, std::string long_name) noexcept :
// 		_short{std::move(short_name)}, _long{std::move(long_name)}, _desc{}, _optional{false}, _default{},
// 		_value{}, _was_set{false} {
// 			_args.emplace_back(std::move(*this));

// 		}

// 	template<typename U = T>
// 	arg(typename std::enable_if<!std::is_enum<U>::value, std::string>::type short_name, std::string long_name, std::string desc) noexcept :
// 		_short{std::move(short_name)}, _long{std::move(long_name)}, _desc{std::move(desc)}, _optional{false}, _default{},
// 		_value{}, _was_set{false} {
// 			_args.emplace_back(std::move(*this));
// 		}

// 	template<typename U = T>
// 	arg(typename std::enable_if<!std::is_enum<U>::value, std::string>::type short_name, std::string long_name, std::string desc, bool optional) noexcept :
// 		_short{std::move(short_name)}, _long{std::move(long_name)}, _desc{std::move(desc)}, _optional{optional},
// 		_default{}, _value{}, _was_set{false} {
// 			_args.emplace_back(std::move(*this));

// 		}

// 	template<typename U = T>
// 	arg(typename std::enable_if<!std::is_enum<U>::value, std::string>::type short_name, std::string long_name, std::string desc,
// 		bool optional, U default_value) noexcept :
// 		_short{std::move(short_name)}, _long{std::move(long_name)}, _desc{std::move(desc)}, _optional{optional},
// 		_default{default_value}, _value{default_value}, _was_set{false} {
// 			_args.emplace_back(std::move(*this));

// 		}

// 	template<typename U = T>
// 	arg(typename std::enable_if<!std::is_enum<U>::value, std::string>::type short_name, std::string long_name, std::string desc,
// 		bool optional, U default_value, std::function<U(const std::string)>  parser) noexcept :
// 		_short{std::move(short_name)}, _long{std::move(long_name)}, _desc{std::move(desc)}, _optional{optional},
// 		_default{default_value}, _parser{parser}, _value{default_value}, _was_set{false} {
// 			_args.emplace_back(std::move(*this));

// 		}


// 	/* Enum constructors */
// 	template<typename U = T>
// 	arg(typename std::enable_if<std::is_enum<U>::value, std::string>::type short_name) noexcept :
// 		_short{std::move(short_name)}, _long{}, _desc{}, _optional{false}, _default{},
// 		_value{}, _was_set{false} {
// 			_args.emplace_back(std::move(arg<std::underlying_type_t<U>>(*this)));
// 		}

// 	template<typename U = T>
// 	arg(typename std::enable_if<std::is_enum<U>::value, std::string>::type short_name, std::string long_name) noexcept :
// 		_short{std::move(short_name)}, _long{std::move(long_name)}, _desc{}, _optional{false}, _default{},
// 		_value{}, _was_set{false} {
// 		_args.emplace_back(std::move(arg<std::underlying_type_t<U>>(*this)));

// 	}

// 	template<typename U = T>
// 	arg(typename std::enable_if<std::is_enum<U>::value, std::string>::type short_name, std::string long_name, std::string desc) noexcept :
// 		_short{std::move(short_name)}, _long{std::move(long_name)}, _desc{std::move(desc)}, _optional{false}, _default{},
// 		_value{}, _was_set{false} {
// 			_args.emplace_back(std::move(arg<std::underlying_type_t<U>>(*this)));

// 		}

// 	template<typename U = T>
// 	arg(typename std::enable_if<std::is_enum<U>::value, std::string>::type short_name, std::string long_name, std::string desc, bool optional) noexcept :
// 		_short{std::move(short_name)}, _long{std::move(long_name)}, _desc{std::move(desc)}, _optional{optional},
// 		_default{}, _value{}, _was_set{false} {
// 			_args.emplace_back(std::move(arg<std::underlying_type_t<U>>(*this)));
// 		}

// 	template<typename U = T>
// 	arg(typename std::enable_if<std::is_enum<U>::value, std::string>::type short_name, std::string long_name, std::string desc,
// 		bool optional, U default_value) noexcept :
// 		_short{std::move(short_name)}, _long{std::move(long_name)}, _desc{std::move(desc)}, _optional{optional},
// 		_default{default_value}, _value{default_value}, _was_set{false} {
// 			_args.emplace_back(std::move(arg<std::underlying_type_t<U>>(*this)));
// 		}

// 	template<typename U = T>
// 	arg(typename std::enable_if<std::is_enum<U>::value, std::string>::type short_name, std::string long_name, std::string desc,
// 		bool optional, U default_value, std::function<U(const std::string)>  parser) noexcept :
// 		_short{std::move(short_name)}, _long{std::move(long_name)}, _desc{std::move(desc)}, _optional{optional},
// 		_default{default_value}, _parser{parser}, _value{default_value}, _was_set{false} {
// 			_args.emplace_back(std::move(arg<std::underlying_type_t<U>>(*this)));
// 		}


// 	arg(const arg&) = delete;
// 	arg(arg&& _arg) noexcept : arg{} { swap(_arg); }
// 	~arg() { /* NOP */ }

// 	template<typename U>
// 	void swap(arg<U> &_arg) {
// 		std::swap(_short, _arg._short);
// 		std::swap(_long, _arg._long);
// 		std::swap(_desc, _arg._desc);
// 		std::swap(_optional, _arg._optional);
// 		std::swap(_default, _arg._default);
// 		std::swap(_parser, _arg._parser);
// 		std::swap(_validate, _arg._validate);
// 		std::swap(_value, _arg._value);
// 		std::swap(_was_set, _arg._was_set);
// 	}

// 	// operator const T() const noexcept { return (_was_set) ? _value : _default; }
// 	// void validator(std::function<bool()>&& validator) noexcept { _validate = std::move(validator); }
// };


uint64_t ParseCli(int argc, const char** argv, std::ostream& output = std::cout);


#endif /* __SNS_CLI_HH__ */
