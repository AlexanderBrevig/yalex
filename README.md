# yalex

> yalex is a minimalist [configurable](CONFIG.md) functional scripting language for embedded systems

Yet Another LEXer.

It's worth noting that y is kind of similar to lambda which is popular for indicating a functional programming language.
What else? To lex is the process of taking text and convert it to tokens. Guess what, yalex does this too!

Yalex uses the reverse polish notation and FORTH as it's main source of inspiration. (I learned about FORTH after I started this project)

You always build up a stack of things and can use operators to modify said stack - and of course produce effects. 
In yalex, these operators are functions and one function can be the operand of another.

## Get the code and start to play

	git clone --recurse-submodules https://github.com/AlexanderBrevig/yalex
	cd yalex
	cmake .
	make
	
	./bin/yalex_tests_app
	./bin/release

## Tested functionality

 Refer to my [TODO](TODO.md) document for more details.

    LITERALS:
        -1 0 10 0xff 
        true false
        "some string"

	BINARY OPERANDS FOR TWO NUMBERS:
        + - * /
	    < > == !=
	    || &&

    STACK OPERATIONS:
	    dup push pop clr
        nop
        _ (alias of 'nop, must be resolved by select or resolve)

    MISCELLANEOUS:
	    X print 
        dump
	    NUM peek
	    NUM 'yes 'no select
        "1 2 3" run

    LAMBDAS:
        'lambda resolve
	    ()
	    :lambda (1 1 +)
	    lambda
	    lambda:

    REGISTERS:
	    NUM R[0..15]S
	    R[0..15]R

## Code example; fibonacci

    :fibstep (R1R R2R + R3S pop R2R R1S pop R3R R2S pop R4R 1 + R4S pop rec)
    :rec (R4R R0R 1 + < 'fibstep _ select)
    :start (R0R 1 - R0S pop rec pop pop pop pop pop pop R3R)
    :fib (R0S 0 R1S 1 R2S 0 R3S 1 R4S R0R 3 < 1 'start select)

Here it is, side by side a similar C implementation:

    #include <stdio.h>
    /*  
        :fibstep (R1R R2R + R3S pop R2R R1S pop R3R R2S pop R4R 1 + R4S pop rec)
        :rec (R4R R0R 1 + < 'fibstep _ select)
        :start (R0R 1 - R0S pop rec pop pop pop pop pop pop R3R)
        :fib (R0S 0 R1S 1 R2S 0 R3S 1 R4S R0R 3 < 1 'start select)
        10 fib
        55
    */
    /// register declarations
    int n;            // R0
    int t1;           // R1
    int t2;           // R2
    int nextTerm;     // R3
    int i;            // R4

    void fibstep() {
        nextTerm = t1 + t2; // R1R R2R + R3S pop
        t1 = t2;			// R2R R1S pop
        t2 = nextTerm;		// R3R R2S pop
        i++;				// R4R 1 + R4S pop
        rec();				// rec
    }

    void rec() {
        if (i < n + 1)      // R4R R0R 1 + <
        {
            fibstep();      // 'fibstep the ' makes it only resolve if selected
        }
        else {
                            // _ an empty else 
        }  
                            // select is the if keyword 
    }

    int start() {
        n = n-1;            // R0R 1 - R0S pop
        rec();              // rec
                            // pop pop pop pop pop pop (cleans up stack)
        return nextTerm;    // R3R
    }

    int fib(int el)     
    {   
        n = el;             // R0S //reads previous stack item as input
        t1 = 0;             // 0 R1S
        t2 = 1;             // 1 R2S
        nextTerm = 0;       // 0 R3S
        i = 1;              // 1 R4S
        if (i < 3) {        // R0R 3 <
            return 1;       // 1
        } else {
            return start(); // 'start
        }
    }

    int main() {
        int f = fib(10);    // 10 fib

        // fib(10) = 55
        printf("fib(10) = %d", f); 
    }

## Register custom C keyword

	#include <stdio.h>
	#include <string.h>

	#include "yalex.h"
	
	void replMessageCallback(const char* ptr) { printf("%s\n", ptr); }

	// sample implementation of the && / and token
	void sample_token_and_exec(yalex_world *world, stack_item **out) {
		yalex_stack_push_sp(world);
		//out gives you tokens down into the stack
		//out[1] is the first token we wrote out[0] the second
		SP.data.number = (out[1]->data.number && out[0]->data.number) ? 1 : 0;
		SP.meta = YALEX_TOKEN_NUM;
	}

	int main() {
		//basic initialization of runtime
		yalex_world world;
		yalex_init(&world, replMessageCallback);

		// in order for the `and` to work, we need two numbers to be on the stack
		// types here https://github.com/AlexanderBrevig/yalex/blob/master/yalex/src/yalex.h#L71
		char and_required_tokens[2] = { YALEX_TOKEN_NUM, YALEX_TOKEN_NUM };
		//register a new system token
		yalex_system_token_register("and", and_required_tokens, 2, sample_token_and_exec);

		//test it!
		yalex_repl(&world, "1 1 and"); //prints 1
		yalex_repl(&world, "1 0 and"); //prints 0
		
		char word[YALEX_SIZE_REPL_STR];
		while (1) {
			word[0] = 0;
			fgets(word, sizeof(word), stdin);
			yalex_repl(&world, word);
		}
		return 0;
	}
