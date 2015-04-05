# sh-llX Project
Class project, details to come after due date.

### Built-in Commands
**setenv variable word**

Status: Complete

This command sets the value of the variable variable to be
word.

**printenv**

Status: Complete

This command prints out the values of all the environment variables, in the format variable=value, one entry per line.

**unsetenv variable**

Status: Complete

This command will remove the binding of variable. If the variable is unbound, the command is ignored.

**cd word directory name**

Status: Partial *tilde not implemented*

This command changes the current directory to the one named. You must handle cd with no arguments, to take you back to the home directory, i.e. it should have the same effect as cd ˜ (see Tilde Expansion).

**alias**

Status: Complete

The alias command with no arguments lists all of the current aliases.

**alias name word**

Status: Partial *nesting not implemented, shorthand not implemented*

This alias command adds a new alias to the shell. An alias is essentially a shorthand form of a long command. For example, you may have an alias *alias lf "/bin/ls -F"* set up so that whenever you type *lf* from the command line, the command that is executed is */bin/ls -F*. Note that alias expansion is only performed on the first word of a command. However, aliases may be nested and the shell detects when an infinite alias expansion occurs.

**unalias name**

Status: Complete

Remove the alias for name. The unalias command is used to remove the alias for name from the alias list.

**bye**

Status: Complete

Gracefully quit the shell. The shell should also exit if it receives the end of file.

### Other Commands
Status: Not implemented

  cmd [arg]* [|cmd [arg]*]* [< fn1] [ >[>] fn2 ] [ 2>fn3 || 2>&1 ] [&]

Any command of this form can be accepted along with its arguments, pipes, and
I/O redirection if present. Note that the I/O redirection can only appear at the
end of the line in your shell. The construct 2>file redirects the standard error of
the program to file, while 2>&1 connects the standard error of the program to its
standard output. If cmd does not contain a /, the shell must check the directories on
the path that is the value of the environment variable path for the commands. Only
check those directories that are on the path, i.e., if the current directory is not in the
path, do not look in the current directory. Only if the file exists and is executable, it
should be run. You must also be able to handle piping and I/O redirection on builtin
commands. If & exists at the end of the line, then the shell will execute this command
in the backgound. If & doesn’t exist, then the shell will wait for this command to
finish

### Environment Variable Expansion ${variable}
Status: Not implemented

It is also possible to include environment variables as part of words inside command lines. The shell reads all the characters from ${ to the next } and assumes it is the name of a variable. The value, if any, of the variable is substituted.

### Wildcard Matching
Status: Not implemented

Many shells do filename generation with wildcarding. This shell will implement a subset of the functionality found in most shells. Before a command is executed, each command word should be scanned for the characters * and ?. If one of these characters appears the word is regarded as a pattern. The word is replaced with alphabetically sorted filenames that match the pattern. If no filename is found that matches the pattern, the word is left unchanged.

A * matches any string, including the null string. A ? matches any single character. The character ‘.’ at the start of a filename or immediately following a ‘/’, as well as the character ‘/’ itself, must be matched explicitly.
