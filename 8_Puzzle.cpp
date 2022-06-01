#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <list>
#include <unordered_map>
#include <chrono>
#include <cmath>
#include <random>

void Puzzle_8_Menu(void);

int nn = 0;

class Cell {
public:
    int value;

    Cell& operator =(const Cell& b) {
        this->value = b.value;
        return *this;
    }

    bool operator ==(const Cell& b) {
        if (this->value == b.value)
            return true;
        else
            return false;
    }

    bool operator !=(const Cell& b) {
        if (this->value != b.value)
            return true;
        else
            return false;
    }

};

int newd_counter = 0;

class Puzzle_8 {
private:
public:
    Puzzle_8() {
        for (int i = 0; i < 3; i++) {
            puzzle[i] = new Cell[3];
        }
    }

    Puzzle_8(int a, int b, int c, int d, int e, int f, int g, int h, int i) {
        for (int i = 0; i < 3; i++) {
            puzzle[i] = new Cell[3];
        }
        puzzle[0][0].value = a;
        puzzle[0][1].value = b;
        puzzle[0][2].value = c;
        puzzle[1][0].value = d;
        puzzle[1][1].value = e;
        puzzle[1][2].value = f;
        puzzle[2][0].value = g;
        puzzle[2][1].value = h;
        puzzle[2][2].value = i;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (puzzle[i][j].value == 0) {
                    this->x_empty = j;
                    this->y_empty = i;
                    break;
                }
            }
        }
    }

    Puzzle_8(int b[9]) {
        for (int i = 0; i < 3; i++) {
            puzzle[i] = new Cell[3];
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                puzzle[i][j].value = b[(3*i)+j];
            }
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (puzzle[i][j].value == 0) {
                    this->x_empty = j;
                    this->y_empty = i;
                    break;
                }
            }
        }
    }

    Puzzle_8(const Puzzle_8& b) {      //COPY CONSTRUCTOR
        for (int i = 0; i < 3; i++) {
            this->puzzle[i] = new Cell[3];
            for (int j = 0; j < 3; j++) {
                this->puzzle[i][j] = b.puzzle[i][j];
            }
        }
        this->y_empty = b.y_empty;
        this->x_empty = b.x_empty;
        what_move = b.what_move;
        father = b.father;
        this->node_number = b.node_number;
        depth = b.depth;
        remained_child = b.remained_child;
        h_n = b.h_n;
    }

    Cell** puzzle = new Cell * [3];
    Puzzle_8* father = nullptr;
    std::string what_move;
    int node_number = 0;
    int y_empty = 0;
    int x_empty = 0;
    int depth = 0;
    int remained_child = 0;
    int h_n = 0;


    bool operator == (const  Puzzle_8& b) {

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (this->puzzle[i][j] != b.puzzle[i][j])
                    return false;
            }
        }
        return true;
    }

    Puzzle_8& operator = (const Puzzle_8& b) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                this->puzzle[i][j] = b.puzzle[i][j];
            }
        }
        this->y_empty = b.y_empty;
        this->x_empty = b.x_empty;
        this->father = b.father;
        this->what_move = b.what_move;
        this->node_number = b.node_number;
        this->depth = b.depth;
        this->remained_child = b.remained_child;
        this->h_n = b.h_n;
        return *this;
    }

    friend std::ostream& operator <<(std::ostream& out_stream, const  Puzzle_8& tmp) {
        out_stream << "MOVE: " << tmp.what_move << "   # " << tmp.node_number <<"     h(n)="<<tmp.h_n<<"    g(n)= "<<tmp.depth << std::endl;
        out_stream << "+------+-------+-------+" << std::endl;
        for (int i = 0; i < 3; i++) {
            out_stream << "|   ";
            for (int j = 0; j < 3; j++) {
                out_stream << tmp.puzzle[i][j].value << "  |    ";
              
            }
            out_stream <<std::endl<< "+------+-------+-------+" << std::endl;
        }
        out_stream << std::endl << std::endl;

        return out_stream;
    }

    friend std::istream& operator >>(std::istream& input_stream, Puzzle_8& tmp) {

        return input_stream;
    }

    bool operator > (const Puzzle_8 & b)const {
        if ((this->h_n+this->depth) > (b.h_n+b.depth) )
            return true;
        else
            return false;
    }

    bool operator <(const Puzzle_8& b)const {
        if ((this->h_n + this->depth) <= (b.h_n + b.depth))
            return true;
        else
            return false;
    }

    ~Puzzle_8() {
        for (int i = 0; i < 3; i++) {
            delete[]puzzle[i];
        }
        delete[] puzzle;
        puzzle = nullptr;
        x_empty = 0;
        y_empty = 0;
        
    }

};

class General_Solver {
private:
    int num = 1;
protected :

    Puzzle_8 Swap(const Puzzle_8& b, int x_a, int y_a) {

        Puzzle_8 a = b;


        a.remained_child = 0;
        a.puzzle[a.y_empty][a.x_empty] = a.puzzle[y_a][x_a];
        a.puzzle[y_a][x_a].value = 0;
        a.y_empty = y_a;
        a.x_empty = x_a;
        return a;
    }

