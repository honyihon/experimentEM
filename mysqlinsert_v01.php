<?php

$filecontent1 = file_get_contents('/home/nlp/corpus_wei/europarl_clean_1-9000.en');
$filecontent2=file_Get_contents('/home/nlp/corpus_wei/europarl_clean_1-9000.fr');

$english_words = preg_split('/[\s]+/', $filecontent1, -1, PREG_SPLIT_NO_EMPTY);

//print_r($words);

//print "words[182128]:"+ $words[0];
$foreign_words = preg_split('/[\s]+/', $filecontent2, -1, PREG_SPLIT_NO_EMPTY);

$combine_words_array = array_merge($english_words,$foreign_words);

//print_r($combine_words_array);

print "=============================";
$combine_arr_size=count($combine_words_array);

//===============================

$servername = "localhost";
$username = "root";
$password = "nlpgroup";
$dbname = "testdb";
$s = microtime(true);
mysql_connect($servername,$username,$password) or die(mysql_error());
mysql_select_db($dbname);
$counter = 0 ;
$execute_count = 9000;
$sql= "INSERT INTO EM (source,target) VALUES ";

$res = array();
foreach($combine_words_array as $v1) {
  foreach($combine_words_array as $v2) {
    $t = array($v1, $v2);
    asort($t);
   // $val = implode('', $t);
    if(!in_array($t, $res)){
      $res[] = $t;
      //print $res ;
      $sql.="('$t[0]','$t[1]'),";
      $counter++;
     if($counter%$execute_count==0){
     mysql_query(substr($sql,0,-1));
     $counter=0;
     $sql= "INSERT INTO EM (source,target) VALUES ";
   }
  }

  }
}
//print_r($res);
/*for($i=0;$i<$combine_arr_size;$i++){
   for($j=0;$j<$combine_arr_size;$j++){
   $sql.="('$i','$j','$combine_words_array[$i]','$combine_words_array[$j]'),";
   $counter++;
   if($counter%$execute_count==0){
     mysql_query(substr($sql,0,-1));
     $counter=0;
     $sql= "INSERT INTO EM (sourceindex, targetindex,source,target) VALUES ";
   }
 }
}*/
//mysql_query(substr($sql,0,-1));

$e = microtime(true);
echo "End of insert!!";
echo $e-$s;

?>
