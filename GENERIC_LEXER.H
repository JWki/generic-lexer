//  MIT License
//
//  Copyright(c) 2017 Jascha Wedowski
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files(the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions :
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
#pragma once


//
// 
//
//
#include <cassert>
#include <cstdint>
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;


namespace generic_lexer
{
    /** simple string type with explicit size field used to refer to non-nullterminated strings */
    struct String
    {
        uint64_t    length  = 0;        // will be padded to 8 byte anyways (on 64 bit platforms) so we can use the full range while we're at it
        char*       buffer  = nullptr;   

        String() = default;
        String(uint64_t len, char* buf) : length(len), buffer(buf) {}
    };

    /** simple strlen implementation to not depend on standard libraries for a single call */
    static size_t strlen(const char* str)
    {
        size_t len = 0;
        while (*str != '\0') {
            ++str;
            ++len;
        }
        return len;
    }

    /***/

    /**  */
	typedef uint16_t TokenType;

	struct DefaultToken
    {
        /** Default token types */
        enum Type : uint16_t
        {
            UNDEFINED = 0,
            IDENTIFIER = 1,  // this used to be GENERIC_TOKEN

            // different bracket types
            PARENTHESES_OPEN = 2,       // (
            PARENTHESES_CLOSE = 3,      // )
            SQUARE_BRACKET_OPEN = 4,    // [
            SQUARE_BRACKET_CLOSE = 5,   // ]
            CURLY_BRACES_OPEN = 6,      // {
            CURLY_BRACES_CLOSE = 7,     // }

            // punctuation
            DOT = 8,            // . 
            COMMA = 9,          // ,
            COLON = 10,          // :
            SEMICOLON = 11,      // ;
            EXCLAMATION = 12,    // !
            QUESTION_MARK = 13,  // ?

            // operators and similars
            PLUS = 14,       // +
            MINUS = 15,      // -
            STAR = 16,       // *
            SLASH = 17,      // /
            BACKSLASH = 18,  // well backslash
            EQUALS = 19,         // =
            DOUBLE_EQUALS = 20,  // ==
            LESS = 21,           // < 
            LEQUALS = 22,        // <=
            GREATER = 23,        // >
            GEQUALS = 24,        // >= 
            NOT_EQUALS = 25,     // !=

            AMPERSAND = 26,              // &
            DOUBLE_AMPERSAND = 27,       // &&
            PIPE = 28,                   // |
            DOUBLE_PIPE = 29,            // ||
            CARET = 30,                  // ^
            DOUBLE_CARET = 31,           // ^^ 
            TILDE = 32,                  // ~

            // more stuff
            AT = 33,             // @
            POUND = 34,          // #
            HASH = POUND,

            // string/character stuff
            QUOTATION_MARK = 35,     // "
            APOSTROPHE = 36,        // '
            STRING_LITERAL = 37,

            CHARACTER_CONSTANT = 38,
            NUMERIC_CONSTANT = 39,

            ARROW = 40,
            DOUBLE_COLON = 41,
            EoF = 42,

            LAST_TYPE = EoF + 1
        };
	};

    /** Printable strings for each default token type */
	static const char* DefaultTokenStrings[DefaultToken::LAST_TYPE] = {
		"UNDEFINED", "IDENTIFIER", 
		"PARENTHESES_OPEN", "PARENTHESES_CLOSE",
		"SQUARE_BRACKET_OPEN", "SQUARE_BRACKET_CLOSE",
		"CURLY_BRACES_OPEN", "CURLY_BRACES_CLOSE",
		"DOT", "COMMA", "COLON", "SEMICOLON", "EXCLAMATION", "QUESTION_MARK",
		"PLUS", "MINUS", "STAR", "SLASH", "BACKSLASH", "EQUALS", "DOUBLE_EQUALS",
		"LESS", "LEQUALS", "GREATER", "GEQUALS", "NOT_EQUALS", "AMPERSAND",
		"DOUBLE_AMPERSAND", "PIPE", "DOUBLE_PIPE", "CARET", "DOUBLE_CARET", "TILDE",
		"AT", "POUND", "QUOTATION_MARK", "APOSTROPHE", "STRING_LITERAL", "CHARACTER_CONSTANT", "NUMERIC_CONSTANT", "ARROW",
        "DOUBLE_COLON", "EoF"
	}; 

    /** Data type for tokens. */
	struct Token
	{
        union  // TODO: this is gross somehow.
        {
            TokenType   type;
            DefaultToken::Type _defTokenType = DefaultToken::UNDEFINED;
        };
        String      text;
        uint32_t    lineNumber = 0;
	};

    // 
    static bool IsEoF(char c)
    {
        return c == '\0';
    }
    static bool IsEndl(char c)
    {
        return c == '\n';
    }
    static bool IsWhitespace(char c)
    {
        return c == ' ' || c == '\r' || c == '\t' || IsEndl(c) || IsEoF(c);
    }
    static bool IsNumeric(char c)
    {
        return (c == '0' || c == '1'
            || c == '2' || c == '3'
            || c == '4' || c == '5'
            || c == '6' || c == '7'
            || c == '8' || c == '9');
    }
    static bool IsAlphabetic(char c)
    {
        return
            (c == 'a' || c == 'A' ||
                c == 'b' || c == 'B' ||
                c == 'c' || c == 'C' ||
                c == 'd' || c == 'D' ||
                c == 'e' || c == 'E' ||
                c == 'f' || c == 'F' ||
                c == 'g' || c == 'G' ||
                c == 'h' || c == 'H' ||
                c == 'i' || c == 'I' ||
                c == 'j' || c == 'J' ||
                c == 'k' || c == 'K' ||
                c == 'l' || c == 'L' ||
                c == 'm' || c == 'M' ||
                c == 'n' || c == 'N' ||
                c == 'o' || c == 'O' ||
                c == 'p' || c == 'P' ||
                c == 'q' || c == 'Q' ||
                c == 'r' || c == 'R' ||
                c == 's' || c == 'S' ||
                c == 't' || c == 'T' ||
                c == 'u' || c == 'U' ||
                c == 'v' || c == 'V' ||
                c == 'w' || c == 'W' ||
                c == 'x' || c == 'X' ||
                c == 'y' || c == 'Y' ||
                c == 'z' || c == 'Z');
    }
    static bool IsAlphaNumeric(char c)
    {
        return IsNumeric(c) || IsAlphabetic(c);
    }

	/** Main data structure for lexing state. */
	struct Lexer
	{
		char*       _bufferStart = nullptr;  // pointer to the buffer to parse 
		uint32_t    _bufferSize = 0;        // size of the buffer to parse 

		Token*      _tokenStreamBuffer = nullptr;  // pointer to where to store the token stream
		uint32_t    _tokenStreamBufferSize = 0;        // size of the buffer holding tokens 
		uint32_t    _tokenStreamBufferOffset = 0;       // index to the next slot to write a token to
		uint32_t 	_numTokens = 0;

		char*       _currentPos = nullptr;  // pointer to the current parse position
        uint32_t    _lineNumber = 0;

		uint16_t    _multilineCommentDepth = 0; // we support nested multiline comments. suck it, C.

        TokenType*  _keywordTokenTypes = nullptr;
        char**      _keywordRegister = nullptr;
        uint32_t    _numKeywords = 0;

		enum Flags : uint16_t
		{
			NONE = 0x0,
			SKIP_MULTILINE_COMMENTS = 1 << 1,
			SKIP_SINGLE_LINE_COMMENTS = 1 << 2,
			SKIP_ALL_COMMENTS = SKIP_MULTILINE_COMMENTS | SKIP_SINGLE_LINE_COMMENTS,
            PRODUCE_STRING_LITERALS = 1 << 3,
            PRODUCE_NUMERIC_CONSTANTS = 1 << 4,
            PRODUCE_CHARACTER_CONSTANTS = 1 << 5,
            PRODUCE_CONSTANTS = PRODUCE_NUMERIC_CONSTANTS | PRODUCE_CHARACTER_CONSTANTS
		} _flags;

		char GetChar()
		{
			return *_currentPos;
		}

		char GetCharWithOffset(uint32_t offset)
		{
			assert(((_currentPos - _bufferStart) + offset) <= _bufferSize);
			return *(_currentPos + offset);
		}

		void AdvanceOne()
		{
            if (IsEndl(*_currentPos)) {
                _lineNumber++;
            }
			_currentPos++;
		}

        int FindKeyword(const char* identifier, uint32_t identifierLength)
        {
            for (auto i = 0u; i < _numKeywords; ++i) {
                char* keyword = _keywordRegister[i];
                if (strlen(keyword) != identifierLength) {
                    continue;
                }
                bool match = true;
                for (auto j = 0u; j < (identifierLength); ++j) {
                    if (keyword[j] != identifier[j]) {
                        match = false;
                        break;
                    }
                }
                if (match) { return i; }
            }
            return -1;
        }
	};

	
    /** Has the lexer eat all the whitespaces and endlines */
    static void EatWhitespaces(Lexer* context)
	{
		while (!IsEoF(context->GetChar()) && IsWhitespace(context->GetChar())) {
			context->AdvanceOne();
		}
	}

    /** Skips a multiline comment including nested multiline comments */
    static void SkipMultilineComment(Lexer* context)
	{
		do {
			char c = context->GetChar();
			char c_next = context->GetCharWithOffset(1);

			if (c == '/' && c_next == '*') {
				context->_multilineCommentDepth++;
				context->AdvanceOne();
				context->AdvanceOne();
				continue;
			}
			if (c == '*' && c_next == '/') {
				context->_multilineCommentDepth--;
				context->AdvanceOne();
				context->AdvanceOne();
				continue;
			}
			context->AdvanceOne();
		} while (context->_multilineCommentDepth > 0);
	}

    /** Skips to the next line */
    static void SkipLine(Lexer* context)
	{
		while(!IsEndl(context->GetChar())) {
			context->AdvanceOne(); 
		}
		context->AdvanceOne();
	}
	
    /** Writes a token to the buffer */
    static void WriteToken(Token token, Lexer* context)
	{
		auto newTokenOffset = context->_tokenStreamBufferOffset + 1;
		assert(sizeof(Token) * newTokenOffset <= context->_tokenStreamBufferSize);
        token.lineNumber = context->_lineNumber;
		context->_tokenStreamBuffer[context->_tokenStreamBufferOffset] = token;
		context->_tokenStreamBufferOffset = newTokenOffset;
		context->_numTokens++;
	}

    

    static void ParseIdentifier(Lexer* context)
	{
		Token token;
		token.type = DefaultToken::IDENTIFIER;
		token.text.buffer = context->_currentPos;
		token.text.length = 0;

        /** We only go in here when the first character is not numeric so this check is fine for the first character. */
		while (IsAlphaNumeric(context->GetChar()))
		{
			context->AdvanceOne();
			token.text.length++;
		}
        // Check whether the identifier is a keyword
        auto index = context->FindKeyword(token.text.buffer, (uint32_t)token.text.length);
        if (index >= 0) {
            token.type = context->_keywordTokenTypes[index];
        }
		// write token to buffer
		WriteToken(token, context);
	}

    static bool ParseStringLiteral(Lexer* context)
    {
        Token token;
        token.type = DefaultToken::STRING_LITERAL;
        token.text.buffer = context->_currentPos;
        token.text.length = 1;
        do{
            context->AdvanceOne();
            token.text.length++;
        } while (context->GetChar() != '"' && !IsEoF(context->GetChar()));
        assert(!IsEoF(context->GetChar()));     // we want a closing quotation mark
        if (IsEoF(context->GetChar())) {
            return false;
        }
        WriteToken(token, context);
        context->AdvanceOne();
    }

   

    // TODO: Maybe move this out and let higher levels (lexer extensions) handle this
    static void ParseNumericConstant(Lexer* context)
    {
        Token token;
        token.type = DefaultToken::NUMERIC_CONSTANT;
        token.text.buffer = context->_currentPos;
        token.text.length = 0;
        do {
            context->AdvanceOne();
            token.text.length++;
        } while (IsNumeric(context->GetChar()) && !IsEoF(context->GetChar()));
        if (context->GetChar() == '.') {
            // floating point expression dot
            do {
                context->AdvanceOne();
                token.text.length++;
            } while (IsNumeric(context->GetChar()) && !IsWhitespace(context->GetChar()) && !IsEoF(context->GetChar()));
        }
        if (context->GetChar() == 'f') {
            // floating point expression
			context->AdvanceOne();
            token.text.length++;
        }
        WriteToken(token, context);
    }

    static bool ParseToken(Lexer* context)
	{
		char c = context->GetChar();
		if (c == '/' && (context->_flags & Lexer::Flags::SKIP_SINGLE_LINE_COMMENTS || context->_flags & Lexer::Flags::SKIP_MULTILINE_COMMENTS)) {
			char nextC = context->GetCharWithOffset(1);
			if (nextC == '/' && (context->_flags & Lexer::Flags::SKIP_SINGLE_LINE_COMMENTS)) {
				SkipLine(context);
				return true;
			}
			if (nextC == '*' && (context->_flags & Lexer::Flags::SKIP_MULTILINE_COMMENTS)) {
				SkipMultilineComment(context);
				return true;
			}
		}

		switch (c) {
			case '(' : 
			{
				Token token;
				token.type = DefaultToken::PARENTHESES_OPEN;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case ')' : 
			{
				Token token;
				token.type = DefaultToken::PARENTHESES_CLOSE;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '[' : 
			{
				Token token;
				token.type = DefaultToken::SQUARE_BRACKET_OPEN;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case ']' : 
			{
				Token token;
				token.type = DefaultToken::SQUARE_BRACKET_CLOSE;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '{' : 
			{
				Token token;
				token.type = DefaultToken::CURLY_BRACES_OPEN;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '}' : 
			{
				Token token;
				token.type = DefaultToken::CURLY_BRACES_CLOSE;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '.' : 
			{
                if (context->_flags & Lexer::Flags::PRODUCE_NUMERIC_CONSTANTS && IsNumeric(context->GetCharWithOffset(1))) {
                    break;
                }
				Token token;
				token.type = DefaultToken::DOT;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case ',' : 
			{
				Token token;
				token.type = DefaultToken::COMMA;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case ':' : 
			{
				Token token;
				token.type = DefaultToken::COLON;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				context->AdvanceOne();
                if (context->GetChar() == ':') {
                    token.type = DefaultToken::DOUBLE_COLON;
                    token.text.length = 2;
                    context->AdvanceOne();
                }
                WriteToken(token, context);
			} break;
			case ';' : 
			{
				Token token;
				token.type = DefaultToken::SEMICOLON;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '!' : 
			{
				Token token;
				token.type = DefaultToken::SQUARE_BRACKET_CLOSE;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;

                context->AdvanceOne();
				if(context->GetChar() == '=') {
				    token.type = DefaultToken::NOT_EQUALS;
				    token.text.length = 2;
                    context->AdvanceOne();
				} 
                WriteToken(token, context);
			} break;
			case '?' : 
			{
				Token token;
				token.type = DefaultToken::QUESTION_MARK;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '+' : 
			{
				Token token;
				token.type = DefaultToken::PLUS;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '-' : 
			{
				Token token;
				token.type = DefaultToken::MINUS;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '*' : 
			{
				Token token;
				token.type = DefaultToken::STAR;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '/' : 
			{
				Token token;
				token.type = DefaultToken::SLASH;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '\\' : 
			{
				Token token;
				token.type = DefaultToken::BACKSLASH;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '=' : 
			{
				Token token;
				token.type = DefaultToken::EQUALS;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '<' : 
			{
				Token token;
				token.type = DefaultToken::LESS;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				context->AdvanceOne();
				if(context->GetChar() == '=') {
					token.type = DefaultToken::LEQUALS;
					token.text.length = 2;
					context->AdvanceOne();
				}
				WriteToken(token, context);
			} break;
			case '>' : 
			{
				Token token;
				token.type = DefaultToken::GREATER;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				context->AdvanceOne();
				if(context->GetChar() == '=') {
					token.type = DefaultToken::GEQUALS;
					token.text.length = 2;
					context->AdvanceOne();
				}
				WriteToken(token, context);
			} break;
			case '&' : 
			{
				Token token;
				token.type = DefaultToken::AMPERSAND;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '|' : 
			{
				Token token;
				token.type = DefaultToken::PIPE;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '^' : 
			{
				Token token;
				token.type = DefaultToken::CARET;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '~' : 
			{
				Token token;
				token.type = DefaultToken::TILDE;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '@' : 
			{
				Token token;
				token.type = DefaultToken::AT;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '#' : 
			{
				Token token;
				token.type = DefaultToken::POUND;
				token.text.buffer = context->_currentPos;
				token.text.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '"' : 
			{
                if (context->_flags & Lexer::Flags::PRODUCE_STRING_LITERALS) {
                    if (!ParseStringLiteral(context)) {
                        return false;
                    }
                }
                else {
                    Token token;
                    token.type = DefaultToken::QUOTATION_MARK;
                    token.text.buffer = context->_currentPos;
                    token.text.length = 1;
                    WriteToken(token, context);
                    context->AdvanceOne();
                }
			} break;
			case '\'' : 
			{
                if (context->_flags & Lexer::Flags::PRODUCE_CHARACTER_CONSTANTS) {
                    Token token;
                    token.type = DefaultToken::CHARACTER_CONSTANT;
                    token.text.buffer = context->_currentPos;
                    token.text.length = 3;
                    WriteToken(token, context);
                    context->AdvanceOne();
                    context->AdvanceOne();  
                    assert(context->GetChar() == '\'');
                    if (context->GetChar() != '\'') {
                        return false;
                    }
                    context->AdvanceOne();
                }
                else {
                    Token token;
                    token.type = DefaultToken::APOSTROPHE;
                    token.text.buffer = context->_currentPos;
                    token.text.length = 1;
                    WriteToken(token, context);
                    context->AdvanceOne();
                }
			} break;
			default:
			{
                if (IsEoF(c)) {
                    return true;
                }
                if (context->_flags & Lexer::Flags::PRODUCE_NUMERIC_CONSTANTS && IsNumeric(c)) {
                    ParseNumericConstant(context);
                }
                else {
                    ParseIdentifier(context);
                }
			} break;
		}
        return true;
	}

	//
    static bool Tokenize(Lexer* context, char* buffer, uint32_t bufferSize, Token* tokenBuffer, uint32_t tokenBufferSize, Lexer::Flags flags, const char** keywordStrings = nullptr, const TokenType* keywordTypes = nullptr, uint32_t numKeywords = 0)
	{
		context->_bufferStart = buffer;
		context->_bufferSize = bufferSize;
		context->_tokenStreamBuffer = tokenBuffer;
		context->_tokenStreamBufferSize = tokenBufferSize;
		context->_currentPos = context->_bufferStart;
		context->_flags = flags;
		context->_numTokens = 0;
        context->_lineNumber = 1;   // gross. but correct.

        context->_numKeywords = numKeywords;
        context->_keywordRegister = (char**)keywordStrings;
        context->_keywordTokenTypes = (TokenType*)keywordTypes;

		while (!IsEoF(context->GetChar())) {
			EatWhitespaces(context);
			if(!ParseToken(context)) {
                return false;
            }
		}
        Token eofToken;
        eofToken.text.buffer = context->_currentPos;
        eofToken.text.length = 1;
        eofToken.type = DefaultToken::EoF;
        WriteToken(eofToken, context);

		return true;
	}

}