    bool is_promising(const Puzzle_8& state, int mode) {
        int n = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (state.puzzle[i][j].value != 0) {
                    for (int k=0; k < 3; k++) {
                        for (int l=0 ; l < 3; l++) {
                            if ((state.puzzle[i][j].value > state.puzzle[k][l].value) && (state.puzzle[k][l].value != 0) && (((i * 3) + j) <= ((k * 3) + l))) {
                                n++;
                              
                            }
                             
                        }
                    }
                }
            }
        }

        if (mode == 0 && (n % 2 == 0)) {  //even mode
            return true;
        }

        else if (mode == 1 && (n % 2 != 0)) { //odd mode
            return true;
        }
        else {
            //std::cout << "It is not Reachable from this initial state to goal state !!!!\t"<<nn<<"\n";
            nn++;
            return false;
        }
    }
   
    Puzzle_8* Left_Right(Puzzle_8* current_state,bool is_reverse){
        if (current_state->x_empty + 1 >= 0 && current_state->x_empty + 1 <= 2 ) //left-right swap
        {
            Puzzle_8* new_puzzle = new Puzzle_8;
            newd_counter++;
            *new_puzzle = Swap(*current_state, current_state->x_empty + 1, current_state->y_empty);
            new_puzzle->father = current_state;
            if(!is_reverse)
                new_puzzle->what_move = "RIGHT";
            else
                new_puzzle->what_move = "LEFT";
            new_puzzle->depth = current_state->depth + 1;
            current_state->remained_child++;
            new_puzzle->node_number = num;
            num++;
            return new_puzzle;
        }
        else {
            return nullptr;
        }
    }

    Puzzle_8 * Right_Left(Puzzle_8 * current_state, bool is_reverse) {
        
        if (current_state->x_empty - 1 >= 0 && current_state->x_empty - 1 <= 2) //Right_Left swap
        {
            Puzzle_8* new_puzzle = new Puzzle_8;
            newd_counter++;
            *new_puzzle = Swap(*current_state, current_state->x_empty - 1, current_state->y_empty);
            new_puzzle->father = current_state;
            if (!is_reverse)
                new_puzzle->what_move = "LEFT";
            else
                new_puzzle->what_move = "RIGHT";
            new_puzzle->depth = current_state->depth + 1;
            current_state->remained_child++;
            new_puzzle->node_number = num;
            num++;
            return new_puzzle;
        }
        else {
            return nullptr;
        }
    }

    Puzzle_8* Up_Down(Puzzle_8* current_state, bool is_reverse) {
        if (current_state->y_empty + 1 >= 0 && current_state->y_empty + 1 <= 2) //left-right swap
        {
            Puzzle_8* new_puzzle = new Puzzle_8;
            newd_counter++;
            *new_puzzle = Swap(*current_state, current_state->x_empty, current_state->y_empty + 1);
            new_puzzle->father = current_state;
            if (!is_reverse)
                new_puzzle->what_move = "DOWN";
            else
                new_puzzle->what_move = "UP";
            new_puzzle->depth = current_state->depth + 1;
            current_state->remained_child++;
            new_puzzle->node_number = num;
            num++;
            return new_puzzle;

        }
        else{
            return nullptr;
        }
    }

    Puzzle_8* Down_Up(Puzzle_8* current_state, bool is_reverse) {
        if (current_state->y_empty - 1 >= 0 && current_state->y_empty - 1 <= 2) //Up_Down swap
        {
            Puzzle_8* new_puzzle = new Puzzle_8;
            newd_counter++;
            *new_puzzle = Swap(*current_state, current_state->x_empty, current_state->y_empty - 1);
            new_puzzle->father = current_state;
            if (!is_reverse)
                new_puzzle->what_move = "UP";
            else
                new_puzzle->what_move = "DOWN";
            new_puzzle->depth = current_state->depth + 1;
            current_state->remained_child++;
            new_puzzle->node_number = num;
            num++;
            return new_puzzle;
        }
        else {
            return nullptr;
        }
    }


public:
    Puzzle_8 goal= { 1,2,3,8,0,4,7,6,5 };

    Puzzle_8 Random_case_generator(void) {
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(0, 8); // define the range
        std::unordered_map<int, bool> ss;


        
        Puzzle_8  temp;
        
            while (ss.size() < 9) {
                int Random = distr(gen);
                if (ss.find(Random) == ss.end()) {
                    ss.insert(std::make_pair(Random, true));
                }
            }

            
            int i = 0;
            int s[9] = { 0 };
            for (auto const& pair : ss) {
                s[i] = pair.first;
                i++;
            }
            Puzzle_8  temp2 (s);
            temp = temp2;

        


        return temp;
    }
};

class BFS : public General_Solver {

private:
    bool Is_new(Puzzle_8* new_node, std::unordered_map<std::string, Puzzle_8*>& total_set) {
       
        if (total_set.find(give_order2(new_node)) == total_set.end())
            return true;
        else
            return false;
    }

    std::string give_order2(Puzzle_8* state) {
        std::string temp;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                temp.push_back(state->puzzle[i][j].value + '0');
            }
        }
        return temp;
    }

    int c=0;
protected:

    std::queue<Puzzle_8*> fringe;
    std::unordered_map<std::string , Puzzle_8*> closed_set;
    std::stack<Puzzle_8*> Solution_set;

    void BFS_GraphSearch_Successor(Puzzle_8 * current_state) {
        Puzzle_8* new_puzzle = Right_Left(current_state, false);
        if (new_puzzle != nullptr)
        {   
                if (Is_new(new_puzzle, closed_set))
                {
                    fringe.push(new_puzzle);
                }
                else {
                    delete new_puzzle;
                    c++;
                }
        }

        new_puzzle = Down_Up(current_state, false);

        if (new_puzzle != nullptr)
        {
            if (Is_new(new_puzzle, closed_set))
            {
                fringe.push(new_puzzle);
            }
            else {
                delete new_puzzle;
                c++;
            }
        }

        new_puzzle = Left_Right(current_state,false);

        if (new_puzzle != nullptr)
        {
            if (Is_new(new_puzzle, closed_set))
            {
                fringe.push(new_puzzle);
            }
            else {
                delete new_puzzle;
                c++;
            }
        }

        new_puzzle = Up_Down(current_state, false);

        if (new_puzzle != nullptr) //left-right swap
        {
            if (Is_new(new_puzzle, closed_set))
            {
                fringe.push(new_puzzle);
            }
            else {
                delete new_puzzle;
                c++;
            }
        }
    }

    void BFS_TreeSearch_Successor(Puzzle_8* current_state) {

        Puzzle_8* new_puzzle = Right_Left(current_state, false);
        if (new_puzzle != nullptr)
        {

                fringe.push(new_puzzle);

        }

        new_puzzle = Down_Up(current_state, false);

        if (new_puzzle != nullptr)
        {
                fringe.push(new_puzzle);
        }

        new_puzzle = Left_Right(current_state, false);

        if (new_puzzle != nullptr)
        {
                fringe.push(new_puzzle);
        }

        new_puzzle = Up_Down(current_state, false);

        if (new_puzzle != nullptr) //left-right swap
        {
                fringe.push(new_puzzle);
        }

    }

