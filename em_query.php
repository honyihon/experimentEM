
<?php
$link = mysqli_connect("localhost", "root", "nlpgroup", "em_im");

/* check connection */
if (mysqli_connect_errno()) {
printf("Connect failed: %s\n", mysqli_connect_error());
exit();
}
//$query .= "";
//$query  = "FOR i IN 1..5";
//$query .= "LOOP";
$query  = "insert into em3 select source,sum(prob) as lundax from em2 group by source;";
$query .= "insert into em4 select source,lundax,pa/lundax alva from em3 cross join (select exp(sum(ln(lundax))) pa from em3) t;";
$query .= "insert into em5 select t1.source,t1.target,t1.prob*t2.alva as count,0 from em2 t1 inner join em4 t2 on t1.source=t2.source;";
$query .= "update em5 set prob = count / (select * from (select sum(count) from em5) q );";
$query .= "truncate em2;";
$query .= "insert em2 select * from em5;";
$query .= "DO SLEEP(20);";
$query .= "truncate em3;";
$query .= "truncate em4;";
$query .= "truncate em5;";
//$query .= "DO SLEEP(10);";
//$query .= "END LOOP;";
//===========
/* execute multi query */
if (mysqli_multi_query($link, $query)) {
    do {
        /* store first result set */
        if ($result = mysqli_store_result($link)) {
            while ($row = mysqli_fetch_row($result)) {
                printf("%s\n", $row[0]);
            }
            mysqli_free_result($result);
        }
        /* print divider */
        if (mysqli_more_results($link)) {
            printf("-----------------\n");
        }
    } while (mysqli_next_result($link));
}
/*$exe=5; // execute_count
for ($i=0;$i<$exe;$i++){
if (mysqli_multi_query($link, $query)) {
}else {
print "error!!";
print "\n";
}
}*/
/* close connection */
mysqli_close($link);
?>

