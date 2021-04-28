#!/bin/bash

echo "#####################9"
for j in {1..5}
do
	./divisores_sequencial 9 
done

echo "####################98765"
for j in {1..5}
do
	./divisores_sequencial 98765 
done

echo "###################98765432"
for j in {1..5}
do
	./divisores_sequencial 98765432 
done
