<?php
$conn = mysqli_connect("localhost", "rodemeng", "computer21#")or die("db connect error");
    
mysqli_select_db($conn, "rodemeng")or die("db connect error");
?>