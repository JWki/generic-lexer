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