public:

    void Graph_Search_operation(Puzzle_8* init,Puzzle_8* new_goal) {
        this->goal = *new_goal;
        Puzzle_8* initial_state = new Puzzle_8;
        newd_counter++;
        *initial_state = *init;
        fringe.push(initial_state);
        int p = 1;
        initial_state->node_number = p;
        auto start = std::chrono::high_resolution_clock::now();

        if (!is_promising(*initial_state, 1))
            return;

        while (1) {
            if (fringe.empty() == true)
                return;

            else {

                Puzzle_8* current_state = fringe.front();
                fringe.pop();
                if (!Is_new(current_state, closed_set))
                {
                    delete current_state;
                    c++;
                    continue;
                }
                if (*current_state == goal) {
                    auto stop = std::chrono::high_resolution_clock::now();
                    std::cout << "Reached\n" << "Total Expanded Nodes : " << p << "\n";
                    std::cout << "Total Memory Allocated : " << (p * sizeof(Puzzle_8) / 1000) << " (KB)\n";
                    auto processed_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
                    if (processed_time >= 3600000000) {
                        std::cout << "Total Processing Time : " << processed_time / 3600000000.0 << " (hour)\n";
                    }
                    else if (processed_time >= 60000000) {
                        std::cout << "Total Processing Time : " << processed_time / 60000000.0 << " (minute)\n";
                    }
                    else if (processed_time >= 1000000) {
                        std::cout << "Total Processing Time : " << processed_time / 1000000.0 << " (seconde)\n";
                    }
                    else if (processed_time >= 10000) {
                        std::cout << "Total Processing Time : " << processed_time / 1000.0 << " (miliseconde)\n";
                    }
                    else {
                        std::cout << "Total Processing Time : " << processed_time << " (microseconde)\n";
                    }
                    std::cout << "==============================\n";
                    Puzzle_8* d = current_state;
                    while (d->father != nullptr) {
                        Solution_set.push(d);
                        d = d->father;
                    }
                    while (!Solution_set.empty()) {
                       // std::cout << *Solution_set.top() << "\n";
                        Solution_set.pop();
                    }
                    delete current_state;
                    c++;
                    return;
                }

                else {

                    closed_set.insert(std::make_pair(give_order2(current_state),current_state));
                    BFS_GraphSearch_Successor(current_state);
                }

            }

        }

    }

    void Tree_Search_operation(Puzzle_8* init,Puzzle_8 * new_goal) {
        this->goal = *new_goal;
        Puzzle_8* initial_state = new Puzzle_8;
        *initial_state = *init;
        fringe.push(initial_state);
        int p = 1;
        auto start = std::chrono::high_resolution_clock::now();

        if (!is_promising(*initial_state, 1))
            return;

        while (1) {
            if (fringe.empty() == true)
                return;

            else {

                Puzzle_8* current_state = fringe.front();

                fringe.pop();

                if (*current_state == goal) {

                    auto stop = std::chrono::high_resolution_clock::now();
                    std::cout << "Reached\n" << "Total Expanded Nodes : " << p << "\n";
                    std::cout << "Total Memory Allocated : " << (p * sizeof(Puzzle_8) / 1000) << " (KB)\n";
                    auto processed_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
                    if (processed_time >= 3600000000) {
                        std::cout << "Total Processing Time : " << processed_time / 3600000000.0 << " (hour)\n";
                    }
                    else if (processed_time >= 60000000) {
                        std::cout << "Total Processing Time : " << processed_time / 60000000.0 << " (minute)\n";
                    }
                    else if (processed_time >= 1000000) {
                        std::cout << "Total Processing Time : " << processed_time / 1000000.0 << " (seconde)\n";
                    }
                    else if (processed_time >= 10000) {
                        std::cout << "Total Processing Time : " << processed_time / 1000.0 << " (miliseconde)\n";
                    }
                    else {
                        std::cout << "Total Processing Time : " << processed_time << " (microseconde)\n";
                    }
                    std::cout << "==============================\n";
                    Puzzle_8* d = current_state;
                    while (d->father != nullptr) {
                        Solution_set.push(d);
                        d = d->father;
                    }
                    while (!Solution_set.empty()) {
                        std::cout << *Solution_set.top() << "\n";
                        Solution_set.pop();
                    }
                    break;
                    return;
                }

                else {

                    BFS_TreeSearch_Successor(current_state);
                    
                }

            }

        }
    }

    ~BFS() {
        int n = 0;
        for (auto & pair : closed_set) {
            delete pair.second;
            n++;
        }
        while (!fringe.empty()) {
            delete fringe.front();
            fringe.pop();
            n++;
        }
        //std::cout << n+c << " items removed from deletes\n";
        //std::cout << newd_counter << "Newed adctually\n";
        c = 0;
        newd_counter = 0;
    }

};

class IDS :public General_Solver {

protected:

    std::stack<Puzzle_8 *> solution_set;

    void LDS_Successor(Puzzle_8 * current_state, std::stack<Puzzle_8* > & fringe) {

        Puzzle_8* new_puzzle = Right_Left(current_state, false);
        if (new_puzzle != nullptr)
        {

            fringe.push(new_puzzle);

        }

        new_puzzle = Down_Up(current_state, false);

        if (new_puzzle != nullptr)
        {
            fringe.push(new_puzzle);
        }

        new_puzzle = Left_Right(current_state, false);

        if (new_puzzle != nullptr)
        {
            fringe.push(new_puzzle);
        }

        new_puzzle = Up_Down(current_state, false);

        if (new_puzzle != nullptr) //left-right swap
        {
            fringe.push(new_puzzle);
        }

    }

