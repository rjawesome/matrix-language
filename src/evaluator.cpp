#include "evaluator.h"

set<char> banned_var_starts{'1','2','3','4','5','6','7','8','9','0','/','-'};
set<char> frac_start{'-','1','2','3','4','5','6','7','8','9'};

inline bool is_ptr(DataType t) {
    return t == TYPE_MATRIX || t == TYPE_VECTOR;
}

DataContainer duplicate(DataContainer value) {
    if (!value.anon && is_ptr(value.type)) {
        if (value.type == TYPE_VECTOR) {
            value.ptr = new vector<Fraction>(*(vector<Fraction>*)value.ptr);
        } 
        if (value.type == TYPE_MATRIX) {
            value.ptr = new vector<vector<Fraction>>(*(vector<vector<Fraction>>*)value.ptr);
        }
    } else {
        value.anon = false;
    }
    return value;
}

DataContainer evaluate(Expression const &e, map<string, DataContainer> &global_frame) {
    if (e.children.size() > 0) {
        if (e.children[0].name == "=") {
            assert(e.children.size() == 3);
            assert(e.children[1].children.size() == 0);
            assert(e.children[1].name.length() > 0 && banned_var_starts.find(e.children[0].name[0]) == banned_var_starts.end());
            string name = e.children[1].name;

            if (global_frame.find(name) != global_frame.end()) {
                if (is_ptr(global_frame[name].type)) {
                    delete global_frame[name].ptr;
                }
            }

            DataContainer value = duplicate(evaluate(e.children[2], global_frame));
            global_frame[name] = value;
            return value;
        } else {
            DataContainer funcData = evaluate(e.children[0], global_frame);
            assert(funcData.type == TYPE_FUNCTION);
            Function f = funcData.function;
            assert(f.arglen == e.children.size() - 1);
            DataContainer args[f.arglen];
            for (int i = 0; i < f.arglen; i++) {
                DataContainer arg = evaluate(e.children[i+1], global_frame);
                assert(arg.type == f.args[i]);
                args[i] = arg;
            }
            DataContainer ret = f.func(args);
            for (DataContainer arg : args) {
                if (arg.anon && is_ptr(arg.type)) {
                    delete arg.ptr;
                }
            }
            return ret;
       }
    } else if (e.name.length() > 0 && frac_start.find(e.name[0]) != frac_start.end()) {
        DataContainer c = {TYPE_FRACTION};
        load_frac(e.name, c.frac);
        return c;
    } else {
        assert(global_frame.find(e.name) != global_frame.end());
        return global_frame[e.name];
    }
}

void print_expression(Expression expression) {
    cout << "value: " << expression.name << endl;;
    for (int i = 0; i < expression.children.size(); i++) {
        cout << "child #" << i << endl;
        print_expression(expression.children[i]);
        cout << "-----" << endl;
    }
}

void print_data(DataContainer c) {
    if (c.type == TYPE_MATRIX) {
        print_matrix(*(vector<vector<Fraction>>*)(c.ptr));
    }
    if (c.type == TYPE_VECTOR) {
        print_vector(*(vector<Fraction>*)(c.ptr));
    }
    if (c.type == TYPE_FRACTION) {
        print_frac(c.frac);
        cout << endl;
    }
    if (c.type == TYPE_FUNCTION) {
        cout << "<function>" << endl;
    }
}

void process_expressions() {
    string line;
    while (true) {
        cout << ">> ";
        getline(cin, line);
        Expression expression = parseString(line);
        DataContainer result = evaluate(expression, base_global_frame);
        print_data(result);
        
        if (result.anon && is_ptr(result.type)) {
            delete result.ptr;
        }
    }
}