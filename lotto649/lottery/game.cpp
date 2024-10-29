#include "game.hpp"

namespace lottery
{
	void game::reset_balls()
	{
		//
		// refill the ball bag, and add a random gold ball.
		//
		std::ranges::fill(balls, ball_type::white);

		utils::select_random_member(balls) = ball_type::gold;
	}

	const std::vector<ticket>& game::get_tickets() const
	{
		return tickets;
	}

	void game::clear_tickets()
	{
		tickets.clear();
	}

	ball_type game::draw_ball()
	{
		//
		// select a random ball.
		//
		const auto ball{ utils::select_random_member(balls) };

		//
		// we want to pop it.
		//
		balls.erase(std::ranges::find(balls, ball));

		//
		// was it gold or empty, refill!
		//
		if (ball == ball_type::gold || balls.empty())
			reset_balls();

		return ball;
	}

	draw_results game::draw() const
	{
		//
		// the gold ball draw ensures that at least one ticket is present.
		// so we can safely select a random ticket.
		//
		const auto& winning_gold_ball_ticket{ utils::select_random_member(tickets) };

		return { .gold_ball{ winning_gold_ball_ticket } };
	}

	game::game()
	{
		reset_balls();
	}
}