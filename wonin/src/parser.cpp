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
bool Parser::isToken(Token const* tokens) const
{
    for (unsigned i = 0; i < N; i++)
        if (isToken(tokens[i]))
            return true;

    return false;
}

bool Parser::ifTokenConsumeIt(Token token)
{
    if (!isToken(token))
        return false;
    
    m_position++;
    return true;
}

void Parser::sync()
{
    Token tokens[] = {
        Token::Semicolon, Token::Class, Token::Fun, Token::Var, Token::For,
        Token::If, Token::While, Token::Print, Token::Return, Token::EoF
    };

    while (!isToken<sizeof(tokens) / sizeof(tokens[0])>(tokens))
        m_position++;
}

StmtPtr Parser::parseDecl()
{
    if (ifTokenConsumeIt(Token::Var))
        return parseVarDecl();

    return parseStmt();
}

StmtPtr Parser::parseVarDecl()
{
    Lexeme const& lexeme = m_lexer.m_lexemes[m_position++];
    if (lexeme.token() != Token::Identifier)
        throw RuntimeError(log::stringify(
            "Variable declaration is missing identifier at line ",
            m_lexer.lineNumber(lexeme)
        ));
    
    auto stmtVarDecl = std::make_unique<StmtVarDecl>(lexeme, ExprPtr{});
    if (ifTokenConsumeIt(Token::Equal))
        stmtVarDecl->m_initializer = parseExpr();

    if (!ifTokenConsumeIt(Token::Semicolon))
        throw RuntimeError(log::stringify(
            "Variable declaration statement is missing ; at line",
            m_lexer.lineNumber(m_lexer.m_lexemes[m_position])
        ));
    
    return stmtVarDecl;
}

StmtPtr Parser::parseStmt()
{
    if (ifTokenConsumeIt(Token::If))
        return parseStmtIf();

    if (ifTokenConsumeIt(Token::Print))
        return parseStmtPrint();

    if (ifTokenConsumeIt(Token::LeftBrace))
        return parseStmtBlock();
    
    if (ifTokenConsumeIt(Token::While))
        return parseStmtWhile();
    
    if (ifTokenConsumeIt(Token(Token::For)))
        return parseStmtFor();

    return parseStmtExpr();
}

StmtPtr Parser::parseStmtExpr()
{
    ExprPtr expr = parseExpr();
    if (ifTokenConsumeIt(Token::Semicolon))
        return std::make_unique<StmtExpr>(std::move(expr));

    throw RuntimeError(log::stringify(
        "Expression statement is missing ; at line ",
        m_lexer.lineNumber(m_lexer.m_lexemes[m_position])
    ));
}

StmtPtr Parser::parseStmtPrint()
{
    ExprPtr expr = parseExpr();
    if (ifTokenConsumeIt(Token::Semicolon))
        return std::make_unique<StmtPrint>(std::move(expr));

    throw RuntimeError(log::stringify(
        "Print statement is missing ; at line ",
        m_lexer.lineNumber(m_lexer.m_lexemes[m_position])
    ));
}

StmtPtr Parser::parseStmtBlock()
{
    std::vector<StmtPtr> stmts{};
    while (!(isEnd() || isToken(Token::RightBrace)))
        stmts.push_back(parseDecl());

    if (isEnd())
        throw RuntimeError("Expected } at the end of block at last line");
    
    if (!ifTokenConsumeIt(Token::RightBrace))
        throw RuntimeError(log::stringify(
            "Expected } at the end of block at line ",
            m_lexer.lineNumber(m_lexer.m_lexemes[m_position])
        ));

    return make_unique<StmtBlock>(std::move(stmts));
}

ExprPtr Parser::parseParenthesizedExpr(char const* where)
{
    if (!ifTokenConsumeIt(Token::LeftParen))
        throw RuntimeError(log::stringify(
            "Expected ( after", where , " at line ",
            m_lexer.lineNumber(m_lexer.m_lexemes[m_position])
        ));
    
    ExprPtr expr = parseExpr();
    
    if (!ifTokenConsumeIt(Token::RightParen))
        throw RuntimeError(log::stringify(
            "Expected ) after condition expression in", where, " statement at line ",
            m_lexer.lineNumber(m_lexer.m_lexemes[m_position])
        ));
    
    return expr;
}

