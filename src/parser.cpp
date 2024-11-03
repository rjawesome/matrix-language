#include "parser.h"
struct Expression {
    string name;
    vector<Expression> children;
};

struct Function {
    vector<DataType> args;
    DataType returnType;
    DataContainer (*func)(vector<DataContainer>);
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

DataContainer evaluate(Expression const &e, map<string, DataContainer> &globalFrame) {
    if (e.children.size() > 0) {
       if (e.name == "=") {
            assert(e.children.size() == 2);
            assert(e.children[0].children.size() == 0);
            assert(e.children[0].name.length() > 0 && banned.find(e.children[0].name[0]) == banned.end());
            string name = e.children[0].name;

            if (globalFrame.find(name) != globalFrame.end()) {
                if (globalFrame[name].type == TYPE_MATRIX || globalFrame[name].type == TYPE_VECTOR) {
                    delete globalFrame[name].ptr;
                }
            }

            DataContainer value = evaluate(e.children[0], globalFrame);
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
            globalFrame[name] = value;
            return value;
        } else {
            DataContainer funcData = evaluate(e.children[0], globalFrame);
            assert(funcData.type == TYPE_FUNCTION);
            Function f = *(Function*)(funcData.ptr);
            assert(f.args.size() == e.children.size() - 1);
            vector<DataContainer> args;
            for (int i = 0; i < f.args.size(); i++) {
                DataContainer arg = evaluate(e.children[i+1], globalFrame);
                assert(arg.type == f.args[i]);
                args.push_back(arg);
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
        assert(globalFrame.find(e.name) != globalFrame.end());
        return globalFrame[e.name];
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
    getline(cin, line);
    queue<string> tokens = tokenize(line);
    Expression expression = parse(tokens, true);
    print_expression(expression);
}