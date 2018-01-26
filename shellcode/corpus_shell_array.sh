#!/bin/bash

#filename=./europarl_clean_1-9000.en
Efilename=$1
Ffilename=$2
Outfilename=$3
declare -a EArray
declare -a Farray
#myArray=( `cat "$filename"`)
#len=${#myArray[@]}
#for (( i = 0 ; i < len ; i++))
#do
#        echo "Element [$i]: ${myArray[$i]}"
#done
#while read line; do
while read  lineE && read  lineF <&3;do
    #myArray =( 'cat "$line"')
    #myArray=( `cat "$line"`)
    #myArray="$line" 
    EArray=($lineE)
    FArray=($lineF)
    lenE=${#EArray[@]}
    lenF=${#FArray[@]}
    #combineArray=( "$(EArray[@])" "$(FArray[@])" ) //wrong
    combineArray=( "${EArray[@]}" "${FArray[@]}" )
    lentotal=${#combineArray[@]}
    for (( i = 0 ; i < lentotal ; i++))
    do
       for (( j = 0 ; j < lentotal ; j++))
         do
         #echo "Element [$i]: ${EArray[$i]}"
         #echo "Flement [$i]: ${FArray[$i]}"
          #echo "Elementi [$i]: ${combineArray[$i]}"
          #echo "Elementj [$j]: ${combineArray[$j]}"
           echo "combineElement [$i][$j]:"
           echo >> $Outfilename "${combineArray[$i]}|||${combineArray[$j]}"

         done
    done

done < $Efilename  3<$Ffilename
