#pragma once

#include "include/err_msg.h"
#include "json.hpp"
#include <fstream>  // ifstream
#include <sstream>  // stringstream
#include <iostream> // cout, endl
#include <iomanip>  // ws
#include <map>      // map
#include <ranges>   // ranges
#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>

using namespace std;
using json = nlohmann::json;

# define INPUT_INVALID_BLANK 1
# define INPUT_INVALID_UNKNOWN 2

# define EMPTY_KEY(key) ("MISSING KEY " + std::string(key))
# define EMPTY_CONTENT(key) ("MISSING CONTENT " + std::string(key))
# define WRONG_TYPE(key, type) ("WRONG TYPE " + std::string(key) + std::string(type))
# define INVALID_INPUT(c) std::string(c) +  " is Invalid (One character is not in the alphabet)."
# define INVALID_INPUT_BLANK(c) std::string(c) +  " is Invalid(The input can't take blank)."

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

struct Configuration
{
    std::string input;
    int pos;
    std::string state;
};

// -Werror -Wall -Wextra
// g++ -std=c++20 main.cpp

char	ft_sequals(const char *s1, const char *s2)
{
    int	i;

	if (!s1 || !s2)
		return (0);
	i = -1;
	while (*(s1 + ++i) || *(s2 + i))
		if (*(s1 + i) != *(s2 + i))
			break ;
	return (*(s1 + i) == *(s2 + i));
}

std::string ft_print_err(const std::string& err, const std::string* i)
{
    if (i == nullptr)
        return "ERR[" + err + "]";
    return "ERR[" + err + "](" + *i + ")";
}   

void	ft_print_help(void)
{
	std::cout << "usage: ft_turing [-h] jsonfile input\n\npositional arguments:\n\tjsonfile\t\t\tjson description of the machine\n\tinput\t\t\t\tinput of the machine\n\noptional arguments:\n\t-h, --help\t\t\tshow this help message and exit\n";
}

std::string remove_leading_whitespace(const std::string& line)
{
    auto first = line.find_first_not_of(" \t");

    return first == std::string::npos ? "" : line.substr(first);
}

std::string extract_file(const std::string& file_name)
{
    std::ifstream file(file_name);

    return {
        std::istreambuf_iterator<char>{file},
        std::istreambuf_iterator<char>{}
    };

}

std::optional<json> parse_json(const std::string& input)
{
    try {
        return json::parse(input);
    } catch (const json::parse_error& e) {
        return std::nullopt;
    }
}

std::optional<std::string> check_required_keys(const json& j)
{
	const std::array<std::string, 7> required_keys = 
    { "name", "alphabet", "blank", "states", "initial", "finals", "transitions" };

    for (const auto& key : required_keys)
    {
		if (!j.contains(key))
			return EMPTY_KEY(key);
	}
    return std::nullopt;
}

std::optional<std::string> check_empty(const json& j)
{
	const std::array<std::string, 7> required_keys = 
    { "name", "alphabet", "blank", "states", "initial", "finals", "transitions" };

    for (const auto& key : required_keys)
    {
        const auto& value = j.at(key);

        if (value.is_null())
            return EMPTY_CONTENT(key);
        if(key == "blank")
            cout << value << endl;
        if (value.is_string() && value.get<std::string>().empty())
            return EMPTY_CONTENT(key);

        if ((value.is_array() || value.is_object()) && value.empty())
            return EMPTY_CONTENT(key);
    }
    return std::nullopt;
}

std::optional<std::string> check_types(const json& j)
{
	const std::array<std::string, 7> required_keys = 
    { "name", "alphabet", "blank", "states", "initial", "finals", "transitions" };

    for (const auto& key : required_keys)
    {
        const auto& value = j.at(key);

        if ((key == "name" || key == "blank" || key == "initial") && !value.is_string())
            return WRONG_TYPE(key, "string");

        if ((key == "alphabet" || key == "states" || key == "finals") && !value.is_array())
            return WRONG_TYPE(key, "array");
        
        if(key == "transitions" && !value.is_object())
            return WRONG_TYPE(key, "object");
    }
    return std::nullopt;
}

