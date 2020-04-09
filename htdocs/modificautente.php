  <?php
    session_start();
    require "database.php";
    if(!isset($_SESSION["username"]))
      header("Location: login.php");
    else
    {
        if($_SESSION["admin"]||($_SESSION["id"]==$_GET["id"]))
        {
            $strSQL="SELECT * FROM utenti WHERE id=".$_GET["id"];
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
<title>MusicBox | Modifica Utente</title>
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
    <h2 class="w3-text-light-grey">Profilo</h2>
    <hr style="width:200px" class="w3-opacity">
    
    <br><p>Modifica i dati:</p><br>

    <form action="<?php echo $_SERVER['PHP_SELF'];?><?php echo '?id='.$_GET["id"];?>" method="post" autocomplete="off" enctype="multipart/form-data">
      <h4 class="w3-text-light-grey">E-Mail: </h4><p>
      <input class="w3-input w3-padding-16" type="email" placeholder="E-Mail" required name="email" value="<?php echo $dati["email"];?>"></p>
      <h4 class="w3-text-light-grey">Username: </h4><p><input class="w3-input w3-padding-16" type="text" placeholder="Username" required name="username" value="<?php echo $dati["username"];?>"></p>
      <h4 class="w3-text-light-grey">Avatar: </h4><p><input class="w3-input w3-padding-16" type="file" name="immagine" value="<?php echo $dati["immagine"];?>"></p>
      <?php
        $_SESSION["id_mod"]=$_GET["id"];
      ?>
      <br><br>
      
      <br><br>
      <h4 class="w3-text-light-grey">Nome: </h4><p><input class="w3-input w3-padding-16" type="text" placeholder="Nome" name="nome" value="<?php echo $dati["Nome"];?>"></p>
      <h4 class="w3-text-light-grey">Cognome: </h4><p><input class="w3-input w3-padding-16" type="text" placeholder="Cognome" name="cognome" value="<?php echo $dati["Cognome"];?>"></p>
      
      <p><h4 class="w3-text-light-grey">Admin: </h4><input class="w3-check" type="checkbox" name="admin" value="true" <?php if($dati["admin"]) echo 'checked="checked"';?>></p>
      <br><br>
      <p>
        <button class="w3-button w3-light-grey w3-padding-large" type="submit" name="invia">
          <i class="fa fa-paper-plane"></i> INVIA
        </button>
      </p>
    </form>
    <p>
    <a href="modificapassword.php"><button class="w3-button w3-light-grey w3-padding-large">
            <i class="fa fa-pencil" aria-hidden="true"></i> MODIFICA PASSWORD
      </button></a>
    <a href="schedautente.php?id=<?php echo $_GET['id'];?>"><button class="w3-button w3-light-grey w3-padding-large">
            <i class="fa fa-close" aria-hidden="true"></i> ANNULLA
        </button></a>
    </p>
  </div>
  <br><br>
</div>

</body>
</html>

<?php
    
    function upload()
    {
      $target_dir = "./avatar/";
      $target_file = $target_dir . basename($_FILES["immagine"]["name"]);
      $uploadOk = 1;
      $imageFileType = pathinfo($target_file,PATHINFO_EXTENSION);
      // Check if image file is a actual image or fake image
      
      // Check if file already exists
      if(file_exists($target_file))
      {
        echo "<script type='text/javascript'>alert('Il file \u00E8 gi\u00E0 presente! Cambiare il nome e riprovare!');</script>";
        $uploadOk = 0;
      } 
      
      $check = getimagesize($_FILES["immagine"]["tmp_name"]);
      if($check == false)
      {
          echo "<script type='text/javascript'>alert('Il file inserito non \u00E8 valido!');</script>";
          $uploadOk = 0;
      }
      
      
      // Check file size
      if($_FILES["immagine"]["size"] > 10000000)
      {
        echo "<script type='text/javascript'>alert('Il file inserito \u00E8 troppo grande!');</script>";
        $uploadOk = 0;
      }
      // Allow certain file formats
      if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"&& $imageFileType != "gif" )
      {

        echo "<script type='text/javascript'>alert('Il file deve essere di tipo: jpg, png, jpeg, gif');</script>";
        $uploadOk = 0;
      }
      // Check if $uploadOk is set to 0 by an error
      if ($uploadOk == 0)
      {
        echo "<script type='text/javascript'>alert('Il file non \u00E8 stato inviato!');</script>";
        die("Errore nel trasferimento del file");
      }
            // if everything is ok, try to upload file
      else
      {
        if (move_uploaded_file($_FILES["immagine"]["tmp_name"], $target_file))
          return $target_file;
        else
          return false;
      }
    }

    if(isset($_POST["invia"]))
    {
      $admin=0;
      if(!isset($_POST["admin"]))
        $admin=0;
      else
        $admin=1;
      $username=$_POST["username"];
      $email=addslashes($_POST["email"]);
      $nome=$_POST["nome"];
      $cognome=$_POST["cognome"];
      $id=$_GET["id"];
      
                $stringa="UPDATE utenti SET username='$username',email='$email',";
                
                if(!empty($nome))
                {
                  $stringa.="Nome='$nome',";
                }
                if(!empty($cognome))
                {
                  $stringa.="Cognome='$cognome',";
                }
              
                if(is_uploaded_file($_FILES["immagine"]["tmp_name"]))
                {
                  $path=upload();
                  $stringa.="avatar='$path',";
                }
                $stringa.="admin=$admin WHERE id=$id";
                
                $ris=mysqli_query($conn, $stringa) or die("Errore nella connessione con il Database");
                $val=mysqli_affected_rows($conn);
                if($val==1)
                {
                    echo "<script type='text/javascript'>alert('Utente aggiornato correttamente!');</script>";
                    if($_SESSION["id"]==$_GET["id"])
                    {
                      $_SESSION["username"]=$_POST["username"];
                      $_SESSION["admin"]=$admin;
                    }
                    echo '<meta http-equiv="Refresh" content="0.1; url=schedautente.php?id='.$id.'">';
                }
           
    }
?>