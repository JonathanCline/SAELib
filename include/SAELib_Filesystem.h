#pragma once

#include <SAELib_Utility.h>
#include <SAELib_Config.h>

#include <filesystem>
#include <ranges>

namespace sae
{
	using path = std::filesystem::path;
	using system_error = std::error_code;

	static bool exists(const path& _path)
	{
		return std::filesystem::exists(_path);
	};
	static bool is_directory(const path& _path)
	{
		return std::filesystem::is_directory(_path);
	};

	static system_error remove(const path& _path, sae::nothrow_t) noexcept
	{
		system_error _err{};
		std::filesystem::remove(_path, _err);
		return _err;
	};
	static system_error remove_all(const path& _path, sae::nothrow_t) noexcept
	{
		system_error _err{};
		std::filesystem::remove_all(_path, _err);
		return _err;
	};

	static bool remove(const path& _path) noexcept(sae::config::no_exceptions_v)
	{
		if constexpr (sae::config::no_exceptions_v)
		{
			const auto _err = remove(_path, sae::nothrow);
			return (_err.value() == 0);
		}
		else
		{
			return std::filesystem::remove(_path);
		};
	};
	static bool remove_all(const path& _path) noexcept(sae::config::no_exceptions_v)
	{
		if constexpr (sae::config::no_exceptions_v)
		{
			const auto _err = remove_all(_path, sae::nothrow);
			return (_err.value() == 0);
		}
		else
		{
			return std::filesystem::remove_all(_path);
		};
	};

	// Returns true if '_child' is a folder / file within '_parent'
	static bool is_child(const path& _parent, const path& _child)
	{
		if constexpr (sae::config::debug_v)
		{
			auto _b = std::filesystem::exists(_parent) && std::filesystem::exists(_child);
			if (!_b) { std::terminate(); };
			return _b;
		};
		return std::filesystem::is_directory(_parent) && std::filesystem::equivalent(_parent, _child.parent_path());
	};
	static bool is_child_recursive(const path& _parent, const path& _child)
	{
		if constexpr (sae::config::debug_v)
		{
			const auto _b = std::filesystem::exists(_parent) && std::filesystem::exists(_child);
			if (!_b) { std::terminate(); };
			return _b;
		};
		auto _rootDirectory = _child.root_directory();
		bool _out = false;
		
		while (!_out && _child.has_parent_path() && !std::filesystem::equivalent(_child, _rootDirectory))
		{
			_out = sae::is_child(_parent, _child);
		};
		return _out;
	};
	
	
	template <typename T>
	requires requires (path& p, const T& t) { p.append(t); }
	static auto append(const path& _path, const T& _sv)
	{
		return path{ _path }.append(_sv);
	};

	template <typename IterT>
	requires requires (path& p, IterT a, const IterT b) { p.append(a, b); }
	static auto append(const  path& _path, IterT _begin, const IterT& _end)
	{
		return  path{_path }.append(_begin, _end);
	};
	
	static auto append(const path& _path, const path& _appended)
	{
		return append(_path, _appended.string());
	};
	
};