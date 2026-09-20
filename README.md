# StackScript
StackScript is a stack-based, interpreted, fast scripting language written in C. The file extension is `.ss`.

## Run
1. Compile StackScript: `gcc main.c -o ss`
2. Run any program: `./ss loop.ss`

## Syntax
The syntax consists of multiple instruction sets, an instruction is a pair of command and an optional arguement: `<command> [<value>]\n`

### Commands

* push `<number>`: Push a value to the stack
* pop: Pop the top value from the stack
* swap: Swap the top two values
* dup: Duplicate the top value and push it to the stack

* add: Pop the last two values and push their sum
* sub: Pop the last two values and push their difference
* mul: Pop the last two values and push their product
* div: Pop the last two values and push their division
* mod: Pop the last two values and push the result of the modulo operation

* print `["<text>""]`: Print the last value of the stack or a text when given
* read `["<label>"]`: Read an input from the console and prepend a label text when given
* debug: Prints the stack

* if `<less | same | more>`: Execute the next instruction only of condition is met
* def `<number`: Define a section
* goto `<number>`: Jump to a previosly defined section
