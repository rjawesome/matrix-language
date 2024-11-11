#include "parser.h"

set<char> special_tokens{')', '(', ',', '+', '*', '^', '='};

set<string> infix_tokens{"+", "*", "^", "="};

map<string, int> priority = {
    {"=", 0},
    {"+", 1},
    {"*", 2},
    {"^", 3}
};

variant<Error, Expression> parseTokens(queue<pair<string, int>> &tokens, bool infix) {
    if (tokens.empty()) return Error{"Expected more tokens", 0};

    Expression cur_expression;
    auto [cur_token, cur_line] = tokens.front(); tokens.pop();

    if (cur_token == ")") return Error{") without matching (", cur_line};

    if (cur_token == "(") {
        variant<Error, Expression> cur_expr_res = parseTokens(tokens, true);
        if (holds_alternative<Error>(cur_expr_res)) return cur_expr_res;
        cur_expression = get<Expression>(cur_expr_res);

        if (tokens.empty() || tokens.front().first != ")") return Error{"( was not closed", cur_line};
        tokens.pop();
    } else {
        cur_expression = {cur_token, cur_line};
    }

    if (tokens.empty()) {
        return cur_expression;
    }

    bool exit = false;

    // we want to keep seeing if the expression needs to be extended (either infix or call expression)
    while (!exit) {
        exit = true;
        if (infix && infix_tokens.find(tokens.front().first) != infix_tokens.end()) {
            exit = false;
            stack<pair<string, int>> operators;
            stack<Expression> results;
            results.push(cur_expression);
            while (!tokens.empty() && infix_tokens.find(tokens.front().first) != infix_tokens.end()) {
                auto [cur_operator, cur_line] = tokens.front(); tokens.pop();

                variant<Error, Expression> next_expr_res = parseTokens(tokens, false);
                if (holds_alternative<Error>(next_expr_res)) return next_expr_res;
                Expression next_expression = get<Expression>(next_expr_res);

                while (operators.size() > 0 && priority[operators.top().first] >= priority[cur_operator]) {
                    if (results.size() < 2) return Error{"Not enough arguments for infix operator", operators.top().second};
                    auto [op, line] = operators.top(); operators.pop();
                    Expression rhs = results.top(); results.pop();
                    Expression lhs = results.top(); results.pop();
                    results.push({"function", line, {{op}, lhs, rhs}});
                }
                
                results.push(next_expression);
                operators.push({cur_operator, cur_line});
            }
            while (operators.size() > 0) {
                if (results.size() < 2) return Error{"Not enough arguments for infix operator", operators.top().second};
                auto [op, line] = operators.top(); operators.pop();
                Expression rhs = results.top(); results.pop();
                Expression lhs = results.top(); results.pop();
                results.push({"function", line, {{op}, lhs, rhs}});
            }

            if (results.size() != 1) return Error{"Not enough arguments for infix operator", cur_line};
            cur_expression = results.top();
        }

        // chcek if calling is happening
        while (!tokens.empty() && tokens.front().first == "(") {
            exit = false;
            cur_expression = {"function", cur_line, {cur_expression}};
            tokens.pop();
            while (!tokens.empty() && tokens.front().first != ")") {

                variant<Error, Expression> child_expr_res = parseTokens(tokens, true);
                if (holds_alternative<Error>(child_expr_res)) return child_expr_res;
                Expression child_expression = get<Expression>(child_expr_res);

                cur_expression.children.push_back(child_expression);

                if (tokens.front().first == ",") {
                    tokens.pop();
                } else if (tokens.front().first != ")") {
                    return Error{"Arguments to function formatted badly", cur_line};
                }
            }
            if (tokens.empty()) return Error{"Arguments to function formatted badly", cur_line};
            tokens.pop();
        }
    }

    return cur_expression;
}

queue<pair<string, int>> tokenize(string line) {
    int start = 0;
    string token = "";
    queue<pair<string, int>> tokens;
    for (int i = 0; i < line.length(); i++) {
        if (line[i] == ' ') {
            start++;
            continue;
        }

        if (special_tokens.find(line[i]) != special_tokens.end()) {
            if (token.size() != 0) tokens.push({token, start});
            tokens.push({string() + line[i], i});
            start = i + 1;
            token = "";
        }
        else {
            token += line[i];
        }
    }
    if (token.size() != 0) {
        tokens.push({token, start});
    }

    return tokens;
}

variant<Error, Expression> parseString(string s) {
    queue<pair<string, int>> tokens = tokenize(s);
    return parseTokens(tokens, true);
}