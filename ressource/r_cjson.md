## Init :
    - cJSON_Parse()
        "create cJSON struct"
        ex:const char *json = "{\"name\":\"Alice\"}"; 
            cJSON *root = cJSON_Parse(json);
### Error :
    - cJSON_GetErrorPtr()
        "For analysing failed parses. This returns a pointer to the parse error. 
        You'll probably need to look a few chars back to make sense of it. 
        Defined when cJSON_Parse() returns 0. 0 when cJSON_Parse() succeeds"
### Getter :
    - cJSON_GetObjectItem()
    - cJSON_GetArraySize()
    - cJSON_GetArrayItem()
### Checker :
    - cJSON_IsObject()
    - cJSON_IsArray()
    - cJSON_IsString()
### Iterator :
    - cJSON_ArrayForEach()
        "Iterate an array"
        ex:
            cJSON *alphabet = cJSON_GetObjectItemCaseSensitive(root, "alphabet");
            cJSON *item = NULL;

            cJSON_ArrayForEach(item, alphabet) {
                printf("%s\n", item->valuestring);
            }
### Create :
    - cJSON_CreateObject()
    - cJSON_CreateArray()
    - cJSON_CreateString()
### Add :
    - cJSON_AddItemToObject()
    - cJSON_AddItemToArray()
    - cJSON_AddStringToObject()
### Print :
    - cJSON_Print()
    - cJSON_PrintUnformatted()
### Free :
    - cJSON_Delete()
        "Delete a cJSON entity and all subentities."
        ex: root->name cJSON_Delete(root) 