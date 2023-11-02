#!/usr/bin/php
<?php
if (isset($_POST['name'])) {
    // Access the 'name' value
    $name = $_POST['name'];
    echo "Hello, $name!";
} else {
    echo "The 'name' key is not defined in the POST data.";
}
?>
