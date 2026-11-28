#include "parser.hpp"
#include "log.hpp"
#include "expr.hpp"
#include "stmt.hpp"

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
bool Parser::ifTokenConsumeIt(Token const* tokens)
{
    for (unsigned i = 0; i < N; i++)
        if (ifTokenConsumeIt(tokens[i]))
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
        Token::If, Token::While, Token::Print, Token::Return
    };

    while (!isEnd() && !ifTokenConsumeIt<sizeof(tokens) / sizeof(tokens[0])>(tokens)) {}
}

stmtptr Parser::parseDecl()
{
    if (ifTokenConsumeIt(Token::Var))
        return parseVarDecl();
    
    if (ifTokenConsumeIt(Token::Fun))
        return parseFunDecl();

    return parseStmt();
}

stmtptr Parser::parseVarDecl()
{
    if (!ifTokenConsumeIt(Token::Identifier))
        throw RuntimeError(log::stringify(
            "Variable declaration is missing identifier at line ",
            m_lexer.lineNumber(current())
        ));
    
    Lexeme const& lexeme = previous();
    exprptr initializer = nullptr;
    if (ifTokenConsumeIt(Token::Equal))
        initializer = parseExpr();

    stmtptr stmtVarDecl = newptr<StmtVarDecl>(
        m_lexer.lexemeString(lexeme),
        std::move(initializer),
        m_lexer.lineNumber(previous())
    );

    if (!ifTokenConsumeIt(Token::Semicolon))
        throw RuntimeError(log::stringify(
            "Variable declaration is missing ; at line",
            m_lexer.lineNumber(current())
        ));
    
    return stmtVarDecl;
}

stmtptr Parser::parseFunDecl()
{
    Lexeme const& lexeme = current();
    if (!ifTokenConsumeIt(Token::Identifier))
        throw RuntimeError(log::stringify(
            "Function declaration is missing identifier at line ",
            m_lexer.lineNumber(lexeme)
        ));
    
    if (!ifTokenConsumeIt(Token::LeftParen))
        throw RuntimeError(log::stringify(
            "Function declaration is missing ( at line",
            m_lexer.lineNumber(current())
        ));
    
    std::vector<str> parameters{};
    if (!isToken(Token::RightParen))
    {
        do
        {
            if (parameters.size() > 255)
                log::error(
                    "Function declaration has more then 255 arguments at line ",
                    m_lexer.lineNumber(current())
                );
            
            if (!ifTokenConsumeIt(Token::Identifier))
                throw RuntimeError(log::stringify(
                    "Invalid function parameter expected identifier at line ",
                    m_lexer.lineNumber(current())
                ));
            
            parameters.push_back(m_lexer.lexemeString(previous()));
            
        } while (ifTokenConsumeIt(Token::Comma));
    }

    if (!ifTokenConsumeIt(Token::RightParen))
        throw RuntimeError(log::stringify(
            "Function declaration is missing ) at line",
            m_lexer.lineNumber(current())
        ));
    
    if (!ifTokenConsumeIt(Token::LeftBrace))
        throw RuntimeError(log::stringify(
            "Function declaration is missing { before body at line",
            m_lexer.lineNumber(current())
        ));

    return newptr<StmtFunDecl>(
        m_lexer.lexemeString(lexeme),
        std::move(parameters),
        std::move(static_cast<StmtBlock*>(parseStmtBlock().get())->m_stmts),
        m_lexer.lineNumber(previous())
    );
}

stmtptr Parser::parseStmt()
{
    if (ifTokenConsumeIt(Token::If))
        return parseStmtIf();

    if (ifTokenConsumeIt(Token::Print))
        return parseStmtPrint();
    
    if (ifTokenConsumeIt(Token::Return))
        return parseStmtReturn();

    if (ifTokenConsumeIt(Token::LeftBrace))
        return parseStmtBlock();
    
    if (ifTokenConsumeIt(Token::While))
        return parseStmtWhile();
    
    if (ifTokenConsumeIt(Token(Token::For)))
        return parseStmtFor();

    return parseStmtExpr();
}

