# [xsh-ll] Project
Class project, details to come after due date.

## Features
```
setenv variable word
printenv
unsetenv variable
cd word (directory name)
alias
alias name
alias name word
unalias name
bye
```

Commands in the form of
```
cmd [arg]* [|cmd [arg]*]* [< fn1] [ >[>] fn2 ] [ 2>fn3 || 2>&1 ] [&]
```

```
piping (needs testing)
input (needs testing)
output (complete, needs testing)
error (needs testing)
background (not yet implemented, code exists)
```

Aliases
```
words set as aliases are replaced.
If an aliases is infinite, the original term is not replaced at all.
```

Environment variable expansion 
```
${variable}
(any words that match an alias in a variable is not replaced, they are static terms)
```

Wildcard matching
```
Not yet implemented
```

Tilde expansion
```
~ on its own expands to home directory of the current user
~name expands to name's home directory (not yet implemented)
(only when ~ is not in quotes)
```

File name completion
```
Not yet implemented
```