std::optional<std::string> check_alphabet(const json& j)
{
    const auto& alphabet = j.at("alphabet");
    const auto& blank = j.at("blank");
    std::unordered_set<std::string> unique;

    if(!blank.is_string() || blank.size() != 1)
        return JSON_INVALID_BLANK_ERR;
    if (std::find(alphabet.begin(), alphabet.end(), blank) == alphabet.end())
        return JSON_BLANK_NOT_IN_ALPHABET_ERR;
    for (const auto& alpha : alphabet)
    {
        if(!alpha.is_string())
            return JSON_INVALID_CHARACTER_ALPHABET_ERR("");
        if(alpha.size() != 1)
            return JSON_INVALID_CHARACTER_ALPHABET_ERR(alpha);
        if (!unique.insert(alpha).second)
            return JSON_DUP_CHARACTER_ALPHABET_ERR;
    }
    return std::nullopt;
}

std::optional<std::string> check_states(const json& j)
{
    const auto& states = j.at("states");
    const auto& initial = j.at("initial");
    const auto& finals = j.at("finals");

    if (std::find(states.begin(), states.end(), initial) == states.end())
        return JSON_INITIAL_NOT_IN_STATE_LIST_ERR;
    for (const auto& final : finals)
    {
        if (std::find(states.begin(), states.end(), final) == states.end())
            return JSON_FINAL_NOT_IN_STATE_LIST_ERR(final);
    }
    return std::nullopt;
}

std::optional<std::string> check_transition_states(const json& j)
{
    const auto& states = j.at("states");
    const auto& transitions = j.at("transitions");

    for (const auto& [state, rules] : transitions.items())
    {
        if (std::find(states.begin(), states.end(), state) == states.end())
            return JSON_INVALID_STATE_TRANS_ERR(state);
    }
    return std::nullopt;
}

std::optional<std::string> check_transition_targets(const json& j)
{
    const auto& states = j.at("states");
    const auto& transitions = j.at("transitions");

    for (const auto& [state, rules] : transitions.items())
    {
        for (const auto& rule : rules)
        {
            const auto& to_state = rule.at("to_state");

            if (std::find(states.begin(), states.end(), to_state) == states.end())
                return JSON_INVALID_TRANS_TO_STATE_ERR(to_state);
        }
    }

    return std::nullopt;
}

std::optional<std::string> check_transition_writes_read(const json& j)
{
    const auto& transitions = j.at("transitions");
    const auto& alphabet = j.at("alphabet");

    for (const auto& [state, rules] : transitions.items())
    {
        for (const auto& rule : rules)
        {
            const auto& read = rule.at("read");
            const auto& write = rule.at("write");

            if (std::find(alphabet.begin(), alphabet.end(), read) == alphabet.end())
                return JSON_TRANS_READ_NOT_IN_ALPHABET_ERR(read);

            if (std::find(alphabet.begin(), alphabet.end(), write) == alphabet.end())
                return JSON_TRANS_WRITE_NOT_IN_ALPHABET_ERR(write);
        }
    }
    return std::nullopt;
}

std::optional<std::string> check_transition_actions(const json& j)
{
    const auto& transitions = j.at("transitions");
    const std::array<std::string, 2> valid_actions = { "LEFT", "RIGHT" };

    for (const auto& [state, rules] : transitions.items())
    {
        for (const auto& rule : rules)
        {
            const auto& action = rule.at("action");

            if (std::find(valid_actions.begin(), valid_actions.end(), action) == valid_actions.end())
                return JSON_INVALID_ACTION_ERR;
        }
    }
    return std::nullopt;
}

