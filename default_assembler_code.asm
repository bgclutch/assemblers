push 21
push AX
push 1 + AX
push [ 1 + AX ]
add
mul
cos
ja NEXT
in
jne START
sin
dump
START:
out
push 14
NEXT:
dump
dump
hlt