StmtPtr Parser::parseStmtIf()
{
    ExprPtr expr = parseParenthesizedExpr("if");
    StmtPtr stmtThen = parseStmt();
    StmtPtr stmtElse = nullptr;
    if (ifTokenConsumeIt(Token::Else))
        stmtElse = std::move(parseStmt());

    return std::make_unique<StmtIf>(
        std::move(expr), std::move(stmtThen), std::move(stmtElse)
    );
}

StmtPtr Parser::parseStmtWhile()
{
    ExprPtr expr = parseParenthesizedExpr("while");
    StmtPtr stmt = parseStmt();

    return std::make_unique<StmtWhile>(std::move(expr), std::move(stmt));
}

StmtPtr Parser::parseStmtFor()
{
    if (!ifTokenConsumeIt(Token::LeftParen))
        throw RuntimeError(log::stringify(
            "Expected ( after for at line ",
            m_lexer.lineNumber(m_lexer.m_lexemes[m_position])
        ));
    
    StmtPtr initializer;
    if (ifTokenConsumeIt(Token::Semicolon))
        initializer = nullptr;
    else if (ifTokenConsumeIt(Token::Var))
        initializer = parseVarDecl();
    else
        initializer = parseStmtExpr();
    
    ExprPtr condition = nullptr;
    if (!isToken(Token::Semicolon))
        condition = parseExpr();
    
    if (!ifTokenConsumeIt(Token::Semicolon))
        throw RuntimeError(log::stringify(
            "Expected ; after for condition expression at line ",
            m_lexer.lineNumber(m_lexer.m_lexemes[m_position])
        ));
    
    ExprPtr increment = nullptr;
    if (!isToken(Token::RightParen))
        increment = parseExpr();
    
    if (!ifTokenConsumeIt(Token::RightParen))
        throw RuntimeError(log::stringify(
            "Expected ) after for increment expression at line ",
            m_lexer.lineNumber(m_lexer.m_lexemes[m_position])
        ));
    
    StmtPtr stmt = parseStmt();

    if (increment.get())
    {
        std::vector<StmtPtr> stmts{};
        stmts.push_back(std::move(stmt));
        stmts.push_back(std::make_unique<StmtExpr>(std::move(increment)));
        stmt = std::make_unique<StmtBlock>(std::move(stmts));
    }
    
    if (!condition.get())
        condition = std::make_unique<ExprLiteral>(true);
    
    stmt = std::make_unique<StmtWhile>(std::move(condition), std::move(stmt));

    if (initializer.get())
    {
        std::vector<StmtPtr> stmts{};
        stmts.push_back(std::move(initializer));
        stmts.push_back(std::move(stmt));
        stmt = std::make_unique<StmtBlock>(std::move(stmts));
    }

    return stmt;
}

ExprPtr Parser::parseExpr()
{
    return parseExprAsgn();
}

ExprPtr Parser::parseExprAsgn()
{
    ExprPtr expr = parseExprOr();

    if (isToken(Token::Equal))
    {
        Lexeme const& lexeme = m_lexer.m_lexemes[m_position++];
        ExprPtr value = parseExprAsgn();
        ExprVar *exprVar = dynamic_cast<ExprVar*>(expr.get());
        if (exprVar != nullptr)
        {
            return std::make_unique<ExprAsgn>(exprVar->m_lexeme, std::move(value));
        }

        throw RuntimeError(log::stringify(
            "Invalid assignment expression, left hand side is lvalue at line ",
            m_lexer.lineNumber(lexeme)
        ));
    }

    return expr;
}

ExprPtr Parser::parseExprOr()
{
    ExprPtr expr = parseExprAnd();
    while (isToken(Token::Or))
    {
        Lexeme const& lexeme = m_lexer.m_lexemes[m_position++];
        ExprPtr right = parseExprAnd();
        expr = std::make_unique<ExprLogical>(lexeme, std::move(expr), std::move(right));
    }
    return expr;
}

ExprPtr Parser::parseExprAnd()
{
    ExprPtr expr = parseExprEquality();
    while (isToken(Token::And))
    {
        Lexeme const& lexeme = m_lexer.m_lexemes[m_position++];
        ExprPtr right = parseExprEquality();
        expr = std::make_unique<ExprLogical>(lexeme, std::move(expr), std::move(right));
    }
    return expr;
}

