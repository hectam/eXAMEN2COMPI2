.data

.globl main
.text
li.s $f0, 5.6
li.s $f0, 6.7
l.s $f0, a

l.s $f1, b

sub.s $f2, $f0, $f1
mov.s $f0, $f2
div.s $f2, $f0, $f1
mov.s $f0, $f2
c.ge.s $f0, $f1
c.le.s $f0, $f1
c.eq.s $f0, $f1