stmtptr Parser::parseStmtExpr()
{
    exprptr expr = parseExpr();
    if (ifTokenConsumeIt(Token::Semicolon))
        return newptr<StmtExpr>(
            std::move(expr),
            m_lexer.lineNumber(previous())
        );

    throw RuntimeError(log::stringify(
        "Expression statement is missing ; at line ",
        m_lexer.lineNumber(current())
    ));
}

stmtptr Parser::parseStmtPrint()
{
    exprptr expr = parseExpr();
    if (ifTokenConsumeIt(Token::Semicolon))
        return newptr<StmtPrint>(
            std::move(expr),
            m_lexer.lineNumber(previous())
        );

    throw RuntimeError(log::stringify(
        "Print statement is missing ; at line ",
        m_lexer.lineNumber(current())
    ));
}

stmtptr Parser::parseStmtReturn()
{
    exprptr expr = nullptr;
    if (!isToken(Token::Semicolon))
        expr = parseExpr();
    
    if (!ifTokenConsumeIt(Token::Semicolon))
        throw RuntimeError(log::stringify(
            "Return statement is missing ; at line ",
            m_lexer.lineNumber(current())
        ));
    
    return newptr<StmtReturn>(
        std::move(expr),
        m_lexer.lineNumber(previous())
    );
}

stmtptr Parser::parseStmtBlock()
{
    std::vector<stmtptr> stmts{};
    while (!(isEnd() || isToken(Token::RightBrace)))
        stmts.push_back(parseDecl());

    if (isEnd())
        throw RuntimeError("Expected } at the end of block at last line");
    
    if (!ifTokenConsumeIt(Token::RightBrace))
        throw RuntimeError(log::stringify(
            "Expected } at the end of block at line ",
            m_lexer.lineNumber(current())
        ));

    return newptr<StmtBlock>(
        std::move(stmts),
        m_lexer.lineNumber(previous())
    );
}

exprptr Parser::parseParenthesizedExpr(char const* where)
{
    if (!ifTokenConsumeIt(Token::LeftParen))
        throw RuntimeError(log::stringify(
            "Expected ( after ", where , " at line ",
            m_lexer.lineNumber(current())
        ));
    
    exprptr expr = parseExpr();
    
    if (!ifTokenConsumeIt(Token::RightParen))
        throw RuntimeError(log::stringify(
            "Expected ) after condition expression in ", where, " statement at line ",
            m_lexer.lineNumber(current())
        ));
    
    return expr;
}

stmtptr Parser::parseStmtIf()
{
    exprptr expr = parseParenthesizedExpr("if");
    stmtptr stmtThen = parseStmt();
    stmtptr stmtElse = nullptr;
    if (ifTokenConsumeIt(Token::Else))
        stmtElse = parseStmt();

    return newptr<StmtIf>(
        std::move(expr),
        std::move(stmtThen),
        std::move(stmtElse),
        m_lexer.lineNumber(previous())
    );
}

stmtptr Parser::parseStmtWhile()
{
    return newptr<StmtWhile>(
        parseParenthesizedExpr("while"),
        parseStmt(),
        m_lexer.lineNumber(previous())
    );
}

