push 0 ; attempts
push 62 ; random number
; guess
def 1
; check if guess limit
swap
push 1
add
push 5
if less
goto 7
pop
swap
; compare guess number
read "Input number: "
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
