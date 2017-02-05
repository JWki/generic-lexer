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
//  TODO: Make lexer CORRECT
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
	typedef uint16_t TokenType;

	struct DefaultToken
    {
        enum Type : uint16_t
        {
            UNDEFINED = 0,
            GENERIC_TOKEN = 1,  // interpretation of this is up to the user 

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

            DOUBLE_COLON = 40,
            EoF = 41,
            LAST_TYPE = EoF
        };
	};

	static const char* DefaultTokenStrings[DefaultToken::LAST_TYPE + 1] = {
		"UNDEFINED", "GENERIC_TOKEN", 
		"PARENTHESES_OPEN", "PARENTHESES_CLOSE",
		"SQUARE_BRACKET_OPEN", "SQUARE_BRACKET_CLOSE",
		"CURLY_BRACES_OPEN", "CURLY_BRACES_CLOSE",
		"DOT", "COMMA", "COLON", "SEMICOLON", "EXCLAMATION", "QUESTION_MARK",
		"PLUS", "MINUS", "STAR", "SLASH", "BACKSLASH", "EQUALS", "DOUBLE_EQUALS",
		"LESS", "LEQUALS", "GREATER", "GEQUALS", "NOT_EQUALS", "AMPERSAND",
		"DOUBLE_AMPERSAND", "PIPE", "DOUBLE_PIPE", "CARET", "DOUBLE_CARET", "TILDE",
		"AT", "POUND", "QUOTATION_MARK", "APOSTROPHE", "STRING_LITERAL", "CHARACTER_CONSTANT", "NUMERIC_CONSTANT",
        "DOUBLE_COLON", "EoF"
	}; 

	struct Token
	{
        union  // TODO: this is gross somehow.
        {
            TokenType   type;
            DefaultToken::Type _defTokenType = DefaultToken::UNDEFINED;
        };
		char*       text = nullptr;     // pointer into the source buffer
		uint32_t    length = 0;         // length of the string containing the token
        uint16_t    userType = 0;       // can be used to add token types in additional passes

        uint32_t    lineNumber = 0;
	};

	//
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
			_currentPos++;
		}
	};

	// 
	bool IsEoF(char c)
	{
		return c == '\0';
	}


	bool IsEndl(char c)
	{
		return c == '\n';
	}

	bool IsWhitespace(char c)
	{
		return c == ' ' || c == '\r' || c == '\t' || IsEndl(c) || IsEoF(c);
	}

	void EatWhitespaces(Lexer* context)
	{
		while (!IsEoF(context->GetChar()) && IsWhitespace(context->GetChar())) {
            if (IsEndl(context->GetChar())) {
                context->_lineNumber++;
            }
			context->AdvanceOne();
		}
	}

	void SkipMultilineComment(Lexer* context)
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
            if (IsEndl(c)) {
                context->_lineNumber++;
            }
			context->AdvanceOne();
		} while (context->_multilineCommentDepth > 0);
	}

	void SkipLine(Lexer* context)
	{
		while(!IsEndl(context->GetChar())) {
			context->AdvanceOne(); 
		}
		context->AdvanceOne();
        context->_lineNumber++;
	}
	
	void WriteToken(Token token, Lexer* context)
	{
		auto newTokenOffset = context->_tokenStreamBufferOffset + 1;
		assert(sizeof(Token) * newTokenOffset <= context->_tokenStreamBufferSize);
        token.lineNumber = context->_lineNumber;
		memcpy(&context->_tokenStreamBuffer[context->_tokenStreamBufferOffset], &token, sizeof(Token));
		context->_tokenStreamBufferOffset = newTokenOffset;
		context->_numTokens++;
	}

	bool IsNonGeneric(char c)
	{
		switch(c)
		{
			case '(':
			case ')':
			case '[':
			case ']':
			case '{':
			case '}':
			case '.':
			case ',':
			case ':':
			case ';':
			case '!':
			case '?':
			case '+':
			case '-':
			case '*':
			case '/':
			case '\\':
			case '=':
			case '<':
			case '>':
			case '&':
			case '|':
			case '^':
			case '~':
			case '@':
			case '#':
			case '\'':
			case '"':
				return true;
			break;
			default:
				return false;
			break;
		};
	}

	void ParseGenericToken(Lexer* context)
	{
		Token token;
		token.type = DefaultToken::GENERIC_TOKEN;
		token.text = context->_currentPos;
		token.length = 0;

		while (!IsNonGeneric(context->GetChar()) && !IsWhitespace(context->GetChar()))
		{
			context->AdvanceOne();
			token.length++;
		}

		// write token to buffer
		WriteToken(token, context);
	}

    void ParseStringLiteral(Lexer* context)
    {
        Token token;
        token.type = DefaultToken::STRING_LITERAL;
        token.text = context->_currentPos;
        token.length = 1;
        do{
            context->AdvanceOne();
            token.length++;
        } while (context->GetChar() != '"' && !IsEoF(context->GetChar()));
       
        WriteToken(token, context);
        context->AdvanceOne();
    }

    bool IsNumeric(char c)
    {
        switch (c) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return true;
                break;
            default:
            {
                return false;
            } break;
        }
    }

    // TODO: Maybe move this out and let higher levels (lexer extensions) handle this
    void ParseNumericConstant(Lexer* context)
    {
        Token token;
        token.type = DefaultToken::NUMERIC_CONSTANT;
        token.text = context->_currentPos;
        token.length = 0;
        do {
            context->AdvanceOne();
            token.length++;
        } while (IsNumeric(context->GetChar()) && !IsEoF(context->GetChar()));
        if (context->GetChar() == '.') {
            // floating point expression dot
            do {
                context->AdvanceOne();
                token.length++;
            } while (IsNumeric(context->GetChar()) && !IsWhitespace(context->GetChar()) && !IsEoF(context->GetChar()));
        }
        if (context->GetChar() == 'f') {
            // floating point expression
			context->AdvanceOne();
            token.length++;
        }
        WriteToken(token, context);
    }

	bool ParseToken(Lexer* context)
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
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case ')' : 
			{
				Token token;
				token.type = DefaultToken::PARENTHESES_CLOSE;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '[' : 
			{
				Token token;
				token.type = DefaultToken::SQUARE_BRACKET_OPEN;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case ']' : 
			{
				Token token;
				token.type = DefaultToken::SQUARE_BRACKET_CLOSE;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '{' : 
			{
				Token token;
				token.type = DefaultToken::CURLY_BRACES_OPEN;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '}' : 
			{
				Token token;
				token.type = DefaultToken::CURLY_BRACES_CLOSE;
				token.text = context->_currentPos;
				token.length = 1;
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
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case ',' : 
			{
				Token token;
				token.type = DefaultToken::COMMA;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case ':' : 
			{
				Token token;
				token.type = DefaultToken::COLON;
				token.text = context->_currentPos;
				token.length = 1;
				context->AdvanceOne();
                if (context->GetChar() == ':') {
                    token.type = DefaultToken::DOUBLE_COLON;
                    token.length = 2;
                    context->AdvanceOne();
                }
                WriteToken(token, context);
			} break;
			case ';' : 
			{
				Token token;
				token.type = DefaultToken::SEMICOLON;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '!' : 
			{
				Token token;
				token.type = DefaultToken::SQUARE_BRACKET_CLOSE;
				token.text = context->_currentPos;
				token.length = 1;

                context->AdvanceOne();
				if(context->GetChar() == '=') {
				    token.type = DefaultToken::NOT_EQUALS;
				    token.length = 2;
                    context->AdvanceOne();
				} 
                WriteToken(token, context);
			} break;
			case '?' : 
			{
				Token token;
				token.type = DefaultToken::QUESTION_MARK;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '+' : 
			{
				Token token;
				token.type = DefaultToken::PLUS;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '-' : 
			{
				Token token;
				token.type = DefaultToken::MINUS;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '*' : 
			{
				Token token;
				token.type = DefaultToken::STAR;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '/' : 
			{
				Token token;
				token.type = DefaultToken::SLASH;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '\\' : 
			{
				Token token;
				token.type = DefaultToken::BACKSLASH;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '=' : 
			{
				Token token;
				token.type = DefaultToken::EQUALS;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '<' : 
			{
				Token token;
				token.type = DefaultToken::LESS;
				token.text = context->_currentPos;
				token.length = 1;
				context->AdvanceOne();
				if(context->GetChar() == '=') {
					token.type = DefaultToken::LEQUALS;
					token.length = 2;
					context->AdvanceOne();
				}
				WriteToken(token, context);
			} break;
			case '>' : 
			{
				Token token;
				token.type = DefaultToken::GREATER;
				token.text = context->_currentPos;
				token.length = 1;
				context->AdvanceOne();
				if(context->GetChar() == '=') {
					token.type = DefaultToken::GEQUALS;
					token.length = 2;
					context->AdvanceOne();
				}
				WriteToken(token, context);
			} break;
			case '&' : 
			{
				Token token;
				token.type = DefaultToken::AMPERSAND;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '|' : 
			{
				Token token;
				token.type = DefaultToken::PIPE;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '^' : 
			{
				Token token;
				token.type = DefaultToken::CARET;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '~' : 
			{
				Token token;
				token.type = DefaultToken::TILDE;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '@' : 
			{
				Token token;
				token.type = DefaultToken::AT;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '#' : 
			{
				Token token;
				token.type = DefaultToken::POUND;
				token.text = context->_currentPos;
				token.length = 1;
				WriteToken(token, context);
				context->AdvanceOne();
			} break;
			case '"' : 
			{
                if (context->_flags & Lexer::Flags::PRODUCE_STRING_LITERALS) {
                    ParseStringLiteral(context);
                }
                else {
                    Token token;
                    token.type = DefaultToken::QUOTATION_MARK;
                    token.text = context->_currentPos;
                    token.length = 1;
                    WriteToken(token, context);
                    context->AdvanceOne();
                }
			} break;
			case '\'' : 
			{
                if (context->_flags & Lexer::Flags::PRODUCE_CHARACTER_CONSTANTS) {
                    Token token;
                    token.type = DefaultToken::CHARACTER_CONSTANT;
                    token.text = context->_currentPos;
                    token.length = 3;
                    WriteToken(token, context);
                    context->AdvanceOne();
                    context->AdvanceOne();  
                    if (context->GetChar() != '\'') {
                        printf("lexer error: ' expected\n");
                        return false;
                    }
                    context->AdvanceOne();
                }
                else {
                    Token token;
                    token.type = DefaultToken::APOSTROPHE;
                    token.text = context->_currentPos;
                    token.length = 1;
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
                    ParseGenericToken(context);
                }
			} break;
		}
        return true;
	}

	//
	bool Tokenize(Lexer* context, char* buffer, uint32_t bufferSize, Token* tokenBuffer, uint32_t tokenBufferSize, Lexer::Flags flags)
	{
		context->_bufferStart = buffer;
		context->_bufferSize = bufferSize;
		context->_tokenStreamBuffer = tokenBuffer;
		context->_tokenStreamBufferSize = tokenBufferSize;
		context->_currentPos = context->_bufferStart;
		context->_flags = flags;
		context->_numTokens = 0;
        context->_lineNumber = 1;   // gross. but correct.

		while (!IsEoF(context->GetChar())) {
			EatWhitespaces(context);
			if(!ParseToken(context)) {
                printf("lexer error\n");
                return false;
            }
		}
        Token eofToken;
        eofToken.text = context->_currentPos;
        eofToken.length = 1;
        eofToken.type = DefaultToken::EoF;
        eofToken.userType = 0;
        WriteToken(eofToken, context);

		return true;
	}

}

