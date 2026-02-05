#include <string>
#include <vector>
#include <unordered_map>

enum class TokenType {
    DEF,
    RETURN,
    IF,
    ELSE,
    PRINT,
    FOR,
    WHILE,

    IDENTIFIER,
    NUMBER,
    STRING,

    PLUS, // +
    MINUS, // -
    STAR, // *
    SLASH, // /
    ASSIGN, // =
    MODULO, // %
    GREATERTHAN, // >
    LESSERTHAN, // <

    LPAREN, // (
    RPAREN, // )

    NEWLINE,
    EOF_TOKEN,

    COMMA, // ,
    COLON, // :
    INDENT,
    DEDENT,
    COMMENT // #
};


class Token{
    public:
    // Main part of a token
    TokenType type;
    std::string value;

    // Used to tell user about error in case error is found
    int line;
    int column;

    // This constructor will allow us to easily make the tokens while coding
    Token(TokenType type, std::string val, int l, int c){
        this->type = type; this->value = val;
        this->line = l; this->column = c;
    }
};


class Lexer{
    public:
        Lexer(std::string input_string){
            this->source_code = preprocess_indents(input_string);
        }
        std::vector<Token> scan_Tokens();

    private:
        std::string source_code;
        static inline const std::unordered_map<std::string, TokenType> keywords = {
            {"def",    TokenType::DEF},
            {"return", TokenType::RETURN},
            {"if",     TokenType::IF},
            {"else",   TokenType::ELSE},
            {"print",  TokenType::PRINT},
            {"=",      TokenType::ASSIGN},
            {"+",      TokenType::PLUS},
            {"-",      TokenType::MINUS},   
            {"*",      TokenType::STAR},
            {"/",      TokenType::SLASH},
            {"(",      TokenType::LPAREN},
            {")",      TokenType::RPAREN},
            {":",      TokenType::COLON},
            {",",      TokenType::COMMA},
            {"\n",     TokenType::NEWLINE},
            {"\t",     TokenType::INDENT},
            {"\r",     TokenType::DEDENT}
        };
        std::vector<Token> tokens; 
        int start = 0;
        int current_index = 0;
        int line = 1;


        // FUNCTIONS NEEDED FOR LEXER TO WORK

        bool isAtEnd();         // Checks for last character
        char advance();         // Return current char and move forward
        char peek();            // Sometimes, we don't actually want to read a character and may only want to peek at it
        char peekNext();        // Peak at the next character

        void addToken(TokenType type);

        // Specific scanners for complex types
        void scanString();
        void scanNumber();
        void scanIdentifier();
        std::string preprocess_indents(std::string raw);
};


std::string Lexer::preprocess_indents(std::string raw) {
    std::string clean_code = "";
    bool atLineStart = true;

    for (int i = 0; i < raw.length(); i++) {
        if (atLineStart) {
            if (raw[i] == ' ') {
                if (i + 3 < raw.length() && raw.substr(i, 4) == "    ") {
                    clean_code += '\t';
                    i += 3;
                    continue;
                }
            }
            else if (raw[i] == '\t') {
                clean_code += '\t';
                continue;
            }
            else if (raw[i] == '\n') {
                clean_code += '\n';
                atLineStart = true;
                continue;
            }
            else {
                atLineStart = false; 
            }
        }
        clean_code += raw[i];

        if (raw[i] == '\n') {
            atLineStart = true;
        }
    }
    
    return clean_code;
}