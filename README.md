# kash
### C posix shell

Design philosophy:

1. `fast is better than slow`


Features:

+ line editing
+ command execution
+ changing directory
+ command chaining with ;
+ reading from file/stdin
+ limited non persistent history
+ .kashrc file
+ alias builtin for single commands
+ singular pipes
+ completion for items in current directory


The goal is to implement most of the POSIX standard and be the fastest shell to
do so. This is just a project for fun and practicing C therefore no guarantee on 
how long this will take ;)

next TODOs:

+ piping and output redirection
+ conditional chaining
+ export builtin
+ improve history
+ loops and boolean expressions
