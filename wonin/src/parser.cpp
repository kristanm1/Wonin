#include "parser.hpp"
#include "log.hpp"

#include <sstream>

using namespace Wonin;
using Type = Token::Type;

std::string ParserError::GetMessage() const
{
    char const* cstr;
    switch (m_type)
    {
        case ParserError::Type::PrimNoMatch:
            cstr = "Primary expression does not match";
            break;
        case ParserError::Type::PrimMissRightParen:
            cstr = "Primary expression missing closing parantesis";
            break;
        case ParserError::Type::MissSemicolon:
            cstr = "Expression is missing semicolon";
            break;
        case ParserError::Type::OnlyEofToken:
            cstr = "Nothing to parse";
            break;
        default :
            cstr = "Unknown error";
            break;
    }
    std::stringstream ss{};
    ss << cstr << ": " << "at line " << m_token.GetLine() << " found token type " << m_token.ToString();
    return ss.str();
}

Parser::Parser(Scanner const& scanner, std::vector<Token> const& tokens)
    : m_scanner(scanner)
    , m_tokens(tokens)
{}

std::vector<StmtPtr> Parser::Parse()
{
    m_position = 0;
    m_hasError = false;
    std::vector<StmtPtr> stmts{};
    while (!IsAtEnd())
    {
        try
        {
            stmts.push_back(ParseStmt());
        }
        catch (RuntimeError const& ex)
        {
            Log::Error("Parser error: ", ex.GetMessage());
            Sync();
        }
    }
    return stmts;
}

template<unsigned N>
bool Parser::IsMatch(Type const* types)
{
    for (unsigned i = 0; i < N; i++)
    {
        if (IsMatch(types[i]))
        {
            return true;
        }
    }

    return false;
}

void Parser::Sync()
{
    Type tokens[] = {
        Type::Semicolon,
        Type::Class,
        Type::Fun,
        Type::Var,
        Type::For,
        Type::If,
        Type::While,
        Type::Print,
        Type::Return,
        Type::EoF,
    };
    for (; !IsMatch<9>(tokens); m_position++) {
        if (IsAtEnd())
            return;
    }
}

StmtPtr Parser::ParseStmt()
{
    if (IsMatch(Type::Print))
    {
        m_position++;
        return ParseStmtPrint();
    }

    return ParseStmtExpr();
}

StmtPtr Parser::ParseStmtExpr()
{
    ExprPtr expr = ParseExpr();
    if (IsMatch(Type::Semicolon))
    {
        m_position++;
        return std::make_unique<StmtExpr>(std::move(expr));
    }

    throw ParserError(ParserError::Type::MissSemicolon, m_tokens[m_position]);
}

StmtPtr Parser::ParseStmtPrint()
{
    ExprPtr expr = ParseExpr();
    if (IsMatch(Type::Semicolon))
    {
        m_position++;
        return std::make_unique<StmtPrint>(std::move(expr));
    }

    throw ParserError(ParserError::Type::MissSemicolon, m_tokens[m_position]);
}


ExprPtr Parser::ParseExpr()
{
    return ParseExprEquality();
}

ExprPtr Parser::ParseExprEquality()
{
    Type tokens[] = { Type::BangEqual, Type::EqualEqual };
    ExprPtr expr = ParseExprComparison();
    while (IsMatch<2>(tokens))
    {
        Token const& token = m_tokens[m_position++];
        ExprPtr right = ParseExprComparison();
        expr = std::make_unique<ExprBinary>(std::move(expr), token, std::move(right));
    }
    return std::move(expr);
}

ExprPtr Parser::ParseExprComparison()
{
    ExprPtr expr = ParseExprTerm();
    Type tokens[] = { Type::Less, Type::LessEqual, Type::Greater, Type::GreaterEqual };
    while (IsMatch<4>(tokens))
    {
        Token const& token = m_tokens[m_position++];
        ExprPtr right = ParseExprTerm();
        expr = std::make_unique<ExprBinary>(std::move(expr), token, std::move(right));
    }
    return std::move(expr);
}

ExprPtr Parser::ParseExprTerm()
{
    Type tokens[] = { Type::Plus, Type::Minus };
    ExprPtr expr = ParseExprFactor();
    while (IsMatch<2>(tokens))
    {
        Token const& token = m_tokens[m_position++];
        ExprPtr right = ParseExprFactor();
        expr = std::make_unique<ExprBinary>(std::move(expr), token, std::move(right));
    }
    return std::move(expr);
}
ExprPtr Parser::ParseExprFactor()
{
    Type tokens[] = { Type::Star, Type::Slash };
    ExprPtr expr = ParseExprUnary();
    while (IsMatch<2>(tokens))
    {
        Token const& token = m_tokens[m_position++];
        ExprPtr right = ParseExprUnary();
        expr = std::make_unique<ExprBinary>(std::move(expr), token, std::move(right));
    }
    return std::move(expr);
}

ExprPtr Parser::ParseExprUnary()
{
    Type tokens[] = { Type::Minus };
    if (IsMatch<1>(tokens))
    {
        Token const& token = m_tokens[m_position++];
        ExprPtr right = ParseExprUnary();
        return std::make_unique<ExprUnary>(token, std::move(right));
    }
    return ParseExprPrimary();
}

ExprPtr Parser::ParseExprPrimary()
{
    if (IsMatch(Type::Number))
    {
        Token const& token = m_tokens[m_position++];
        char const* begin = &m_scanner.m_source[token.GetPositios()];
        char* end = const_cast<char*>(begin) + token.GetLength();
        return std::make_unique<ExprLiteral>(std::strtod(begin, &end));
    }

    if (IsMatch(Type::String))
    {
        Token const& token = m_tokens[m_position++];
        return std::make_unique<ExprLiteral>(std::string(&m_scanner.m_source[token.GetPositios()], token.GetLength()));
    }

    if (IsMatch(Type::LeftParen))
    {
        m_position++;
        ExprPtr expr = ParseExpr();
        if (IsMatch(Type::RightParen))
        {
            m_position++;
            return std::move(expr);
        }
        else
        {
            throw ParserError(ParserError::Type::PrimMissRightParen, m_tokens[m_position]);
        }
    }

    if (IsAtEnd())
        throw ParserError(ParserError::Type::OnlyEofToken, m_tokens[m_position]);

    throw ParserError(ParserError::Type::PrimNoMatch, m_tokens[m_position]);
}
