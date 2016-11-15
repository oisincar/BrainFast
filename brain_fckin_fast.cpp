#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
using namespace std;

char *dataPtr;
char tape[10000];

map <int, int> blah_;

class Operator {
public:
    Operator(int offset){offset_ = offset;}
    int offset_;
    virtual void Execute(){ }
};


class Plus    : public Operator {
public: int times_;
    Plus(int offset, int times) : Operator(offset){
        times_ = times;
    }

    void Execute() {
        *(dataPtr + offset_) += times_;
    }
};

class Shift          : public Operator { public: Shift   (int offset) : Operator(offset){} void Execute(){ dataPtr += offset_; } };
class Dot            : public Operator { public: Dot     (int offset) : Operator(offset){} void Execute(){ putchar(*(dataPtr + offset_)); } };
class Comma          : public Operator { public: Comma   (int offset) : Operator(offset){} void Execute(){ cin >> *(dataPtr + offset_); } };
class ResetReg       : public Operator { public: ResetReg(int offset) : Operator(offset){} void Execute(){ *(dataPtr + offset_) = 0; } }; // a loop which looks like [-] or [+]

class LinearOperands : public Operator { // increments or decrements with no special operations inbetween (I.e. no ,.[]). Usually followed by a Shift operator.
protected:
    map<int, int> pairs_;
public:
    LinearOperands(map<int, int> pairs, int offset) : Operator(offset){
        // for(auto elem : pairs)
        //     pairs_[elem.first + offset] = elem.second;
        pairs_ = pairs;
    }
    virtual void Execute(){
        for(auto elem : pairs_)
            *(dataPtr + elem.first + offset_) += elem.second;
    }
};

// TODO: Fix bug with specific simple loops where the starting value isn't changed by 1 or -1.
class MultiplyLoop    : public LinearOperands { // loops which start and end on the same point in the ticker.. Eg [>++<-] which kinda multiplies the current buffer by 2
public:
    MultiplyLoop(map<int, int> pairs, int offset) : LinearOperands(pairs, offset) {}
    void Execute() {
        char currVal = *(dataPtr + offset_);

        if (currVal == 0) return; // Continue if we start at 0.

        // TODO: Handle complex case, where the base offset isn't 1 or -1. Note this is currently filtered upstream..
        // number of times the 'loop' would go.
        int loop_t = (pairs_[0] > 0) ? 256-currVal : currVal;

        for(auto elem : pairs_)
            *(dataPtr + elem.first + offset_) += (elem.second * loop_t);
    }
};

class Grouping       : public Operator {
protected:
    vector<Operator*> operators_;
public:
    Grouping(Operator *first_op, int offset, vector<char> operators) : Operator(offset) {

        if (first_op) operators_.push_back(first_op);

        int loop_offset = offset;

        int i = 0;
        while (i < operators.size()) {

            char c = operators[i];

            if (c == '.') operators_.push_back(new Dot(loop_offset));
            else if (c == ',') operators_.push_back(new Comma(loop_offset));
            else {
                // handle the group-able operations..
                bool isCompleteLoop = (c == '[');
                if (isCompleteLoop) i++;

                // Get a list of all operations until next 'complex' ( ,.[] ) operation
                map<int, int> increments; // mapping between offset and increment/decrement
                int offset = 0;
                while (i < operators.size()) {
                    char d = operators[i];

                    if      (d == '<') offset--;
                    else if (d == '>') offset++;
                    else if (d == '+') increments[offset]++;
                    else if (d == '-') increments[offset]--;

                    else if (d == ']'){
                        // i++;
                        break;
                    }
                    // break on ,.[]
                    else if (d == ',' || d == '.' || d == '[') {
                        isCompleteLoop = false;
                        break;
                    }
                    i++;

                    // otherwise char is comment or newline
                }

                if (isCompleteLoop && offset == 0) { // simple loop, can be calculated in one step
                    if (increments.size() == 1) // either infinite loop or resetReg, assume latter.
                        operators_.push_back(new ResetReg(loop_offset));
                    else
                        operators_.push_back(new MultiplyLoop(increments, loop_offset));
                }
                else {
                    Operator *l_oper = NULL;
                    if (increments.size() > 0) {
                        if (increments.size() == 1) {
                            auto b = *(increments.begin());
                            l_oper = new Plus(b.first + loop_offset, b.second);
                        }
                        else {
                            l_oper = new LinearOperands(increments, loop_offset);
                        }
                    }

                    if (c == '[') {  // if we're at the start of a loop, we have to pass what we calculated down recursively.

                        // Deal with offset changes.
                        if (loop_offset != 0){
                            if (l_oper) l_oper->offset_ -= loop_offset;
                            operators_.push_back(new Shift(loop_offset));
                            loop_offset = 0;
                        }

                        // chew until closing bracket and recurse.
                        int bracketLvl = 1;
                        vector<char> subOperators;
                        while (true){

                            if (operators[i] == '[') bracketLvl++;
                            else if (operators[i] == ']'){
                                bracketLvl--;
                                if (bracketLvl == 0) break;
                            }
                            subOperators.push_back(operators[i]);
                            i++;
                        }
                        operators_.push_back(new Grouping(l_oper, offset, subOperators));
                    }
                    else {
                        if (l_oper)
                            operators_.push_back(l_oper);
                        loop_offset += offset;
                        i--;
                    }
                }
            }
            i++;
        }

        // Deal with offset changes.
        if (loop_offset != 0){
            operators_.push_back(new Shift(loop_offset));
            loop_offset = 0;
        }
    }

    void Execute(){
        while (*dataPtr != 0)
            for(auto const& o : operators_)
                o->Execute();
    }
};

class Program : public Grouping {
public:
    Program(vector<char> operators) : Grouping(NULL, 0, operators) {} // Bool is true if this is the head node (ie the base program);
    void Execute(){
        for(auto const& o : operators_)
            o->Execute();
    }
};

int main() {
    dataPtr = &tape[0];

    // ifstream t("hanoi.bf");
    ifstream t("mandel.b");
    // ifstream t("square.bf");
    stringstream str;
    str << t.rdbuf();
    string r_prog = str.str();

    vector<char> p_vector(r_prog.begin(), r_prog.end());
    Program *prog = new Program(p_vector);

    prog->Execute();

    cout << endl;
}
