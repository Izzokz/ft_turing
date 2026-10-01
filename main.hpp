#pragma once

#include "include/err_msg.h"
#include "json.hpp"
#include <fstream>  // ifstream
#include <sstream>  // stringstream
#include <iostream> // cout, endl
#include <iomanip>  // ws
#include <map>      // map
#include <ranges> // ranges
#include <numeric>
#include <string>
#include <unordered_set>
using namespace std;
using json = nlohmann::json;

# define INPUT_INVALID_BLANK 1
# define INPUT_INVALID_UNKNOWN 2

# define EMPTY_KEY(key) ("MISSING KEY " + std::string(key))
# define EMPTY_CONTENT(key) ("MISSING CONTENT " + std::string(key))
# define WRONG_TYPE(key, type) ("WRONG TYPE " + std::string(key) + std::string(type))

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
    exit(0);
}

std::string remove_leading_whitespace(const std::string& line)
{
    auto first = line.find_first_not_of(" \t");

    return first == std::string::npos ? "" : line.substr(first);
}

std::string extract_file(const std::string& file_name)
{
    std::ifstream file(file_name);

    std::string result;

    for (const auto& line : std::ranges::istream_view<std::string>(file))
        result += remove_leading_whitespace(line);

    return result;
}

std::optional<json> parse_json(const std::string& input)
{
    try {
        return json::parse(input);
    } catch (const json::parse_error& e) {
        return std::nullopt;
    }
}

std::optional<std::string> check_required_keys(const std::optional<json>& j)
{
	const std::array<std::string, 7> required_keys = 
    { "name", "alphabet", "blank", "states", "initial", "finals", "transitions" };

    for (const auto& key : required_keys)
    {
		if (!j->contains(key))
			return EMPTY_KEY(key);
	}
    return std::nullopt;
}

std::optional<std::string> check_empty(const std::optional<json>& j)
{
	const std::array<std::string, 7> required_keys = 
    { "name", "alphabet", "blank", "states", "initial", "finals", "transitions" };

    for (const auto& key : required_keys)
    {
        const auto& value = j->at(key);

        if (value.is_null())
            return EMPTY_CONTENT(key);

        if (value.is_string() && value.get<std::string>().empty())
            return EMPTY_CONTENT(key);

        if ((value.is_array() || value.is_object()) && value.empty())
            return EMPTY_CONTENT(key);
    }
    return std::nullopt;
}

std::optional<std::string> check_types(const std::optional<json>& j)
{
	const std::array<std::string, 7> required_keys = 
    { "name", "alphabet", "blank", "states", "initial", "finals", "transitions" };

    for (const auto& key : required_keys)
    {
        const auto& value = j->at(key);

        if ((key == "name" || key == "blank" || key == "initial") && !value.is_string())
            return WRONG_TYPE(key, "string");

        if ((key == "alphabet" || key == "states" || key == "finals") && !value.is_array())
            return WRONG_TYPE(key, "array");
        
        if(key == "transitions" && !value.is_object())
            return WRONG_TYPE(key, "object");
    }
    return std::nullopt;
}

std::optional<std::string> check_alphabet(const std::optional<json>& j)
{
    const auto& alphabet = j->at("alphabet");
    const auto& blank = j->at("blank");
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

std::optional<std::string> check_states(const std::optional<json>& j)
{
    const auto& states = j->at("states");
    const auto& initial = j->at("initial");
    const auto& finals = j->at("finals");

    if (std::find(states.begin(), states.end(), initial) == states.end())
        return JSON_INITIAL_NOT_IN_STATE_LIST_ERR;
    for (const auto& final : finals)
    {
        if (std::find(states.begin(), states.end(), final) == states.end())
            return JSON_FINAL_NOT_IN_STATE_LIST_ERR(final);
    }
    return std::nullopt;
}

std::optional<std::string> check_transition_states(const std::optional<json>& j)
{
    const auto& states = j->at("states");
    const auto& transitions = j->at("transitions");

    for (const auto& [state, rules] : transitions.items())
    {
        if (std::find(states.begin(), states.end(), state) == states.end())
            return JSON_INVALID_STATE_TRANS_ERR(state);
    }
    return std::nullopt;
}

std::optional<std::string> check_transition_targets(const std::optional<json>& j)
{
    const auto& states = j->at("states");
    const auto& transitions = j->at("transitions");

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

std::optional<std::string> check_transition_writes_read(const std::optional<json>& j)
{
    const auto& transitions = j->at("transitions");
    const auto& alphabet = j->at("alphabet");

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

std::optional<std::string> check_transition_actions(const std::optional<json>& j)
{
    const auto& transitions = j->at("transitions");
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

std::optional<std::string> check_transitions(const std::optional<json>& j)
{
    if(auto error = check_transition_states(*j))
        return error;
    if(auto error = check_transition_targets(*j))
        return error;   
    if(auto error = check_transition_writes_read(*j))
        return error;
    if(auto error = check_transition_actions(*j))
        return error;

    return std::nullopt;
}

std::optional<std::string> check_json(const std::optional<json>& j)
{
	if (!j)
        return "JSON is empty";

	if (auto error = check_required_keys(*j))
        return error;

    if (auto error = check_empty(*j))
        return error;

    if (auto error = check_types(*j))
        return error;

    if (auto error = check_alphabet(*j))
        return error;

    if (auto error = check_states(*j))
        return error;

    if (auto error = check_transitions(*j))
        return error;

    return std::nullopt;
}
