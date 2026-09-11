//testeur
#include <stdio.h>
#include "../../include/cjson/cJSON.h"

// char *create_monitor(void)
// {
//     const unsigned int resolution_numbers[3][2] = {
//         {1280, 720},
//         {1920, 1080},
//         {3840, 2160}
//     };
//     char *string = NULL;
//     cJSON *name = NULL;
//     cJSON *resolutions = NULL;
//     cJSON *resolution = NULL;
//     cJSON *width = NULL;
//     cJSON *height = NULL;
//     size_t index = 0;

//     cJSON *monitor = cJSON_CreateObject();
//     if (monitor == NULL)
//     {
//         goto end;
//     }

//     name = cJSON_CreateString("Awesome 4K");
//     if (name == NULL)
//     {
//         goto end;
//     }
//     /* after creation was successful, immediately add it to the monitor,
//      * thereby transferring ownership of the pointer to it */
//     cJSON_AddItemToObject(monitor, "name", name);

//     resolutions = cJSON_CreateArray();
//     if (resolutions == NULL)
//     {
//         goto end;
//     }
//     cJSON_AddItemToObject(monitor, "resolutions", resolutions);

//     for (index = 0; index < (sizeof(resolution_numbers) / (2 * sizeof(int))); ++index)
//     {
//         resolution = cJSON_CreateObject();
//         if (resolution == NULL)
//         {
//             goto end;
//         }
//         cJSON_AddItemToArray(resolutions, resolution);

//         width = cJSON_CreateNumber(resolution_numbers[index][0]);
//         if (width == NULL)
//         {
//             goto end;
//         }
//         cJSON_AddItemToObject(resolution, "width", width);

//         height = cJSON_CreateNumber(resolution_numbers[index][1]);
//         if (height == NULL)
//         {
//             goto end;
//         }
//         cJSON_AddItemToObject(resolution, "height", height);
//     }

//     string = cJSON_Print(monitor);
//     if (string == NULL)
//     {
//         fprintf(stderr, "Failed to print monitor.\n");
//     }

// end:
//     cJSON_Delete(monitor);
//     return string;
// }


int main(void)
{
    const char *json = "{\"name\":\"Alice\",\"age\":30,\"active\":true}";

    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        printf("Invalid JSON\n");
        return 1;
    }

    cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON *age  = cJSON_GetObjectItemCaseSensitive(root, "age");

    if (cJSON_IsString(name) && cJSON_IsNumber(age)) {
        printf("Name: %s\n", name->valuestring);
        printf("Age: %d\n", age->valueint);
    }

    cJSON_Delete(root);
    return 0;
}



// int main(void)
// {
//     char *str = create_monitor();
//     printf("%s\n", str);
// }


/* useful funct:


*/





/*
JSON file
    ↓
cJSON parser
    ↓
Machine struct
    ↓
Turing machine simulator
    ↓
Output
*/