#include "Token.hpp"

Wonin::Token::Token(usize position, usize line, usize length, Type type)
    : m_type(type)
    , m_position(position)
    , m_length(length)
    , m_line(line)
{}

char const* Wonin::Token::ToString(Type type)
{
    switch (type)
    {
        case Type::LeftParen:    return "left paren";
        case Type::RightParen:   return "right paren";
        case Type::LeftBrace:    return "left brace";
        case Type::RightBrace:   return "right brace";
        case Type::Comma:        return "comma";
        case Type::Dot:          return "dot";
        case Type::Minus:        return "minus";
        case Type::Plus:         return "plus";
        case Type::Slash:        return "slash";
        case Type::Star:         return "star";
        case Type::Semicolon:    return "semicolon";

        case Type::Bang:         return "bang";
        case Type::BangEqual:    return "bang equal";
        case Type::Equal:        return "equal";
        case Type::EqualEqual:   return "equal equal";
        case Type::Greater:      return "greater";
        case Type::GreaterEqual: return "greater equal";
        case Type::Less:         return "less";
        case Type::LessEqual:    return "less equal";

        case Type::Identifier:   return "identifier";
        case Type::Number:       return "number literal";
        case Type::String:       return "string literal";

        case Type::And:          return "and";
        case Type::Or:           return "or";
        case Type::If:           return "if";
        case Type::Else:         return "else";
        case Type::True:         return "true";
        case Type::False:        return "false";

        case Type::Class:        return "class";
        case Type::This:         return "this";
        case Type::Super:        return "super";
        case Type::Fun:          return "fun";
        case Type::Var:          return "var";
        case Type::For:          return "for";
        case Type::While:        return "while";
        case Type::Print:        return "print";
        case Type::Nil:          return "nill";
        case Type::Return:       return "return";
        
        case Type::EoF:          return "eof";

        default:                 return "unknown";
    }
}
