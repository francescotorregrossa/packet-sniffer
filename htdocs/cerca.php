<!DOCTYPE html>
  <?php
    session_start();
    if(!isset($_SESSION["username"]))
      header("Location: login.php");

  ?>
<html>
<head>
<title>MusicBox | Cerca</title>
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
  <i class="fa fa-home" aria-hidden="true"></i>
  <a href="home.php" class="w3-bar-item w3-button w3-padding-large w3-hover-black">
    <i class="fa fa-home w3-xxlarge"></i>
    <p>HOME</p>
  </a>
  <a href="cerca.php" class="w3-bar-item w3-button w3-padding-large w3-black">
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
      <h2 class="w3-text-light-grey">Cerca brano</h2>
      <hr style="width:200px" class="w3-opacity">
      
      <br>
  
      <form action="<?php $_SERVER['PHP_SELF'] ?>" method="post" autocomplete="off">
        <p><input class="w3-input w3-padding-16" type="text" placeholder="Titolo" name="titolo"></p>
        <p><input class="w3-input w3-padding-16" type="text" placeholder="Autore" name="autore"></p>
        <p><input class="w3-input w3-padding-16" type="text" placeholder="Album" name="album"></p>
        <p><input class="w3-input w3-padding-16" type="text" placeholder="Anno" name="anno" maxlength="4"></p>
        <p>
          <button class="w3-button w3-light-grey w3-padding-large" type="submit" name="cerca">
            <i class="fa fa-search"></i> CERCA
          </button>
          <button class="w3-button w3-light-grey w3-padding-large" type="reset">
            <i class="fa fa-trash-o" aria-hidden="true"></i> CANCELLA
          </button>
        </p>
      </form>
    </div>

<?php
if(isset($_POST["cerca"]))
  {
    require "database.php";
    $sql= "SELECT B.id, B.titolo, A.Nome AS autore, B.album, B.anno, A.id AS id_aut FROM brani AS B, autori AS A WHERE B.id=A.id AND ";
    $cont=0;
    $titolo=strtolower($_POST["titolo"]);
    $autore=strtolower($_POST["autore"]);
    $album=strtolower($_POST["album"]);
    $anno=$_POST["anno"];
    if(!empty($titolo))
    {
    	$sql.="B.titolo LIKE '%$titolo%' ";
    	$cont++;
    }
    if(!empty($autore))
    {
    	if($cont==0)
        {
    	    $sql.="A.Nome LIKE '%$autore%' ";
    	    $cont++;
    	}
        else
        {
    	    $sql.="AND A.Nome LIKE '%$autore%' ";
    	    $cont++;
    	}
    }
    if(!empty($album))
    {
    	if($cont==0)
        {
	    $sql.="B.album LIKE '%$album%'";
	    $cont++;
	}
        else
        {
            $sql.="AND B.album LIKE '%$album%' ";
	    $cont++;
	}
    }
    if(!empty($anno))
    {
	if($cont==0)
        {
	    $sql.="B.anno LIKE '%$anno%' ";
	    $cont++;
	}
        else
        {
	    $sql.="AND B.anno LIKE '%$anno%' ";
	    $cont++;
	}
    }
    
    $query=mysqli_query($conn, $sql) or die("Errore nella connessione con il Database");
    
    
    echo '<div class="w3-content w3-justify w3-text-grey w3-padding-64">
    <h2 class="w3-text-light-grey">Risultati</h2>
    <hr style="width:200px" class="w3-opacity"><br>';
        
    
    
    
    if(mysqli_affected_rows($conn)>0)
    {
        while($riga=mysqli_fetch_array($query))
        {
            echo '<p>';
            echo '<a href="schedabrano.php?id='.$riga["id"].'" style="text-decoration:none"><span class="w3-large w3-text-light-grey w3-margin-right">'.$riga["titolo"].'</span></a>';
            echo '&nbsp&nbsp&nbsp|&nbsp&nbsp&nbsp';
            echo '<a href="schedaautore.php?id='.$riga["id_aut"].'" style="text-decoration:none"><span class="w3-large w3-text-light-grey w3-margin-right">'.$riga["autore"].'</span></a>';
            echo '&nbsp&nbsp&nbsp|&nbsp&nbsp&nbsp';
            echo $riga["album"];
            echo '&nbsp&nbsp&nbsp|&nbsp&nbsp&nbsp';
            echo $riga["anno"];        
            echo '</p>';
            
        }
    }
    else
        echo "<p>Nessun risultato.</p>";
    
  }?>
  <br><br>

</div>

</body>
</html>
