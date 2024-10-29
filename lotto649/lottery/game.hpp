#pragma once

#include <ranges>
#include <vector>
#include <lottery/ticket/ticket.hpp>
#include <utils/utils.hpp>

namespace lottery
{
	struct draw_results
	{
		lotto649_line_ty line{};
		lotto649_encore_ty encore{};
		const ticket& gold_ball;
	};

	class game
	{
		std::vector<ticket> tickets;
		std::vector<ball_type> balls{ 30 };

		void reset_balls();

	public:
		template <typename... args_ty>
		decltype(auto) create_ticket(args_ty&&... args)
		{
			return tickets.emplace_back(std::forward<args_ty>(args)...);
		}

		const std::vector<ticket>& get_tickets() const;
		void clear_tickets();

		ball_type draw_ball();
		draw_results draw() const;

		explicit game();
	};
}