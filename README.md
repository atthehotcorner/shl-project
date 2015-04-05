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