    void Delete_Not_Promising(Puzzle_8* current_state) {
        Puzzle_8* delete_pointer = current_state;
        while (delete_pointer->remained_child <= 0) {
            Puzzle_8* temp = delete_pointer;
            delete_pointer = delete_pointer->father;
            if (delete_pointer == nullptr)
                break;
            delete_pointer->remained_child--;
            delete temp;

        }

    }

    void LDS(Puzzle_8* initial_state, int depth) {
        long p = 1;

        std::stack<Puzzle_8*> fringe;

        fringe.push(initial_state);
        while (1) {
            if (fringe.empty() == true) {
                return;
            }

            else {

                Puzzle_8* current_state = fringe.top();
                fringe.pop();

                if (*current_state == goal) {
                    Puzzle_8* d = current_state;
                    while (d->father != nullptr) {
                        solution_set.push(d);
                        d = d->father;
                    }
                    return;
                }

                else {

                    if (current_state->depth < depth) {

                        LDS_Successor(current_state, fringe);

                    }

                    else {
                        Delete_Not_Promising(current_state);
                    }
                }
            }
        }
    }

public :

    void IDS_operation(Puzzle_8* initial_state) {
        auto start = std::chrono::high_resolution_clock::now();
        int depth = 1;
        while (1) {
            LDS(initial_state, depth);
            if (solution_set.empty())
            {
                depth++;
            }

            else {
                auto stop = std::chrono::high_resolution_clock::now();
                //std::cout << "Reached\n" << "Total Expanded Nodes : " << p << "\n";
                //std::cout << "Total Memory Allocated : " << (p * sizeof(Puzzle_8) / 1000) << " (KB)\n";
                auto processed_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
                if (processed_time >= 3600000000) {
                    std::cout << "Total Processing Time : " << processed_time / 3600000000.0 << " (hour)\n";
                }
                else if (processed_time >= 60000000) {
                    std::cout << "Total Processing Time : " << processed_time / 60000000.0 << " (minute)\n";
                }
                else if (processed_time >= 1000000) {
                    std::cout << "Total Processing Time : " << processed_time / 1000000.0 << " (seconde)\n";
                }
                else if (processed_time >= 10000) {
                    std::cout << "Total Processing Time : " << processed_time / 1000.0 << " (miliseconde)\n";
                }
                else {
                    std::cout << "Total Processing Time : " << processed_time << " (microseconde)\n";
                }
                std::cout << "==============================\n";
                while (!solution_set.empty()) {
                    std::cout << *solution_set.top() << "\n";
                    solution_set.pop();
                }
                break;
            }
        }

    }

};

class A_star : public General_Solver {

private:
    std::list<Puzzle_8*> fringe;
    std::stack<Puzzle_8*>solution_set;
    std::unordered_map <std::string, Puzzle_8*> closed_set;
    bool Is_new(Puzzle_8* new_node, std::unordered_map<std::string, Puzzle_8*>& total_set) {

        if (total_set.find(give_order2(new_node)) == total_set.end())
            return true;
        else
            return false;
    }
    std::string give_order2(Puzzle_8* state) {
        std::string temp;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                temp.push_back(state->puzzle[i][j].value + '0');
            }
        }
        return temp;
    }

protected :

    int Calculate_Manhattan_distance(const Puzzle_8& p , Puzzle_8 * goal_state) {
        int total_distance = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (p.puzzle[i][j].value != 0)
                {
                    for (int m = 0; m < 3; m++) {
                        bool finish = false;
                        for (int n = 0; n < 3; n++) {
                            if (p.puzzle[i][j] == goal_state->puzzle[m][n]) {
                                total_distance += (abs(i - m) + abs(j - n));
                                finish = true;
                                break;
                            }
                        }
                        if (finish)
                            break;
                    }
                }

            }
        }
        return total_distance;
    }

    void Push_with_sort(std::list<Puzzle_8*>& fringe, Puzzle_8* new_item) {

        std::list<Puzzle_8*>::iterator my_iterator;
        if (fringe.empty()) {
            fringe.push_back(new_item);
        }
        else {
            for (my_iterator = fringe.begin(); my_iterator != fringe.end(); ++my_iterator) {
                if ((new_item->h_n + new_item->depth) <= (my_iterator._Ptr->_Myval->h_n + my_iterator._Ptr->_Myval->depth)) {
                    fringe.insert(my_iterator, new_item);
                    return;
                }
            }
            fringe.push_back(new_item);
        }

    }

    void A_star_Successor(Puzzle_8 * current_state){

        Puzzle_8* new_puzzle = Right_Left(current_state, false);
        if (new_puzzle != nullptr)
        {
            if(Is_new(new_puzzle,closed_set)){
                new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, &goal);
                Push_with_sort(fringe, new_puzzle);
            }
            else {
                delete new_puzzle;
            }

        }

        new_puzzle = Down_Up(current_state, false);

        if (new_puzzle != nullptr)
        {

            if (Is_new(new_puzzle, closed_set)) {
                new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, &goal);
                Push_with_sort(fringe, new_puzzle);
            }
            else {
                delete new_puzzle;
            }
        }

        new_puzzle = Left_Right(current_state, false);

        if (new_puzzle != nullptr)
        {

            if (Is_new(new_puzzle, closed_set)) {
                new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, &goal);
                Push_with_sort(fringe, new_puzzle);
            }
            else {
                delete new_puzzle;
            }
        }

        new_puzzle = Up_Down(current_state, false);

        if (new_puzzle != nullptr) //left-right swap
        {

            if (Is_new(new_puzzle, closed_set)) {
                new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, &goal);
                Push_with_sort(fringe, new_puzzle);
            }
            else {
                delete new_puzzle;
            }
        }
    }



