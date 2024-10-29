#pragma once

#include <type_traits>
#include <limits>
#include <random>
#include <iostream>
#include <cstdint>

namespace utils
{
	//
	// utility function to generate a random number, while ensuring runtime and compile-time safety.
	//
	template <typename ty = std::uint64_t> requires std::is_integral_v<ty>
	static ty generate_random_number(const std::size_t min = 0, const std::size_t max = std::numeric_limits<ty>::max())
	{
		if (min < static_cast<std::size_t>(std::numeric_limits<ty>::min()) || max > static_cast<std::size_t>(std::numeric_limits<ty>::max()))
			throw std::invalid_argument{ "The range is invalid." };

		std::random_device seeder;
		std::uniform_int_distribution<std::uint64_t> distribution{ min, max };

		if constexpr (sizeof(ty) <= sizeof(std::uint32_t))
		{
			std::mt19937 engine{ seeder() };
			return static_cast<ty>(distribution(engine));
		}
		else
		{
			std::mt19937_64 engine{ seeder() };
			return static_cast<ty>(distribution(engine));
		}
	}

	//
	// given any container type, select a random member from it.
	//
	static decltype(auto) select_random_member(auto& container)
	{
		if (container.empty())
			throw std::runtime_error{ "The container is empty." };

		const auto random{ generate_random_number(0, container.size() - 1) };
		
		return container[random];
	}

	//
	// utiltiy function to get input from the user.
	//
	template <typename ty>
	static ty get_input(const std::string& message)
	{
		std::cout << message << ": ";

		ty value;
		std::cin >> value;

		if constexpr (std::is_same_v<ty, std::string>)
			std::ranges::transform(value, value.begin(), ::tolower);

		return value;
	}

	//
	// utility function to get the current date.
	//
	static std::string get_current_date()
	{
		const auto now{ std::chrono::system_clock::now() };
		const auto time{ std::chrono::system_clock::to_time_t(now) };

		std::stringstream date;
		date << std::put_time(std::localtime(&time), "%Y_%m_%d");

		return std::move(date.str());
	}
}