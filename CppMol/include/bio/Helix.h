#pragma once

struct Helix {
	/*
	1: right-handed alpha
	2: right-handed omega
	3: right-handed pi
	4: right-handed gamma
	5: right-handed 3/10
	6: left-handed alpha
	7: left-handed omega
	8: left-handed gamma
	9: 2/7 ribbon/helix
	10: polyproline
	*/
	const int type;
	const char chain;
	const int residueStart;
	const int residueEnd;
};
