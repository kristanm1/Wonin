#include "parser.hpp"
#include "log.hpp"

#include <sstream>
#include <vector>

using namespace wonin;

bool Parser::parse()
{
    m_stmts.clear();
    m_position = 0;
    m_hasError = false;
    while (!isEnd())
    {
        try
        {
            m_stmts.push_back(parseDecl());
        }
        catch (RuntimeError const& err)
        {
            m_hasError = true;
            log::error("Parser error: ", err.what());

            sync();
        }
    }
    return !m_hasError;
}

template<unsigned N>
bool Parser::isToken(Token const* tokens)
{
    for (unsigned i = 0; i < N; i++)
        if (isToken(tokens[i]))
            return true;

    return false;
}

void Parser::sync()
{
    Token tokens[] = {
        Token::Semicolon, Token::Class, Token::Fun, Token::Var, Token::For,
        Token::If, Token::While, Token::Print, Token::Return,
    };

    while (!isEnd())
    {
        if (!isToken<9>(tokens))
            return;
        
        m_position++;
    }
}

StmtPtr Parser::parseDecl()
{
    if (isToken(Token::Var))
    {
        m_position++;
        return parseVarDecl();
    }

    return parseStmt();
}

StmtPtr Parser::parseVarDecl()
{
    Lexeme const& lexeme = m_scanner.m_lexemes[m_position++];
    if (lexeme.token() != Token::Identifier)
        throw RuntimeError(log::stringify(
            "Variable declaration is missing identifier at line ",
            m_scanner.lineNumber(lexeme)
        ));
    
    auto stmtVarDecl = std::make_unique<StmtVarDecl>(lexeme, ExprPtr{});
    if (m_scanner.m_lexemes[m_position].token() == Token::Equal)
    {
        m_position++;
        stmtVarDecl->m_expr = parseExpr();
    }

    if (!isToken(Token::Semicolon))
        throw RuntimeError(log::stringify(
            "Variable declaration statement is missing ; at line",
            m_scanner.lineNumber(m_scanner.m_lexemes[m_position])
        ));
    
    m_position++;
    return stmtVarDecl;
}

StmtPtr Parser::parseStmt()
{
    if (isToken(Token::Print))
    {
        m_position++;
        return parseStmtPrint();
    }

    if (isToken(Token::LeftBrace))
    {
        m_position++;
        return parseStmtBlock();
    }

    return parseStmtExpr();
}

StmtPtr Parser::parseStmtExpr()
{
    ExprPtr expr = parseExpr();
    if (isToken(Token::Semicolon))
    {
        m_position++;
        return std::make_unique<StmtExpr>(std::move(expr));
    }

    throw RuntimeError(log::stringify(
        "Expression statement is missing ; at line ",
        m_scanner.lineNumber(m_scanner.m_lexemes[m_position])
    ));
}

StmtPtr Parser::parseStmtPrint()
{
    ExprPtr expr = parseExpr();
    if (isToken(Token::Semicolon))
    {
        m_position++;
        return std::make_unique<StmtPrint>(std::move(expr));
    }

    throw RuntimeError(log::stringify(
        "Print statement is missing ; at line ",
        m_scanner.lineNumber(m_scanner.m_lexemes[m_position])
    ));
}

StmtPtr Parser::parseStmtBlock()
{
    std::vector<StmtPtr> stmts{};
    while (!(isEnd() || isToken(Token::RightBrace)))
    {
        stmts.push_back(parseDecl());
    }

    if (isEnd())
        throw RuntimeError("Expected } at the end of block at last line");
    
    if (!isToken(Token::RightBrace))
        throw RuntimeError(log::stringify(
            "Expected } at the end of block at line ",
            m_scanner.lineNumber(m_scanner.m_lexemes[m_position])
        ));

    m_position++;
    return make_unique<StmtBlock>(std::move(stmts));
}

ExprPtr Parser::parseExpr()
{
    return parseExprAsgn();
}

