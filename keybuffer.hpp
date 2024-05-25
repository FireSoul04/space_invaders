#ifndef KEYBUFFER_H
#define KEYBUFFER_H

#define BUFFER_SIZE 3

class KeyBuffer {
public:
    KeyBuffer() {
        top = 0;
        size = BUFFER_SIZE;
        for (int i = 0; i < BUFFER_SIZE; i++) {
            keys[i] = 0;
        }
    }

    bool is_empty() {
        return top == 0;
    }

    bool is_full() {
        return top == size;
    }

    bool find_key(int key) {
        bool found = false;
        for (int i = 0; i < BUFFER_SIZE && !found; i++) {
            if (keys[i] == key) {
                found = true;
            }
        }
        return found;
    }

    void push(int key) {
        if (is_full()) {
            return;
        }
        keys[top++] = key;
    }

    int pop() {
        if (is_empty()) {
            return -1;
        }
        top--;
        int key = keys[top];
        keys[top] = 0;
        return key;
    }

    void remove_key(int key) {
		if (is_empty()) {
			return;
		}
        KeyBuffer temp = KeyBuffer();
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

private:
    int keys[BUFFER_SIZE];
    int top, size;
};

#endif