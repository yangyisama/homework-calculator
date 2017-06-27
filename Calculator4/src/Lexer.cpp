#include<string>   // std::string, std::to_string
#include<iostream> // std::cerr
#include<cstdlib>  // exit(int n)
#include<conio.h>  // getch
#include "CharMethods.h"

class Token
{
public:
    enum TokenName { NUM, LEFTPAREN, RIGHTPAREN, DOT, PLUS, MINUS, MUL, FAC, COS, END };
    TokenName getName() {return name;}
    int getVal() {return val;}
    Token(TokenName n, int v) {name = n; val = v;}
    Token(TokenName n) {name = n; val = -1;}
    std::string toString();

private:
    TokenName name;
    int val;
    std::string getEnumString(TokenName n);
};

std::string Token::toString()
{
    if (val != -1)
        return std::string("(") + getEnumString(name) + ": " + int_to_string(val) + ")";
    else
        return std::string("(") + getEnumString(name) + ")";
}

std::string Token::getEnumString(TokenName n)
{
    switch(n)
    {
    case NUM:
        return std::string("NUM");
    case LEFTPAREN:
        return std::string("LEFTPAREN");
    case RIGHTPAREN:
        return std::string("RIGHTPAREN");
    case DOT:
        return std::string("DOT");
    case PLUS:
        return std::string("PLUS");
    case MINUS:
        return std::string("MINUS");
    case MUL:
        return std::string("MUL");
    case FAC:
        return std::string("FAC");
    case COS:
        return std::string("COS");
    case END:
        return std::string("END");
    default:
        return std::string("??ERROR??");
    }
}

class Lexer
{
public:
    void putback(Token t);
    Token* get();
    Lexer(std::string s);
private:
    char readInChar();
    void fail(std::string msg);
    Token* last;
    std::string source;
    int next_index;
    char peek;
};

Lexer::Lexer(std::string s)
{
    source = s;
    source.append("$");
    last = nullptr;
    peek = ' ';
    next_index = 0;
}

void Lexer::fail (std::string msg)
{
    std::cerr << msg;
    getch();
    exit(-69);
}

void Lexer::putback(Token t)
{
    if (!last)
        last = &t;
    else
        fail("Can't put back more than one token");
}

char Lexer::readInChar()
{
    return source[next_index++];
}

Token* Lexer::get()
{
    if (last)
    {
        last = nullptr;
        return last;
    }
    // last is null, fetch next token:

    while(true) // skip through whitespace
    {
        if (!isWhitespace(peek))
            break;
        peek = readInChar();
    }

    if (isDigit(peek)) // read a number
    {
        int v = 0;
        do {
            v = 10*v + digitVal(peek);
            peek = readInChar();
        } while(isDigit(peek));
        return new Token(Token::NUM, v);
    }

    else
    {
        switch (peek)
        {
        case 'c':
            peek = readInChar();
            if (peek != 'o')
                fail(std::string("Unrecognised symbol \'") + "c" + peek + "\'");

            peek = readInChar();
            if (peek != 's')
                fail(std::string("Unrecognised symbol \'") + "co" + peek + "\'");
            else
            {
                peek = ' '; // setting peek to whitespace makes us read in the next char
                            // in the 1st while loop in next call to get
                return new Token(Token::COS);
            }
            break;
        case '+':
            peek = ' ';
            return new Token(Token::PLUS);
        case '-':
            peek = ' ';
            return new Token(Token::MINUS);
        case '*':
            peek = ' ';
            return new Token(Token::MUL);
        case '!':
            peek = ' ';
            return new Token(Token::FAC);
        case '(':
            peek = ' ';
            return new Token(Token::LEFTPAREN);
		case ')':
			peek = ' ';
			return new Token(Token::RIGHTPAREN);
		case '.':
			peek = ' ';
			return new Token(Token::DOT);
        case '$':
            peek = ' ';
            return new Token(Token::END);
        default:
            fail(std::string("Unrecognised symbol \'") + peek + "\'");
        }
    }

}