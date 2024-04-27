# N-Puzzle-AI
An AI that can solve the N-puzzle

![](resources/images/github/NPuzzle-Board.png)

This Project is about creating an AI that can solve the N-Puzzle for N=3,4,5,...

The N-Puzzle is a generalization of the [8-Puzzle](https://en.wikipedia.org/wiki/15_Puzzle) problem invented by a postmaster that went by the name of Noyes Palmer Chapman, in the late 19th century.

As far as it concerns AI, this problem is often referred to as a "toy problem", meaning one of those problems whose solutions are not really relevant for the "real" world, but can be utilized as a test ground by researchers for newly discovered algorithms.
The N-Puzzle, in particular, is *NP-Hard*, so it's not an easy problem even if the word "toy" may make you think so; this problem has been studied a lot and it's very relevant for when it comes to evaluating the effectiveness and performance of AI search algorithms.

The particular area of AI in which we are moving refers to the design of problem solving agents; for this particular project, the environment is (for obvious reasons) assumed to be observable, discrete, known and deterministic.
With this in mind, the general high level architecture of our agent can be described by the following loop

- Formulate (a Goal and Problem)
- Search
- Execute

The agent will perform its actions only after a solution has been found by the search phase.
This is the matter of out project: design an efficient algorithm to search for solutions for our puzzle.

The precise requirements of the project are described [here](en.subject.pdf), with one of the only constraints being the use of the *A\* search algorithm* ore one of its variants.

I devised a variant of the A* search algorithm; particularly: a ***Parallel Bidirectional A**** search algorithm.

1. [The A* search algorithm](README.md#the-a*-search-algorithm)
2. [The N-Puzzle](README.md#the-n-puzzle)
3. [The Parallel Bidirectional A* search](README.md#the-parallel-bidirectional-a*-search)

## The A* search algorithm

## The N-Puzzle


## Valgrind with suppressions

```
valgrind \
--leak-check=full --show-leak-kinds=all --track-origins=yes \
--error-limit=no --verbose --gen-suppressions=all --log-file=valgrind-out.log \
./npuzzle
```

### Generate minimal suppression file

`cat valgrind-out.log | ./parse_valgrind_suppressions.sh > minimal.supp`
