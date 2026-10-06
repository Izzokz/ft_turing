#include "main.hpp"

char check_write(char c, const std::string &states, const std::unordered_map<std::string, std::vector<Transition>>& transitions)
{
	for (const auto& [state, state_transitions] : transitions)
    {
		if (state != states)
            continue;

        for (const auto& transition : state_transitions)
        {
			if (transition.read != c)
                continue;
			return transition.write;
		}
	}
	return c;
}

std::string change_input(const std::string& input, int pos, char write, const Machine& machine)
{
    std::string new_input = input;

    if (pos < 0)
    {
        new_input.insert(0, machine.blank);
        pos = 0;
    }

    while (static_cast<int>(new_input.size()) <= pos)
        new_input += machine.blank;

    new_input[pos] = write;

    return new_input;
}


const Transition* find_transition(char c, const std::string& state, const std::unordered_map<std::string, std::vector<Transition>>& transitions)
{
    for (const auto& [transition_state, state_transitions] : transitions)
    {
        if (transition_state != state)
            continue;

        for (const auto& transition : state_transitions)
        {
            if (transition.read == c)
                return &transition;
        }
    }

    return nullptr;
}

std::string print_end(const std::string &input, int pos, const std::string& state, const Machine& machine)
{
	std::string text;

	text = first_part(input, pos, machine.blank);
	text += " - " + state;
	return text + "\n";
}

std::string print_output(const std::string& input, const Machine& machine)
{
	int pos = 0;
	std::string text;
	std::string current_input = input;
	std::string state = machine.initial;
	
	text += "\"" + input + "\"\n";
	while(std::find(machine.finals.begin(), machine.finals.end(), state) == machine.finals.end())
	{
        text += print_input(current_input, pos, state, machine);
        const char read = get_read(current_input, pos, machine.blank);
        const Transition* transition = find_transition(read, state, machine.transitions);
        if (transition == nullptr)
            break;
        current_input = change_input(current_input, pos, check_write(get_read(current_input, pos, machine.blank), state, machine.transitions), machine);
        pos = change_pos(pos, transition->action);
        state = transition->to_state;
	}
	text += print_end(current_input, pos, state, machine);
	return text;
}


/*"00"
<00> - replaceleft[0] => scaneright; writes x; goes R
<x0> - scaneright[0] => scaneright; writes 0; goes R
x0X> - scaneright[.] => replaceright; writes .; goes L
<x0.> - replaceright[0] => scanleft; writes x; goes L
<xx.> - scanleft[x] => replaceleft; writes x; goes R
<xx.> - replaceleft[x] => answer_y; writes x; goes R
<xx.> - answer_y[.] => HALT; writes y; goes L
<xxy> - HALT
*/

int main(int ac, char **av)
{
    if (ac == 1)
		{ cerr << ft_print_err(NO_ARG_ERR, nullptr); return 1; }
	if (ac == 2)
	{
		if (ft_sequals(av[1], "--help") || ft_sequals(av[1], "-h"))
			{ ft_print_help(); return 0; }
		else
			{ cerr << ft_print_err(NOT_ENOUGH_ARG_ERR, nullptr); return 1; }
	}
	std::string file_content = extract_file(av[1]);
	if(file_content.empty())
		{ cerr << ft_print_err(READ_FILE_ERR, nullptr); return 1; }
	if (auto j = parse_json(file_content))
	{
		auto error = check_json(j);
		if (error)
			{ cerr << ft_print_err(*error, nullptr) << endl; return 1; }
		Machine machine = set_machine(j);
		cout << print_machine_description(machine);
		error = check_input(av[2], machine.alphabet);
		if(error)
			{ cerr << *error << endl; return 1; }
		cout << print_output(av[2], machine);
	}
	else
		{ cerr << ft_print_err(INVALID_JSON_ERR, nullptr); return 1; }	
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