public:

    void A_star_operation(Puzzle_8* init) {


        Puzzle_8* initial_state = new Puzzle_8;
        *initial_state = *init;

        if (!is_promising(*initial_state, 1))
            return;

        initial_state->h_n = Calculate_Manhattan_distance(*initial_state,&goal);
        fringe.push_back(initial_state);
        int num = 1;

        while (true) {

            if (fringe.empty() == true) {
                std::cout << "Has not answer!\n";
            }


            Puzzle_8* current_state = fringe.front();
            fringe.pop_front();

            if (!Is_new(current_state, closed_set)) {
                delete current_state;
                continue;
            }

            if (*current_state == goal) {
                Puzzle_8* d = current_state;
                while (d != nullptr) {
                    solution_set.push(d);
                    d = d->father;
                }

                std::stack<Puzzle_8*>solution_set2 = solution_set;
                //std::cout << "Solution: ";

                while (!solution_set.empty())
                {
                    //std::cout << solution_set.top()->what_move << "-->";
                    solution_set.pop();
                }
                //std::cout << "\n";
                while (!solution_set2.empty())
                {
                    //std::cout << *solution_set2.top();
                    solution_set2.pop();
                }
                delete current_state;
                break;
            }
            else {
              
                closed_set.insert(std::make_pair(give_order2(current_state), current_state));
                A_star_Successor(current_state);
            }
        }
    }

    ~A_star() {

        for (auto& pair : closed_set) {
            //std::cout << *pair.second << std::endl;
            delete pair.second;

        }
        while (!fringe.empty()) {
            delete fringe.front();
            fringe.pop_front();

        }
    }

};

int max = 0;

class Bidirectional_Serach {
protected :

    Puzzle_8* Find_Intersect(Puzzle_8* new_node, std::unordered_map<std::string,Puzzle_8*> & total_set) {
        std::string key = give_order(new_node);
        if (total_set.find(key) == total_set.end())
            return nullptr;
        else
            return total_set.find(key)._Ptr->_Myval.second;
    }

    void Trace_Solution(std::vector<Puzzle_8*>& solution,Puzzle_8* top_common_node,Puzzle_8 * down_common_node) {

        Puzzle_8* middle_top_res = top_common_node;
        Puzzle_8* middle_down_res = down_common_node;
        std::stack<Puzzle_8*> temp1;
        std::queue<Puzzle_8*> temp2;
        while (middle_top_res != nullptr) {
            temp1.push(middle_top_res);
            middle_top_res = middle_top_res->father;
        }
        while (!temp1.empty()) {
            solution.push_back(temp1.top());
            temp1.pop();
        }
        //---
        
        while (middle_down_res != nullptr) {
            temp2.push(middle_down_res);
            middle_down_res = middle_down_res->father;
        }

        while (!temp2.empty()) {
            static Puzzle_8* temporary =nullptr;
            static std::string temp_move;
            static int temp_depth ;
            
            if (temp2.front() == down_common_node)
            {
                temporary = top_common_node;
                temp_move = down_common_node -> what_move;
                temp_depth = top_common_node->depth;
            }
            else{
                temp_depth++;
                std::string temp_move2 = temp2.front()->what_move;
                temp2.front()->what_move=temp_move;
                temp_move = temp_move2;
                temp2.front()->father = temporary;
                temp2.front()->depth = temp_depth;
                temporary = temp2.front();
                solution.push_back(temp2.front());
            }

            temp2.pop();
        }
    }

    void Goal_Finded(Puzzle_8* node1,Puzzle_8* node2 ) {
     
            //Common Node Finded
            std::vector<Puzzle_8*> solution;
            Trace_Solution(solution, node1, node2);
            if (solution[solution.size() - 1]->depth > max)
                max = solution[solution.size() - 1]->depth;
           
            for (int i = 0; i < solution.size(); i++) {
               // std::cout << *solution[i] << "\n";
            }
            return;
        
    }

    std::string give_order(Puzzle_8* state) {
        std::string temp;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                temp.push_back(state->puzzle[i][j].value + '0');
            }
        }
        return temp;
    }

    void Push_TOTALS(std::unordered_map < std::string, Puzzle_8*>& total_set, Puzzle_8* new_puzzle) {
        std::string key = give_order(new_puzzle);
        if (total_set.find(key) == total_set.end())
        {
            total_set.insert(std::make_pair(key, new_puzzle));
        }
        else {
            //if(total_set.find(key)._Ptr->_Myval.second->depth > new_puzzle->depth){
            //  //  delete total_set.find(key)._Ptr->_Myval.second;
            //    Sholud_Remove.insert(std::make_pair(total_set.find(key)._Ptr->_Myval.second,false));
            //    total_set.erase(key);
            //    total_set.insert(std::make_pair(key, new_puzzle));
            //}
           // else {
                //int y;
               // std::cout <<1<<std::endl;
                delete new_puzzle;
            //}
        }
    }

    std::unordered_map<Puzzle_8*, bool> Sholud_Remove;

public:
    ~Bidirectional_Serach() {
       // std::cout << Sholud_Remove.size()<<"\n";
        for (auto const& pair : Sholud_Remove) {
            delete pair.first;
        }
    }

};

class BFS_BFS : Bidirectional_Serach, private BFS {
private:
    std::queue<Puzzle_8*> Top_fringe;
    std::queue<Puzzle_8*> Down_fringe;
    std::unordered_map<std::string, Puzzle_8*> Total_Top;
    std::unordered_map<std::string, Puzzle_8*> Total_Down;

    bool Is_new2(Puzzle_8* new_node, std::unordered_map<std::string,Puzzle_8*>& total_set) {
        std::string key = give_order(new_node);
        if (total_set.find(key) == total_set.end())
            return true;
        else
            return false;
    }

    bool Successor1(Puzzle_8* current_state, Puzzle_8* goal_state) {
        Puzzle_8* new_puzzle;
        new_puzzle= Right_Left(current_state, false);
        if (new_puzzle != nullptr)
        {
            if(Is_new2(new_puzzle,Total_Top))
                Top_fringe.push(new_puzzle);

            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Down);
            if (common_node != nullptr)
            {
                Goal_Finded(new_puzzle, common_node);
                return true;
            }
            Push_TOTALS(Total_Top, new_puzzle);

        }

        new_puzzle = Down_Up(current_state, false);

