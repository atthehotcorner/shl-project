# [xsh-ll] Project
Class project, details to come after due date.

## Known issues
```
When piping commands, [xshell] sometimes states that valid input is invalid. Nonetheless, it will still run.
[xshell] sometimes doesn't print the current directory after commands complete. Nonetheless, any commands given on the newline function as normal.
```

## Features
```
setenv variable word
printenv
unsetenv variable
cd (directory name)
alias
alias name
alias name word
unalias name
bye
```

Commands in the form of
```
cmd [arg]* [|cmd [arg]*]* [< fn1] [>[>] fn2] [2>fn3 || 2>&1] [&]

piping
input (in progress)
output
error
background (complete, needs testing)
```

Aliases
```
words set as aliases are replaced.
(If an aliases is infinite, the original term is not replaced at all)
```

Environment variable expansion 
```
${variable}
(any words that match an alias in a variable are not replaced, variables are static terms)
```

Wildcard matching
```
Not implemented
```

Tilde expansion
```
~ on its own expands to home directory of the current user
~name expands to name's home directory
(only when ~ is not in quotes)
```

File name completion
```
Not implemented
```

