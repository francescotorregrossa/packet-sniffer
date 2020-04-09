<!DOCTYPE html>
  <?php
    session_start();
    require "database.php";
    if(!isset($_SESSION["username"]))
      header("Location: login.php");
    else
    {
        $strSQL="SELECT * FROM brani AS B LEFT JOIN autori AS A ON B.autore=A.id WHERE A.id=".$_GET["id"];
        $ris=mysqli_query($conn, $strSQL) or die("Errore nell'esecuzione della query");
        $dati=mysqli_fetch_array($ris);
    }
  ?>
<html>
<title>MusicBox | Scheda Brano</title>
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
<body class="w3-black">
    
 <!-- Icon Bar (Sidebar - hidden on small screens) -->
<nav class="w3-sidebar w3-bar-block w3-small w3-hide-small w3-center">
  <!-- Avatar image in top left corner -->
  <i class="fa fa-home" aria-hidden="true"></i>
  <a href="home.php" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <i class="fa fa-home w3-xxlarge"></i>
    <p>HOME</p>
  </a>
  <a href="cerca.php" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <i class="fa fa-search w3-xxlarge"></i>
    <p>CERCA</p>
  </a>
  <a href="schedautente.php?id=<?php echo $_SESSION['id'];?>" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <i class="fa fa-user w3-xxlarge"></i>
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
    <i class="fa fa-sign-out w3-xxlarge"></i>
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

  <!-- Search Section -->
  
  <div class="w3-content w3-justify w3-text-grey w3-padding-64">
    <h2 class="w3-text-light-grey">Brano</h2>
    <hr style="width:200px" class="w3-opacity">
    <br>
    <div class="w3-row-padding" style="margin:0 -16px">
        <div class="w3-half margin-bottom w3-center">
            <br><br><br><img src="<?php echo get_image($dati["immagine"]);?>" height="200" width="200" alt="Copertina" class="w3-margin-right"><br>
        </div>
    
        <div class="w3-half">
            <h4 class="w3-text-light-grey">Titolo: </h4><?php echo $dati["Titolo"];?>
            <h4 class="w3-text-light-grey">Album: </h4><?php echo $dati["Album"];?><br>
            <h4 class="w3-text-light-grey">Anno: </h4><?php echo $dati["Anno"];?><br>
            <h4 class="w3-text-light-grey">Autore: </h4><a href="schedaautore.php?id=<?php echo $dati["autore"];?>" style="text-decoration:none"><?php echo $dati["Nome"];?></a><br>
            <h4 class="w3-text-light-grey">Riproduci: </h4><br>
            <audio src="<?php echo $dati["file"];?>"  controls="controls">
                Il tuo browser non supporta l'audio gestito con HTML5.
            </audio><br><br>
            <?php
            if($_SESSION["admin"])
                {
                    echo '<a href="modificabrano.php?id='.$_GET['id'].'"><button class="w3-button w3-light-grey w3-padding-large">
                        <i class="fa fa-pencil" aria-hidden="true"></i> MODIFICA
                    </button></a>';
                }
            ?>
        </div>

    </div>
    
  </div>
  
</div>

</body>
</html>

<?php

    function get_image($img)
    {
        if($img==NULL)
            return "./copertine/defaultcover.png";
        else
            return $img;
    }
?>