#!/bin/bash

for i in {1..5}
do
	echo "#####################9"
	for j in {1..5}
	do
		./divisores_concorrente 9 $i
	done

	echo "####################98765"
	for j in {1..5}
	do
		./divisores_concorrente 98765 $i
	done

	echo "###################98765432"
	for j in {1..5}
	do
		./divisores_concorrente 98765432 $i
	done
done
