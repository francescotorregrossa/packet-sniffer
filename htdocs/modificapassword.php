<!DOCTYPE html>
  <?php
    session_start();
    require "database.php";
    if(!isset($_SESSION["username"]))
      header("Location: login.php");
    else
    {
        if($_SESSION["admin"]||($_SESSION["id"]==$_SESSION["id_mod"]))
        {
            $strSQL="SELECT id, password FROM utenti WHERE id=".$_SESSION["id_mod"];
            $ris=mysqli_query($conn, $strSQL) or die("Errore nell'esecuzione della query");
            $dati=mysqli_fetch_array($ris);
        }
        else
        {
            echo "<script type='text/javascript'>alert('Questa pagina \u00E8 accessibile soltanto da un amministratore!');</script>";
            header("Refresh: 0.1; url=home.php");
        }
    }
  ?>
<html>
<head>
<title>MusicBox | Modifica Password Utente</title>
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
    <h2 class="w3-text-light-grey">Modifica Password</h2>
    <hr style="width:200px" class="w3-opacity">
    
    <br><p>Inserisci i dati:</p><br>

    <form action="<?php $_SERVER['PHP_SELF'] ?>" method="post" autocomplete="off">
      <h4 class="w3-text-light-grey">Password corrente: </h4><p><input class="w3-input w3-padding-16" type="password" placeholder="Password corrente" required name="password"></p>
      <h4 class="w3-text-light-grey">Nuova password: </h4><p><input class="w3-input w3-padding-16" type="password" placeholder="Nuova password" required name="newpwd"></p>
      <h4 class="w3-text-light-grey">Conferma nuova password: </h4><p><input class="w3-input w3-padding-16" type="password" placeholder="Conferma nuova password" required name="conf_pwd"></p>
      <br><br>
      <p>
        <button class="w3-button w3-light-grey w3-padding-large" type="submit" name="invia">
          <i class="fa fa-paper-plane"></i> INVIA
        </button>
      </p>
    </form>
    <p>
    <a href="schedautente.php?id=<?php echo $_SESSION['id_mod'];?>"><button class="w3-button w3-light-grey w3-padding-large">
            <i class="fa fa-close" aria-hidden="true"></i> ANNULLA
        </button></a>
    </p>
  </div>
  
  <br><br>

<!-- END PAGE CONTENT -->
</div>

</body>
</html>

<?php

    if(isset($_POST["invia"]))
    {
      $password=$_POST["password"];
      $newpwd=$_POST["newpwd"];
      $confpwd=$_POST["conf_pwd"];
      echo $_SESSION["id"];
      $id=$_SESSION["id"];
      
        if($newpwd==$confpwd)
        {
             if($dati["password"]==$password)
             {
                
                $stringa="UPDATE utenti SET password='".$newpwd."' WHERE id=$id";
                
                $ris=mysqli_query($conn, $stringa) or die("Errore nella connessione con il Database");
                $val=mysqli_affected_rows($conn);
                if($val==1)
                {
                    echo "<script type='text/javascript'>alert('Password utente aggiornata correttamente!');</script>";
                    unset($_SESSION['id_mod']);
                    session_write_close();
                    echo '<meta http-equiv="Refresh" content="0.1; url=schedautente.php?id='.$id.'">';
                }
             }
             else
             {
                echo "<script type='text/javascript'>alert('La password corrente inserita non \u00E8 corretta!');</script>";
             }
        }
        else
        {
            echo "<script type='text/javascript'>alert('La conferma della password \u00E8 errata!');</script>";
           
        }
    }
?>