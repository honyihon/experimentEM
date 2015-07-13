
<?php

$link = mysql_connect("localhost", "root", "nlpgroup");
mysql_select_db("testdb", $link);

$result = mysql_query("SELECT * FROM EM2", $link);
$num_rows = mysql_num_rows($result);

echo "$num_rows Rows\n";

?>