stmtptr Parser::parseStmtFor()
{
    if (!ifTokenConsumeIt(Token::LeftParen))
        throw RuntimeError(log::stringify(
            "Expected ( after for at line ",
            m_lexer.lineNumber(current())
        ));
    
    stmtptr initializer;
    if (ifTokenConsumeIt(Token::Semicolon))
        initializer = nullptr;
    else if (ifTokenConsumeIt(Token::Var))
        initializer = parseVarDecl();
    else
        initializer = parseStmtExpr();
    
    exprptr condition = nullptr;
    if (!isToken(Token::Semicolon))
        condition = parseExpr();
    
    if (!ifTokenConsumeIt(Token::Semicolon))
        throw RuntimeError(log::stringify(
            "Expected ; after for condition expression at line ",
            m_lexer.lineNumber(current())
        ));
    
    exprptr increment = nullptr;
    if (!isToken(Token::RightParen))
        increment = parseExpr();
    
    if (!ifTokenConsumeIt(Token::RightParen))
        throw RuntimeError(log::stringify(
            "Expected ) after for increment expression at line ",
            m_lexer.lineNumber(current())
        ));
    
    stmtptr stmt = parseStmt();

    if (increment.get())
    {
        std::vector<stmtptr> stmts{};
        stmts.push_back(std::move(stmt));
        stmts.push_back(newptr<StmtExpr>(
            std::move(increment),
            increment->lineNumber()
        ));
        stmt = newptr<StmtBlock>(std::move(stmts), -1);
    }
    
    if (!condition.get())
        condition = newptr<ExprLiteral>(true, -1);
    
    stmt = newptr<StmtWhile>(
        std::move(condition), std::move(stmt), -1
    );

    if (initializer.get())
    {
        std::vector<stmtptr> stmts{};
        stmts.push_back(std::move(initializer));
        stmts.push_back(std::move(stmt));
        stmt = newptr<StmtBlock>(std::move(stmts), -1);
    }

    return stmt;
}

exprptr Parser::parseExpr()
{
    return parseExprAsgn();
}

exprptr Parser::parseExprAsgn()
{
    exprptr expr = parseExprOr();
    if (ifTokenConsumeIt(Token::Equal))
    {
        Lexeme const& lexeme = previous();
        auto *exprVar = dynamic_cast<ExprVariable*>(expr.get());
        if (exprVar != nullptr)
            return newptr<ExprAsgn>(
                std::move(exprVar->m_name),
                parseExprAsgn(),
                m_lexer.lineNumber(lexeme)
            );

        throw RuntimeError(log::stringify(
            "Invalid assignment expression, left hand side is lvalue at line ",
            m_lexer.lineNumber(lexeme)
        ));
    }

    return expr;
}

exprptr Parser::parseExprOr()
{
    exprptr expr = parseExprAnd();
    while (ifTokenConsumeIt(Token::Or))
    {
        Lexeme const& op = previous();
        expr = newptr<ExprLogical>(
            op.token(),
            std::move(expr),
            parseExprAnd(),
            m_lexer.lineNumber(op)
        );
    }

    return expr;
}

exprptr Parser::parseExprAnd()
{
    exprptr expr = parseExprEquality();
    while (ifTokenConsumeIt(Token::And))
    {
        Lexeme const& op = previous();
        expr = newptr<ExprLogical>(
            op.token(),
            std::move(expr),
            parseExprEquality(),
            m_lexer.lineNumber(op)
        );
    }

    return expr;
}

exprptr Parser::parseExprEquality()
{
    exprptr expr = parseExprComparison();
    Token tokens[] = { Token::BangEqual, Token::EqualEqual };
    while (ifTokenConsumeIt<sizeof(tokens) / sizeof(tokens[0])>(tokens))
    {
        Lexeme const& lexeme = previous();
        expr = newptr<ExprBinary>(
            lexeme.token(),
            std::move(expr),
            parseExprComparison(),
            m_lexer.lineNumber(lexeme)
        );
    }

    return expr;
}

exprptr Parser::parseExprComparison()
{
    exprptr expr = parseExprTerm();
    Token tokens[] = { Token::Less, Token::LessEqual, Token::Greater, Token::GreaterEqual };
    while (ifTokenConsumeIt<sizeof(tokens) / sizeof(tokens[0])>(tokens))
    {
        Lexeme const& lexeme = previous();
        expr = newptr<ExprBinary>(
            lexeme.token(),
            std::move(expr),
            parseExprTerm(),
            m_lexer.lineNumber(lexeme)
        );
    }
    return expr;
}

