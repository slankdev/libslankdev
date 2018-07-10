
```
$ ps alx  | awk '{printf ("%d\t%s\n", $8,$13)}' | sort -nr | head -10
```
