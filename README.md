#BrainFast

My take on an optimising interpreter for brainf*ck.

Here's some of BrainFast's times for some common benchmark brainf*ck programs, ran on my old laptop (2.2 GHz Intel Core i7, 8gbs of ram), where times are an average of 10.
 
| Program  | Time (sec) |
| ---------|-----------:|
| Mandel.b | 6.385      |
| Hanoi.b  | 0.292      |
| Long.b   | 1.488      |


These times put it comfortably in 3rd place in the very prestigious and rigorous competition of "Fastest BF Interpreters I Could Find".

For a comparison of times for these programs run in the more well-known interpreters see:
https://esolangs.org/wiki/User:David.werecat/BFBench

Make sure to compile with optimisation for the best results.

```
g++ -O3 -march=native -std=c++11 -o brainf brainfast.cpp
time ./brainf < file.bf
```

_Todo:_ Write about optimising techniques used.
