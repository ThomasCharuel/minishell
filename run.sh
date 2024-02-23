#clear && make && valgrind --leak-check=full --show-leak-kinds=all -s --suppressions=readline.supp --track-origins=yes ./minishell
clear && make && valgrind --leak-check=full --track-fds=yes --suppressions=readline.supp --trace-children=yes -s ./minishell

#clear && make && ./minishell

## more lines
##

##
