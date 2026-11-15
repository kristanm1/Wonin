#include "Parser.hpp"
#include "Log.hpp"

using namespace Wonin;

char const* ParserError::GetMessage() const
{
    switch (m_type)
    {

        case ParserError::Type::PrimNoMatch: return "Primary expression does not match";
        case ParserError::Type::PrimMissRightParen: return "Primary expression missing closing parantesis";
        case ParserError::Type::MissSemicolon: return "Expression is missing semicolon";
        case ParserError::Type::OnlyEofToken: return "Nothing to parse";
        default : return "Unknown error";
    }
}

Parser::Parser(Scanner const& scanner, std::vector<Token> const& tokens)
    : m_scanner(scanner)
    , m_tokens(tokens)
{}

ExprPtr Parser::Parse()
{
    m_position = 0;
    m_hasError = false;
    try
    {
        return ParseExpr();
    }
    catch (RuntimeError const& ex)
    {
        Log::Info("BANANA");
        Log::Error("Catch: ", ex.GetMessage());
        return nullptr;
    }
}

template<unsigned N>
bool Parser::IsMatch(Token::Type const* types)
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
    for (; !IsAtEnd(); m_position++)
    {
        switch (m_tokens[m_position].m_type)
        {
            case Token::Type::Semicolon:
            case Token::Type::Class:
            case Token::Type::Fun:
            case Token::Type::Var:
            case Token::Type::For:
            case Token::Type::If:
            case Token::Type::While:
            case Token::Type::Print:
            case Token::Type::Return:
                return;
        }
    }
}

/*
Statement* Parser::PrintStatement()
{
    return nullptr;
}

Statement* Parser::ExpressionStatement()
{
    ExprPtr expr = ParseExpr();
    if (IsMatch(Token::Token::Type::Semicolon))
    {
        m_position++;
        return StmtExpression(expr);
    }

    throw ParserError(ParserError::Type::PrimaryMissingRightParenthesis);
}

Statement* Parser::Statement()
{
    return ExpressionStatement();
}
*/

ExprPtr Parser::ParseExpr()
{
    return ParseEquality();
}

ExprPtr Parser::ParseEquality()
{
    Token::Type tokens[] = { Token::Type::BangEqual, Token::Type::EqualEqual };
    ExprPtr expr = ParseComparison();
    while (IsMatch<2>(tokens))
    {
        Token::Type op = m_tokens[m_position++].m_type;
        ExprPtr right = ParseComparison();
        expr = std::make_unique<ExprBinary>(std::move(expr), op, std::move(right));
    }
    return std::move(expr);
}

ExprPtr Parser::ParseComparison()
{
    ExprPtr expr = ParseTerm();
    Token::Type tokens[] = { Token::Type::Less, Token::Type::LessEqual, Token::Type::Greater, Token::Type::GreaterEqual };
    while (IsMatch<4>(tokens))
    {
        Token::Type op = m_tokens[m_position++].m_type;
        ExprPtr right = ParseTerm();
        expr = std::make_unique<ExprBinary>(std::move(expr), op, std::move(right));
    }
    return std::move(expr);
}

ExprPtr Parser::ParseTerm()
{
    Token::Type tokens[] = { Token::Type::Plus, Token::Type::Minus };
    ExprPtr expr = ParseFactor();
    while (IsMatch<2>(tokens))
    {
        Token::Type op = m_tokens[m_position++].m_type;
        ExprPtr right = ParseFactor();
        expr = std::make_unique<ExprBinary>(std::move(expr), op, std::move(right));
    }
    return std::move(expr);
}
ExprPtr Parser::ParseFactor()
{
    Token::Type tokens[] = { Token::Type::Star, Token::Type::Slash };
    ExprPtr expr = ParseUnary();
    while (IsMatch<2>(tokens))
    {
        Token::Type op = m_tokens[m_position++].m_type;
        ExprPtr right = ParseUnary();
        expr = std::make_unique<ExprBinary>(std::move(expr), op, std::move(right));
    }
    return std::move(expr);
}

ExprPtr Parser::ParseUnary()
{
    Token::Type tokens[] = { Token::Type::Minus };
    if (IsMatch<1>(tokens))
    {
        Token op = m_tokens[m_position++];
        ExprPtr right = ParseUnary();
        return std::make_unique<ExprUnary>(op.m_type, std::move(right));
    }
    return ParsePrimary();
}

ExprPtr Parser::ParsePrimary()
{
    if (IsMatch(Token::Type::Number))
    {
        Token const& token = m_tokens[m_position++];
        char const* begin = &m_scanner.m_source[token.m_position];
        char* end = const_cast<char*>(begin) + token.m_length;
        return std::make_unique<ExprNumber>(std::strtod(begin, &end));
    }

    if (IsMatch(Token::Type::LeftParen))
    {
        m_position++;
        ExprPtr expr = ParseExpr();
        if (IsMatch(Token::Type::RightParen))
        {
            m_position++;
            return std::move(expr);
        }
        else
        {
            throw ParserError(ParserError::Type::PrimMissRightParen);
        }
    }

    if (IsAtEnd())
        throw ParserError(ParserError::Type::OnlyEofToken);

    throw ParserError(ParserError::Type::PrimNoMatch);
}
