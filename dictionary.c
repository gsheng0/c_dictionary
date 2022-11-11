#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Dictionary {
    int* keys;
    void* values; //either a string or integer
    char f_values;
    int length;
    int capacity;
    void (*action)(void* (*)(void*), void*);
};

struct Param {
    struct Dictionary* dict;
    int key;
    void* value;
};

char* add_string(char* arr, char* str, int* current){
    for(int i = 0; i < strlen(str); i++){
        arr[*current + i] = str[i];
    }
    *current += strlen(str);
    arr[*current] = '\0';
    (*current)++;
    return arr;
}

char* resize_string_arr(char* arr, int current, int to_add_length){
    char* temp = (char*)(malloc(sizeof(char) * current));
    for(int i = 0; i < current; i++){
        temp[i] = arr[i];
    }
    free(arr);
    arr = (char*)(malloc(sizeof(char) * (current + to_add_length)));
    for(int i = 0; i < current; i++){
        arr[i] = temp[i];
    }
    arr[current] = '\0';
    free(temp);
    return arr;
}

int* resize_int_array(int* arr, int length, int* capacity){
    //double array capacity if elements have filled up 50% of capacity
    int* temp = (int *)(malloc(sizeof(int) * length));
    for(int i = 0; i < length; i++){
        temp[i] = arr[i];
    }
    free(arr);
    (*capacity) *= 2;
    arr = (int *)(malloc(sizeof(int) * (*capacity)));
    
    for(int i = 0; i < length; i++){
        arr[i] = temp[i];
    }
    free(temp);
    return arr;   
}

void add_integer(int* arr, int element, int* length){
    arr[*length] = element;
    (*length)++;
}

char* read_string(){
    char* buff = (char*)(malloc(sizeof(char) * 16));
    int tracker = 0;
    fgets(buff, 16, stdin);
    while(buff[strlen(buff) - 1] != '\n'){
        //mallocing a char array to temporarily store the characters that have already been read in
        char* buff1 = (char*)(malloc(sizeof(char) * strlen(buff))); 

        //copying contents from buff to buff1
        for(int i = 0; i < strlen(buff); i++){
            //copies everything except for newline characters/null terminators
            buff1[i] = buff[i];
        }
        buff1[strlen(buff)] = '\0';

        //freeing buffer to make a new array with more space
        free(buff);
        int increase = strlen(buff1);
        //reallocating buff to be able to hold more characters
        buff = (char*)(malloc(sizeof(char) * strlen(buff1) + increase));

        //copying contents from buff1 back to buff
        for(int i = 0; i < strlen(buff1); i++){
            buff[i] = buff1[i];
        }

        //reads in 16 more characters, starting at the end of the original content in buff
        fgets(&buff[strlen(buff1)], increase, stdin);

        //freeing buff1 because its content has been transferred to buff already and is no longer needed
        free(buff1);
    }
    buff[strlen(buff) - 1] = '\0';
    return buff;
}

void initialize_arrays(struct Dictionary* dict){
    if(dict->f_values == 'd'){
        dict->keys = (int*)(malloc(sizeof(int) * 16));
        dict->values = (int*)(malloc(sizeof(int) * 16));
        dict->capacity = 16;
    }
    else if(dict->f_values == 's'){
        dict->keys = (int*)(malloc(sizeof(int) * 16));
        dict->keys = (int*)(malloc(sizeof(char) * 128));
        dict->capacity = 16;
    }
    else{
        printf("Error: dictionary type has not been set.\n");
    }
}

int find_end_of_char_array(char* arr, int words){
    int i = 0;
    while(words > 0){
        if(arr[i] == '\0'){
            words--;
        }
        i++;
    }
    return i;
}

void set_type(struct Dictionary* dict, char type){
    dict->f_values = type;
}

void* insert_pair(void* args){
    struct Param params = *((struct Param*)args);
    struct Dictionary* dict = params.dict;
    if(dict->f_values == 'd'){
        int value = *((int*)params.value);
        int key = params.key;
        if(dict->length > dict->capacity / 2){
            dict->keys = resize_int_array(dict->keys, dict->length, &dict->capacity);
            dict->values = resize_int_array((int*)dict->values, dict->length, &dict->capacity);
        }
        dict->keys[dict->length] = key;
        int* ptr = (int*)dict->values;
        ptr[dict->length] = value;
        dict->length++;
    }
    else if(dict->f_values == 's'){
        int key = params.key;
        char* str = (char*)params.value;
        int insert_length = strlen(str);
        if(dict->length > dict->capacity / 2){
            dict->keys = resize_int_array(dict->keys, dict->length, &dict->capacity);
        }
        int current = find_end_of_char_array((char*)dict->values, dict->length);
        dict->values = resize_string_arr((char*)dict->values, current, insert_length);
        add_string((char*)dict->values, str, &current);
        //add_integer(dict->keys, key, &dict->length);
        dict->keys[dict->length] = key;
        dict->length++;
         

    }
    else{

    }
    return 0;
}

char* get_string(char* arr, int index){
    int current = 0;
    while(index > 0){
        if(arr[current] == '\0')
        {
            index--;
        }
        current++;
    }
    return &arr[current];
}

void* get_value(void* args){
    struct Param* params = (struct Param*) args;
    struct Dictionary* dict = (*params).dict;
    if(dict->f_values == 'd'){
        int* arr = (int*)dict->values;
        params->value = &arr[params->key];
    }
    else if(dict->f_values == 's'){
        char* arr = (char*)dict->values;
        params->value = get_string(arr, params->key);
    }
    return 0;
}

void* display(void* args){
    struct Param params = *((struct Param*)args);
    struct Dictionary* dict = params.dict;
    printf("Printing dictionary!\n");
    printf("Length: %d\n", dict->length);
    printf("Capacity: %d\n", dict->capacity);
    if(dict->f_values == 'd'){
        for(int i = 0; i < dict->length; i++){
            printf("\t%d: %d\n", dict->keys[i], ((int*)dict->values)[i]);
        }
    }
    else if(dict->f_values == 's'){
        int prev = 0;
        int index = 0;
        for(int i = 0; i < dict->length; i++){
            printf("\t%d: ", dict->keys[i]);
            char* arr = (char*)dict->values;
            while(arr[index] != '\0'){
                printf("%c", arr[index]);
                index++;
            }
            index++;
            printf("\n");
        }
    }
    return 0;
}
void run_action(void* (*fun)(void* args), void* args){
    (*fun)(args);
}

int main(){
    struct Dictionary d;
    set_type(&d, 's');
    initialize_arrays(&d);
    d.action = run_action;

    while(1){
        struct Param params;
        memset(&params, '\0', sizeof(params));
        params.dict = &d;
        int x;
        printf("Enter your key value: ");
        scanf("%d", &x);
        fflush(stdin);
        params.key = x;
        printf("Enter a string: ");
        params.value = read_string();
        d.action(insert_pair, &params);
        d.action(display, &params);
        struct Param params1;
        memset(&params1, 0, sizeof(params1));
        params1.dict = &d;
        params1.key = d.length - 1;
        char* out;
        params1.value = out;
        d.action(get_value, &params1);

        printf("String at %d: %s\n", d.length - 1, out);
    }
    
    return 0;
    
}