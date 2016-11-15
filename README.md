#BrainFast

My take on an optimising interpreter for brainf*ck.

It can run hanoi.b in 0.29 seconds, mandel.b in 7.26 seconds, and long.b in 2.4 seconds
which makes it one of the fastest brainfuck interpreters in the world right now. (Maybe even second fastest!(?!))

For a comparison of times for these programs run in the more well-known interpreters see:
http://sree.kotay.com/2013/02/implementing-brainfuck.html

Make sure to compile with optimisation for the best results.

```
g++ brainfast.cpp -O3
time ./a.out < file.bf
```


*Todo:* Write about optimising techniques used.
