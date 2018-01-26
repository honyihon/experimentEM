#!/bin/bash
uniq sim.key|sed 's/|||/ /g' >sim.tmp
awk {'print $1'} sim.tmp |sort |uniq > unique_source
