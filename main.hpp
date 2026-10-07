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

enum class Action
{
    LEFT,
    RIGHT
};

struct Transition
{
    char read;
    std::string to_state;
    char write;
    Action action;
};

struct Machine
{
    std::string name;
    std::vector<std::string> alphabet;
    std::string blank;
    std::vector<std::string> states;
    std::string initial;
    std::vector<std::string> finals;
    std::vector<std::pair<std::string, std::vector<Transition>>> transitions;
};

struct Configuration
{
    std::string input;
    int pos;
    std::string state;

    bool operator==(const Configuration& other) const
    {
        return input == other.input
            && pos == other.pos
            && state == other.state;
    }
};

struct History
{
    Configuration config;
    std::shared_ptr<const History> next;
};

using HistoryPtr = std::shared_ptr<const History>;


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
    const std::array<std::string, 7> keys = {
        "name",
        "alphabet",
        "blank",
        "states",
        "initial",
        "finals",
        "transitions"
    };

    const auto missing = std::find_if(
        keys.begin(),
        keys.end(),
        [&j](const auto& key)
        {
            return !j.contains(key);
        }
    );

    return missing == keys.end()
        ? std::nullopt
        : std::optional<std::string>(EMPTY_KEY(*missing));
}

std::optional<std::string> check_empty(const json& j)
{
    const std::array<std::string, 7> keys = {
        "name",
        "alphabet",
        "blank",
        "states",
        "initial",
        "finals",
        "transitions"
    };

    const auto invalid = std::find_if(
        keys.begin(),
        keys.end(),
        [&j](const auto& key)
        {
            const auto& value = j.at(key);

            return value.is_null()
                || (value.is_string() && value.get<std::string>().empty())
                || ((value.is_array() || value.is_object()) && value.empty());
        }
    );

    return invalid == keys.end()
        ? std::nullopt
        : std::optional<std::string>(EMPTY_CONTENT(*invalid));
}

std::optional<std::string> check_types(const json& j)
{
    const auto& name = j.at("name");
    const auto& blank = j.at("blank");
    const auto& initial = j.at("initial");
    const auto& alphabet = j.at("alphabet");
    const auto& states = j.at("states");
    const auto& finals = j.at("finals");
    const auto& transitions = j.at("transitions");

    if (!name.is_string())
        return WRONG_TYPE("name", "string");

    if (!blank.is_string())
        return WRONG_TYPE("blank", "string");

    if (!initial.is_string())
        return WRONG_TYPE("initial", "string");

    if (!alphabet.is_array())
        return WRONG_TYPE("alphabet", "array");

    if (!states.is_array())
        return WRONG_TYPE("states", "array");

    if (!finals.is_array())
        return WRONG_TYPE("finals", "array");

    if (!transitions.is_object())
        return WRONG_TYPE("transitions", "object");

    return std::nullopt;
}

bool has_duplicate(
    const json& values,
    std::size_t index)
{
    if (index >= values.size())
        return false;

    const auto count = std::count(
        values.begin(),
        values.end(),
        values.at(index)
    );

    return count > 1 || has_duplicate(values, index + 1);
}

std::optional<std::string> check_alphabet(const json& j)
{
    const auto& alphabet = j.at("alphabet");
    const auto& blank = j.at("blank");

    if (!blank.is_string() || blank.size() != 1)
        return JSON_INVALID_BLANK_ERR;

    if (std::find(
            alphabet.begin(),
            alphabet.end(),
            blank
        ) == alphabet.end())
    {
        return JSON_BLANK_NOT_IN_ALPHABET_ERR;
    }

    if (has_duplicate(alphabet, 0))
        return JSON_DUP_CHARACTER_ALPHABET_ERR;

    const auto invalid = std::find_if(
        alphabet.begin(),
        alphabet.end(),
        [](const auto& alpha)
        {
            return !alpha.is_string() || alpha.size() != 1;
        }
    );

    if (invalid != alphabet.end())
    {
        if (!invalid->is_string())
            return JSON_INVALID_CHARACTER_ALPHABET_ERR("");

        return JSON_INVALID_CHARACTER_ALPHABET_ERR(
            invalid->get<std::string>()
        );
    }

    return std::nullopt;
}

