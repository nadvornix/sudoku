#!/bin/bash
 
# Zmente, pokud se vas binarni soubor jmenuje jinak
PROGRAM=./a.out
 
# Pro vsechny soubory s nazvem "*in.txt"...
for TEST_FILE in $(ls *in.txt); do
	echo -n ">>> Testing $TEST_FILE "
 
	# Spust program, na vstup mu presmeruj obsah souboru TEST_FILE (testovaci vstup) a vystup uloz do "vystup.txt"
	$PROGRAM < $TEST_FILE > ./vystup.txt
	# Zjisti rozdil mezi vystupem programu (vystup.txt) a vzorovym vystupem (to je to ${TEST_FILE/in/out} a uloz ho do promenne $DIFF
	DIFF=`diff ${TEST_FILE/in/out} ./vystup.txt`
 
	# Pokud je $DIFF prazdna (mezi vystupem programu a vzorovym vystupem neni rozdil)...
	if [ "$DIFF" == "" ]; then
		# Napis "OK" a jdi na dalsi test
		echo "- OK"
	# Jinak...
	else
		echo "- FAILED"
		echo "Input:"
		# Vypis obsah testovaciho vstupu (tedy data, ktera chybu zpusobila)
		cat $TEST_FILE
		echo
		echo "Output diff:"
		# Vypis rozdil mezi vystupem programu a vzorovym vystupem
		echo "$DIFF"
		echo
	fi
done
 
# Nakonec bez reci odstran vystup.txt, ktery jsme pouzivali pro ukladani vystupu programu.
\rm ./vystup.txt
