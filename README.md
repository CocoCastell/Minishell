# About Minishell

This two-person project was made with [@satodv](https://github.com/satodv).  
The aim of the project is to create our own (mini) version of a shell, using bash as a reference.  

# How to run

First, compile in the directory with  
```
make
```
Then, run the programm with  
```
./minishell
```

# Implementation

The code is divided into three main parts:  

Tokenization:  
Splits the input written by the user into individual tokens (commands, arguments, operators...).  

Parsing:  
Parses the tokens, checks for syntax errors and redirection issues.  
We use a recursive binary tree as the main data structure to represent the command structure.  

Execution:  
Executes the parsed commands, handling pipes, logical operators (AND, OR), and implementing built-in shell commands.  

# Grade
