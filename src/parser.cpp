#include <bits/stdc++.h>
using namespace std;

struct Thing {
    string type;
    string name;
    vector<Thing> children;
};

set<char> special_tokens{')', '(', ',', '+', '*'};
set<string> infix_tokens{"+", "*"};

map<string, int> priority = {
    {"+", 0},
    {"*", 1},
};

pair<int, Thing> parse(int start, vector<string> const &tokens, bool infix) {
    if (start >= tokens.size()) assert(false);

    Thing parentThing;

    if (tokens[start] == ")") assert(false);

    if (tokens[start] == "(") {
        auto [s, t] = parse(start + 1, tokens, true);
        parentThing = t;
        start = s;
        assert(start < tokens.size() && tokens[start] == ")");
    } else {
        parentThing = {"value", tokens[start]};
    }

    if (start + 1 >= tokens.size()) {
        return {start + 1, parentThing};
    }

    // looking ahead
    start += 1;

    if (infix && start < tokens.size() && infix_tokens.find(tokens[start]) != infix_tokens.end()) {
        stack<string> operators;
        stack<Thing> results;
        results.push(parentThing);
        while (start < tokens.size() && infix_tokens.find(tokens[start]) != infix_tokens.end()) {
            auto [newstart, nextexpr] = parse(start + 1, tokens, false);
            while (operators.size() > 0 && priority[operators.top()] > priority[tokens[start]]) {
                assert(results.size() >= 2);
                string op = operators.top(); operators.pop();
                Thing infixThing{"function", op};
                Thing rhs = results.top(); results.pop();
                Thing lhs = results.top(); results.pop();
                infixThing.children.push_back(lhs);
                infixThing.children.push_back(rhs);
                results.push(infixThing);
            }
            
            results.push(nextexpr);
            operators.push(tokens[start]);
            start = newstart;
        }
        while (operators.size() > 0) {
            assert(results.size() >= 2);
            string op = operators.top(); operators.pop();
            Thing infixThing{"function", op};
            Thing rhs = results.top(); results.pop();
            Thing lhs = results.top(); results.pop();
            infixThing.children.push_back(lhs);
            infixThing.children.push_back(rhs);
            results.push(infixThing);
        }

        assert(results.size() == 1);
        parentThing = results.top();
    }
    // no support for first class functions
    else if (tokens[start] == "(") {
        parentThing.type = "function";
        start += 1;
        while (start < tokens.size() && tokens[start] != ")") {
            auto [s, t] = parse(start, tokens, true);
            parentThing.children.push_back(t);
            assert(s < tokens.size());

            if (tokens[s] == ",") {
                start = s + 1;
            } else if (tokens[s] == ")") {
                start = s;
            } else {
                assert(false);
            }
        }
    }

    return { start, parentThing };
}

vector<string> tokenize(string line) {
    string token = "";
    vector<string> tokens;
    for (int i = 0; i < line.length(); i++) {
        if (line[i] == ' ') {
            continue;
        }

        if (special_tokens.find(line[i]) != special_tokens.end()) {
            if (token.size() != 0) tokens.push_back(token);
            tokens.push_back(string() + line[i]);
            token = "";
        }
        else {
            token += line[i];
        }
    }
    if (token.size() != 0) {
        tokens.push_back(token);
    }

    return tokens;
}

void print_thing(Thing thing) {
    cout << "value: " << thing.name << " type: " << thing.type << endl;;
    for (int i = 0; i < thing.children.size(); i++) {
        cout << "child #" << i << endl;
        print_thing(thing.children[i]);
        cout << "-----" << endl;
    }
}

int main() {
    string line;
    getline(cin, line);
    auto [_, thing] = parse(0, tokenize(line), true);
    print_thing(thing);
}