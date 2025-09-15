## optimal5

The `optimal5` repository contains a database of the lowest-cost
implementations of all 616126 NPN classes of 5-input Boolean functions,
along with code for efficiently retrieving an optimal circuit for a
given truth table. That is to say, given an arbitrary one of $`2^{32}`$
possible 5-input Boolean functions, `optimal5` will:

 - determine to which of the 616126 equivalence classes (under permutation
   and negation of inputs/outputs) the function belongs, by obtaining a
   canonical representative;
 - retrieve an optimal circuit for that canonical representative;
 - transform that circuit to give the original function rather than
   the canonical representative of the function.

This approach allows fast lookups of optimal circuits without requiring
excessive memory consumption (as would be the case if all $`2^{32}`$
truth tables were stored in a hashtable). We deviate from the existing
academic literature by using _Boolean canonisation_ instead of _Boolean
matching_.

![penteract](images/polytope4.png)

The optimal implementations themselves were computed by Donald Knuth
whilst writing _The Art of Computer Programming_ using his software
[BOOLCHAINS](https://www-cs-faculty.stanford.edu/~knuth/programs.html).
Owing to Knuth's approach, the solutions were originally split across
three places:

 - a tool `5chainsx.w` which explains how to recursively express a
   given function in terms of simpler functions (either in a 'top-down'
   or 'bottom-up' manner);
 - several text files containing 'special' irreducible chains for the
   functions where neither the top-down or bottom-up approach yields
   the optimal solution;
 - the README file, which contained an erratum with six special chains
   of length 11.

The purpose of this repository is twofold:

 - to unify these solutions into a single structured database for greater
   convenience;
 - to provide a function to efficiently obtain the optimal circuit for
   any of the $`2^{32}`$ Boolean functions of five inputs.

A combination of group-theoretic ideas and extensive precomputation enabled
the amortized circuit lookup time to be reduced to 200 nanoseconds.

### Description of the tools

The program `human_readable.cpp`, when compiled, accepts truth tables
provided via the standard input stream (with one truth table per line).
When provided with the input `0x169ae443`, for example, it will promptly
regurgitate the following sequence of operations:

    x6 = x4 & x2;
    x7 = x4 | x2;
    x8 = x5 &~ x7;
    x9 = x7 &~ x5;
    x10 = x1 ^ x8;
    x11 = x10 & x3;
    x12 = x11 | x9;
    x13 = x6 & x1;
    x14 = x12 &~ x13;
    x15 = x4 &~ x3;
    x16 = x14 ^ x15;
    x17 = x16 ^ x1;
    result = x17;

(Note: the actual program prints all of this on one line, so that lines
in the output are in bijective correspondence with lines in the input.
To reproduce the effect shown above, it suffices to insert line-breaks
after each semicolon.)

The variables `x1` through to `x5` represent the five inputs, and higher
subscripts indicate intermediate values. Exactly four binary Boolean
operators can appear (`&`, `|`, `^`, and `&~`). If the truth table you
provide is not zero-preserving (or, equivalently, it is numerically
greater than `0x7fffffff`), then the final statement will include a
unary negation `~` operation.

The program `full_check.cpp` is only designed to run in the continuous
integration process, to ensure absolutely every one of the $`2^{32}`$
possible inputs works correctly. Since each call takes 200 nanoseconds
on a fast computer, the entire process takes just under 15 minutes.

The program `hamilton.cpp` exists only to compute and print the header
file `paths2.h`. It computes shortest-length paths for visiting all
elements of various subgroups of the hyperoctahedral group, and
converts those paths into straight-line C functions and a table of
function pointers.

### Frequently asked questions

#### Why restrict to just the 'zero-preserving' gates?

The restriction to zero-preserving gates is not a necessity, but rather an
optimisation that can be done 'for free'. In particular, after topologically
sorting the gates in the Boolean circuit, one can iterate through them
sequentially. Whenever a non-zero-preserving gate is encountered, it
can be split into a zero-preserving gate followed by a NOT gate. This
NOT gate can be 'pushed along' and incorporated into the following gate(s).

This process will wring out all of the NOT gates (except possibly at the
outputs of the circuit, depending on whether the whole circuit is itself
zero-preserving), resulting in a circuit composed only of zero-preserving
2-input gates. There are only 8 of these, namely:

 - constant 0;
 - a;
 - b;
 - a XOR b;
 - a AND b;
 - a OR b;
 - a AND NOT b;
 - b AND NOT a;

and the first three of those can be removed by pruning unused variables
and propagating constants. The result is composed entirely of 2-input
gates, no more than were present in the original circuit, from the set
{XOR, AND, OR, AND-NOT}.

Another reason to especially favour these four gates is that they are
precisely the bitwise operators supported by many vectorised instruction
sets, such as SSE and AVX. Minimising the number of zero-preserving
2-input gates, for these architectures at least, maps exactly onto the
problem of minimising instruction count.

#### Why are only 2-input gates used as primitives?

Again, another good question: both AVX-512 with its `vplogternd`
instruction, and the assembly code of modern NVIDIA GPUs, support
instructions which effect arbitrary 3-input lookup tables. Similarly,
FPGAs support primitives which effect even higher-arity lookup tables.

The reason is that a circuit of 2-input zero-preserving gates forms
a good 'intermediate representation' on which one can apply various
optimisation strategies to reduce its complexity (gate count and
depth) without worrying about the details of the target architecture.
After iterating through rounds of optimisation, the intermediate
representation can undergo a final architecture-specific optimisation
step which takes full advantage of advantage of the available set of
instructions and takes into consideration the idiosyncracies of the
architecture (instructions latency/throughput, register count, etc.).

#### What if my circuit has more than 5 inputs?

That is a good question. Logic synthesis tools such as Berkeley's ABC
operate on huge circuits by performing local optimisations on small
subcircuits, typically 4-input 'cuts'. In a recent paper, Nan Li and
Elena Dubrova showed that superior performance could be attained by
adding a selection of 5-input replacements to the rewriting engine:

[AIG rewriting using 5-input cuts](https://arxiv.org/abs/1108.3675)

The approach of `optimal5` is to take this to its ultimate logical
conclusion: instead of curating a set of just over 1000 common NPN
classes of Boolean functions, provide optimal implementations of all
616126 of them! Whereas Dubrova and Li used Boolean matching, our
approach of Boolean canonisation scales much better: the lookup time
does not increase as a result of having 616126 circuits as opposed to
1000.

#### What future directions are planned?

Currently, `optimal5` is a database of optimal implementations of 5-input
1-output Boolean functions. There are plans to generalise this by adding
4-input 2-output Boolean functions; when used in a rewriting engine,
this would allow automatic 'logic sharing' where two computations are
refactored so as to share a common intermediate subexpression.

Beyond that, making a full rewriting engine would be the ultimate goal.
Although I started this as a spin-off from my cellular automata library,
[lifelib](https://gitlab.com/apgoucher/lifelib), a rewriting engine is
vastly more useful in practice: it could be used inside an optimising
compiler, or an FPGA logic synthesis tool, or in many other applications.

#### Why does optimal5 have that geometric design as its logo?

This is the Petrie projection of the five-dimensional cube into the
two-dimensional plane. The significance of the five-dimensional cube
is that a 5-input Boolean function can be viewed as a way to partition
its vertices into two classes, and the automorphisms of the cube
correspond exactly to permuting and negating the inputs of the function.
When designing the algorithm for obtaining the canonical representative
of a function's NPN equivalence class, it was helpful to think in these
geometric terms.


