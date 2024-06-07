#pragma once

#define BUFFER_SIZE 3

class KeyBuffer {
public:
    KeyBuffer();
    bool is_empty();
    bool is_full();
    bool find_key(int key);
    void push(int key);
    int pop();
    void remove_key(int key);

private:
    int keys[BUFFER_SIZE];
    int top, size;
};