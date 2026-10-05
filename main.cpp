#include "main.hpp"

std::string print_output(const std::string& input, Machine machine)
{
	
}

int main(int ac, char **av)
{
    if (ac == 1)
		{ cout << ft_print_err(NO_ARG_ERR, nullptr); return 1; }
	if (ac == 2)
	{
		if (ft_sequals(av[1], "--help") || ft_sequals(av[1], "-h"))
			{ ft_print_help(); return 0; }
		else
			{ cout << ft_print_err(NOT_ENOUGH_ARG_ERR, nullptr); return 1; }
	}
	std::string file_content = extract_file(av[1]);
	if(file_content.empty())
		{ cout << ft_print_err(READ_FILE_ERR, nullptr); return 1; }
	if (auto j = parse_json(file_content))
	{
		auto error = check_json(j);
		if (error)
			{ cout << ft_print_err(*error, nullptr) << endl; return 1; }
		Machine machine = set_machine(j);
		cout << print_machine_description(machine);
		error = check_input(av[2], machine.alphabet);
		if(error)
			{ cout << *error << endl; return 1; }
		cout << print_output(av[2], machine);
	}
	else
		{ cout << ft_print_err(INVALID_JSON_ERR, nullptr); return 1; }
}

// J.contains
// auto blank = J.find("blank");
// cout << *blank;
// string file_content = extract_file(av[1]);
// string input = av[2];
// json J = json::parse(file_content);

/*

switch (transition.action)
{
    case Action::LEFT:
        // move left
        break;

    case Action::RIGHT:
        // move right
        break;
}

if (transition.action == Action::RIGHT)
{
    // move right
}
else if (transition.action == Action::LEFT)
{
    // move left
}

*/