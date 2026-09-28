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

std::string 


int main(int ac, char **av)
{
    if (ac == 1)
		ft_print_err(NO_ARG_ERR, nullptr);
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
		//check j
	}
	else 
	{ cout << ft_print_err(INVALID_JSON_ERR, nullptr); return 1; }

	//struct conf
	//set transitions
	
	//print machine description
	//check input
	//print input
}


// J.contains
// auto blank = J.find("blank");
// cout << *blank;
// string file_content = extract_file(av[1]);
// string input = av[2];
// json J = json::parse(file_content);