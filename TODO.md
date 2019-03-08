# TODO

I pledge to try and keep this TODO up to date with actual code.

## DONE

- [x] basic operators	`+ - * /`
- [x] basic boolean     `< > == != && ||`
- [x] basic stack       `pop push dup clr` NB `clr` clears _everything_
- [x] basic literals    `true false`
- [x] basic prints      `print dump`  dump prints out all of stack and lambdas
- [x] handle no work    `nop` and `_` can be used as discard values, `nop` will immediately go, `_` must be resolved by `select` or `resolve`   
- [x] peek for hardware `address peek` will get the value at said address
- [x] string handling   `"string this is"`
- [x] select statement  `1 2 < yes no select` this will leave `yes` on stack since `1 2 <` is `true`
- [x] register set      `1 R0S` will set register `R0` to `1`
- [x] register set      setting a register will not upset stack, can pop pop R1S push push to look ahead   
- [x] register read     `R0R` will evaluate to the value of register `R0`
- [x] lambdas nop       `()` will simply evaluate and disappear
- [x] lambdas pack      `(1 2)` will be evaluated and leave `1 2` on stack
- [x] lambdas named     `:add1 (1 +)` can be invoked by `add1` and will then leave `1 +` on stack which will interpret
- [x] lambdas named     `add1:` will undefine `add1` if it exists
- [x] add register printout to `dump`  

## PLANNED

- [ ] hex parse by `0x` prefix
- [ ] `"1 2 3" resolve` adds 1 2 3 to stack
- [ ] redefine lambda by same name

## VERSION 1 RELEASE TASKS

- [ ] measure speed and memory footprint
- [ ] optimize speed and memory footprint
- [ ] replace C dependencies which may not be available on target platforms (string, stdio, stdlib, ctype etc)

## UNLIKELY

- [ ] add lambda requirements `<NUM NUM NAN>` syntax?
