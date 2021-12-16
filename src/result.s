.data

.globl main
.text
l.s $f0, a

l.s $f1, b

sub.s $f2, $f0, $f1
mov.s $f0, $f2

mov.s $f12, $f2
li $v0, 2
syscall
c.ge.s $f0, $f1

mov.s $f12, $f0
li $v0, 2
syscall
c.le.s $f0, $f1

mov.s $f12, $f0
li $v0, 2
syscall
c.eq.s $f0, $f1

mov.s $f12, $f0
li $v0, 2
syscall

