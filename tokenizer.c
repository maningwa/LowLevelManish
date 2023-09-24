#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_TYPE_INVALID,
    TOKEN_TYPE_INT_LITERAL,
    TOKEN_TYPE_FLOAT_LITERAL,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_PLUS,
    TOKEN_TYPE_MINUS,
    TOKEN_TYPE_MULTIPLY,
    TOKEN_TYPE_DIVIDE,
    TOKEN_TYPE_OPEN_PAREN,
    TOKEN_TYPE_CLOSE_PAREN,
    TOKEN_TYPE_SEMICOLON
} token_type_t;

typedef struct {
    token_type_t type;
    char value[MAX_TOKEN_LEN];
} token_t;

void get_token(char* input, int* offset, token_t* token)
{
    int i = 0;

    // Skip whitespace
    while (input[*offset] == ' ') {
        (*offset)++;
    }

    // Check for end of input
    if (input[*offset] == '\0') {
        token->type = TOKEN_TYPE_INVALID;
        return;
    }

    // Check for numeric literals
    if (isdigit(input[*offset])) {
        token->type = TOKEN_TYPE_INT_LITERAL;
        while (isdigit(input[*offset]) || input[*offset] == '.') {
            token->value[i++] = input[(*offset)++];
        }
        token->value[i] = '\0';
        return;
    }

    // Check for identifiers
    if (isalpha(input[*offset])) {
        token->type = TOKEN_TYPE_IDENTIFIER;
        while (isalnum(input[*offset]) || input[*offset] == '_') {
            token->value[i++] = input[(*offset)++];
        }
        token->value[i] = '\0';
        return;
    }

    // Check for operators
    switch (input[*offset]) {
        case '+':
            token->type = TOKEN_TYPE_PLUS;
            break;
        case '-':
            token->type = TOKEN_TYPE_MINUS;
            break;
        case '*':
            token->type = TOKEN_TYPE_MULTIPLY;
            break;
        case '/':
            token->type = TOKEN_TYPE_DIVIDE;
            break;
        case '(':
            token->type = TOKEN_TYPE_OPEN_PAREN;
            break;
        case ')':
            token->type = TOKEN_TYPE_CLOSE_PAREN;
            break;
        case ';':
            token->type = TOKEN_TYPE_SEMICOLON;
            break;
        default:
            token->type = TOKEN_TYPE_INVALID;
            break;
    }

    // Add the operator to the token value
    token->value[i++] = input[(*offset)++];
    token->value[i] = '\0';
}

int main()
{
    // Parse a test input string
    char* input = "2 * (3 + 4);";
    int offset = 0;
    token_t token;
    while (token.type != TOKEN_TYPE_INVALID) {
        get_token(input, &offset, &token);
        printf("Token: type=%d value=%s\n", token.type, token.value);
    }

    return 0;
}