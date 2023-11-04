<?php
// Read data from stdin (standard input)
$data = "";
while ($line = fgets(STDIN)) {
    $data .= $line;
}

// Output the received data
echo "Data received from parent process:\n";
echo $data;
?>
