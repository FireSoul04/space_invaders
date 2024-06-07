#include "keybuffer.hpp"

KeyBuffer::KeyBuffer() {
    top = 0;
    size = BUFFER_SIZE;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        keys[i] = 0;
    }
}

bool KeyBuffer::is_empty() {
    return top == 0;
}

bool KeyBuffer::is_full() {
    return top == size;
}

bool KeyBuffer::find_key(int key) {
    bool found = false;
    for (int i = 0; i < BUFFER_SIZE && !found; i++) {
        if (keys[i] == key) {
            found = true;
        }
    }
    return found;
}

void KeyBuffer::push(int key) {
    if (is_full()) {
        return;
    }
    keys[top++] = key;
}

int KeyBuffer::pop() {
    if (is_empty()) {
        return -1;
    }
    top--;
    int key = keys[top];
    keys[top] = 0;
    return key;
}

void KeyBuffer::remove_key(int key) {
    if (is_empty()) {
        return;
    }
    KeyBuffer temp;
    int k;
    // Takes all the keys that are not the key
    // wanted to remove and save them in stack order
    while (!this->is_empty()) {
        k = this->pop();
        if (k != key) {
            temp.push(k);
        }
    }
    while (!temp.is_empty()) {
        k = temp.pop();
        this->push(k);
    }
}