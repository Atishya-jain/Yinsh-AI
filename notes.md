1. Atishya says, he has used the constant 5 instead of trail_length
2. marker trail has to be removed that are non-overlapping so that we can remove max rings
3. I have used the assumption that in a row their can't be 2 consecutive trail_lengths i.e in a game of 5 a trail of 10 is not possible. This will be violated if input is not consistent with this.
4. Manage (erase) opponent trails


Structure of the value returned by Ati:-

the return all moves works with the following main variable
[
	<H,[
		<action_on_ring,<<x1,y1>,<x2,y2>>>,
		...
		]>,
	...

]

Strategies:-
1. block opponent's moves : simply count the number of moves the opponent has
2. my_markers - opponents_markers
4. try to stop opponent from forming a row
5. place each ring on a new line, to expand your horizon - initial placement
6. dont be afraid to flip your tiles - sometimes it happens that the opponent would be forced to flip that tile back again
7. push opponent outside
8. David Yinsh tries to put his ring adjacent to my marker
9. try to build rows where the opponent cannot interfere easily - help in initial placement
10. prevent your opponent from easily setting up a row himself
Black has an advantage towards goal #2, so white should focus more on goal #1. If the white player doesn't know what he is doing, black can often easily find a corner to place his last ring, where it is already impossible from the set-up for white to block black's first row!
As black, you should try to set up a place to put your last ring by cleverly blocking white's rings. Put your rings on the same line as white's rings, especially at the intersection point of two such lines!
As white, you should put your rings in mobile positions, not on the same lines as black's rings, and sometimes behind black's blockade.
11. avoid placing rings on the same line 
12. An expert knows what to look at - the rings. It is the rings which control areas and pieces, and block the opponent's control of same. It is the rings which extent your pieces into rows, and cut or block your opponent's rows. 
13. Laying markers in short rows of 2's and 3's is rarely a good idea in the beginning
14. Don't work on building rows at those places which can easily be destroyed by the opponent