        if (new_puzzle != nullptr)
        {
            if (Is_new2(new_puzzle, Total_Top))
                Top_fringe.push(new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Down);
            if (common_node != nullptr)
            {
                Goal_Finded(new_puzzle, common_node);
                return true;
            }
            Push_TOTALS(Total_Top, new_puzzle);

        }

        new_puzzle = Left_Right(current_state, false);

        if (new_puzzle != nullptr)
        {
            if (Is_new2(new_puzzle, Total_Top))
                Top_fringe.push(new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Down);
            if (common_node != nullptr)
            {
                Goal_Finded(new_puzzle, common_node);
                return true;
            }
            Push_TOTALS(Total_Top, new_puzzle);

        }

        new_puzzle = Up_Down(current_state, false);

        if (new_puzzle != nullptr) //left-right swap
        {
            if (Is_new2(new_puzzle, Total_Top))
                Top_fringe.push(new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Down);
            if (common_node != nullptr)
            {
                Goal_Finded(new_puzzle, common_node);
                return true;
            }
            Push_TOTALS(Total_Top, new_puzzle);

        }
        return false;
    }

    bool Successor2(Puzzle_8* current_state, Puzzle_8* goal_state) {
        Puzzle_8* new_puzzle;
        new_puzzle= Right_Left(current_state, true);
        if (new_puzzle != nullptr)
        {
            if (Is_new2(new_puzzle, Total_Down))
                Down_fringe.push(new_puzzle);
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Top);
            if (common_node != nullptr)
            {
                Goal_Finded(common_node, new_puzzle);
                return true;
            }
            Push_TOTALS(Total_Down, new_puzzle);

        }

        new_puzzle = Down_Up(current_state, true);

        if (new_puzzle != nullptr)
        {
            if (Is_new2(new_puzzle, Total_Down))
                Down_fringe.push(new_puzzle);
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Top);
            if (common_node != nullptr)
            {
                Goal_Finded(common_node, new_puzzle);
                return true;
            }
            Push_TOTALS(Total_Down, new_puzzle);

        }

        new_puzzle = Left_Right(current_state, true);

        if (new_puzzle != nullptr)
        {
            if (Is_new2(new_puzzle, Total_Down))
                Down_fringe.push(new_puzzle);
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Top);
            if (common_node != nullptr)
            {
                Goal_Finded(common_node, new_puzzle);
                return true;
            }
            Push_TOTALS(Total_Down, new_puzzle);

        }

        new_puzzle = Up_Down(current_state, true);

        if (new_puzzle != nullptr) //left-right swap
        {
            if (Is_new2(new_puzzle, Total_Down))
                Down_fringe.push(new_puzzle);
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Top);
            if (common_node != nullptr)
            {
                Goal_Finded(common_node, new_puzzle);
                return true;
            }
            Push_TOTALS(Total_Down, new_puzzle);

        }
        return false;
    }

    void BFS_BFS_Graph_operation(Puzzle_8* initial_state, Puzzle_8* goal_state) {

        Top_fringe.push(initial_state);
        Down_fringe.push(goal_state);
        Total_Top.insert(std::make_pair(give_order(initial_state), initial_state));
        Total_Down.insert(std::make_pair(give_order(goal_state), goal_state));

        if (!is_promising(*initial_state, 1)) {
            return;
        }

        while (true) {
            if (Top_fringe.empty() || Down_fringe.empty()) {
                std::cout << "Have not Answer !\n";
                return;
            }

            //BFS_UP->DOWN
            Puzzle_8* current_state = Top_fringe.front();
            Top_fringe.pop();
            if (Successor1(current_state, goal_state))
                return;

            //BFS_DOWN->UP
            current_state = Down_fringe.front();
            Down_fringe.pop();
            if (Successor2(current_state, initial_state))
                return;
        }
    }
    
protected:

public:

    void Run(Puzzle_8 init, Puzzle_8 gol) {
        Puzzle_8* initial_state = new Puzzle_8;
        *initial_state = init;
        Puzzle_8* goal_state = new Puzzle_8;
        *goal_state = gol;
        BFS_BFS_Graph_operation(initial_state, goal_state);
    }

    ~BFS_BFS() {
        for (auto const& pair : Total_Top) {
            delete pair.second;
        }
        for (auto const& pair : Total_Down) {
            delete pair.second;
        }
    }
};

class  AS_AS : Bidirectional_Serach ,private A_star {

private:

    std::list<Puzzle_8*> Top_fringe;
    std::list<Puzzle_8*> Down_fringe;
    std::unordered_map<std::string, Puzzle_8*> Total_Top;
    std::unordered_map<std::string, Puzzle_8*> Total_Down;

    bool Is_new2(Puzzle_8* new_node, std::unordered_map<std::string, Puzzle_8*>& total_set) {
        std::string key = give_order(new_node);
        if (total_set.find(key) == total_set.end())
            return true;
        else
            return false;
    }

    void Initialization(Puzzle_8* initial_state, Puzzle_8* goal_state) {

        initial_state->h_n = Calculate_Manhattan_distance(*initial_state, goal_state);
        goal_state->h_n = initial_state->h_n;
        Top_fringe.push_back(initial_state);
        Down_fringe.push_back(goal_state);
        Total_Top.insert(std::make_pair(give_order(initial_state), initial_state));
        Total_Down.insert(std::make_pair(give_order(goal_state), goal_state));
    }

    bool Successor1(Puzzle_8 * current_state,Puzzle_8 * goal_state) {
        
        Puzzle_8* new_puzzle = Right_Left(current_state, false);
        if (new_puzzle != nullptr)
        {
            new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, goal_state);
            if (Is_new2(new_puzzle, Total_Top))
                Push_with_sort(Top_fringe, new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Down);
            if (common_node != nullptr)
            {
                Goal_Finded(new_puzzle, common_node);
                return true;
            }
            Push_TOTALS(Total_Top, new_puzzle);

        }

        new_puzzle = Down_Up(current_state, false);

