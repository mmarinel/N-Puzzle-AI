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
	- [Existence of Solutions](README.md#existence-of-a-solution)
	- [Uniqueness of Optimal Solution](README.md#uniqueness-of-optimal-solution)
3. [The Parallel Bidirectional A* search](README.md#the-parallel-bidirectional-a*-search)

## The A* search algorithm

## The N-Puzzle

### Existence of a Solution
In order to not doom our agent to failure, we must understand when the puzzle is solvable

*Definition*: An inversion, inside a configuration, is formed whenever there is a tile with a greater number than another successive tile encountered while reading through the grid left-right, top-bottom starting from said tile

*Definition*: We define as polarity of a configuration the parity of its inversions

*Example*: The following configuration has an odd polarity (the '0' represents the empty tile)

|   |   |  | 
|---|---|---|
| 4  | 5  | 2  
| 1  |  0 |  8 
|  7 | 6  | 3  

starting from the rightmost and bottomost tile, and reading all the successive tiles, we have infact 

0 inversions for tile '3';
1 inversions for tile '6';
2 inversions for tile '7';
3 inversions for tile '8';
0 inversions for tile '1';
1 inversions for tile '2';
3 inversions for tile '5';
3 inversions for tile '4';

for a total of 13 inversions


*CASE 1*: N is odd

*Claim*: when a move is made, polarity does not change

*Proof*:

If the tile is moved horizontally, clearly the polarity does not change as the order remains untouched (we just moved the adjacent tile into the empty space without touching the order with respect to other tiles)

If a move is made vertically, we instead have just changed the order of this tile with the next or previous N - 1 tiles (if we moved upwards, the order with respect to the previous N - 1 tiles has been changed, if we moved downwards, the order with respect to the successive N - 1 has been changed).

Now, since N is odd, n' = N - 1 is EVEN.
So 2t = n' for some t.

if k was the number of inversion that the moved tile had with respect to the other n' tiles, we would have

2t = n' = (n' - k) + k

Now, we fixed the k inversions but messed up the remaining (n' - k); so the new number of inversions can be obtained by subtracting, from the previous total number of inversions, the number of inversions that the move fixed and then adding the new number of inversions that the move messed up involving these tiles.

This gives the following factor

-k + (n' - k) = -2k + n' = -2k +2t = 2(t-k).

Which is an even number and therefore does not change the parity of the previous number of inversions.


*Theo*: When N is ODD, if the puzzle is solvable then the polarity of the initial state must be EVEN.

*Proof*: Since making a move does not change polarity, and since the goal state has an EVEN polarity because there are no inversions in it, then it would be impossible to start from an initial state of ODD polarity and reach the goal state.

CASE 2: N is EVEN

In this case, n' = N - 1 = 2t + 1 for some t.

So, when we make a vertical move, we are now changing the order with respect to an ODD number of tiles and the previous argument does not hold anymore.

*Claim* 2.1: Each horizontal move does not change polarity and parity of empty tile row index (counting 1 from bottom)

*Proof*: The first assertion has already been proved, for what concerns the second one, obviously moving the tile horizontally does not change its current row.

*Claim* 2.2: Each vertical move changes both polarity and parity of empty tile row index

*Proof*: For the second assertion this is obvious, as we can only go up or down one row at a time.
For the second assertion, assuming we have k inversions with the soon to be involved n' tiles, we have

2t + 1 = n' = (n' - k) + k

as before, to obtain the new number of inversions we have to add the following term

+(n' - k) - k = n' - 2k = 2t + 1 - 2k = 2(t - k) + 1

Which is an ODD number and since ODD + ODD = EVEN \&\& EVEN + ODD = ODD, the following term changes the polarity.

*Theo*: When N is EVEN, if the N-Puzzle is solvable then the initial state must exhibit the following property

(polarity EVEN \&\& parity of empty tile row index ODD) OR (polarity ODD \&\& parity of empty tile row index EVEN)

*Proof*: This is because the final state has both EVEN polarity (as there are no inversions in it) and ODD parity of empty tile row index (as the empty tile is in the bottomost rightmost corner, so at row 1).

Given the previous invariant, the only way to reach such a state is when the initial configuration does not share parity of inversions with parity of empty tile row index.

</br>
The following theorems provide a necessary, but not sufficient condition for the existence of solutions.
To prove this condition is also sufficient, one can observe that each configuration can be uniquely identified with the pair

- number of inversion
- position of empty tile

Let's in fact imagine there exist two configurations sharing the same values for both properties.
One of them must be turned to the other one by moving one of the tiles; we cannot move the empty tile so we must swap at least two tiles.

When we swap two tiles, the order between them gets broken, and that gives at least a -1 or +1 to the number of inversions; moreover, if there were other tiles in between the two swapped tiles, each of those tiles would either add a +1,-1 to the number of inversions with respect to both swapped tiles.

So each tile in between would add either
- 0 (the leftomst swapped tile either solved or introduced an inversion with the in-between tile and rightmost did the opposite -- one fixed, one broken--),
- -2 (the tile in between was greater than the rightmost swapped but lesser than the leftmost swapped one),
- +2 (the three tiles were perfectly ordered relatively to each other, so the move introduced the maximum number of inversions for the in-between tile).

So the number of inversions definitely does change.
Therefore, it is not possible to turn one configuration into the other.

Now, one can prove that using only legal moves, starting from the goal state, one can reach configurations for all values of those two properties with respect to the theorems we just proved; and since there must be only one configuration for all set of values for those two properties, each configuration can be effectively reached by the goal state.

### Uniqueness of Optimal Solution

There can be many solutions to our puzzle, each of them solving it with a different number moves.
An optimal solution, in our case, is the one involving the minimal number of moves.
One thing that would help our agent is to find out wether there is one or multiple of such solutions (more on that in the last chapter).

Let's assume there are 2 optimal solutions with the following sequence of moves having the same length

A_1 A_2 A_3 --- A_k

B_1 B_2 B_3 --- B_k

If we start from the initial state and solve two N-puzzle instances for each of the action sequences, after applying a certain number of moves the two paths would have diverged; meaning they will be on different configurations (otherwise they would be the same exact solution)

*Claim*: Given two different configurations on two different action sequences, after applying one move from each of the sequences the resulting configurations would still be different

*Proof*: 

CASE 1: Empty tile in same place

If the next moves are different, then the empty tile would go onto two different places and therefore the resulting configurations would still be different.

If the moves are the same, either the same number gets dragged in the empty place, but in this case there must be an untouched tile which is different in both configurations (otherwise the two configurations would be the same), or a different number gets dragged in the empty place, making the two resulting configurations different.

CASE 2: Empty tile in different place

The only thing to prove is that when the empty tile gets moved into the same square in both configurations, the resulting configurations would still be different.

When the empty tile gets moved in the same square, the number that was in that square ends up in a different one in both configurations (as the empty tile starting point was not the same in both configurations); so, since each number in the grid is unique, the two resulting configurations would still be different.

</br>
Now, with the previous claim in mind, there can only be one action sequence that gets to the goal within a minimal number of moves because each move produces two different configurations in two different actions sequences; therefore the second one will need some more moves to reach the goal state.

