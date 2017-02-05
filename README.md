# generic-lexer
A simple header-only lexer that can produce token streams for languages with C-like syntax, written in orthodox C++.
Widely undocumented and unclean code.

Known flaws:

None. This is a flawless, bug-free piece of software.

Seriously I don't know of any obvious bugs at the time of this writing but I don't guarantee bug-freeness.
The API is simple but can and will be improved on, as detailed below. Numeric literals are still pretty icky and the API for that will change soon.

## Usage

No linking or anything required, just include the header.
The only dependencies are <cassert> and <cstdint> and those will be ripped out too at some point. No memory is allocated or freed at any point.
Usage is straightforward:

```
/** Get a file to scan from somewhere. */
Textfile file("foobar.txt");

/** Allocate space to store tokens*/
constexpr size_t MAX_NUM_TOKENS = 1024;
constexpr size_t tokenBufferSize = sizeof(Token) * MAX_NUM_TOKENS;
Token* tokenBuffer = static_cast<Token*>(malloc(tokenBufferSize));

using namespace generic_lexer;
Lexer lexer;
if(!Tokenize(&lexer, file.buffer, file.bufferSize, tokenBuffer, tokenBufferSize, Lexer::Flags::NONE)) {
  printf("failed to tokenize input\n");
}
```
The snippet above will scan the contents of the text file and write the resulting token stream into the provided buffer.
It will NOT skip any comments and it will not create tokens for string literals or numeric literals but rather split those in seperate tokens (emit tokens for quotation marks, etc.).
That behaviour can be altered using the flags passed into the Tokenize() call.

## Advanced Usage: Keywords

You can also define your own keywords and corresponding token types:

```
struct KeywordToken
{
    enum Type : uint16_t
    {
        UNDEFINED = 0 + generic_lexer::DefaultToken::LAST_TYPE,
        VOID = 2 + generic_lexer::DefaultToken::LAST_TYPE,
        INT = 3 + generic_lexer::DefaultToken::LAST_TYPE,
        FLOAT = 4 + generic_lexer::DefaultToken::LAST_TYPE,
        DOUBLE = 5 + generic_lexer::DefaultToken::LAST_TYPE,
        BOOL = 6 + generic_lexer::DefaultToken::LAST_TYPE,
        CHAR = 7 + generic_lexer::DefaultToken::LAST_TYPE
    };
};


const char* KeywordStrings[] = {
    "undefined", "void", 
    "int", "float", "double", "bool", "char"
};

const generic_lexer::TokenType Keywords[] = {
    KeywordToken::UNDEFINED, KeywordToken::VOID,
    KeywordToken::INT, KeywordToken::FLOAT, KeywordToken::DOUBLE, KeywordToken::BOOL, KeywordToken::CHAR
};
constexpr size_t NUM_KEYWORDS = sizeof(KeywordStrings) / sizeof(char*);

(...)

using namespace generic_lexer;
Lexer lexer;
if(!Tokenize(&lexer, file.buffer, file.bufferSize, tokenBuffer, tokenBufferSize, Lexer::Flags((int)Lexer::Flags::SKIP_ALL_COMMENTS | (int)Lexer::Flags::PRODUCE_STRING_LITERALS | (int)Lexer::Flags::PRODUCE_CONSTANTS),
            KeywordStrings, Keywords, NUM_KEYWORDS)) {
  printf("failed to tokenize input\n");
}
``` 

The snippet above defines a number of keywords as enum values, starting with the first value not reserved by the header, and the corresponding strings.
It then feeds the strings and the values into the Tokenize() function. The lexer will use these to match string values of any identifiers it parses against the provided strings and if they match, assign
the token value with the corresponding index from the keyword array to the token.
Not how this snippet also demonstrates the usage of flags to skip comments and produce string literals as well as character constants and numeric literals.