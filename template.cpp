 #include <iostream>


int main(int ac, char **av)
{
    std::string str = av[1];
    std::string str2 = av[2];
    std::string str3 = av[3];
    std::string str4 = av[4];

    // std::cout << str << std::endl;
    if(ac != 5)
        return 1;
    
    if(str4 == "r")
        std::cout << "{ \"read\" : \"" << str << "\", \"to_state\": \"" << str2 << "\", \"write\": \""  << str3 << "\", \"action\": \"" << "RIGHT" << "\"}" << std::endl;
    else if(str4 == "l")
        std::cout << "{ \"read\" : \"" << str << "\", \"to_state\": \"" << str2 << "\", \"write\": \""  << str3 << "\", \"action\": \"" << "LEFT" << "\"}" << std::endl;
    else
        std::cout << "{ \"read\" : \"" << str << "\", \"to_state\": \"" << str2 << "\", \"write\": \""  << str3 << "\", \"action\": \"" << str4 << "\"}" << std::endl;
}