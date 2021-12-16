.data

.globl main
.text
c.eq.s $f0, $f1

bc1f endif0
l.s $f0, b

move $v0, $f0


endif0 :
c.eq.s $f0, $f1

bc1f endif1
l.s $f0, a

move $v0, $f0


endif1 :
c.eq.s $f0, $f1

bc1f endif2
l.s $f0, a

move $v0, $f0


endif2 :
c.ge.s $f0, $f1

bc1f endif3
l.s $f0, a

move $v0, $f0


endif3 :
l.s $f0, a

move $v0, $f0
l.s $f0, b

mov.s $f12, $f0
li $v0, 2
syscall

