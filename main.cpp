#include <iostream>
#include <fstream>
#include <utility>
#include <cassert>

// Defining class names so I can use them wherever
class TapeCell;
class Tape;
class TransitionNode;
class State;
class TuringMachine;

using namespace std;

class TapeCell {
private:
    string symbol;

public:
    TapeCell* left_cell;
    TapeCell* right_cell;

    string read_cell() {
        return symbol;
    }

    void write_cell(string new_symbol) {
        symbol = new_symbol;
    }

    void print_cell() {
        cout << " [ " + symbol + " ] ";
    }
};

class Tape {
private:
    TapeCell* tape_head;

public:
    string read_head() {
        return tape_head->read_cell();
    }

    void write_head(string new_symbol) {
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
            tape_head->right_cell->write_cell("_");
            tape_head->right_cell->left_cell = tape_head;
        }
        tape_head = tape_head->right_cell;
    }

    Tape() {
        tape_head = new TapeCell();
    }

    void print_tape() {
        TapeCell* start_cell = tape_head;

        while (start_cell->left_cell != nullptr) {
            start_cell = start_cell->left_cell;
        }

        TapeCell* curr_cell = start_cell;

        while (curr_cell->right_cell != nullptr) {
            if (curr_cell == tape_head) {
                cout << "   ↓   ";
            }
            else {
                cout << "       ";
            }
            curr_cell = curr_cell->right_cell;
        }

        cout << endl;

        curr_cell = start_cell;

        while (curr_cell->right_cell != nullptr) {
            curr_cell->print_cell();
            curr_cell = curr_cell->right_cell;
        }
        cout << endl;
    }
};

class TransitionNode {
public:
    TransitionNode* next;
    TransitionNode* prev;

    string symbol_read;

    string symbol_write;
    string left_right;
    State* next_state;
    string next_state_name;
};

class State {
private:
    string name;

public:
    bool is_initial;
    bool is_accept;
    bool is_reject;

    State* next_state_list;
    State* prev_state_list;

    TransitionNode* transition_list_head;

    void set_name(string new_name) {
        name = std::move(new_name);
    }