exprptr Parser::parseExprTerm()
{
    exprptr expr = parseExprFactor();
    Token tokens[] = { Token::Plus, Token::Minus };
    while (ifTokenConsumeIt<sizeof(tokens) / sizeof(tokens[0])>(tokens))
    {
        Lexeme const& lexeme = previous();
        expr = newptr<ExprBinary>(
            lexeme.token(),
            std::move(expr),
            parseExprFactor(),
            m_lexer.lineNumber(lexeme)
        );
    }

    return expr;
}

exprptr Parser::parseExprFactor()
{
    exprptr expr = parseExprUnary();
    Token tokens[] = { Token::Star, Token::Slash };
    while (ifTokenConsumeIt<sizeof(tokens) / sizeof(tokens[0])>(tokens))
    {
        Lexeme const& lexeme = previous();
        expr = newptr<ExprBinary>(
            lexeme.token(),
            std::move(expr),
            parseExprUnary(),
            m_lexer.lineNumber(lexeme)
        );
    }

    return expr;
}

exprptr Parser::parseExprUnary()
{
    Token tokens[] = { Token::Minus, Token::Bang };
    if (ifTokenConsumeIt<sizeof(tokens) / sizeof(tokens[0])>(tokens))
    {
        Lexeme const& lexeme = previous();
        return newptr<ExprUnary>(
            lexeme.token(),
            parseExprUnary(),
            m_lexer.lineNumber(lexeme)
        );
    }
    
    return parseExprCall();
}

exprptr Parser::parseExprCall()
{
    exprptr expr = parseExprPrimary();
    while (true)
        if (ifTokenConsumeIt(Token::LeftParen))
            expr = parseCallArguments(std::move(expr));
        else
            break;

    return expr;
}

exprptr Parser::parseCallArguments(exprptr&& expr)
{
    std::vector<exprptr> arguments{};
    if (!isToken(Token::RightParen))
    {
        do
        {
            if (arguments.size() > 255) // todo: check >255 if ok??
                log::error(
                    "Function call has more then 255 arguments at line ",
                    m_lexer.lineNumber(current())
                );

            arguments.push_back(parseExpr());
        } while(ifTokenConsumeIt(Token::Comma));
    }

    if (!ifTokenConsumeIt(Token::RightParen))
        throw RuntimeError(log::stringify(
            "Missing ) after function call at line ",
            m_lexer.lineNumber(current())
        ));

    return newptr<ExprCall>(
        std::move(expr),
        std::move(arguments),
        m_lexer.lineNumber(previous())
    );
}

exprptr Parser::parseExprPrimary()
{
    if (ifTokenConsumeIt(Token::Number))
        return newptr<ExprLiteral>(
            m_lexer.lexemeNumber(previous()),
            m_lexer.lineNumber(previous())
        );

    if (ifTokenConsumeIt(Token::String))
        return newptr<ExprLiteral>(
            m_lexer.lexemeString(previous()),
            m_lexer.lineNumber(previous())
        );

    if (ifTokenConsumeIt(Token::False))
        return newptr<ExprLiteral>(false, m_lexer.lineNumber(previous()));

    if (ifTokenConsumeIt(Token::True))
        return newptr<ExprLiteral>(true, m_lexer.lineNumber(previous()));

    if (ifTokenConsumeIt(Token::Nil))
        return newptr<ExprLiteral>(std::nullptr_t{}, m_lexer.lineNumber(previous()));

    if (ifTokenConsumeIt(Token::Identifier))
        return newptr<ExprVariable>(
            m_lexer.lexemeString(previous()),
            m_lexer.lineNumber(previous())
        );

    if (ifTokenConsumeIt(Token::LeftParen))
    {
        exprptr expr = parseExpr();
        if (ifTokenConsumeIt(Token::RightParen))
            return expr;
        else
            throw RuntimeError(log::stringify(
                "Expression is missing ) at line ",
                m_lexer.lineNumber(current())
            ));
    }

    if (isEnd())
        throw RuntimeError("Nothing to parse");

    throw RuntimeError(log::stringify(
        "Invalid primary expression at line ",
        m_lexer.lineNumber(m_lexer.m_lexemes[m_position++])
    ));
}
