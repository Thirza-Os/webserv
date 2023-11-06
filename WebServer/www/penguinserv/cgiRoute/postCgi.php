#!/usr/bin/php
<?php
$entityBody = stream_get_contents(STDIN);

// Parse the data from the request body into the $_POST array
parse_str($entityBody, $_POST);

if (isset($_POST['name'])) {
    // Access the 'name' value
    $name = $_POST['name'];
    echo "Hello, $name!";
} else {
    echo "The 'name' key is not defined in the POST data.";
}
?>
