#pragma once

#include <array>
#include <string>
#include <algorithm>
#include <vector>
#include <functional>
#include <filesystem>
#include <fstream>
#include <set>
#include <numeric>
#include <array>
#include <cstdint>
#include <utils/utils.hpp>

namespace lottery
{
    constexpr auto max_line_number = 49;
    constexpr auto max_lines = 10;
    constexpr auto max_encores = 10;

    constexpr auto line_length = 6;
    constexpr auto encores_length = 7;
    constexpr auto gold_ball_length = 10;

    constexpr auto line_price = 3;
    constexpr auto encore_price = 1;

    constexpr auto encore_prizes = std::to_array({ 2, 5, 10, 100, 1000, 100000, 1000000 });
	constexpr auto line_prizes = std::to_array({ 0, 0, 5, 25, 500, 25000, 5000000 });

    enum class ball_type
    {
        white,
        gold
    };

    //
    // todo: inherit from a base class, as the code is duplicated.
    //
    template <std::size_t digits>
    class serial
    {
        std::array<std::uint8_t, digits> numbers;
        std::string string{ digits + 1, '\0' };

    public:
        const std::string& to_string() const { return string; }
        operator const std::string&() const { return to_string(); }

        const std::array<std::uint8_t, digits>& get_numbers() const { return numbers; }

        explicit serial()
        {
            std::ranges::generate(numbers, std::bind(utils::generate_random_number<std::uint8_t>, 0, 9));
            std::ranges::transform(numbers, string.begin(), [](const std::uint8_t number) { return '0' + number; });
        }
    };

    //
    // todo: inherit from a base class, as the code is duplicated.
    //
    template <std::size_t numbers_count, std::size_t max>
    class line
    {
        std::set<std::uint8_t> numbers;
        std::string string;

    public:
        const std::string& to_string() const { return string; }
		operator const std::string&() const { return to_string(); }
        
        const std::set<std::uint8_t>& get_numbers() const { return numbers; }

		explicit line()
		{
            while (numbers.size() < numbers_count)
                numbers.insert(utils::generate_random_number<std::uint8_t>(0, max));

            for (const auto number : numbers)
            {
                if (number < 10) string += "0";

                string += std::to_string(number) + " ";
            }

            string.pop_back();
		}
    };

    using lotto649_line_ty = line<line_length, max_line_number>;
    using lotto649_encore_ty = serial<gold_ball_length>;
    using lotto649_gold_ball_ty = serial<encores_length>;

	class ticket
	{
	    std::vector<lotto649_line_ty> lines;
        lotto649_gold_ball_ty gold_ball;

        std::vector<lotto649_encore_ty> encores;

    public:
		const std::uint64_t guid{ utils::generate_random_number() };

        std::uint32_t get_price() const;
        std::uint32_t get_encore_prize(const lotto649_encore_ty& winning_numbers) const;
		std::uint32_t get_lines_prize(const lotto649_line_ty& winning_numbers) const;
		std::uint32_t get_gold_ball_prize(const ball_type type) const;
        const lotto649_gold_ball_ty& get_gold_ball() const;
        
        std::string to_string() const;

        explicit ticket(const std::size_t extra_lines_count = 0, const std::size_t encores_count = 0);
	};
}