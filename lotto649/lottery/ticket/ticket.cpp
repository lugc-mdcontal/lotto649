#include "ticket.hpp"

namespace lottery
{
	std::uint32_t ticket::get_price() const
	{
		return (line_price * lines.size()) + (encore_price * encores.size());
	}

	std::uint32_t ticket::get_encore_prize(const lotto649_encore_ty& winning_numbers) const
	{
		//
		// start from 0, and for each encore number, check how many digits match the winning numbers.
		// if the number is 0, then the prize is 0, otherwise, the prize is the prize at the matched digits index.
		//
		return std::accumulate(encores.cbegin(), encores.cend(), 0u, [&](const std::uint32_t total_prize, const lotto649_encore_ty& encore) {
			const auto& numbers{ encore.get_numbers() };
			const auto mismatch_it{ std::mismatch(numbers.crbegin(), numbers.crend(), winning_numbers.get_numbers().crbegin()) };
			const auto matched_digits{ std::distance(numbers.crbegin(), mismatch_it.first) };

			const auto prize{ matched_digits ? encore_prizes[matched_digits] : 0u };

			return total_prize + prize;
		});
	}

	std::uint32_t ticket::get_lines_prize(const lotto649_line_ty& winning_numbers) const
	{
		//
		// start from 0, and for each line, check how many numbers match the winning numbers.
		// if the number is 0, then the prize is 0, otherwise, the prize is the prize at the matched numbers index.
		//
		return std::accumulate(lines.cbegin(), lines.cend(), 0u, [&](const std::uint32_t total_prize, const lotto649_line_ty& line) {
			const auto& numbers{ line.get_numbers() };
			const auto matched_numbers{ std::ranges::count_if(numbers, [&](const std::uint8_t number) {
				return std::ranges::find(winning_numbers.get_numbers(), number) != winning_numbers.get_numbers().cend();
			}) };

			const auto prize{ line_prizes[matched_numbers] };

			return total_prize + prize;
		});
	}

	std::uint32_t ticket::get_gold_ball_prize(const ball_type type) const
	{
		return type == ball_type::white ? 1000000 : 68000000;
	}

	const lotto649_gold_ball_ty& ticket::get_gold_ball() const
	{
		return gold_ball;
	}

	std::string ticket::to_string() const
	{
		std::stringstream file;

		file << "**********************************\n";
		file << "********* LOTTERY TICKET *********\n";
		file << "**********************************\n";

		file << "\n* GUID: " << guid << " *\n";
		file << "\n* Price: $" << get_price() << " *\n";

		file << "\n";

		for (auto i{ 0ull }; i < lines.size(); ++i)
			file << "* Line " << (i < 9 ? "0" : "") << i + 1 << ": " << lines[i].to_string() << " *\n";
			

		file << "\n";

		file << "* Gold Ball: " << "G" << gold_ball.to_string().c_str() << " *\n";

		if (!encores.empty())
		{
			file << "\n";

			for (auto i{ 0ull }; i < encores.size(); ++i)
				file << "* Encore " << i + 1 << ": " << encores[i].to_string().c_str() << " *\n";
		}

		file << "**********************************\n";

		return std::move(file.str());
	}

	ticket::ticket(const std::size_t extra_lines_count, const std::size_t encores_count)
	{
		if (extra_lines_count + 2 > max_lines || encores_count > max_encores)
			throw std::invalid_argument{ "extra_lines_count or encores_count is greater than the maximum allowed." };

		lines = std::vector<lotto649_line_ty>{ 2 + extra_lines_count };

		if (encores_count > 0)
			encores = std::vector<lotto649_encore_ty>{ encores_count };
	}
}