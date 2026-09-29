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

std::string check_json(auto j)
{
	cJSON	*obj = cJSON_GetObjectItem(g_json, "name");
	if (!cJSON_IsString(obj) || !(*obj).valuestring)
		ft_print_err(JSON_INVALID_NAME_ERR, (*obj).valuestring);
	g_conf.name = strdup((*obj).valuestring);
	if (!g_conf.name)
		ft_print_err(ALLOC_ERR, 0);

	obj = cJSON_GetObjectItem(g_json, "alphabet");
	int		arsize;
	if (!cJSON_IsArray(obj) || (arsize = cJSON_GetArraySize(obj)) < 1)
		ft_print_err(JSON_INVALID_ALPHABET_ERR, 0);
	if (!(g_conf.alphabet = malloc(arsize + 1)))
		ft_print_err(ALLOC_ERR, 0);
	int		i = -1;
	char	c;
	obj = (*obj).child;
	while (obj)
	{
		if (!cJSON_IsString(obj) || strlen((*obj).valuestring) != 1)
			ft_print_err(JSON_INVALID_CHARACTER_ALPHABET_ERR, (*obj).valuestring);
		*(g_conf.alphabet + ++i) = c = *((*obj).valuestring);
		for (int x = 0; x < i; ++x)
			if (c == *(g_conf.alphabet + x))
				ft_print_err(JSON_DUP_CHARACTER_ALPHABET_ERR, (*obj).valuestring);
		obj = (*obj).next;
	}
	*(g_conf.alphabet + arsize) = 0;

	obj = cJSON_GetObjectItem(g_json, "blank");
	if (!cJSON_IsString(obj) || !(*obj).valuestring || (strlen((*obj).valuestring) != 1))
		ft_print_err(JSON_INVALID_BLANK_ERR, (*obj).valuestring);
	g_conf.blank = c = *((*obj).valuestring);
	for (int x = 0; *(g_conf.alphabet + x); ++x)
	{
		if (c == *(g_conf.alphabet + x))
		{
			c = 0;
			break ;
		}
	}
	if (c)
		ft_print_err(JSON_BLANK_NOT_IN_ALPHABET_ERR, (*obj).valuestring);

	obj = cJSON_GetObjectItem(g_json, "states");
	if (!cJSON_IsArray(obj) || (g_max_I = cJSON_GetArraySize(obj)) < 1)
		ft_print_err(JSON_INVALID_STATE_LIST_ERR, 0);
	if (!(g_conf.states = calloc((g_max_I + 1), sizeof(void *))))
		ft_print_err(ALLOC_ERR, 0);
	i = -1;
	char	*str;
	obj = (*obj).child;
	while (obj)
	{
		if (!cJSON_IsString(obj))
			ft_print_err(JSON_INVALID_STATE_ERR, 0);
		if (!(*(g_conf.states + ++i) = str = strdup((*obj).valuestring)))
			ft_print_err(ALLOC_ERR, 0);
		for (int x = 0; x < i; ++x)
			if (ft_sequals(str, *(g_conf.states + x)))
				ft_print_err(JSON_DUP_STATE_ERR, (*obj).valuestring);
		obj = (*obj).next;
	}

	obj = cJSON_GetObjectItem(g_json, "initial");
	if (!cJSON_IsString(obj) || !(str = (*obj).valuestring))
		ft_print_err(JSON_INVALID_INITIAL_ERR, (*obj).valuestring);
	for (int x = 0; *(g_conf.states + x); ++x)
	{
		if (ft_sequals(str, *(g_conf.states + x)))
		{
			str = 0;
			g_conf.initial = x;
			break ;
		}
	}
	if (str)
		ft_print_err(JSON_INITIAL_NOT_IN_STATE_LIST_ERR, str);

	obj = cJSON_GetObjectItem(g_json, "finals");
	if (!cJSON_IsArray(obj) || (g_conf.fsize = cJSON_GetArraySize(obj)) < 1)
		ft_print_err(JSON_INVALID_FINAL_LIST_ERR, 0);
	if (!(g_conf.finals = malloc(g_conf.fsize * sizeof(int))))
		ft_print_err(ALLOC_ERR, 0);
	i = -1;
	obj = (*obj).child;
	while (obj)
	{
		if (!cJSON_IsString(obj) || !(str = (*obj).valuestring))
			ft_print_err(JSON_INVALID_FINAL_ERR, (*obj).valuestring);
		for (int x = 0; *(g_conf.states + x); ++x)
		{
			if (ft_sequals(str, *(g_conf.states + x)))
			{
				str = 0;
				*(g_conf.finals + ++i) = x;
				break ;
			}
		}
		if (str)
			ft_print_err(JSON_FINAL_NOT_IN_STATE_LIST_ERR, str);
		for (int x = 0; x < i; ++x)
			if (*(g_conf.finals + i) == *(g_conf.finals + x))
				ft_print_err(JSON_DUP_FINAL_ERR, (*obj).valuestring);
		obj = (*obj).next;
	}
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
		// cout << (*j)["name"]; //"unary_add"
		// check_json(j);
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