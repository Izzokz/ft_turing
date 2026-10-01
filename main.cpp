#include "main.hpp"
#include <vector>
#include <unordered_map>

enum class Action {
    LEFT,
    RIGHT
};

struct Transition {
    char read;
    std::string to_state;
    char write;
    Action action;
};

struct Machine {
    std::string name;
    std::vector<std::string> alphabet;
    std::string blank;
    std::vector<std::string> states;
    std::string initial;
    std::vector<std::string> finals;
    std::unordered_map<std::string, std::vector<Transition>> transitions;
};

std::string get_name(const std::optional<json>& j)
{
	return j->at("name").get<std::string>();
}

std::vector<std::string> get_alphabet(const std::optional<json>& j)
{
	const auto& alphabet = j->at("alphabet");
	std::vector<std::string> r_alphabet;
	r_alphabet.reserve(alphabet.size());
	for (const auto& alpha : alphabet)
		r_alphabet.emplace_back(alpha.get<std::string>());
	return r_alphabet;
}

std::string get_blank(const std::optional<json>& j)
{
	return j->at("blank").get<std::string>();
}

std::vector<std::string> get_states(const std::optional<json>& j)
{
	const auto& states = j->at("states");
	std::vector<std::string	> r_states;
	r_states.reserve(states.size());
	for (const auto& state : states)
		r_states.emplace_back(state.get<std::string>());
	return r_states;
}

std::string get_initial(const std::optional<json>& j)
{
	return j->at("initial").get<std::string>();
}

std::vector<std::string> get_finals(const std::optional<json>& j)
{
	const auto& finals = j->at("finals");
	std::vector<std::string> r_finals;
	r_finals.reserve(finals.size());
	for (const auto& final : finals)
		r_finals.emplace_back(final.get<std::string>());
	return r_finals;
}
/*    
char read;
std::string to_state;
char write;
Action action;
*/

std::unordered_map<std::string, std::vector<Transition>> get_transitions(const std::optional<json>& j)
{
	//char read;
    // std::string to_state;
    // char write;
    // Action action;
}

Machine set_machine(const std::optional<json>& j)
{
    return Machine{
		.name = get_name(j),
		.alphabet = get_alphabet(j),
		.blank = get_blank(j),
		.states = get_states(j),
		.initial = get_initial(j),
		.finals = get_finals(j),
		.transitions = get_transitions(j)
	};
}

std::string print_machine_description()
{
	return "PLACEHOLDER";
}

int main(int ac, char **av)
{
    if (ac == 1)
		{ cout << ft_print_err(NO_ARG_ERR, nullptr); return 1; }
	if (ac == 2)
	{
		if (ft_sequals(av[1], "--help") || ft_sequals(av[1], "-h"))
			ft_print_help();
		else
			cout << ft_print_err(NOT_ENOUGH_ARG_ERR, nullptr);
		return 1;
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
	}
	else
		{ cout << ft_print_err(INVALID_JSON_ERR, nullptr); return 1; }

	//set transitions
	
	// cout << print_machine_description();
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



bool is_final(const Machine& machine, const std::string& state)
{
    return std::find(
        machine.finals.begin(),
        machine.finals.end(),
        state
    ) != machine.finals.end();
}

is_final(machine, state); // function operating on data


*/