std::optional<std::string> check_transitions(const json& j)
{
    if(auto error = check_transition_states(j))
        return error;
    if(auto error = check_transition_targets(j))
        return error;   
    if(auto error = check_transition_writes_read(j))
        return error;
    if(auto error = check_transition_actions(j))
        return error;

    return std::nullopt;
}

std::optional<std::string> check_json(const json& j)
{
	if (!j)
        return "JSON is empty";

	if (auto error = check_required_keys(j))
        return error;

    if (auto error = check_empty(j))
        return error;

    if (auto error = check_types(j))
        return error;

    if (auto error = check_alphabet(j))
        return error;

    if (auto error = check_states(j))
        return error;

    if (auto error = check_transitions(j))
        return error;

    return std::nullopt;
}

std::string get_name(const json& j)
{
	return j.at("name").get<std::string>();
}

std::vector<std::string> get_alphabet(const json& j)
{
	const auto& alphabet = j.at("alphabet");
	std::vector<std::string> r_alphabet;
	r_alphabet.reserve(alphabet.size());
	for (const auto& alpha : alphabet)
		r_alphabet.emplace_back(alpha.get<std::string>());
	return r_alphabet;
}

std::string get_blank(const json& j)
{
	return j.at("blank").get<std::string>();
}

std::vector<std::string> get_states(const json& j)
{
	const auto& states = j.at("states");
	std::vector<std::string	> r_states;
	r_states.reserve(states.size());
	for (const auto& state : states)
		r_states.emplace_back(state.get<std::string>());
	return r_states;
}

std::string get_initial(const json& j)
{
	return j.at("initial").get<std::string>();
}

std::vector<std::string> get_finals(const json& j)
{
	const auto& finals = j.at("finals");
	std::vector<std::string> r_finals;
	r_finals.reserve(finals.size());
	for (const auto& final : finals)
		r_finals.emplace_back(final.get<std::string>());
	return r_finals;
}

Action get_action(const json& j)
{
    if (j["action"] == "LEFT")
        return Action::LEFT;
    return Action::RIGHT;
}

Transition get_transition(const json& j)
{
    return Transition{
        .read = j.at("read").get<std::string>()[0],
        .to_state = j.at("to_state").get<std::string>(),
        .write = j.at("write").get<std::string>()[0],
        .action = get_action(j)
    };
}

std::vector<Transition> get_transition_list(const json& j)
{
    std::vector<Transition> result;

    std::transform(
        j.begin(),
        j.end(),
        std::back_inserter(result),
        get_transition
    );

    return result;
}

std::unordered_map<std::string, std::vector<Transition>> get_transitions(const json& j)
{
    std::unordered_map<std::string, std::vector<Transition>> result;

    for (const auto& [state, transitions] : j["transitions"].items())
        result.emplace(state, get_transition_list(transitions));

    return result;
}

Machine set_machine(const json& j)
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
	return text;
}

const char* action_to_string(const Action& action)
{
    switch (action)
    {
        case Action::LEFT:  return "L";
        case Action::RIGHT: return "R";
	}
    return "?";
}

std::string print_transitions(const std::unordered_map<std::string, std::vector<Transition>>& transitions)
{
	std::string text = "]\n# Transitions:\n";
	for (const auto& [state, state_transitions] : transitions)
    {
        for (const auto& transition : state_transitions)
        {
            text += state;
            text += "[";
            text += transition.read;
            text += "] => ";
            text += transition.to_state;
            text += "; writes ";
            text += transition.write;
            text += "; goes ";
        	text += action_to_string(transition.action);
            text += "\n";
        }
    }
	return text + "\n";
}

std::string print_machine_description(const Machine& machine)
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

std::optional<std::string> check_input(const std::string& input, const std::vector<std::string>& alphabet, const std::string& blank)
{
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        std::string c(1, input[i]);
        if (std::find(alphabet.begin(), alphabet.end(), c) == alphabet.end())
            return INVALID_INPUT(c);
        else if(input[i] == blank[0])
            return INVALID_INPUT_BLANK(c);
    }
	return std::nullopt;
}

