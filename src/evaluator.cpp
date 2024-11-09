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

variant<string_view, DataContainer> evaluate(Expression const &e, map<string, DataContainer> &global_frame) {
    if (e.children.size() > 0) {
        if (e.children[0].name == "=") {
            expect(e.children.size() == 3, "Wrong amount of operands for equals");
            expect(e.children[1].children.size() == 0, "Cannot assign to expression");
            expect(e.children[1].name.length() > 0 && banned_var_starts.find(e.children[0].name[0]) == banned_var_starts.end(), "Bad variable name");
            string name = e.children[1].name;

            variant<string_view, DataContainer> value = evaluate(e.children[2], global_frame);
            if (holds_alternative<string_view>(value)) {
                return value;
            }

            if (global_frame.find(name) != global_frame.end()) {
                if (is_ptr(global_frame[name].type)) {
                    delete global_frame[name].ptr;
                }
            }

            DataContainer dupl_value = duplicate(get<DataContainer>(value));
            global_frame[name] = dupl_value;
            return dupl_value;
        } else {
            variant<string_view, DataContainer> func_data_res = evaluate(e.children[0], global_frame);
            if (holds_alternative<string_view>(func_data_res)) {
                return func_data_res;
            }
            DataContainer func_data = get<DataContainer>(func_data_res);

            expect(func_data.type == TYPE_FUNCTION, "Cannot call non-function");
            Function f = func_data.function;
            expect(f.arglen == e.children.size() - 1, "Wrong amount of arguments for function");

            DataContainer args[f.arglen];
            int evaluated_args = 0;
            variant<string_view, DataContainer> ret;
            for (int i = 0; i < f.arglen; i++) {
                variant<string_view, DataContainer> arg_res = evaluate(e.children[i+1], global_frame);
                if (holds_alternative<string_view>(arg_res)) {
                    ret = arg_res;
                    break;
                }
                DataContainer arg = get<DataContainer>(arg_res);
                if (arg.type != f.args[i]) {
                    ret = "Function argument with invalid type";
                    break;
                }

                args[i] = arg;
                evaluated_args++;
            }
            if (evaluated_args == f.arglen) {
                ret = f.func(args);
            }
            for (int i = 0; i < evaluated_args; i++) {
                DataContainer arg = args[i];
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
        expect(global_frame.find(e.name) != global_frame.end(), "Variable is not defined");
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
        variant<string_view, Expression> parse_result = parseString(line);
        if (holds_alternative<string_view>(parse_result)) {
            cout << get<string_view>(parse_result) << endl;
            continue;
        }
        variant<string_view, DataContainer> result = evaluate(get<Expression>(parse_result), base_global_frame);
        if (holds_alternative<string_view>(result)) {
            cout << get<string_view>(result) << endl;
            continue;
        }
        DataContainer data = get<DataContainer>(result);
        print_data(data);
        
        if (data.anon && is_ptr(data.type)) {
            delete data.ptr;
        }
    }
}