std::optional<std::string> check_states(const json& j)
{
    const auto& states = j.at("states");
    const auto& initial = j.at("initial");
    const auto& finals = j.at("finals");

    if (std::find(
            states.begin(),
            states.end(),
            initial
        ) == states.end())
    {
        return JSON_INITIAL_NOT_IN_STATE_LIST_ERR;
    }

    const auto invalid_final = std::find_if(
        finals.begin(),
        finals.end(),
        [&states](const auto& final)
        {
            return std::find(
                states.begin(),
                states.end(),
                final
            ) == states.end();
        }
    );

    if (invalid_final != finals.end())
        return JSON_FINAL_NOT_IN_STATE_LIST_ERR(
            invalid_final->get<std::string>()
        );

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
        const auto invalid = std::find_if(
            rules.begin(),
            rules.end(),
            [&states](const auto& rule)
            {
                const auto& target = rule.at("to_state");

                return std::find(
                    states.begin(),
                    states.end(),
                    target
                ) == states.end();
            }
        );

        if (invalid != rules.end())
            return JSON_INVALID_TRANS_TO_STATE_ERR(
                invalid->at("to_state")
            );
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
    std::vector<std::string> result(alphabet.size());

    std::transform(
        alphabet.begin(),
        alphabet.end(),
        result.begin(),
        [](const auto& value) {
            return value.get<std::string>();
        }
    );

    return result;
}


std::string get_blank(const json& j)
{
	return j.at("blank").get<std::string>();
}

std::vector<std::string> get_states(const json& j)
{
    const auto& states = j.at("states");

    std::vector<std::string> result(states.size());

    std::transform(
        states.begin(),
        states.end(),
        result.begin(),
        [](const auto& state) {
            return state.get<std::string>();
        }
    );

    return result;
}


std::string get_initial(const json& j)
{
	return j.at("initial").get<std::string>();
}

std::vector<std::string> get_finals(const json& j)
{
    const auto& finals = j.at("finals");

    std::vector<std::string> result(finals.size());

    std::transform(
        finals.begin(),
        finals.end(),
        result.begin(),
        [](const auto& value)
        {
            return value.get<std::string>();
        }
    );

    return result;
}


Action get_action(const json& j)
{
    return j.at("action") == "LEFT"
        ? Action::LEFT
        : Action::RIGHT;
}


std::vector<std::pair<std::string, std::vector<Transition>>>
get_transitions(const json& j)
{
    const auto& transitions = j.at("transitions");

    std::vector<std::pair<std::string, std::vector<Transition>>> result;
    result.reserve(transitions.size());

    std::transform(
        transitions.items().begin(),
        transitions.items().end(),
        std::back_inserter(result),
        [](const auto& item)
        {
            return std::make_pair(
                item.key(),
                get_transition_list(item.value())
            );
        }
    );

    return result;
}



std::vector<Transition> get_transition_list(const json& j)
{
    std::vector<Transition> result(j.size());

    std::transform(
        j.begin(),
        j.end(),
        result.begin(),
        get_transition
    );

    return result;
}


std::vector<std::pair<std::string, std::vector<Transition>>>
get_transitions(const json& j)
{
    const auto& transitions = j.at("transitions");

    std::vector<std::pair<std::string, std::vector<Transition>>>
        result(transitions.size());

    std::transform(
        transitions.begin(),
        transitions.end(),
        result.begin(),
        [](const auto& item)
        {
            return std::make_pair(
                item.key(),
                get_transition_list(item.value())
            );
        }
    );

    return result;
}

Machine set_machine(const json& j)
{
    return {
        get_name(j),
        get_alphabet(j),
        get_blank(j),
        get_states(j),
        get_initial(j),
        get_finals(j),
        get_transitions(j)
    };
}

std::string print_name(const std::string& name)
{
	return "####\n##" + name + "\n####";
}

std::string print_list(
    const std::vector<std::string>& values,
    std::size_t index)
{
    if (index == values.size())
        return "";

    return values[index]
        + (index + 1 == values.size()
            ? ""
            : ", ")
        + print_list(values, index + 1);
}

std::string print_alphabet(
    const std::vector<std::string>& alphabet)
{
    return "\n# Alphabet: ["
        + print_list(alphabet, 0)
        + "]";
}

std::string print_states(
    const std::vector<std::string>& states)
{
    return "\n# States: ["
        + print_list(states, 0)
        + "]";
}


std::string print_intial(const std::string& initial)
{
	return "\n# Initial: " + initial;
}