std::string first_part(const std::string &input, const int& pos, const std::string& blank)
{
	std::string text = "<";
    if (pos < 0)
	{
        text += "\033[41m";
		text += blank;
		text += "\033[0m";
        text += input;
    }
    else if(pos < input.size())
    {
        text += input.substr(0, pos);
        text += "\033[41m";
		text += input[pos];
		text += "\033[0m";
		text += input.substr(pos + 1);
    }
	else
	{
		text += input;
        text += "\033[41m";
		text += blank;
		text += "\033[0m";
	}
    return text + ">";
}

std::string print_transition(const char& c, const std::string& states, const std::unordered_map<std::string, std::vector<Transition>>& transitions)
{
	std::string text;

	text = " - ";
	for (const auto& [state, state_transitions] : transitions)
    {
		if (state != states)
            continue;

        for (const auto& transition : state_transitions)
        {
			if (transition.read != c)
                continue;
            text += state;
            text += "[";
            text += transition.read;
            text += "] => ";
            text += transition.to_state;
            text += "; writes ";
            text += transition.write;
            text += "; goes ";
        	text += action_to_string(transition.action);
            text += "\n";
        }
    }
	return text;
}

char get_read(const std::string& input, const int& pos, const std::string& blank)
{
	if (pos < 0 || pos >= static_cast<int>(input.size()))
		return blank[0];
	return input[pos];
}

std::string print_input(const std::string &input, const int& pos, const std::string& state, const Machine& machine)
{
	std::string text;

	text = first_part(input, pos, machine.blank);
    text += print_transition(get_read(input, pos, machine.blank), state, machine.transitions);
    return text;
}

int change_pos(const int& pos,const Action& action)
{
	switch (action)
    {
		case Action::LEFT:  return pos - 1;
        case Action::RIGHT: return pos + 1;
	}
	return 0;
}

std::string change_input(const std::string& input, const int& pos, const char& write, const Machine& machine)
{
    if (pos < 0)
    {
        auto new_input = machine.blank + input;
        new_input[0] = write;
        return new_input;
    }

    if (pos >= input.size())
    {
        auto new_input = input + machine.blank;
        new_input[pos] = write;
        return new_input;
    }

    auto new_input = input;
    new_input[pos] = write;
    return new_input;
}

std::optional<Transition> find_transition(const char& c, const std::string& state, const std::unordered_map<std::string, std::vector<Transition>>& transitions)
{
    const auto state_it = transitions.find(state);
    if (state_it == transitions.end())
        return std::nullopt;
    for (const auto& transition : state_it->second)
    {
        if (transition.read == c)
            return transition;
    }
    return std::nullopt;
}

std::string print_end(const std::string &input, const int& pos, const std::string& state, const Machine& machine)
{
	std::string text;

	text = first_part(input, pos, machine.blank);
	text += " - " + state;
	return text + "\n";
}

Configuration step(const Configuration& config, const Machine& machine)
{
    const char read = get_read(config.input, config.pos, machine.blank);
	const auto transition = find_transition(read, config.state, machine.transitions);

    if (!transition)
        return config;

    return Configuration
	{
        .input = change_input(config.input, config.pos, transition->write, machine),
        .pos = change_pos(config.pos, transition->action),
        .state = transition->to_state
    };
}

std::string print_output(const std::string& input, const Machine& machine)
{
    Configuration config{ .input = input, .pos = 0, .state = machine.initial};
    std::string text = "\"" + input + "\"\n";
    while (std::find(machine.finals.begin(), machine.finals.end(), config.state) == machine.finals.end())
    {
        text += print_input(config.input, config.pos, config.state, machine);
        const auto next = step(config, machine);
        if (next.state == config.state && next.pos == config.pos && next.input == config.input)
            break;
        config = next;
    }
    text += print_end(config.input, config.pos, config.state, machine);
    return text;
}
