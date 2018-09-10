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