push [ 13* dx ]
push bx - 3
ja START
push 12
pop [ 5 + ax ]
jne NEXT
cos
dump
NEXT:
dump
dump
START:
hlt