<?php

$filecontent1 = file_get_contents('/home/hadoop/corpus_wei/europarl_clean_1-9000.en');
$filecontent2=file_Get_contents('/home/hadoop/corpus_wei/europarl_clean_1-9000.fr');

$english_words = preg_split('/[\s]+/', $filecontent1, -1, PREG_SPLIT_NO_EMPTY);

//print_r($words);

//print "words[182128]:"+ $words[0];
$foreign_words = preg_split('/[\s]+/', $filecontent2, -1, PREG_SPLIT_NO_EMPTY);

$combine_words_array = array_merge($english_words,$foreign_words);

//print_r($combine_words_array);

print "=============================";
$english_arr_size=count($english_words);
$foreign_arr_size=count($foreign_words);
$combine_arr_size=count($combine_words_array);
print $combine_words_array[0];
print "\n";
print $combine_words_array[182128];
print "\n";
print $combine_words_array[182129];
print "\n";
print $combine_words_array[379199];
print "\n";

/*
print $english_arr_size;
print "\n";
print $foreign_arr_size;
*/
?>
