<!DOCTYPE html>
  <?php
    session_start();
    if(!isset($_SESSION["username"]))
      header("Location: index.php");
  ?>
<html>
<head>
<title>MusicBox</title>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="layout.css">
<link rel="stylesheet" href="font.css">
<style>
body, h1,h2,h3,h4,h5,h6 {font-family: "Montserrat", sans-serif}
.w3-row-padding img {margin-bottom: 12px}
/* Set the width of the sidebar to 120px */
.w3-sidebar {width: 120px;background: #222;}
/* Add a left margin to the "page content" that matches the width of the sidebar (120px) */
#main {margin-left: 120px}
/* Remove margins from "page content" on small screens */
@media only screen and (max-width: 600px) {#main {margin-left: 0}}
</style>
</head>
<body class="w3-black">

<!-- Icon Bar (Sidebar - hidden on small screens) -->
<nav class="w3-sidebar w3-bar-block w3-small w3-hide-small w3-center">
  <!-- Avatar image in top left corner -->
  <a href="home.php" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <p>HOME</p>
  </a>
  <a href="cerca.php" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <p>CERCA</p>
  </a>
  <a href="schedautente.php?id=<?php echo $_SESSION['id'];?>" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <p>PROFILO</p>
  </a>
  <?php
    if($_SESSION["admin"])
    {
      echo '<a href="aggiungibrano.php" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
              <i class="fa fa-plus w3-xxlarge"></i>
              <p>AGGIUNGI</p>
            </a>';
    }
  ?>
  <a href="logout.php" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <p>LOGOUT</p>
  </a>
</nav>

<!-- Navbar on small screens (Hidden on medium and large screens) -->
<div class="w3-top w3-hide-large w3-hide-medium" id="myNavbar">
  <div class="w3-bar w3-black w3-opacity w3-hover-opacity-off w3-center w3-small">
    <a href="index.php" class="w3-bar-item w3-button" style="width:25% !important">HOME</a>
    <a href="cerca.php" class="w3-bar-item w3-button" style="width:25% !important">CERCA</a>
    <a href="schedautente.php?id=<?php echo $_SESSION['id'];?>" class="w3-bar-item w3-button" style="width:25% !important">PROFILO</a>
    <?php
       if($_SESSION["admin"])
      {
      echo  '<a href="aggiungibrano.php" class="w3-bar-item w3-button" style="width:25% !important">AGGIUNGI</a>';
      }
    ?>
  </div>
</div>
<!-- Page Content -->
<div class="w3-padding-large" id="main">
  <!-- Header/Home -->
  <header class="w3-container w3-padding-32 w3-center w3-black" id="home">
    <h1 class="w3-jumbo"><span class="w3-hide-small">MusicBox</span></h1>
</header>

</div>

</body>
</html>