ExprPtr Parser::parseExprEquality()
{
    ExprPtr expr = parseExprComparison();
    Token tokens[] = { Token::BangEqual, Token::EqualEqual };
    while (isToken<sizeof(tokens) / sizeof(tokens[0])>(tokens))
    {
        Lexeme const& lexeme = m_lexer.m_lexemes[m_position++];
        ExprPtr right = parseExprComparison();
        expr = std::make_unique<ExprBinary>(lexeme, std::move(expr), std::move(right));
    }
    return expr;
}

ExprPtr Parser::parseExprComparison()
{
    ExprPtr expr = parseExprTerm();
    Token tokens[] = { Token::Less, Token::LessEqual, Token::Greater, Token::GreaterEqual };
    while (isToken<sizeof(tokens) / sizeof(tokens[0])>(tokens))
    {
        Lexeme const& lexeme = m_lexer.m_lexemes[m_position++];
        ExprPtr right = parseExprTerm();
        expr = std::make_unique<ExprBinary>(lexeme, std::move(expr), std::move(right));
    }
    return expr;
}

ExprPtr Parser::parseExprTerm()
{
    ExprPtr expr = parseExprFactor();
    Token tokens[] = { Token::Plus, Token::Minus };
    while (isToken<sizeof(tokens) / sizeof(tokens[0])>(tokens))
    {
        Lexeme const& lexeme = m_lexer.m_lexemes[m_position++];
        ExprPtr right = parseExprFactor();
        expr = std::make_unique<ExprBinary>(lexeme, std::move(expr), std::move(right));
    }
    return expr;
}

ExprPtr Parser::parseExprFactor()
{
    ExprPtr expr = parseExprUnary();
    Token tokens[] = { Token::Star, Token::Slash };
    while (isToken<sizeof(tokens) / sizeof(tokens[0])>(tokens))
    {
        Lexeme const& lexeme = m_lexer.m_lexemes[m_position++];
        ExprPtr right = parseExprUnary();
        expr = std::make_unique<ExprBinary>(lexeme, std::move(expr), std::move(right));
    }
    return expr;
}

ExprPtr Parser::parseExprUnary()
{
    Token tokens[] = { Token::Minus, Token::Bang };
    if (isToken<sizeof(tokens) / sizeof(tokens[0])>(tokens))
    {
        Lexeme const& lexeme = m_lexer.m_lexemes[m_position++];
        ExprPtr right = parseExprUnary();
        return std::make_unique<ExprUnary>(lexeme, std::move(right));
    }
    return parseExprPrimary();
}

ExprPtr Parser::parseExprPrimary()
{
    if (isToken(Token::Number))
        return std::make_unique<ExprLiteral>(m_lexer.lexemeNumber(m_lexer.m_lexemes[m_position++]));

    if (isToken(Token::String))
        return std::make_unique<ExprLiteral>(m_lexer.lexemeString(m_lexer.m_lexemes[m_position++]));

    if (ifTokenConsumeIt(Token::False))
        return std::make_unique<ExprLiteral>(false);

    if (ifTokenConsumeIt(Token::True))
        return std::make_unique<ExprLiteral>(true);

    if (ifTokenConsumeIt(Token::Nil))
        return std::make_unique<ExprLiteral>(std::nullptr_t{});

    if (isToken(Token::Identifier))
        return std::make_unique<ExprVar>(m_lexer.m_lexemes[m_position++]);

    if (ifTokenConsumeIt(Token::LeftParen))
    {
        ExprPtr expr = parseExpr();
        if (ifTokenConsumeIt(Token::RightParen))
            return expr;
        else
            throw RuntimeError(log::stringify(
                "Expression is missing ) at line ",
                m_lexer.lineNumber(m_lexer.m_lexemes[m_position])
            ));
    }

    if (isEnd())
        throw RuntimeError("Nothing to parse");

    throw RuntimeError(log::stringify(
        "Invalid primary expression at line ",
        m_lexer.lineNumber(m_lexer.m_lexemes[m_position++])
    ));
}
