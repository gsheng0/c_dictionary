
struct Dictionary{
    int* keys;
    void* values;
    char f_values;
    int length;
    int capacity;
    void (*action)(void* (*)(void*), void*);
};

struct Param{
    int key;
    void* value;
    struct Dictionary* dict;
};

void* insert_pair(void* args);

void* get_value(void* args);

void* display(void* args);

void run_action(void* (*fun)(void* args), void* args);