<?php
    session_start();

    unset($_SESSION['username']);
    unset($_SESSION['admin']);
    unset($_SESSION['id']);
    if(isset($_SESSION["id_mod"]))
        unset($_SESSION['id_mod']);

    session_write_close();
    header("Location: index.php");
?>