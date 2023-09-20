#include <iostream>

// Defining class names so I can use them wherever
class TapeCell;
class Tape;
class TransitionNode;
class State;
class TuringMachine;

class TapeCell {
private:
    char symbol;

public:
    TapeCell* left_cell;
    TapeCell* right_cell;

    char read_cell() {
        return symbol;
    }

    void write_cell(char new_symbol) {
        symbol = new_symbol;
   }
};

class Tape {
private:
    TapeCell* tape_head;

public:
    char read_head() {
        return tape_head->read_cell();
    }

    void write_head(char new_symbol) {
        tape_head->write_cell(new_symbol);
    }

    void move_head_left() {
        if (tape_head->left_cell != nullptr) {
            tape_head = tape_head->left_cell;
        }
    }

    void move_head_right() {
        if (tape_head->right_cell == nullptr) {
            tape_head->right_cell = new TapeCell();
            tape_head->left_cell = tape_head;
        }
        tape_head = tape_head->right_cell;
    }

    Tape() {
        tape_head = new TapeCell();
    }
};

class TransitionNode {
public:
    TransitionNode* next;
    TransitionNode* prev;

    char* symbol_read;
    char* symbol_write;
    char left_right;
    State* next_state;
};

class State {
private:
    bool is_initial;
    bool is_accept;
    bool is_reject;
    char* name;

    TransitionNode* transition_list_head;

public:
    State(bool initial, bool accept, bool reject, char* state_name) {
        is_accept = accept;
        is_reject = reject;
        is_initial = initial;
        name = state_name;
        transition_list_head = nullptr;
    }
};

class TuringMachine {
private:
    Tape* tape;
    State* initial_state;
    State* accept_state;
    State* reject_state;

public:
    TuringMachine() {
        tape = new Tape();
        initial_state = nullptr;
        accept_state = nullptr;
        reject_state = nullptr;
    }

    int load_from_file(char* filename) {
        // Remember to write the input string to the tape first
        return -1;
    }


    // TODO - figure out how to do transitions, either treat each transition as a class and create based on read or bake it into the states, i think its easier to make a class for transitions

};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
