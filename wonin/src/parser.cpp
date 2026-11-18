#include "parser.hpp"
#include "log.hpp"

#include <sstream>

using namespace Wonin;
using Type = Token::Type;

std::string ParserError::GetMessage() const
{
    using Type = ParserError::Type;

    char const* cstr;
    switch (m_type)
    {
        case Type::PrimNoMatch:
            cstr = "Primary expression does not match";
            break;
        case Type::PrimMissRightParen:
            cstr = "Primary expression missing closing parantesis";
            break;
        case Type::MissSemicolon:
            cstr = "Expression is missing semicolon";
            break;
        case Type::OnlyEofToken:
            cstr = "Nothing to parse";
            break;
        case Type::DeclIdMiss:
            cstr = "Declaration is missing identifier";
            break;
        default :
            cstr = "Unknown error";
            break;
    }
    std::stringstream ss{};
    ss << cstr << ": " << "at line " << m_token.GetLine() << " found token type " << m_token.ToString();
    return ss.str();
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
    };
    for (; !IsMatch<9>(tokens); m_position++) {
        if (IsAtEnd())
            return;
    }
}

bool Parser::Parse()
{
    m_position = 0;
    m_hasError = false;
    while (!IsAtEnd())
    {
        try
        {
            m_stmts.push_back(ParseDecl());
        }
        catch (RuntimeError const& ex)
        {
            Log::Error("Parser error: ", ex.GetMessage());
            Sync();
        }
    }
    return !m_hasError;
}

StmtPtr Parser::ParseDecl()
{
    if (IsMatch(Type::Var))
    {
        m_position++;
        return ParseVarDecl();
    }

    return ParseStmt();
}

StmtPtr Parser::ParseVarDecl()
{
    Token const& token = m_scanner.GetToken(m_position);
    if (token.GetType() != Type::Identifier)
        throw ParserError(ParserError::Type::DeclIdMiss, token);
    
    auto stmtVarDecl = std::make_unique<StmtVarDecl>(token, ExprPtr{});
    if (m_scanner.IsTokenMatch(Type::Equal, ++m_position))
    {
        m_position++;
        stmtVarDecl->m_expr = ParseExpr();
    }

    if (!IsMatch(Type::Semicolon))
        throw ParserError(ParserError::Type::MissSemicolon, m_scanner.GetToken(m_position));
    
    m_position++;
    return stmtVarDecl;
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

    throw ParserError(ParserError::Type::MissSemicolon, m_scanner.GetToken(m_position));
}

StmtPtr Parser::ParseStmtPrint()
{
    ExprPtr expr = ParseExpr();
    if (IsMatch(Type::Semicolon))
    {
        m_position++;
        return std::make_unique<StmtPrint>(std::move(expr));
    }

    throw ParserError(ParserError::Type::MissSemicolon, m_scanner.GetToken(m_position));
}


ExprPtr Parser::ParseExpr()
{
    return ParseExprEquality();
}

ExprPtr Parser::ParseExprEquality()
{
    ExprPtr expr = ParseExprComparison();
    Type tokens[] = { Type::BangEqual, Type::EqualEqual };
    while (IsMatch<2>(tokens))
    {
        Token const& token = m_scanner.GetToken(m_position++);
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
        Token const& token = m_scanner.GetToken(m_position++);
        ExprPtr right = ParseExprTerm();
        expr = std::make_unique<ExprBinary>(std::move(expr), token, std::move(right));
    }
    return std::move(expr);
}

ExprPtr Parser::ParseExprTerm()
{
    ExprPtr expr = ParseExprFactor();
    Type tokens[] = { Type::Plus, Type::Minus };
    while (IsMatch<2>(tokens))
    {
        Token const& token = m_scanner.GetToken(m_position++);
        ExprPtr right = ParseExprFactor();
        expr = std::make_unique<ExprBinary>(std::move(expr), token, std::move(right));
    }
    return std::move(expr);
}
ExprPtr Parser::ParseExprFactor()
{
    ExprPtr expr = ParseExprUnary();
    Type tokens[] = { Type::Star, Type::Slash };
    while (IsMatch<2>(tokens))
    {
        Token const& token = m_scanner.GetToken(m_position++);
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
        Token const& token = m_scanner.GetToken(m_position++);
        ExprPtr right = ParseExprUnary();
        return std::make_unique<ExprUnary>(token, std::move(right));
    }
    return ParseExprPrimary();
}

ExprPtr Parser::ParseExprPrimary()
{
    if (IsMatch(Type::Number))
    {
        Token const& token = m_scanner.GetToken(m_position++);
        char const* begin = &m_scanner.m_source[token.GetPositios()];
        char* end = const_cast<char*>(begin) + token.GetLength();
        return std::make_unique<ExprLiteral>(std::strtod(begin, &end));
    }

    if (IsMatch(Type::String))
    {
        Token const& token = m_scanner.GetToken(m_position++);
        return std::make_unique<ExprLiteral>(std::string(&m_scanner.m_source[token.GetPositios()], token.GetLength()));
    }

    if (IsMatch(Type::False))
    {
        m_position++;
        return std::make_unique<ExprLiteral>(false);
    }

    if (IsMatch(Type::True))
    {
        m_position++;
        return std::make_unique<ExprLiteral>(true);
    }

    if (IsMatch(Type::Nil))
    {
        m_position++;
        return std::make_unique<ExprLiteral>(std::nullptr_t{});
    }

    if (IsMatch(Type::Identifier))
        return std::make_unique<ExprVar>(m_scanner.GetToken(m_position++));

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
            throw ParserError(ParserError::Type::PrimMissRightParen, m_scanner.GetToken(m_position));
        }
    }

    if (IsAtEnd())
        throw ParserError(ParserError::Type::OnlyEofToken, m_scanner.GetToken(m_position));

    throw ParserError(ParserError::Type::PrimNoMatch, m_scanner.GetToken(m_position++));
}
