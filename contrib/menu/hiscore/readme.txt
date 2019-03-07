This file contains a collection of hiscore of the MAME games.

It's derived from the html file at http://come.to/replay (precisely the
file http://marp.retrogames.com/txt/scores.htm), and converted
with the hiscore.awk script with this command:

	awk -f hiscore.awk < scores.htm > hiscore.ini

To display the hiscore in AdvanceMENU copy the file hiscore.ini
in the configuration directory and add this line at the file
advmenu.rc. You need to set the string EMULATOR at the name given
at the AdvanceMAME or MAME emulator.

info_import ini "EMULATOR" "hiscore.ini" "Hiscore"
