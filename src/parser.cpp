#include "parser.h"
struct Expression {
    string name;
    vector<Expression> children;
};

set<char> special_tokens{')', '(', ',', '+', '*', '^'};
set<char> banned{'1','2','3','4','5','6','7','8','9','0','/','-'};
set<string> infix_tokens{"+", "*", "^"};

map<string, int> priority = {
    {"=", 0},
    {"+", 1},
    {"*", 2},
    {"^", 3}
};

DataContainer get_matrix_adjusted(DataContainer args[]) {
    vector<vector<Fraction>>* matrix = new vector<vector<Fraction>>(move(get_matrix()));
    return {TYPE_MATRIX, false, .ptr = matrix};
}

map<string, DataContainer> base_global_frame = {
    {"get_matrix", {TYPE_FUNCTION, false, .function = {0, {}, TYPE_MATRIX, get_matrix_adjusted} }},
};

Expression parse(queue<string> &tokens, bool infix) {
    if (tokens.empty()) assert(false);

    Expression cur_expression;
    string cur_token = tokens.front(); tokens.pop();

    if (cur_token == ")") assert(false);

    if (cur_token == "(") {
        cur_expression = parse(tokens, true);
        assert(!tokens.empty() && tokens.front() == ")");
        tokens.pop();
    } else {
        cur_expression = {cur_token};
    }

    if (tokens.empty()) {
        return cur_expression;
    }

    if (infix && infix_tokens.find(tokens.front()) != infix_tokens.end()) {
        stack<string> operators;
        stack<Expression> results;
        results.push(cur_expression);
        while (!tokens.empty() && infix_tokens.find(tokens.front()) != infix_tokens.end()) {
            string cur_operator = tokens.front(); tokens.pop();
            Expression next_expression = parse(tokens, false);
            while (operators.size() > 0 && priority[operators.top()] >= priority[cur_operator]) {
                assert(results.size() >= 2);
                string op = operators.top(); operators.pop();
                Expression rhs = results.top(); results.pop();
                Expression lhs = results.top(); results.pop();
                results.push({"function", {{op}, lhs, rhs}});
            }
            
            results.push(next_expression);
            operators.push(cur_operator);
        }
        while (operators.size() > 0) {
            assert(results.size() >= 2);
            string op = operators.top(); operators.pop();
            Expression rhs = results.top(); results.pop();
            Expression lhs = results.top(); results.pop();
            results.push({"function", {{op}, lhs, rhs}});
        }

        assert(results.size() == 1);
        cur_expression = results.top();
    }

    // chcek if calling is happening
    while (!tokens.empty() && tokens.front() == "(") {
        cur_expression = {"function", {cur_expression}};
        tokens.pop();
        while (!tokens.empty() && tokens.front() != ")") {
            Expression child_expression = parse(tokens, true);
            cur_expression.children.push_back(child_expression);

            if (tokens.front() == ",") {
                tokens.pop();
            } else if (tokens.front() != ")") {
                assert(false);
            }
        }
        assert(!tokens.empty());
        tokens.pop();
    }

    return cur_expression;
}

DataContainer evaluate(Expression const &e, map<string, DataContainer> &global_frame) {
    if (e.children.size() > 0) {
       if (e.name == "=") {
            assert(e.children.size() == 2);
            assert(e.children[0].children.size() == 0);
            assert(e.children[0].name.length() > 0 && banned.find(e.children[0].name[0]) == banned.end());
            string name = e.children[0].name;

            if (global_frame.find(name) != global_frame.end()) {
                if (global_frame[name].type == TYPE_MATRIX || global_frame[name].type == TYPE_VECTOR) {
                    delete global_frame[name].ptr;
                }
            }

            DataContainer value = evaluate(e.children[0], global_frame);
            if (!value.anon && (value.type == TYPE_MATRIX || value.type == TYPE_VECTOR)) {
                if (value.type == TYPE_VECTOR) {
                    value.ptr = new vector<Fraction>(*(vector<Fraction>*)value.ptr);
                } 
                if (value.type == TYPE_MATRIX) {
                    value.ptr = new vector<vector<Fraction>>(*(vector<vector<Fraction>>*)value.ptr);
                }
            } else {
                value.anon = false;
            }
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
                if (arg.anon && (arg.type == TYPE_MATRIX || arg.type == TYPE_VECTOR)) {
                    delete arg.ptr;
                }
            }
            return ret;
       }
    } else {
        assert(global_frame.find(e.name) != global_frame.end());
        return global_frame[e.name];
    }
}

queue<string> tokenize(string line) {
    string token = "";
    queue<string> tokens;
    for (int i = 0; i < line.length(); i++) {
        if (line[i] == ' ') {
            continue;
        }

        if (special_tokens.find(line[i]) != special_tokens.end()) {
            if (token.size() != 0) tokens.push(token);
            tokens.push(string() + line[i]);
            token = "";
        }
        else {
            token += line[i];
        }
    }
    if (token.size() != 0) {
        tokens.push(token);
    }

    return tokens;
}

void print_expression(Expression expression) {
    cout << "value: " << expression.name << endl;;
    for (int i = 0; i < expression.children.size(); i++) {
        cout << "child #" << i << endl;
        print_expression(expression.children[i]);
        cout << "-----" << endl;
    }
}

void process_expression() {
    string line;
    getline(cin, line); // clear
    getline(cin, line);
    queue<string> tokens = tokenize(line);
    Expression expression = parse(tokens, true);
    DataContainer result = evaluate(expression, base_global_frame);
    print_matrix(*(vector<vector<Fraction>>*)(result.ptr));
    delete result.ptr;
}