        if (new_puzzle != nullptr)
        {
            new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, goal_state);
            if (Is_new2(new_puzzle, Total_Top))
            Push_with_sort(Top_fringe, new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Down);
            if (common_node != nullptr)
            {
                Goal_Finded(new_puzzle, common_node);
                return true;
            }
            Push_TOTALS(Total_Top, new_puzzle);

        }

        new_puzzle = Left_Right(current_state, false);

        if (new_puzzle != nullptr)
        {
            new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, goal_state);
            if (Is_new2(new_puzzle, Total_Top))
                Push_with_sort(Top_fringe, new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Down);
            if (common_node != nullptr)
            {
                Goal_Finded(new_puzzle, common_node);
                return true;
            }
            Push_TOTALS(Total_Top, new_puzzle);

        }

        new_puzzle = Up_Down(current_state, false);

        if (new_puzzle != nullptr) //left-right swap
        {
            new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, goal_state);
            if (Is_new2(new_puzzle, Total_Top))
                Push_with_sort(Top_fringe, new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Down);
            if (common_node != nullptr)
            {
                Goal_Finded(new_puzzle, common_node);
                return true;
            }
            Push_TOTALS(Total_Top, new_puzzle);

        }
        return false;
    }

    bool Successor2(Puzzle_8* current_state, Puzzle_8* goal_state) {

        Puzzle_8* new_puzzle = Right_Left(current_state, true);
        if (new_puzzle != nullptr)
        {
            new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, goal_state);
            if (Is_new2(new_puzzle, Total_Down))
                Push_with_sort(Down_fringe, new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Top);
            if (common_node != nullptr)
            {
                Goal_Finded(common_node, new_puzzle);
                return true;
            }
            Push_TOTALS(Total_Down, new_puzzle);

        }

        new_puzzle = Down_Up(current_state, true);

        if (new_puzzle != nullptr)
        {
            new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, goal_state);
            if (Is_new2(new_puzzle, Total_Down))
                Push_with_sort(Down_fringe, new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Top);
            if (common_node != nullptr)
            {
                Goal_Finded(common_node, new_puzzle);
                return true;
            }
            Push_TOTALS(Total_Down, new_puzzle);

        }

        new_puzzle = Left_Right(current_state, true);

        if (new_puzzle != nullptr)
        {
            new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, goal_state);
            if (Is_new2(new_puzzle, Total_Down))
                Push_with_sort(Down_fringe, new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Top);
            if (common_node != nullptr)
            {
                Goal_Finded(common_node, new_puzzle);
                return true;
            }
            Push_TOTALS(Total_Down, new_puzzle);

        }

        new_puzzle = Up_Down(current_state, true);

        if (new_puzzle != nullptr) //left-right swap
        {
            new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, goal_state);
            if (Is_new2(new_puzzle, Total_Down))
                Push_with_sort(Down_fringe, new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Top);
            if (common_node != nullptr)
            {
                Goal_Finded(common_node, new_puzzle);
                return true;
            }
            Push_TOTALS(Total_Down, new_puzzle);

        }
        return false;
    }

    void AS_AS_operation(Puzzle_8* init, Puzzle_8* gol) {


        Puzzle_8* initial_state = new Puzzle_8;
        *initial_state = *init;
        Puzzle_8* goal_state = new Puzzle_8;
        *goal_state = *gol;
        if (!is_promising(*initial_state, 1)) {
            return;
        }
        Initialization(initial_state, goal_state);

        while (true) {
            if (Top_fringe.empty() || Down_fringe.empty()) {
                std::cout << "Have not Answer !\n";
                return;
            }

            //AS_Up->dOWN

            Puzzle_8* current_state = Top_fringe.front();
            Top_fringe.pop_front();

            if (Successor1(current_state, goal_state))
               return;

            //AS_DOWN->UP

            current_state = Down_fringe.front();
            Down_fringe.pop_front();
            if (Successor2(current_state, initial_state))
                return;
        }
    }

public:


    void Run(Puzzle_8 init, Puzzle_8 gol) {
        Puzzle_8* initial_state = new Puzzle_8;
        *initial_state = init;
        Puzzle_8* goal_state = new Puzzle_8;
        *goal_state = gol;
        AS_AS_operation(initial_state, goal_state);
    }

    ~AS_AS() {
        for (auto const& pair : Total_Top) {
            delete pair.second;
        }
        for (auto const& pair : Total_Down) {
            delete pair.second;
        }

        Total_Top.clear();
        Total_Down.clear();
    }

};

class AS_BFS:Bidirectional_Serach,private A_star{
private:
    std::list<Puzzle_8*> Top_fringe;
    std::queue<Puzzle_8*> Down_fringe;
    std::unordered_map<std::string, Puzzle_8*> Total_Top;
    std::unordered_map<std::string, Puzzle_8*> Total_Down;
    Puzzle_8* initial_state;
    Puzzle_8* goal_state;
    bool Is_new2(Puzzle_8* new_node, std::unordered_map<std::string, Puzzle_8*>& total_set) {
        std::string key = give_order(new_node);
        if (total_set.find(key) == total_set.end())
            return true;
        else
            return false;
    }

    void Initialization(Puzzle_8* init, Puzzle_8* gol) {
        initial_state = new Puzzle_8;
        *initial_state = *init;
        goal_state =  new Puzzle_8;
        *goal_state = *gol;
        init = initial_state;
        gol = goal_state;
        initial_state->h_n = Calculate_Manhattan_distance(*initial_state, goal_state);
        Top_fringe.push_back(initial_state);
        Down_fringe.push(goal_state);
        Total_Top.insert(std::make_pair(give_order(initial_state), initial_state));
        Total_Down.insert(std::make_pair(give_order(goal_state), goal_state));
    }

    bool Successor1(Puzzle_8* current_state, Puzzle_8* goal_state) {

        Puzzle_8* new_puzzle;

        new_puzzle= Right_Left(current_state, false);


        if (new_puzzle != nullptr)
        {

            new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, goal_state);

            if(Is_new2(new_puzzle,Total_Top))
                Push_with_sort(Top_fringe, new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Down);
            if (common_node != nullptr)
            {
                Goal_Finded(new_puzzle, common_node);
                return true;
            }
            Push_TOTALS(Total_Top, new_puzzle);

        }

