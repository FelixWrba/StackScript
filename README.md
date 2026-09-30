# StackScript
StackScript is a stack-based, interpreted, fast scripting language written in C. The file extension is `.ss`.

## Run
1. Compile StackScript: `gcc main.c -o ss`
2. Run any program: `./ss loop.ss`

## Syntax
The syntax consists of multiple instruction sets: `<command> [<argument>]\n`

### Commands
* push `<number>`: Push a value to the stack
* pop: Pop the top value from the stack
* swap: Swap the top two values
* dup: Duplicate the top value and push it to the stack

* add: Pop the top two values and push their sum
* sub
* mul
* div
* mod

* print `["<text>"]`: Print the top value of the stack or a given text
* read `["<text>"]`: Read a user input and prepend a text when given
* debug: Print the stack

* if `<less | same | more>`: Skip the next instruction when given condition is not met
* def `<number>`: Define a section
* goto `<number>`: Jump to a defined section
