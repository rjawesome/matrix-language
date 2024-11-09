#include "parser.h"

set<char> special_tokens{')', '(', ',', '+', '*', '^', '='};

set<string> infix_tokens{"+", "*", "^", "="};

map<string, int> priority = {
    {"=", 0},
    {"+", 1},
    {"*", 2},
    {"^", 3}
};

variant<string_view, Expression> parseTokens(queue<string> &tokens, bool infix) {
    if (tokens.empty()) return "Expected more tokens";

    Expression cur_expression;
    string cur_token = tokens.front(); tokens.pop();

    if (cur_token == ")") return ") without matching (";

    if (cur_token == "(") {
        variant<string_view, Expression> cur_expr_res = parseTokens(tokens, true);
        if (holds_alternative<string_view>(cur_expr_res)) return cur_expr_res;
        cur_expression = get<Expression>(cur_expr_res);

        expect(!tokens.empty() && tokens.front() == ")", "( was not closed");
        tokens.pop();
    } else {
        cur_expression = {cur_token};
    }

    if (tokens.empty()) {
        return cur_expression;
    }

    bool exit = false;

    // we want to keep seeing if the expression needs to be extended (either infix or call expression)
    while (!exit) {
        exit = true;
        if (infix && infix_tokens.find(tokens.front()) != infix_tokens.end()) {
            exit = false;
            stack<string> operators;
            stack<Expression> results;
            results.push(cur_expression);
            while (!tokens.empty() && infix_tokens.find(tokens.front()) != infix_tokens.end()) {
                string cur_operator = tokens.front(); tokens.pop();

                variant<string_view, Expression> next_expr_res = parseTokens(tokens, false);
                if (holds_alternative<string_view>(next_expr_res)) return next_expr_res;
                Expression next_expression = get<Expression>(next_expr_res);

                while (operators.size() > 0 && priority[operators.top()] >= priority[cur_operator]) {
                    expect(results.size() >= 2, "Not enough arguments for infix operator");
                    string op = operators.top(); operators.pop();
                    Expression rhs = results.top(); results.pop();
                    Expression lhs = results.top(); results.pop();
                    results.push({"function", {{op}, lhs, rhs}});
                }
                
                results.push(next_expression);
                operators.push(cur_operator);
            }
            while (operators.size() > 0) {
                expect(results.size() >= 2, "Not enough arguments for infix operator");
                string op = operators.top(); operators.pop();
                Expression rhs = results.top(); results.pop();
                Expression lhs = results.top(); results.pop();
                results.push({"function", {{op}, lhs, rhs}});
            }

            expect(results.size() == 1, "Infix expression does not evaluate to one value");
            cur_expression = results.top();
        }

        // chcek if calling is happening
        while (!tokens.empty() && tokens.front() == "(") {
            exit = false;
            cur_expression = {"function", {cur_expression}};
            tokens.pop();
            while (!tokens.empty() && tokens.front() != ")") {

                variant<string_view, Expression> child_expr_res = parseTokens(tokens, true);
                if (holds_alternative<string_view>(child_expr_res)) return child_expr_res;
                Expression child_expression = get<Expression>(child_expr_res);

                cur_expression.children.push_back(child_expression);

                if (tokens.front() == ",") {
                    tokens.pop();
                } else if (tokens.front() != ")") {
                    return "Arguments to function formatted badly";
                }
            }
            expect(!tokens.empty(), "Arguments to function formatted badly");
            tokens.pop();
        }
    }

    return cur_expression;
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

variant<string_view, Expression> parseString(string s) {
    queue<string> tokens = tokenize(s);
    return parseTokens(tokens, true);
}