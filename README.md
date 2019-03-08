# yalex

> yalex is a minimalist functional scripting language for embedded systems

Yet Another LEXer. Also, I'm Alex.
It's worth noting that y is kind of similar to lambda which is popular for indicating a functional programming language.
What else? To lex is the process of taking text and convert it to tokens. Guess what, yalex does this too!

Yalex uses the reverse polish notation and FORTH as it's main source of inspiration. (I learned about FORTH after I started this project)

You always build up a stack of things and can use operators to modify said stack - and of course produce effects. In yalex, these operators are functions and one function can be the operand of another.

## Code example; (almost) fibonacci

    :fibstep (R1R R2R + R3S pop R2R R1S pop R3R R2S pop R4R 1 + R4S pop rec)
    :rec (R4R R0R 1 + < 'fibstep _ select)
    :fib (R0S 0 R1S 1 R2S 0 R3S 1 R4S rec pop pop pop pop pop pop R3R)

Here it is, side by side a similar C implementation:

    #include <stdio.h>
    /*  
        :fibstep (R1R R2R + R3S pop R2R R1S pop R3R R2S pop R4R 1 + R4S pop rec)
        :rec (R4R R0R 1 + < 'fibstep _ select)
        :fib (R0S 0 R1S 1 R2S 0 R3S 1 R4S rec pop pop pop pop pop pop R3R)
        10 fib
        89
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

    int fib(int el)     
    {   
        n = el;             // R0S //reads previous stack item as input
        t1 = 0;             // 0 R1S
        t2 = 1;             // 1 R2S
        nextTerm = 0;       // 0 R3S
        i = 1;              // 1 R4S
        rec();              // rec
                            // pop pop pop pop pop pop 
        return nextTerm;    // R3R
    }

    int main() {
        int f = fib(10);    // 10 fib

        // fib(10) = 89
        printf("fib(10) = %d", f); 
    }

## Get the code and start to play

	git clone --recurse-submodules https://github.com/AlexanderBrevig/yalex/

## Tested functionality

	+ - * /
	< > == !=
	|| &&
	dup push pop
	print dump
	peek
	select
	()
	:lambda (1 1 +)
	lambda
	lambda:
	1 R[0..15]S
	R[0..15]R

## Register custom C keyword

	void replMessageCallback(const char* ptr) { printf("%s\n", ptr); }

	// sample implementation of the && / and token
	void sample_token_and_exec(yalex_world *world, stack_item **out) {
		yalex_stack_push_sp(world);
		//out gives you tokens down into the stack
		//out[1] is the first token we wrote out[0] the second
		SP.data.number = (out[1]->data.number && out[0]->data.number) ? 1 : 0;
		SP.meta = MICROLANG_NUM;
	}

	int main() {
		//basic initialization of runtime
		yalex_world world;
		yalex_init(&world, replMessageCallback);

		//register a new system token
		char basic_op_req[2] = { MICROLANG_NUM, MICROLANG_NUM };
		// in order for the and to work, we need two numbers to be on the stack
		yalex_system_token_register(&world, "and", basic_op_req, 2, sample_token_and_exec);

		//test it!
		yalex_repl(&world, "1 1 and"); //prints 1
		yalex_repl(&world, "1 0 and"); //prints 0
	}
