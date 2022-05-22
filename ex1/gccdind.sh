#!/bin/bash

word=$2
if [[ $# > 1 ]]; then
	for i in $(ls -v $1/*.out 2>/dev/null)
	do
		rm $i
	done
	for i in $(ls -v $1/*.c 2>/dev/null)
	do
		len=$(cat $i | grep -i -w $2 | wc -l)
		if [[ $len > 0 ]]; then
			gcc $i -w -o ${i::-2}.out
		fi

	done
	if [[ $3 = "-r" ]]; then
		for j in $(ls -d $1/*/ 2>/dev/null)
		do
			$0 ${j::-1} $word -r
		done
	fi
else
	echo Not enough parameters
fi
