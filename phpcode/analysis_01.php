<?php

$filecontent1 = file_get_contents('/home/hadoop/corpus_wei/europarl_clean_1-9000.en');
$filecontent2=file_Get_contents('/home/hadoop/corpus_wei/europarl_clean_1-9000.fr');
/*
$filecontent1 = file_get_contents('/home/nlp/corpus_wei/easy.en');
$filecontent2=file_Get_contents('/home/nlp/corpus_wei/easy.fr');*/
$english_words = preg_split('/[\s]+/', $filecontent1, -1, PREG_SPLIT_NO_EMPTY);

//print_r($words);

//print "words[182128]:"+ $words[0];
$foreign_words = preg_split('/[\s]+/', $filecontent2, -1, PREG_SPLIT_NO_EMPTY);

$combine_words_array = array_merge($english_words,$foreign_words);

print_r($combine_words_array);

print "=============================";
$combine_arr_size=count($combine_words_array);

set_time_limit(0);
$current=0;
$en_co=0;
$ef_a=0;
$fe_a=0;
$fr_co=0;
$str1='of';
$str2=',';
for($i=0;$i<$combine_arr_size;$i++){
   for($j=0;$j<$combine_arr_size;$j++){
    if((strcmp($combine_words_array[$i],$str1)==0&&strcmp($combine_words_array[$j],$str2)==0)||(strcmp($combine_words_array[$i],$str2)==0&&strcmp($combine_words_array[$j],$str1)==0)){
       if($i<182129&&$j<182129){
		   $en_co++;
		   //en_collocate
	   }else if($i<182129&&$j>182128){
		   //e-f align
		   $ef_a++;
	   }else if ($i>182128&&$j<182129){
		 //f-e align
           $fe_a++;		 
	   }else {
		   //fr_collocate
		   $fr_co++;
	   }
    }    
 }
}
print "\n";
print "word pard:";
print $str1;
print "|";
print $str2;
print "\n";
print "en_collocate:";
print $en_co;
print "\n";
print "e-f align:";
print $ef_a;
print "\n";
print "f-e align:";
print $fe_a;
print "\n";
print "fr_collocate:";
print $fr_co++;
print "\n";
/*
$e = microtime(true);
echo "End of insert!!";
echo $e-$s;*/

?>
