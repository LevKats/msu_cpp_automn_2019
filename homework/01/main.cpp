#include <iostream>
#include <vector>
#include <string>
#include <climits>

void parse_plus_mult_div(std::vector<std::string> &result,
        std::string &operators, char c) {
    if (result.size() == 0) {
        std::string mes = 
                "there must be - or digit on pos 0: str[0]=";
        throw std::invalid_argument(
                mes + c
                );
    }
    if (operators.length() != 0) {
        std::string mes = "can't parse ";
        throw std::invalid_argument(
                mes + operators + c
                );
    }
    operators += c;
    result.push_back(std::string{c});
}

void parse_minus(std::vector<std::string> &result,
        std::string &operators, char c) {
    if (result.size() == 0)
        result.push_back(std::string{'u'});
    else if (operators.length() == 0)
        result.push_back(std::string{'-'});
    else if (operators.length() == 1)
        result.push_back(std::string{'u'});
    else {
        std::string mes = "can't parse ";
        throw std::invalid_argument(
                mes + operators + c
                );
    }
    operators += c;
}

std::vector<std::string> parse_tokens(const char *str) {
    std::vector<std::string> result;
    std::string last, operators;
    auto is_digit = [](char c) { return '0' <= c && c <= '9'; };
    for (int i = 0; str[i] != '\0'; ++i) {
        char c = str[i];
        if (is_digit(c)) {
            last += c;
            operators.clear();
        }
        else {
            if (last.length() != 0) {
                result.push_back(last);
                last.clear();
            }
        if (c == ' ') {}
        else if (c == '+' || c == '*' || c == '/')
                parse_plus_mult_div(result, operators, c);
            else if (c == '-')
                parse_minus(result, operators, c);
            else {
                std::string mes = "invalid character: ";
                throw std::invalid_argument(
                        mes + c
                        );
            }
        }
    }
    
    if (operators.length() != 0) {
        std::string mes = "there can't be operators in the end: ";
        throw std::invalid_argument(mes + operators);
    }
    
    result.push_back(last);
    return result;
}

typedef std::vector<std::string>::reverse_iterator iter;

int const_uminus_proc(iter rbeg, iter rend) {
    if (rend - rbeg == 1) {
        try {
            return stoi(*rbeg);
        }
        catch (std::out_of_range &e) {
            std::string mes = "maybe too long number ";
            throw std::invalid_argument(mes + *rbeg + " " + e.what());
        }
        catch (std::invalid_argument &e) {
        std::string mes = "that can't happen but stoi can't convert";
            mes += "string of digits with base 10 to int: ";
        throw std::invalid_argument(mes + e.what());
        }
    }
    if (rend - rbeg <= 1) {
        throw std::invalid_argument("invalid iterators in const_uminus_proc");
    }
    if (rend - rbeg > 2 || *(rend - 1) != "u") {
        // it can't happen too...        
        std::string mes_what;
        std::string mes = "very strange constant ";
        for (auto it = rbeg; it != rend; ++it)
            mes_what = *it + mes_what;
        throw std::invalid_argument(mes + mes_what);
    }
    return -const_uminus_proc(rbeg, rend - 1);
}

int mult_div_proc(iter rbeg, iter rend) {
    for (auto it = rbeg; it != rend; ++it) {
        if (*it == "*") {
            int left = mult_div_proc(it + 1, rend);
            int right = mult_div_proc(rbeg, it);
            long long res = (long long)left * right;
            if (res > INT_MAX || res < INT_MIN) 
                throw std::domain_error("to big numbers to multiply");
            return left * right;
        }
        else if (*it == "/") {
            int left = mult_div_proc(it + 1, rend);
            int right = mult_div_proc(rbeg, it);
            if (right == 0) 
                throw std::domain_error("zero division");
            return left / right;
        }
    }
    return const_uminus_proc(rbeg, rend);
} 

int plus_minus_proc(iter rbeg, iter rend) {
    for (auto it = rbeg; it != rend; ++it) {
        if (*it == "+" || *it == "-") {
            int left = plus_minus_proc(it + 1, rend);
            int right = plus_minus_proc(rbeg, it);
            if (*it == "-")
                right = -right;
            long long res = (long long)left + right;
            if (res > INT_MAX || res < INT_MIN) 
                throw std::domain_error("to big numbers to sum");
            return left + right;
        }
    }
    return mult_div_proc(rbeg, rend);
}

int calc(char *str) {
    auto tokens = parse_tokens(str);
    return plus_minus_proc(tokens.rbegin(), tokens.rend()); 
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        std::cout << "there must be string in argv\n";
        return 1;
    }
    try { 
        std::cout << calc(argv[1]) << "\n";
    }
    catch (std::domain_error &er) {
        std::cerr << "error: " << er.what() << "\n";
        return 2;
    }
    catch (std::invalid_argument &er) {
        std::cerr << "error: " << er.what() << "\n";
        return 3;
    }
    return 0;
}

