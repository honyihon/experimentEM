#!/bin/sh
Efilename=$1
Ffilename=$2

#create pair
./corpus_shell_array.sh $1 $2 sim.key
sort sim.key |uniq -c >sim.count
sed 's/|||/\t/g' sim.count >sim.space
awk 'NR==FNR{a[$2]+=$1;next}{ print $1,$2,$3,sprintf("%.16f",$1/a[$2])}' sim.space sim.space>sim.prob
awk {'print $2"|||"$3,$1,$4'} sim.prob > sim.final