    string get_name() {
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

    State* current_state;

public:
    TuringMachine() {
        tape = new Tape();
        initial_state = nullptr;
        accept_state = nullptr;
        reject_state = nullptr;
    }

    int load_machine_from_file(string filename) {
        // Remember to write the input string to the tape first
        ifstream config_file(filename.c_str());
//        config_file.open(filename);

        assert(config_file.is_open());

        string delim = (string) ", ";
        State *curr_node;

        string line;
        string transition_information[5];

        unsigned long pos;

        // Read from config file, this will contain information about state transitions and the transition function
        // Read in the initial, accept, and reject states
        getline(config_file, line);
        state_list_head = (State*)(malloc(sizeof(State)));
        state_list_head->transition_list_head = (TransitionNode*) malloc(sizeof(TransitionNode));
        state_list_head->is_initial = true;
        state_list_head->is_accept = false;
        state_list_head->is_reject = false;
        state_list_head->set_name(line);
        initial_state = state_list_head;

        getline(config_file, line);
        state_list_head->next_state_list = (State*)(malloc(sizeof(State)));
        curr_node = state_list_head->next_state_list;
        curr_node->prev_state_list = state_list_head;
        curr_node->transition_list_head = (TransitionNode*) malloc(sizeof(TransitionNode));
        curr_node->is_initial = false;
        curr_node->is_accept = true;
        curr_node->is_reject = false;
        curr_node->set_name(line);
        accept_state = curr_node;

        getline(config_file, line);
        curr_node->next_state_list = (State*)(malloc(sizeof(State)));
        curr_node->next_state_list->prev_state_list = curr_node;
        curr_node = curr_node->next_state_list;
        curr_node->transition_list_head = (TransitionNode*) malloc(sizeof(TransitionNode));
        curr_node->is_initial = false;
        curr_node->is_accept = false;
        curr_node->is_reject = true;
        curr_node->set_name(line);
        reject_state = curr_node;

        while (getline(config_file, line)) {
            for (auto & i : transition_information) {
                pos = line.find(delim);
                i = line.substr(0, pos);
                line.erase(0, pos + 2);
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
            }
            else {
                // Frikinnnnnnnnn UHHHHHH malloc a new one
                curr_node->next_state_list = (State*) (malloc(sizeof (State)));
                curr_node->next_state_list->prev_state_list = curr_node;
                curr_node = curr_node->next_state_list;
                curr_node->set_name(transition_information[0]);
                curr_node->transition_list_head = (TransitionNode*) (malloc(sizeof (TransitionNode)));
                curr_transition_node = curr_node->transition_list_head;
                curr_transition_node->symbol_read = transition_information[1];
                curr_transition_node->next_state_name = transition_information[2];
                curr_transition_node->symbol_write = transition_information[3];
                curr_transition_node->left_right = transition_information[4];
            }
        }

        // Make a pass over all the states and transitions to update the transition nodes with the states instead of state names
        curr_node = state_list_head;
        string state_name;
        TransitionNode* curr_transition_node;
        State* curr_state_node_internal;
        while(curr_node != nullptr) {
            curr_transition_node = curr_node->transition_list_head;
            while (curr_transition_node != nullptr) {
                state_name = curr_transition_node->next_state_name;
                curr_state_node_internal = state_list_head;
                while(curr_state_node_internal != nullptr) {
                    if (curr_state_node_internal->get_name() == state_name) {
                        curr_transition_node->next_state = curr_state_node_internal;
                    }
                    curr_state_node_internal = curr_state_node_internal->next_state_list;
                }
                curr_transition_node = curr_transition_node->next;
            }
            curr_node = curr_node->next_state_list;
        }
        return 0;
    }

    int load_configuration(string filename) {
        // Read config file, this will allow the machine to be loaded in different configuraitons

        // Consdier the file that has the configuration of the TM in the form x,q,y where q is the state, x is whats on the tape before the tape head, and y is whats on the tape such that the tape head is on the first character of y
        // x on line 1
        // state name on line 2
        // y on line 3

        ifstream config_file;
        config_file.open(filename);

        string line;

        getline(config_file, line);
        string symbol = "0";
        int i = 0;

        // read the x string, _ denotes blank, ~ means empty string

        if (line != "~") {
            symbol = line[i];
            while (symbol != "\0") {
                tape->write_head(symbol);
                tape->move_head_right();
                i++;
                symbol = line[i];
            }
        }

        getline(config_file, line); // At this point line contains the name of the state that the TM starts at

        // Search the state list for the state that the machine needs to start in
        State* curr_node = state_list_head;
        while (curr_node != nullptr) {
            if (curr_node->get_name() == line) {
                current_state = curr_node;
                break;
            }
            curr_node = curr_node->next_state_list;
        }

        if (curr_node == nullptr) {
            cerr << "Initial state not found in state configuration file." << endl;
        }

        getline(config_file, line); // Reading in y, the tape head starts at the first character of y

        int count = 0;
        i = 0;

        if (line != "~") {
            for (i = 0; i < line.length(); i++) {
                symbol = line[i];
                tape->write_head(symbol);
                tape->move_head_right();
                count++;
            }
        }

        for (i = 0; i < count; i++) {
            tape->move_head_left();
        }

        print_tape();

        return 0;
    }

    bool run_simulation(int max_steps) {
        string read_symbol;
        string write_symbol;
        State* next_state;
        string left_right;
        TransitionNode* transition_search_node;

        for (int iter = 0; iter < max_steps; iter++) {
            read_symbol = tape->read_head();

            transition_search_node = current_state->transition_list_head;

            // Find the transition corresponding to the read symbol and state
            while (transition_search_node != nullptr) {
                if (transition_search_node->symbol_read == read_symbol) {
                    write_symbol = transition_search_node->symbol_write;
                    next_state = transition_search_node->next_state;
                    left_right = transition_search_node->left_right;
                    break;
                }
                transition_search_node = transition_search_node->next;
            }

            if (transition_search_node == nullptr) {
                // Reject
                return false;
            }
            else {
                tape->write_head(write_symbol);
                if (left_right == "L") {
                    tape->move_head_left();
                }
                else {
                    tape->move_head_right();
                }
                current_state = next_state;

                if (current_state->is_accept) {
                    return true;
                }
                else if (current_state->is_reject) {
                    return false;
                }
            }
            print_tape();
        }

        return false;
    }

    void print_tape() {
        tape->print_tape();
    }

};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Incorrect number of arguments, please enter a filename" << endl;
    }
    string state_machine_file = argv[1];
    string config_file = argv[2];

    auto* TM = new TuringMachine();

    TM->load_machine_from_file(state_machine_file);
    TM->load_configuration(config_file);

    bool accept_reject = TM->run_simulation(100);

    if (accept_reject) {
        cout << "String Accepted." << endl;
    }
    else {
        cout << "String Rejected." << endl;
    }

    return 0;
}
