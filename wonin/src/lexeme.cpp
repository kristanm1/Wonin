#include "lexeme.hpp"

using namespace wonin;

cstr Lexeme::cstrToken(Token token)
{
    switch (token)
    {
        case Token::LeftParen:    return "left paren";
        case Token::RightParen:   return "right paren";
        case Token::LeftBrace:    return "left brace";
        case Token::RightBrace:   return "right brace";
        case Token::Comma:        return "comma";
        case Token::Dot:          return "dot";
        case Token::Minus:        return "minus";
        case Token::Plus:         return "plus";
        case Token::Slash:        return "slash";
        case Token::Star:         return "star";
        case Token::Semicolon:    return "semicolon";

        case Token::Bang:         return "bang";
        case Token::BangEqual:    return "bang equal";
        case Token::Equal:        return "equal";
        case Token::EqualEqual:   return "equal equal";
        case Token::Greater:      return "greater";
        case Token::GreaterEqual: return "greater equal";
        case Token::Less:         return "less";
        case Token::LessEqual:    return "less equal";

        case Token::Identifier:   return "identifier";
        case Token::Number:       return "number literal";
        case Token::String:       return "string literal";

        case Token::And:          return "and";
        case Token::Or:           return "or";
        case Token::If:           return "if";
        case Token::Else:         return "else";
        case Token::True:         return "true";
        case Token::False:        return "false";

        case Token::Class:        return "class";
        case Token::This:         return "this";
        case Token::Super:        return "super";
        case Token::Fun:          return "fun";
        case Token::Var:          return "var";
        case Token::For:          return "for";
        case Token::While:        return "while";
        case Token::Print:        return "print";
        case Token::Nil:          return "nill";
        case Token::Return:       return "return";
        
        case Token::EoF:          return "eof";

        default:                 return "unknown";
    }
}
