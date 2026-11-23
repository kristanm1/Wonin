#ifndef _WONIN_SCANNER_
#define _WONIN_SCANNER_

#include "types.hpp"
#include "utils.hpp"
#include "lexeme.hpp"

#include <vector>
#include <string>

namespace wonin
{

    class Scanner
    {
        friend class Parser;
        friend class Interpreter;

    public:
        Scanner(std::string const& source) : m_source(source) {};

        bool scan();
        void print();

    private:
        bool isEnd(usize offset)          const { return m_position + offset >= m_source.size(); }
        bool isMatch(chr c, usize offset) const { return c == m_source[m_position + offset]; }
        bool isDigit(usize offset)        const { return utils::isDigit(m_source[m_position + offset]); }
        bool isAlphaNumeric(usize offset) const { return utils::isAlphaNumeric(m_source[m_position + offset]); }

        usize addLexeme(usize position, usize length, Token token);
        usize matchLineComment();
        usize matchString();
        usize matchNumber();
        usize matchIdentifier();
        usize matchToken();

        strview lexemeView(Lexeme const& lexeme)   const;
        str     lexemeString(Lexeme const& lexeme) const;
        double  lexemeNumber(Lexeme const& lexeme) const;
        usize   lineNumber(usize position)         const;
        usize   lineNumber(Lexeme const& lexeme)   const { return lineNumber(lexeme.position()); }

        void printLexeme(usize position) const;

    private:
        static utils::unordered_map<Token> s_keywords;
        
    private:
        std::string const&  m_source;
        std::vector<Lexeme> m_lexemes{};
        usize               m_position = 0;
        bool                m_hasError = false;
    };

}

#endif
