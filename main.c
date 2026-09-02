#include <fcntl.h> //open

typedef struct s_conf
{
    char *name;
    char *alphabet; 
    char blank;
    char **states;
    char *initial;
    char **finals;
} t_conf;

int main(void)
{
    
}

// name  str
// alphabet [c, c, c, c]
// blank c
// states [str, str, str]
// initial str 
// finals [str]

// TRANSITION: {
//     stat[1]
//     {
//         [alphabe[], states[], states[], ]]
//         [alphabe[], states[], states[], ]]
//         [alphabe[], states[], states[], ]]    
//     }
//     ...
// }

// transition = states[i]

// [transition[alphabe[], states[], states[], ]]
