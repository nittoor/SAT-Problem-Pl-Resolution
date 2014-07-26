SAT-Problem-Pl-Resolution
=========================

Suppose you have a wedding to plan, and want to arrange the wedding seating for a
certain number of guests in a hall. The hall has a certain number tables for seating.
Some pairs of guests are couples or close Friends (F), and want to sit together at the
same table. Some other pairs of guests are Enemies (E), and must be separated into
different tables. The rest pairs are Indifferent (I) with each other, and do not mind sitting
together or not. However, each pair of guests can only have one relationship, (F), (E) or
(I). You need to find a seating arrangement that satisfies all the constraints.

Suppose there are <M> guests in total, and there are <N> tables in the hall. The number
of pairs of Friends is <F>, and the number of pairs of Enemies is <E>. Given relationships
of the wedding guests, here we use a matrix R with elements Rij = 1, -1 or 0 to represent
whether guest i and j are Friends (F), Enemies (E) or Indifferent (I). The table
arrangement task can be represented as First-order Logic (FOL) and further encoded as a
Boolean Satisfaction problem (SAT). We introduce Boolean variables Xmn to represent
whether each guest m will be seated at a specific table n. You are asked to write FOL to
represent the constraints using variables Xmn. Then you need to construct clauses and
transform the FOL into CNF sentence. To decompose the arrangement task, there are
three constraints you have to satisfy:
(a) Each guest i should be seated at one and only one table.
(b) Any two guests i and j who are Friends (F) should be seated at the same table.
(c) Any two guests i and j who are Enemies (E) should be seated at different tables.
Note that, for simplicity, you do NOT need to consider the capacity constraint of a table.
This means the size of each table is assumed to be large enough to seat all the guests.

The input will be formatted as follows:
N M
<Relationship matrix>
For example,
2 3
0 1 -1
1 0 0
-1 0 0
This input tells us that there are 2 tables (N) and 3 people (M). Also, we can see that the
first person has 1 friend and 1 enemy. The 2nd and 3rd person are indifferent to each
other. All relationships are mutual, so the matrix is symmetrical. You may assume that
the input will always be valid, with each number separated by a single space character.

PL-Resolution is a sound and complete algorithm that it can be used to determine
satisfiability and unsatisfiability with certainty. Your program's output will be very
simple: 0 if the input is unsatisfiable, or 1 if it is satisfiable
