#include "Parser.h"
#include "Log.h"

Wonin::Parser::Parser(Scanner const& scanner)
    : m_scanner(scanner)
{}

template<unsigned Count>
bool Wonin::Parser::IsMatch(Token::Type types[Count])
{
    for (unsigned i = 0; i < Count; i++)
    {
        if (IsMatch(types[i]))
        {
            return true;
        }
    }

    return false;
}

Wonin::Expression* Wonin::Parser::Expr()
{
    return Equality();
}

Wonin::Expression* Wonin::Parser::Equality()
{
    Token::Type tokens[] = { Token::Type::NotEqual, Token::Type::Equal };
    Expression* expr = Comparison();
    while (IsMatch<2>(tokens))
    {
        Token op = m_scanner.m_tokens[m_position++];
        Expression *right = Comparison();
        expr = new Binary(expr, op.m_type, right);
    }
    return expr;
}

Wonin::Expression* Wonin::Parser::Comparison()
{
    Token::Type tokens[] = { Token::Type::Less, Token::Type::LessOrEqual, Token::Type::Greater, Token::Type::GreaterOrEqual };
    Expression* expr = Term();
    while (IsMatch<4>(tokens))
    {
        Token op = m_scanner.m_tokens[m_position++];
        Expression *right = Term();
        expr = new Binary(expr, op.m_type, right);
    }
    return expr;
}

Wonin::Expression* Wonin::Parser::Term()
{
    Token::Type tokens[] = { Token::Type::Plus, Token::Type::Minus };
    Expression *expr = Factor();
    while (IsMatch<2>(tokens))
    {
        Token op = m_scanner.m_tokens[m_position++];
        Expression *right = Factor();
        expr = new Binary(expr, op.m_type, right);
    }
    return expr;
}
Wonin::Expression* Wonin::Parser::Factor()
{
    Token::Type tokens[] = { Token::Type::Star, Token::Type::Slash };
    Expression *expr = Unary();
    while (IsMatch<2>(tokens))
    {
        Token op = m_scanner.m_tokens[m_position++];
        Expression *right = Unary();
        expr = new Binary(expr, op.m_type, right);
    }
    return expr;
}

Wonin::Expression* Wonin::Parser::Unary()
{
    Token::Type tokens[] = { Token::Type::Minus };
    if (IsMatch<1>(tokens))
    {
        Token op = m_scanner.m_tokens[m_position++];
        Expression *right = Unary();
        return new Wonin::Unary(op.m_type, right);
    }
    return Primary();
}

Wonin::Expression* Wonin::Parser::Primary()
{
    if (IsMatch(Token::Type::NumberLiteral))
    {
        unsigned i = m_scanner.m_tokens[m_position++].m_position;
        return new Number(std::stod(&m_scanner.m_src[i]));
    }

    if (IsMatch(Token::Type::LeftParenthesis))
    {
        m_position++;
        Expression *expr = Expr();
        if (IsMatch(Token::Type::RightParenthesis))
        {
            m_position++;
            return expr;
        }
        else
        {
            Error("Expected ) after expression!");
        }
    }

    Error("Unknown symbol in expression!");
    return nullptr;
}
