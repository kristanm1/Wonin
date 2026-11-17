#ifndef _WONIN_SCANNER_
#define _WONIN_SCANNER_

#include "types.hpp"
#include "utils.hpp"
#include "token.hpp"

#include <vector>

namespace Wonin
{

    class Scanner
    {
        friend class Parser;
    public:
        Scanner(std::string const& source)
            : m_source(source)
        {};

        std::vector<Token> ScanTokens();
        void PrintTokens(std::vector<Token> const& tokens);
        bool HasError() { return m_hasError; }

    private:
        void ScannerError(std::string message);
        void ScannerError(char character);

        bool IsAtEnd(usize offset)
        {
            return m_position + offset >= m_source.size();
        }

        bool IsMatchAt(char c, usize offset)
        {
            return !IsAtEnd(offset) && c == m_source[m_position + offset];
        }

        bool IsDigitAt(usize offset)
        {
            return Utils::IsDigit(m_source[m_position + offset]);
        }

        void AddToken(usize length, Token::Type type, std::vector<Token> tokens);

        Token MatchLineComment();
        Token MatchString();
        Token MatchDigit();
        Token MatchIdentifier();
        Token MatchKeyword();
        Token GetNextToken();

        std::string const& m_source;
        usize m_position = 0;
        usize m_line = 1;
        bool m_hasError = false;
    };

}

#endif
