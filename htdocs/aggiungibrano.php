<!DOCTYPE html>
  <?php
    session_start();
    require ("database.php");
    if(!isset($_SESSION["username"]))
      header("Location: login.php");
    else
    {
        if(!$_SESSION["admin"])
        {
            echo "<script type='text/javascript'>alert('Questa pagina \u00E8 accessibile soltanto da un amministratore!');</script>";
            header("Refresh: 0.1; url=home.php");
        }
    }
  ?>
<html>
<head>
<title>MusicBox | Aggiungi Brano</title>
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
    <h2 class="w3-text-light-grey">Brano</h2>
    <hr style="width:200px" class="w3-opacity">
    
    <br><p>Compila il Form:</p>

    <form action="<?php echo $_SERVER['PHP_SELF'];?>" method="post" autocomplete="off" enctype="multipart/form-data">
      <h4 class="w3-text-light-grey">Titolo: </h4><p><input class="w3-input w3-padding-16" type="text" placeholder="Titolo" required name="titolo"></p>
      <h4 class="w3-text-light-grey">Album: </h4><p><input class="w3-input w3-padding-16" type="text" placeholder="Album" required name="album"></p>
      <h4 class="w3-text-light-grey">Anno: </h4><p><input class="w3-input w3-padding-16" type="text" placeholder="Anno" maxlength="4" required name="anno"></p>
      <h4 class="w3-text-light-grey">Autore: </h4>
      <?php
        
        echo '<select class="w3-select" required name="autore"><option value="null">Nessuna selezione</option>';
        $query="SELECT id,Nome FROM autori";
        $ris=mysqli_query($conn, $query) or die("Errore durante la connessione con il Database");
        while($riga=mysqli_fetch_array($ris))
        {
            echo '<option value="'.$riga["id"].'">'.$riga["Nome"].'</option>';
        }
        echo '</select>';
      ?>
      <br><br>
      <h4 class="w3-text-light-grey">File Audio: </h4><p><input class="w3-input w3-padding-16" type="file" required name="audio"></p>
      <h4 class="w3-text-light-grey">Copertina: </h4><p><input class="w3-input w3-padding-16" type="file" name="immagine"></p>
      <br><br>
      <p>
        <button class="w3-button w3-light-grey w3-padding-large" type="submit" name="invia">
          <i class="fa fa-paper-plane"></i> INVIA
        </button>
      </p>
    </form>
    <p>
        <a href="aggiungiautore.php"><button class="w3-button w3-light-grey w3-padding-large">
            <i class="fa fa-plus" aria-hidden="true"></i> AGGIUNGI AUTORE
        </button></a>
    </p>
  </div> 
  <br><br>
</div>

</body>
</html>

<?php
  
 
    
    
    
    function uploading($dir, $nome)
    {
      $target_dir = $dir;
      $target_file = $target_dir . basename($_FILES[$nome]["name"]);
      $uploadOk = 1;
      $imageFileType = pathinfo($target_file,PATHINFO_EXTENSION);
      // Check if image file is a actual image or fake image
      
      // Check if file already exists
      if(file_exists($target_file))
      {
        echo "<script type='text/javascript'>alert('Il file \u00E8 gi\u00E0 presente! Cambiare il nome e riprovare!');</script>";
        $uploadOk = 0;
      } 
      
      $check = getimagesize($_FILES[$nome]["tmp_name"]);
      if($check == false)
      {
          echo "<script type='text/javascript'>alert('Il file inserito non \u00E8 valido!');</script>";
          $uploadOk = 0;
      }
      
      
      // Check file size
      if($_FILES[$nome]["size"] > 100000000)
      {
        echo "<script type='text/javascript'>alert('Il file inserito \u00E8 troppo grande!');</script>";
        $uploadOk = 0;
      }
      // Allow certain file formats
      if($imageFileType != "mp3" && $imageFileType != "ogg" && $imageFileType != "wav")
      {
        echo "<script type='text/javascript'>alert('Il file deve essere di tipo: mp3, ogg, wav');</script>";
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
        if (move_uploaded_file($_FILES[$nome]["tmp_name"], $target_file))
          return $target_file;
        else
          return false;
      }
    }

  if(isset($_POST["canc_brano"]))
  {
    
      $str="DELETE FROM brani WHERE id=".$_GET['id'];
      mysqli_query($conn, $str) or die("Errore durante la connessione con il Database");
      if(mysqli_affected_rows($conn)==1)
      {
        
        echo "<script type='text/javascript'>alert('Brano eliminato correttamente!');</script>";
  
        
        echo '<meta http-equiv="Refresh" content="0.1; url=home.php">';
      }
    }
  
    

    if(isset($_POST["invia"]))
    {
            if($_POST["autore"]!="null")
            {
                $titolo=$_POST["titolo"];
                $album=$_POST["album"];
                $anno=$_POST["anno"];
                $autore=$_POST["autore"];
                
                
                
                if((is_uploaded_file($_FILES["immagine"]["tmp_name"])))
                {
                    $copertina=uploading("./copertine/", "immagine");
                    $audio=uploading("./brani/", "audio");
                  //$stringa="baubau micio micio";
                  $stringa="INSERT INTO brani (Titolo,Album,Anno,autore,immagine,file) VALUES ('$titolo','$album','$anno','$autore','$copertina','$audio')";
                }
                else
                {
                    $audio=uploading("./brani/", "audio");
                    $stringa="INSERT INTO brani (Titolo,Album,Anno,autore,file) VALUES ('$titolo','$album','$anno','$autore','$audio')";
                }
                
                $ris=mysqli_query($conn, $stringa) or die("Errore nella connessione con il Database");
                $val=mysqli_affected_rows($conn);
                if($val==1)
                {
                    echo "<script type='text/javascript'>alert('Brano aggiunto correttamente!');</script>";
                    echo '<meta http-equiv="Refresh" content="0.1; url=home.php">';
                }
            }
            else
            {
                echo "<script type='text/javascript'>alert('Nessun autore selezionato! Se non \u00E8 presente tra quelli elencati, clicca sull\u00B4apposito bottone per aggiungerlo!');</script>";
            }
            
    }
    
?>