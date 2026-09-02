#include <fcntl.h> //open
#include <unistd.h>
#include <stdio.h> //printf
#include "include/cjson/cJSON.h"

typedef struct s_conf
{
    char *name;
    char *alphabet; 
    char blank;
    char **states;
    char *initial;
    char **finals;
} t_conf;

int main(void) {
    const char *text = "{\"name\":\"Alice\",\"age\":30}";

    cJSON *json = cJSON_Parse(text);

    cJSON *name = cJSON_GetObjectItem(json, "name");
    cJSON *age  = cJSON_GetObjectItem(json, "age");

    printf("Name: %s\n", name->valuestring);
    printf("Age: %d\n", age->valueint);

    cJSON_Delete(json);
    return 0;
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
