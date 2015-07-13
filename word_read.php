<?php
$filecontent = file_get_contents('/home/hadoop/corpus_wei/europarl_clean_1-9000.en');

$words = preg_split('/[\s]+/', $filecontent, -1, PREG_SPLIT_NO_EMPTY);

print_r($words);

//print "words[182128]:"+ $words[0];


//print $words[1];

?>
