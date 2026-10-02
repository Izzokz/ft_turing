#include "main.hpp"

std::string print_name(const std::string& name)
{
	return "####\n##" + name + "\n####";
}

std::string print_alphabet(const std::vector<std::string>& alphabet)
{
    std::string text = "\n# Alphabet: [";

    for (size_t i = 0; i != alphabet.size(); ++i)
    {
        if (i != 0)
            text += ", ";
        text += alphabet[i];
    }
    return text + "]";
}

std::string print_states(const std::vector<std::string>& states)
{
	std::string text = "\n# States: [";

	for (size_t i = 0; i != states.size(); ++i)
    {
        if (i != 0)
            text += ", ";
        text += states[i];
    }
	return text + "]";
}

std::string print_intial(const std::string& initial)
{
	return "\n# Initial: " + initial;
}

std::string print_finals(const std::vector<std::string>& finals)
{
	std::string text = "\n# Finals: [";
	for (size_t i = 0; i != finals.size(); ++i)
    {
        if (i != 0)
            text += ", ";
        text += finals[i];
    }
	return text + "]";
}

std::string print_transitions(const std::unordered_map<std::string, std::vector<Transition>>& transitions)
{
	std::string text = "]\n# Transitions:\n";

	// for (int i = 0; i < g_max_I; ++i)
	// {
	// 	if (!*(g_transet + i))
	// 		continue ;
	// 	for (int j = 0; *((int **)*(g_transet + i) + j); ++j)
	// 		printf("%s[%c] => %s; writes %c; goes %c\n", *(g_conf.states + i), **((int **)*(g_transet + i) + j), *(g_conf.states + *(*((int **)*(g_transet + i) + j) + 1)), *(*((int **)*(g_transet + i) + j) + 2), *(*((int **)*(g_transet + i) + j) + 3));
	// }

	return text + "\n";
}

std::string print_machine_description(Machine machine)
{
	std::string text;
	text = print_name(machine.name);
	text += print_alphabet(machine.alphabet);
	text += print_states(machine.states);
	text += print_intial(machine.initial);
	text += print_finals(machine.finals);
	text += print_transitions(machine.transitions);
	return text;
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
	string file_content = extract_file(av[1]);
	if(file_content.empty())
		{ cout << ft_print_err(READ_FILE_ERR, nullptr); return 1; }
	if (auto j = parse_json(file_content))
	{
		auto error = check_json(j);
		if (error)
			{ cout << ft_print_err(*error, nullptr) << endl; return 1; }
		Machine machine = set_machine(j);
		cout << print_machine_description(machine);
	}
	else
		{ cout << ft_print_err(INVALID_JSON_ERR, nullptr); return 1; }
	//set transitions
	
	//check input
	//print input
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