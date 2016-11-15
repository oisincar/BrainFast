#BrainFast

My take on an optimising interpreter for brainf*ck.

It can run hanoi.b in 0.29 seconds, mandel.b in 7.26 seconds, and long.b in 2.4 seconds
which makes it arguabley the second fastest brainfuck interpreter in the world right now.

For a comparison of times for these programs run in the more well-known interpreters see:
http://sree.kotay.com/2013/02/implementing-brainfuck.html

Make sure to compile with optimisation for the best results.

``` g++ BrainFast -O3 ```


*Todo:* Write more about optimising techniques used...
