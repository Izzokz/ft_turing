#include "include/err_msg.h"
#include "json.hpp" // C++11 | -std=c++11
#include <fstream>  // ifstream
#include <sstream>  // stringstream
#include <iostream> // cout, endl
#include <iomanip>  // ws
#include <map>      // map
#include <ranges> // ranges
using namespace std;
// -Werror -Wall -Wextra
// g++ -std=c++20 main.cpp


char	ft_sequals(char *s1, char *s2)
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

void	ft_print_err(char *err, char *i)
{
	if (!i)
		printf("ERR[%s]\n", err);
	else
		printf("ERR[%s](%s)\n", err, i);
	exit(1);
}

void	ft_print_help(void)
{
	std::cout << "usage: ft_turing [-h] jsonfile input\n\npositional arguments:\n\tjsonfile\t\t\tjson description of the machine\n\tinput\t\t\t\tinput of the machine\n\noptional arguments:\n\t-h, --help\t\t\tshow this help message and exit\n";
    exit(0);
}

std::string remove_leading_whitespace(std::string line)
{
    auto first = line.find_first_not_of(" \t");

    return first == std::string::npos
        ? ""
        : line.substr(first);
}


std::string extract_file(const std::string& file_name)
{
    std::ifstream file(file_name);

    auto lines = std::ranges::istream_view<std::string>(file);

    return std::ranges::fold_left(
        lines,
        std::string{},
        [](std::string result, std::string line) {
            return result + remove_leading_whitespace(std::move(line));
        }
    );
}

int main(int ac, char **av)
{
    if (ac == 1)
		ft_print_err(NO_ARG_ERR, 0);
	if (ac == 2)
	{
		if (ft_sequals(*++av, "--help") || ft_sequals(*av, "-h"))
			ft_print_help();
		else
			ft_print_err(NOT_ENOUGH_ARG_ERR, 0);
	}
    extract_file(av[1]);
}
