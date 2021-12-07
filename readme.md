Programming: Principles and Practice Using C++, 2nd Edition
Chap 7 exercises 1,2, and 3

1)Allow underscores in the calculator’s variable names.

2)Provide an assignment operator, =, so that you can change the value of a variable after you introduce it using let.
Discuss why that can be useful and how it can be a source of problems.

3)Provide named constants that you really can’t change the value of. Hint: You have to add a member to Variable that
distinguishes between constants and variables and check for it in set_value(). If you want to let the user define constants
(rather than just having pi and e defined as constants), you’ll have to add a notation to let the user express that, for
example, const pi = 3.14;