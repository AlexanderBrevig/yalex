# yalex

> yalex is a minimalist functional scripting language for embedded systems

Yet Another LEXer. Also, I'm Alex.
It's worth noting that y is kind of similar to lambda which is popular for indicating functional programming language.
What else? To lex is the process of taking text and convert it to tokens. Guess what, yalex does this too!

Yalex uses the reverse polish notation and FORTH as it's main source of inspiration. (I learned about FORTH after I started this project)

You always build up a stack of things and can use operators to modify said stack - and of course produce effects. In yalex, these operators are functions and one function can be the operand of another.

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