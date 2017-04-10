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

    'hello world' print
    
Call this program some time later by simply issuing this token to the stack ` hw ` and you'll be the hero for printing out this fine text.

### Tech Tip: everything is a token
A function, a list, a number, a string, an operator. We are all data. We are all tokens. 
We are legion. We are many. Expect us.

## Define a function that takes 1 argument

    "sleep an amount of ms, be sure to prepare stack with at least 1 value"
    sleepms 1 ( pop delayms )
    
## Peripheral interface

We can do PORT reads, writes, we can do bit trickery and we can call other functions. This depends on the particular Board Support Package you're running. For this example I assume that PORTA is exposed and handles a port write for the microcontroller.

    "blink two leds in a loop"
    (
        PORTA 0b11 PORTA 0b11 read ! write 
        1000 sleepms 
    ) loop

## Control structures

    errors 0 = ( 'no errors' print ) ( errors ' errors' 2 cat print ) ?

## Event handler

    errors 2 > ( PORTA LED_RED 1 write ) when

## List of tokens

All entries use `a` and `b` to indicate operands to the operator. 
Capital letters designate blocks.
An operand is either a single token, or a block.
... indicate a variable list of operands.

I use this convention for representing status:
 * // `not imlemented yet`
 * `implemented`

### Math

`a b +` => _ plus operator of `a` + `b`_

`a b -` => _minus operator of `a` - `b`_

`a b /` => _division of `a` / `b`_

`a b *` => _multiplication of `a` * `b`_

`a b >` => _is `a` greater than `b`?_

`a b <` => _is `a` lesser than `b`?_

`a b =` => _is `a` and `b` equal?_

`a ?` => _force `a` to a truthy value where 0 == false, rest is true_

`a b !` => _`a` is not `b`_

`a b |` => _`a` or `b`_

`a b &` => _`a` and `b`_

`a b ^` => _`a` xor `b`_

### Stack operations

`a pop` => _pop `a` off the stack so if [1,2,a] then pop -> [1,2]_

`a dup` => _dublicates `a` top of stack so if [1,2,a] then dup -> [1,2,a,a]_

`... clr` => _clears the stack so if [1,2,a] then clr -> []_

`a rot` => _rotate the stack so if [1,2,3] then rot -> [3,2,1]_

### Data manipulation

// `a b set` => _set the value of `a` to the value of `b`_

// `a b read` => _read value from `a` with bit mask `b`_

// `a b write` => _write value to `a` with bit mask `b`_

// `a b index` => _indexes a by b places, similar to `a[b]` can be followed by a set_

// `... x cat` => _string concatenate `x` number of values off of stack_

### yalex specific

`a print` => _print `a` to UART/radio/stdout_

`true` => _a truthy value of 1.0f_

`false` => _a falsy value of 0.0f_

 `nop` => _no operation, empty dummy token (for explicit I DON'T WANT ANYTHING HERE)_

// `a? b? (` => _start a block with optional name `a` and optional arg length `b`_

// `... )` => _end a block_

// `a B when` => _when event `a` is captured then block `B` is executed_

// `a X Y ?` => _if `a` then block `X` else block `Y`_

// `A loop` => _loop block `A` until it is `kill`ed_

// `a kill` => _kill variable `a` if it is user specified, builtins cannot be deleted_

