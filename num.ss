push 0
push 62
; guess
def 1
swap
push 1
add
push 5
if more
read "Input number: "
pop
swap
if same
goto 5
if less
goto 4
if more
goto 3
; bigger
def 4
print "The number is bigger"
pop
goto 1
; smaller
def 3
print "The number is smaller"
pop
goto 1
; win
def 5
print "You win!"
goto 0
; loose
def 7
print "You loose!"
; exit
def 0
