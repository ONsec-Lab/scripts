<?php
$host = $argv[1];
$port = intval($argv[2]);
echo "Start: $host:$port\n";
for($j=0;$j<256;$j++){
for($i=0;$i<256;$i++){
$data = 'GET / HTTP/1.1'.chr($j).chr($i).'Host: '.$host.'
Connection: close

';
$f=fsockopen($host,$port);
fwrite($f,$data);
while(($resp = fread($f,20))!=null){
	if(strpos($resp,"HTTP/1.1 200 OK")===0){
		echo $j." ".$i."\n";
	}
}
fclose($f);
}
}
?>
