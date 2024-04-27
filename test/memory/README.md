# Valgrind QT clash

## Valgrind with suppressions

```
valgrind \
--leak-check=full --show-leak-kinds=all --track-origins=yes \
--error-limit=no --verbose --gen-suppressions=all --log-file=valgrind-out.log \
./npuzzle
```

### Generate minimal suppression file

`cat valgrind-out.log | ./parse_valgrind_suppressions.sh > minimal.supp`

## References
- [Official Open Issue](https://bugreports.qt.io/browse/QTBUG-8756)
- [Open Issue 2](https://forum.qt.io/topic/22879/qt5-qfiledialog-is-leaking-memory/12)
- https://stackoverflow.com/questions/15674422/unable-to-detect-why-qdialog-is-memory-leaking
- https://wiki.wxwidgets.org/Valgrind_Suppression_File_Howto
- https://wiki.wxwidgets.org/Parse_valgrind_suppressions.sh
- https://stackoverflow.com/questions/24512584/qfiledialog-memory-leak
- https://stackoverflow.com/questions/9174326/suppression-files-for-qt-memory-leaks-with-valgrind
- https://stackoverflow.com/questions/59873842/c-qt-valgrind-uninitialized-bytes