ExprPtr Parser::parseExprAsgn()
{
    ExprPtr expr = parseExprEquality();

    if (isToken(Token::Equal))
    {
        Lexeme const& lexeme = m_scanner.m_lexemes[m_position++];
        ExprPtr value = parseExprAsgn();
        ExprVar *exprVar = dynamic_cast<ExprVar*>(expr.get());
        if (exprVar != nullptr)
        {
            return std::make_unique<ExprAsgn>(exprVar->m_lexeme, std::move(value));
        }

        throw RuntimeError(log::stringify(
            "Invalid assignment expression, left hand side does not evaluate into identifier at line ",
            m_scanner.lineNumber(lexeme)
        ));
    }

    return expr;
}

ExprPtr Parser::parseExprEquality()
{
    ExprPtr expr = parseExprComparison();
    Token tokens[] = { Token::BangEqual, Token::EqualEqual };
    while (isToken<2>(tokens))
    {
        Lexeme const& lexeme = m_scanner.m_lexemes[m_position++];
        ExprPtr right = parseExprComparison();
        expr = std::make_unique<ExprBinary>(std::move(expr), lexeme, std::move(right));
    }
    return std::move(expr);
}

ExprPtr Parser::parseExprComparison()
{
    ExprPtr expr = parseExprTerm();
    Token tokens[] = { Token::Less, Token::LessEqual, Token::Greater, Token::GreaterEqual };
    while (isToken<4>(tokens))
    {
        Lexeme const& lexeme = m_scanner.m_lexemes[m_position++];
        ExprPtr right = parseExprTerm();
        expr = std::make_unique<ExprBinary>(std::move(expr), lexeme, std::move(right));
    }
    return std::move(expr);
}

ExprPtr Parser::parseExprTerm()
{
    ExprPtr expr = parseExprFactor();
    Token tokens[] = { Token::Plus, Token::Minus };
    while (isToken<2>(tokens))
    {
        Lexeme const& lexeme = m_scanner.m_lexemes[m_position++];
        ExprPtr right = parseExprFactor();
        expr = std::make_unique<ExprBinary>(std::move(expr), lexeme, std::move(right));
    }
    return std::move(expr);
}

ExprPtr Parser::parseExprFactor()
{
    ExprPtr expr = parseExprUnary();
    Token tokens[] = { Token::Star, Token::Slash };
    while (isToken<2>(tokens))
    {
        Lexeme const& lexeme = m_scanner.m_lexemes[m_position++];
        ExprPtr right = parseExprUnary();
        expr = std::make_unique<ExprBinary>(std::move(expr), lexeme, std::move(right));
    }
    return std::move(expr);
}

ExprPtr Parser::parseExprUnary()
{
    if (isToken(Token::Minus))
    {
        Lexeme const& lexeme = m_scanner.m_lexemes[m_position++];
        ExprPtr right = parseExprUnary();
        return std::make_unique<ExprUnary>(lexeme, std::move(right));
    }
    return parseExprPrimary();
}

ExprPtr Parser::parseExprPrimary()
{
    if (isToken(Token::Number))
        return std::make_unique<ExprLiteral>(m_scanner.lexemeNumber(m_scanner.m_lexemes[m_position++]));

    if (isToken(Token::String))
        return std::make_unique<ExprLiteral>(m_scanner.lexemeString(m_scanner.m_lexemes[m_position++]));

    if (isToken(Token::False))
    {
        m_position++;
        return std::make_unique<ExprLiteral>(false);
    }

    if (isToken(Token::True))
    {
        m_position++;
        return std::make_unique<ExprLiteral>(true);
    }

    if (isToken(Token::Nil))
    {
        m_position++;
        return std::make_unique<ExprLiteral>(std::nullptr_t{});
    }

    if (isToken(Token::Identifier))
        return std::make_unique<ExprVar>(m_scanner.m_lexemes[m_position++]);

    if (isToken(Token::LeftParen))
    {
        m_position++;
        ExprPtr expr = parseExpr();
        if (isToken(Token::RightParen))
        {
            m_position++;
            return std::move(expr);
        }
        else
        {
            throw RuntimeError(log::stringify(
                "Expression is missing ) at line ",
                m_scanner.lineNumber(m_scanner.m_lexemes[m_position])
            ));
        }
    }

    if (isEnd())
        throw RuntimeError("Nothing to parse");

    throw RuntimeError(log::stringify(
        "Invalid primary expression at line ",
        m_scanner.lineNumber(m_scanner.m_lexemes[m_position++])
    ));
}
