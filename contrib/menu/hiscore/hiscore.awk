BEGIN {
	ORS="";
	FS="[;:]"
	PROCESS=0;
}
/---- game ----/{
	PROCESS=1
	print "[Hiscore]\n"
	next;
}
/html/{
	PROCESS=0
	next;
}
{
	if (PROCESS == 1) {
		GAME=gensub(" +","","g",$1)
		RECORD=gensub(" +","","g",$2)
		if (GAME != "" && RECORD != "--") {
			print GAME "=" RECORD "\n"
		}
		next;
	}
}
