#CONFIG

You can override these to customize the `yalex` runtime to suit your needs.
This will heavily impact memory footprint.

##Runtime resource management
Let's define a system suitable for embedded devices.

    #define YALEX_SIZE_STACK 10
        This one determines how many items can be on the stack at any moment
        20 should be plentiful, fibonnaci uses 10

    #define YALEX_SIZE_LAMBDAS_STACK 5
        Defines the number of lambdas that can be added

    #define YALEX_SIZE_TOKEN_STR 11
        This field is used for the max characters of each token. 
        For a system with long long we need to store 22 chars.
        For embedded system, I would consider using integers, and maybe set this to 11

    #define YALEX_SIZE_USER_TOKENS 2
        If you want to add tokens (c bindings) to the language, define this number accordingly

    #define YALEX_SIZE_LAMBDA_STACK_STR 32
        This determines how long the stack of a lambda can be.

    #define YALEX_SIZE_REGISTERS 6
        Configure how many registers are available

    #define YALEX_SIZE_MAX_DEPENDABLE_STACK 2
        Configure how deep the requirements listing for tokens can be. 
        Internally, nothing depends on more than 2

##Type dependent defines
Let's take a look at one possible configuration using stdlib stuff.

    #define YALEX_NUMERIC_TYPE int
        Defines the basic type for use with yalex

    #define YALEX_ADDRESS_TYPE unsigned int
        Defines the address/memory word of your system
    
    #define YALEX_NUM_TO_STR itoa
        Defines and overrides the yalex internal itoa. Used for rendering numbers to ascii

    #define YALEX_STR_TO_NUM strtol
        Defines and overrides the yalex internal strtol. Used for parsing ascii numbers to values

    #define YALEX_STRCPY strcpy_s
        Override internal strcpy, override if you already link a strcpy_s

    #define YALEX_STRCAT strcat_s
        Override internal strcat, override if you already link a strcat_s

    #define YALEX_STRLEN strlen
        Override internal strlen, override if you already link a strlen

    #define YALEX_STRCMP strcmp
        Override internal strcmp, override if you already link a strcmp

    #define YALEX_MEMSET memset
        Override internal memset, override if you already link a memset