std::string print_finals(
    const std::vector<std::string>& finals)
{
    return "\n# Finals: ["
        + print_list(finals, 0)
        + "]";
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

std::string print_transition_rules(
    char c,
    const std::string& state,
    const std::vector<Transition>& rules,
    std::size_t index)
{
    if (index == rules.size())
        return "";

    const auto& transition = rules[index];

    if (transition.read != c)
        return print_transition_rules(
            c,
            state,
            rules,
            index + 1
        );

    return " - "
        + state
        + "["
        + transition.read
        + "] => "
        + transition.to_state
        + "; writes "
        + transition.write
        + "; goes "
        + action_to_string(transition.action)
        + "\n"
        + print_transition_rules(
            c,
            state,
            rules,
            index + 1
        );
}

std::string print_transition(
    char c,
    const std::string& state,
    const std::vector<
        std::pair<std::string, std::vector<Transition>>
    >& transitions)
{
    const auto it = std::find_if(
        transitions.begin(),
        transitions.end(),
        [&state](const auto& item)
        {
            return item.first == state;
        }
    );

    if (it == transitions.end())
        return " - ";

    return print_transition_rules(
        c,
        state,
        it->second,
        0
    );
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
    return action == Action::LEFT
        ? pos - 1
        : pos + 1;
}


std::string change_input(
    const std::string& input,
    int pos,
    char write,
    const Machine& machine)
{
    if (pos < 0)
    {
        return std::string(1, write)
            + input;
    }

    if (pos >= static_cast<int>(input.size()))
    {
        return input
            + std::string(1, write);
    }

    return input.substr(0, pos)
        + std::string(1, write)
        + input.substr(pos + 1);
}


std::optional<Transition> find_transition_in_state(
    char c,
    const std::vector<Transition>& transitions,
    std::size_t index)
{
    if (index == transitions.size())
        return std::nullopt;

    if (transitions[index].read == c)
        return transitions[index];

    return find_transition_in_state(
        c,
        transitions,
        index + 1
    );
}

std::optional<Transition> find_transition(
    const char c,
    const std::string& state,
    const std::vector<std::pair<std::string, std::vector<Transition>>>& transitions)
{
    const auto state_it = std::find_if(
        transitions.begin(),
        transitions.end(),
        [&state](const auto& item)
        {
            return item.first == state;
        }
    );

    if (state_it == transitions.end())
        return std::nullopt;

    const auto transition_it = std::find_if(
        state_it->second.begin(),
        state_it->second.end(),
        [c](const Transition& transition)
        {
            return transition.read == c;
        }
    );

    if (transition_it == state_it->second.end())
        return std::nullopt;

    return *transition_it;
}



std::string print_end(const std::string &input, const int& pos, const std::string& state, const Machine& machine)
{
	std::string text;

	text = first_part(input, pos, machine.blank);
	text += " - " + state;
	return text + "\n";
}

std::optional<Configuration> step(
    const Configuration& config,
    const Machine& machine)
{
    const auto transition = find_transition(
        get_read(config.input, config.pos, machine.blank),
        config.state,
        machine.transitions
    );

    if (!transition)
        return std::nullopt;

    return Configuration{
        change_input(
            config.input,
            config.pos,
            transition->write,
            machine
        ),
        change_pos(
            config.pos,
            transition->action
        ),
        transition->to_state
    };
}

HistoryPtr make_history(
    const Configuration& config,
    HistoryPtr next)
{
    return std::make_shared<History>(
        History{config, next}
    );
}

std::vector<Configuration> prepend(
    const Configuration& config,
    const std::vector<Configuration>& history)
{
    std::vector<Configuration> result;
    result.reserve(history.size() + 1);

    result.push_back(config);
    result.insert(result.end(), history.begin(), history.end());

    return result;
}

HistoryPtr run(
    const Configuration& config,
    const Machine& machine)
{
    if (std::find(
            machine.finals.begin(),
            machine.finals.end(),
            config.state
        ) != machine.finals.end())
    {
        return make_history(config, nullptr);
    }

    const auto next = step(config, machine);

    if (!next)
        return make_history(config, nullptr);

    return make_history(
        config,
        run(*next, machine)
    );
}

std::string print_history(
    const HistoryPtr& history,
    const Machine& machine)
{
    if (!history)
        return "";

    return print_input(
        history->config.input,
        history->config.pos,
        history->config.state,
        machine
    ) + print_history(
        history->next,
        machine
    );
}

Configuration last_configuration(
    const HistoryPtr& history)
{
    if (!history->next)
        return history->config;

    return last_configuration(history->next);
}

std::string print_output(
    const std::string& input,
    const Machine& machine)
{
    const Configuration initial{
        input,
        0,
        machine.initial
    };

    const auto history = run(initial, machine);
    const auto final = last_configuration(history);

    return "\"" + input + "\"\n"
        + print_history(history, machine)
        + print_end(
            final.input,
            final.pos,
            final.state,
            machine
        );
}
