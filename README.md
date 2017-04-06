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

// `a !` => _not `a`_

// `a b !` => _`a` is not `b`_

// `a b |` => _`a` or `b`_

// `a b &` => _`a` and `b`_

// `a b ^` => _`a` xor `b`_

### Stack operations

`a pop` => _pop `a` off the stack so if [1,2,a] then pop -> [1,2] and the pop takes value of a_

`a dup` => _dublicates `a` top of stack so if [1,2,a] then dup -> [1,2,a,a]_

`a rot` => _rotate the stack so if [1,2,3] then [3,2,1]_

### Data manipulation

// `a b set` => _set the value of `a` to the value of `b`_

// `a b read` => _read value from `a` with bit mask `b`_

// `a b write` => _write value to `a` with bit mask `b`_

// `a b index` => _indexes a by b places, similar to `a[b]` can be followed by a set_

// `... x cat` => _string concatenate `x` number of values off of stack_

### yalex specific

`a print` => _print `a` to UART/radio/stdout_

// `a? b? (` => _start a block with optional name `a` and optional arg length `b`_

// `... )` => _end a block_

// `a B when` => _when event `a` is captured then block `B` is executed_

// `a X Y either` => _if `a` then block `X` else block `Y`_

