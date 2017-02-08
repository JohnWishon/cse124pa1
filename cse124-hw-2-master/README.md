# cse124-hw2

Starter code for CSE 124's Homework 2, which is the echo client/server
that we went through in class.

Objective:

1. Modify the server so that it returns N copies of the string that the client
sends it, intead of just one copy.  N is passed into the server as a
command-line argument

2. Modify the client so that it prints out the value of N, learned by analyzing
the length of the string that the server returns.  The only way the client
should learn N is by analyzing the server's reply (not via e.g., a command-line
option or anything like that).  We've already included the code that prints
N--you just need to calculate it.

You only have to examine the length of the returned string to determine N.  We
won't be analyzing or testing the case where the server returns bad data, some
fractional portion of the message, or any other error conditions.  Please
don't modify the printf() statement that prints N
