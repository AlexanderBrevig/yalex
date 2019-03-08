# TODO

I pledge to try and keep this TODO up to date with actual code.

## DONE

- [x] basic operators	`+ - * /`
- [x] basic boolean     `< > == != && ||`
- [x] basic stack       `pop push dup`
- [x] basic prints      `print dump`  dump prints out all of stack and lambdas
- [x] peek for hardware `address peek` will get the value at said address
- [x] string handling   `"string this is"`
- [x] select statement  `1 2 < yes no select` this will leave `yes` on stack since `1 2 <` is `true`
- [x] register set      `1 R0S` will set register `R0` to `1`
- [x] register read     `R0R` will evaluate to the value of register `R0`
- [x] lambdas nop       `()` will simply evaluate and disappear
- [x] lambdas pack      `(1 2)` will be evaluated and leave `1 2` on stack
- [x] lambdas named     `:add1 (1 +)` can be invoked by `add1` and will then leave `1 +` on stack which will interpret
- [x] lambdas named     `add1:` will undefine `add1` if it exists

## PLANNED

- [ ] hex parse by `0x` prefix
- [ ] string parse by `' '` clause
- [ ] `clr` command
- [ ] have registers not affect stack (ability to register popped values)
- [ ] `nop`
- [ ] `true` `false`
- [ ] redefine lambda by same name
- [ ] add lambda requirements `<NUM NUM NAN>` syntax?
- [ ] add register printout to `dump`
- [ ] measure speed and memory footprint
- [ ] optimize speed and memory footprint
- [ ] replace C dependencies which may not be available on target platforms (string, stdio, stdlib, ctype etc)
