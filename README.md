# N-Puzzle-AI
An AI that can play the N-puzzle game

## Valgrind with suppressions

```
valgrind \
--leak-check=full --show-leak-kinds=all --track-origins=yes \
--error-limit=no --verbose --gen-suppressions=all --log-file=valgrind-out.log \
./npuzzle
```

### Generate minimal suppression file

`cat valgrind-out.log | ./parse_valgrind_suppressions.sh > minimal.supp`
