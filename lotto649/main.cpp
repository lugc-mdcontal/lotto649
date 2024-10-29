#include <iostream>
#include <lottery/game.hpp>

int main()
{
	//
	// create the initial game instance.
	//
	lottery::game game;

	//
	// print the rules to the user.
	//
	std::cout << "Welcome to the lottery!\n";
	std::cout << "You are allowed up to 8 extra lines, and up to 10 encore numbers.\n";
	std::cout << "You are given 2 free lines per ticket.\n";
	std::cout << "Line price: $" << lottery::line_price << "\n";
	std::cout << "Encore price: $" << lottery::encore_price << "\n";
	std::cout << "Encore prizes: ";
	for (const auto prize : lottery::encore_prizes)
		std::cout << "$" << prize << " ";
	std::cout << "\n";
	std::cout << "Line prizes: ";
	for (const auto prize : lottery::line_prizes)
		std::cout << "$" << prize << " ";
	std::cout << "\n\n";

	//
	// an exception may occur during a constructor, or utility, so let's catch it, and print the error message.
	//
	try
	{
		while (true)
		{
			//
			// do we want to create a ticket, or draw the winning numbers?
			//
			const auto option{ utils::get_input<std::string>("Enter 'create' to create a ticket, or 'draw' to draw the winning numbers") };
			if (option == "create")
			{
				//
				// fetch the number of extra lines, and encore numbers the user wants to play.
				const auto line_count{ utils::get_input<std::size_t>("Enter the number of extra lines you want to play") };
				const auto encore_count{ utils::get_input<std::size_t>("Enter the number of encore numbers you want to play") };
				const auto& ticket{ game.create_ticket(line_count, encore_count) };

				//
				// save the ticket to a file, and print it to the console.
				//
				std::ofstream{ "ticket_" + utils::get_current_date()
					+ "_" + std::to_string(ticket.guid) + ".txt"}
				<< ticket.to_string();

				std::cout << ticket.to_string() << "\n";
			}
			else if (option == "draw")
			{
				//
				// we want to draw, so fetch it from the game manager.
				//
				const auto& draw_results{ game.draw() };

				//
				// save the draw results to a file, and print it to the console.
				//
				std::ofstream file{ "draw_" + utils::get_current_date()
					+ "_" + std::to_string(utils::generate_random_number()) + ".txt" };

				file << "**********************************\n";
				file << "********* LOTTERY RESULT *********\n";
				file << "**********************************\n";

				file << "\n* Line: " << draw_results.line.to_string() << " *\n";
				file << "* Encore: " << draw_results.encore.to_string() << " *\n";
				file << "* Gold Ball: " << "G" << draw_results.gold_ball.get_gold_ball().to_string().c_str() << " *\n";

				file.close();

				std::cout << "Winning line: " << draw_results.line.to_string() << "\n";
				std::cout << "Winning encore: " << draw_results.encore.to_string().c_str() << "\n";
				std::cout << "Winning gold ball: G" << draw_results.gold_ball.get_gold_ball().to_string().c_str() << "\n\n";

				for (const auto& ticket : game.get_tickets())
				{
					const auto lines_prize{ ticket.get_lines_prize(draw_results.line) };
					if (lines_prize)
						std::cout << "Ticket " << ticket.guid << " won $" << lines_prize << " from the lines!\n";

					const auto encore_prize{ ticket.get_encore_prize(draw_results.encore) };
					if (encore_prize)
						std::cout << "Ticket " << ticket.guid << " won $" << encore_prize << " from the encore!\n";
				}

				//
				// for the gold ball draw, we need to pop a ball from the game, and check if it's the gold ball.
				//
				const auto ball = game.draw_ball();
				std::cout << "Ticket " << draw_results.gold_ball.guid << " won $" << draw_results.gold_ball.get_gold_ball_prize(ball) << " from the gold ball draw!\n";
			
				//
				// draw complete, clear all pending tickets.
				//
				game.clear_tickets();
			}
			else break;

			//
			// do we want to play again?
			//
			const auto play_again{ utils::get_input<std::string>("Do you want to play again? (yes/no)") };
			if (play_again != "yes")
				break;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";

		return 1;
	}

	return 0;
}