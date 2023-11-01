<?php

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Access POST data
    $name = $_POST['name'];
    echo "Hello, $name!";
} else {
    echo "This script expects a POST request.";
}
?>
