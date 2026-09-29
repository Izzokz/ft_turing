#include "include/err_msg.h"
#include "json.hpp" // C++11 | -std=c++11
#include <fstream>  // ifstream
#include <sstream>  // stringstream
#include <iostream> // cout, endl
#include <iomanip>  // ws
#include <map>      // map
#include <ranges> // ranges
#include <numeric>
#include <string>
using namespace std;
using json = nlohmann::json;

# define INPUT_INVALID_BLANK 1
# define INPUT_INVALID_UNKNOWN 2

# define EMPTY_KEY(key) ("MISSING KEY " + std::string(key))

// # define ERROR_MESSAGE(err) ("ERR[" + std::string(err) + "]")
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

    return first == std::string::npos
        ? ""
        : line.substr(first);
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

std::string print_machine_description()
{
	return "PLACEHOLDER";
}

std::optional<std::string> check_json(const std::optional<json>& j)
{
	if (!j)
        return "JSON is empty";

    const std::array<std::string, 7> required_keys = {
        "name",
        "alphabet",
        "blank",
        "states",
        "initial",
        "finals",
        "transitions"
    };

    for (const auto& key : required_keys)
    {
		if (!j->contains(key))
			return EMPTY_KEY(key);
	}

	//check

    return std::nullopt;
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

		// struct conf
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
#include <string>
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

    std::unordered_map<
        std::string,
        std::vector<Transition>
    > transitions;
};


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