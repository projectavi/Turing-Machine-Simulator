#include <iostream>
#include <fstream>

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

    std::string symbol_read;

    std::string symbol_write;
    std::string left_right;
    State* next_state;
    std::string next_state_name;
};

class State {
private:
    std::string name;

public:
    bool is_initial;
    bool is_accept;
    bool is_reject;

    State* next_state_list;
    State* prev_state_list;

    TransitionNode* transition_list_head;

    void set_name(std::string new_name) {
        name = new_name;
    }

    std::string get_name() {
        return name;
    }
};

class TuringMachine {
private:
    Tape* tape;
    State* initial_state;
    State* accept_state;
    State* reject_state;
    State* state_list_head;

public:
    TuringMachine() {
        tape = new Tape();
        initial_state = nullptr;
        accept_state = nullptr;
        reject_state = nullptr;
    }

    int load_machine_from_file(char* filename) {
        // Remember to write the input string to the tape first
        std::ifstream config_file;
        config_file.open(filename);

        char* delim = (char*) ", ";
        State *curr_node;

        std::string line;
        std::string transition_information[5];

        unsigned long pos;

        // Read from config file, this will contain information about state transitions and the transition function
        // Read in the initial, accept, and reject states
        for (int i = 0; i < 3; i++) {
            getline(config_file, line);
            state_list_head = (State*)(malloc(sizeof(State)));
            state_list_head->is_initial = true;
            state_list_head->is_accept = false;
            state_list_head->is_reject = false;
            state_list_head->set_name(line);

            getline(config_file, line);
            state_list_head->next_state_list = (State*)(malloc(sizeof(State)));
            curr_node = state_list_head->next_state_list;
            curr_node->is_initial = false;
            curr_node->is_accept = true;
            curr_node->is_reject = false;
            curr_node->set_name(line);

            getline(config_file, line);
            curr_node->next_state_list = (State*)(malloc(sizeof(State)));
            curr_node = curr_node->next_state_list;
            curr_node->is_initial = false;
            curr_node->is_accept = true;
            curr_node->is_reject = false;
            curr_node->set_name(line);
        }

        while (getline(config_file, line)) {
            for (auto & i : transition_information) {
                while (pos < line.size()) {
                    pos = line.find(delim);
                    i = line.substr(0, pos);
                    line.erase(0, pos + 2);
                }
            }

            // Build a transition object

            // Find the state in the state linked list
            bool new_state_needed = true;
            curr_node = state_list_head;
            TransitionNode* curr_transition_node;
            while (curr_node->next_state_list != nullptr) {
                if (curr_node->get_name() == transition_information[0]) {
                    new_state_needed = false;
                    curr_transition_node = curr_node->transition_list_head;
                    while (curr_transition_node->next != nullptr) {
                        curr_transition_node = curr_transition_node->next;
                    }
                    curr_transition_node->next = (TransitionNode*) (malloc(sizeof (TransitionNode)));
                    curr_transition_node = curr_transition_node->next;
                    curr_transition_node->symbol_read = transition_information[1];
                    curr_transition_node->next_state_name = transition_information[2];
                    curr_transition_node->symbol_write = transition_information[3];
                    curr_transition_node->left_right = transition_information[4];
                    break;
                }
                curr_node = curr_node->next_state_list;
            }
        }
        return -1;
    }

    int load_configuration(char* filename) {
        // Read config file, this will allow the machine to be loaded in different configuraitons
        return -1;
    }

};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect number of arguments, please enter a filename" << std::endl;
    }

    char* filename = argv[1];

    auto* TM = new TuringMachine();

    TM->load_machine_from_file(filename);

    return 0;
}
