<?php

$filecontent1 = file_get_contents('/home/hadoop/corpus_wei/europarl_clean_1-9000.en');
$filecontent2=file_Get_contents('/home/hadoop/corpus_wei/europarl_clean_1-9000.fr');

$english_words = preg_split('/[\s]+/', $filecontent1, -1, PREG_SPLIT_NO_EMPTY);

//print_r($words);

//print "words[182128]:"+ $words[0];
$foreign_words = preg_split('/[\s]+/', $filecontent2, -1, PREG_SPLIT_NO_EMPTY);

$combine_words_array = array_merge($english_words,$foreign_words);

print_r($combine_words_array);

print "=============================";
$combine_arr_size=count($combine_words_array);
print $combine_arr_size;

//===============================
/*
$servername = "localhost";
$username = "root";
$password = "nlpgroup";
$dbname = "testdb";

try {
    $conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
    // set the PDO error mode to exception
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    $sql = "INSERT INTO MyGuests (firstname, lastname, email)
    VALUES ('John', 'Doe', 'john@example.com')";
    // use exec() because no results are returned
    $conn->exec($sql);
    echo "New record created successfully";
    }
catch(PDOException $e)
    {
    echo $sql . "<br>" . $e->getMessage();
    }

$conn = null;
 
*/
?>