        new_puzzle = Down_Up(current_state, false);


        if (new_puzzle != nullptr)
        {


            new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, goal_state);
            if (Is_new2(new_puzzle, Total_Top))
            Push_with_sort(Top_fringe, new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Down);
            if (common_node != nullptr)
            {
                Goal_Finded(new_puzzle, common_node);
                return true;
            }


            Push_TOTALS(Total_Top, new_puzzle);

        }
        

        new_puzzle = Left_Right(current_state, false);


        if (new_puzzle != nullptr)
        {
            new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, goal_state);
            if (Is_new2(new_puzzle, Total_Top))
                Push_with_sort(Top_fringe, new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Down);
            if (common_node != nullptr)
            {
                Goal_Finded(new_puzzle, common_node);
                return true;
            }


            Push_TOTALS(Total_Top, new_puzzle);

        }

        new_puzzle = Up_Down(current_state, false);

        if (new_puzzle != nullptr) //left-right swap
        {
            new_puzzle->h_n = Calculate_Manhattan_distance(*new_puzzle, goal_state);
            if (Is_new2(new_puzzle, Total_Top))
            Push_with_sort(Top_fringe, new_puzzle);
            //Check intersection with other tree 
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Down);
            if (common_node != nullptr)
            {
                Goal_Finded(new_puzzle, common_node);
                return true;
            }


            Push_TOTALS(Total_Top, new_puzzle);

        }
        return false;
    }

    bool Successor2(Puzzle_8* current_state, Puzzle_8* goal_state) {

        Puzzle_8* new_puzzle;
        new_puzzle= Right_Left(current_state, true);
        if (new_puzzle != nullptr)
        {
            if (Is_new2(new_puzzle, Total_Down))
                Down_fringe.push(new_puzzle);
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Top);
            if (common_node != nullptr)
            {
                Goal_Finded(common_node, new_puzzle);
                return true;
            }
            Push_TOTALS(Total_Down, new_puzzle);

        }

        new_puzzle = Down_Up(current_state, true);

        if (new_puzzle != nullptr)
        {
            if (Is_new2(new_puzzle, Total_Down))
                Down_fringe.push(new_puzzle);
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Top);
            if (common_node != nullptr)
            {
                Goal_Finded(common_node, new_puzzle);
                return true;
            }
            Push_TOTALS(Total_Down, new_puzzle);

        }

        new_puzzle = Left_Right(current_state, true);

        if (new_puzzle != nullptr)
        {
            if (Is_new2(new_puzzle, Total_Down))
                Down_fringe.push(new_puzzle);
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Top);
            if (common_node != nullptr)
            {
                Goal_Finded(common_node, new_puzzle);
                return true;
            }
            Push_TOTALS(Total_Down, new_puzzle);

        }

        new_puzzle = Up_Down(current_state, true);

        if (new_puzzle != nullptr) //left-right swap
        {
            if (Is_new2(new_puzzle, Total_Down))
                Down_fringe.push(new_puzzle);
            Puzzle_8* common_node = Find_Intersect(new_puzzle, Total_Top);
            if (common_node != nullptr)
            {
                Goal_Finded(common_node, new_puzzle);
                return true;
            }
            Push_TOTALS(Total_Down, new_puzzle);

        }
        return false;
    }

    void AS_BFS_operation(Puzzle_8* initial_state, Puzzle_8* goal_state) {

        if (!is_promising(*initial_state, 1)) {
            return;
        }
        Initialization(initial_state, goal_state);

        while (true) {
            if (Top_fringe.empty() || Down_fringe.empty()) {
                std::cout << "Have not Answer !\n";
                return;
            }

            //AS_Up->dOWN

            Puzzle_8* current_state = Top_fringe.front();
            Top_fringe.pop_front();


            if (Successor1(current_state, goal_state))
                return;

            //AS_DOWN->UP

            current_state = Down_fringe.front();
            Down_fringe.pop();
            if (Successor2(current_state, initial_state))
                return;
        }
    }

public:

    void Run(Puzzle_8 init,Puzzle_8 gol){
        Puzzle_8* initial_state = new Puzzle_8;
        *initial_state = init;
        Puzzle_8* goal_state = new Puzzle_8;
        *goal_state = gol;
        AS_BFS_operation(initial_state, goal_state);
    }
    ~AS_BFS() {
        for (auto const& pair : Total_Top) {
            delete pair.second;
        }
        for (auto const& pair : Total_Down) {
            delete pair.second;
        }

    }
};



int main() {


    //Easy
    //Puzzle_8  p = { 1,3,4,8,6,2,7,0,5 };
    //Medium 
    //Puzzle_8  p = { 2,8,1,0,4,3,7,6,5 };
    //Hard
   // Puzzle_8  p = { 2,8,1,4,6,3,0,7,5 };
    //Worst
    //Puzzle_8 p = { 5,6,7,4,0,8,3,2,1 };
    //Insane
    //Puzzle_8  p = { 1,2,7,4,0,8,3,6,5 };
    //Puzzle_8 p = { 2,3,1,8,0,4,7,6,5 };
    //Puzzle_8  p = { 5,2,7,4,6,1,3,0,8 };

    A_star s;
    //s.A_star_operation(&p);

    Puzzle_8  i = { 5,6,7,4,0,8,3,2,1 };
    Puzzle_8 g = { 1,2,3,8,0,4,7,6,5 };

    Puzzle_8 newcase = s.Random_case_generator();

    for (int hi = 0; hi < 10000; hi++) {
        newcase = s.Random_case_generator();
        AS_AS * c= new AS_AS;
        c->Run(newcase,g);
        delete c;
        //std::cout << "Puzzle" << i+1 << " is OK .\n";
    }
    std::cout << "5000 Random Puzzle Solved !!\n";
    int l;
    std::cin >> l;
}

void Puzzle_8_Menu(void)
{

}
