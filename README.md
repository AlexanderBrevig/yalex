# yalex

```                                      
WORK IN PROGRESS ! ! !
```

> yalex will be a minimalist functional scripting language for embedded systems

Yet another language. Also, I'm Alex.
It's worth noting that y is kind of similar to lambda which is popular for indicating functional programming language.
What else? To lex is the process of taking text and convert it to tokens. Guess what, yalex does this too!

Yalex uses the reverse polish notation as it's main source of inspiration.
You always build up a stack of things and can use operators to modify said stack - and of course produce effects. In yalex, these operators are functions and one function can be the operand of another.


So, let's get right to it! Here's the hello world:

## The hello world program

    hw begin
    'hello world' print
    end
    
Call this program some time later by simply issuing this token to the stack ` hw ` and you'll be the hero for printing out this fine text.

### Tech Tip: everything is a token
A function, a list, a number, a string, an operator. We are all data. We are all tokens. 
We are legion. We are many. Expect us.

## Define a function that takes 1 argument

    "sleep an amount of ms, be sure to prepare stack with at least 1 value"
    sleepms 1 begin pop delayms end
    
### Tech Tip: Whitespace does not matter but double quotes does
' is better than " do you know why? One less key to press! Double quoted strings serves as comments, and if they appear above a token that is `begin`ed or `when`ed, it serves as its documentation.

Oh did I mention it ignores whitespaces outside;
```
'string 

literals'?
``` 
You can still format those RAW HTTP responses if you want to :)

## Peripheral interface!

We can do PORT reads, writes, we can do bit trickery and we can call other functions.

    blinky begin
    PORTA 0b11 PORTA 0b11 read not write "blink two leds at a time"
    1000 sleepms
    repeat "this means it's looping"

### Tech Tip: you can use PORT and pin mask, or a digit and make it a pin

    "if any pin on port C is HIGH, toggle pin 13"
    toggleLed when PORTC 0xff read 0 > then 
        13 pin, 13 pin read toggle, write
    end 

### Tech Tip: we ignore `,` which makes it slightly easier to write readable code

## Let's investigate at what it could look like if we wanted to make a quick server

Let's look at one more, here's a server:

    server when requests count 0 > then
    	'*/hello/[*]' requests 0 read url match do 
    		1 previous hello "get the url match, contents of [*] and send to hello token which will make html"
    	else
    		index "default handler"
    	done
    end

    server

## Silly brainstorming code, just to plot down some ideas while I think about this

```
// This is just a PSEUDOCODE implementation brainstorm for myself
enum default_tok {
	begin = 0,    // structual begin
	end, // loads of tokens here...
	STD_TOK
};

#define DO_OR_FAIL(_OP, _ERR, _ERR_CODE) if ( ((_ERR) == NO_ERROR) && !(_OP)) _ERR = _ERR_CODE;
// we've got one function per token
static error read_tok(token *tok, stack *stack) {
	error err = { .code = NO_ERROR, .token = read };
	token *args[2];
    token result;
    ducktype_val read;
	DO_OR_FAIL(stack_pop_begin(stack, args, 2), err.code, STACK_TOO_SHALLOW);
	DO_OR_FAIL(token_is_readable(args[0]), err.code, NOT_READABLE);
	DO_OR_FAIL(token_is_number(args[1]), err.code, NOT_A_NUMBER);
	DO_OR_FAIL(stack_pop_commit(stack), err.code, STACK_POP_FAIL); //valid data, let's do stack manipulations
    DO_OR_FAIL(args[0]->readWithTo(args[1]->val->number, &read) == NO_ERROR, err.code, READ_FAIL);
    DO_OR_FAIL(token_set_number(&result, number), err.code, SET_NUMBER_FAIL);
    DO_OR_FAIL(stack_push(stack, result), err.code, STACK_PUSH_FAIL);
	return err;
}

// THREE HOURS LATER
const uint8_t TOKEN_LEN = STD_TOK + USER_TOK /*room for USER_TOK tokens*/
token tokens[TOKEN_LEN];
tokens[5] = {.token="read", .needs=2, .action=&read_tok}```
