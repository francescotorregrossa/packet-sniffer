<?php
session_start();
require("database.php");
if((!isset($_SESSION["usename"])) || (!isset($_SESSION["password"])))
{
	if(isset($_POST["username"]))
	{   
		$check="SELECT username,email FROM utenti;";
		$risultato=mysqli_query($conn, $check) or die("errore nella registazione");
		while($riga=mysqli_fetch_array($risultato))
		{
			if(($_POST["username"]==$riga["username"])||($_POST["email"]==$riga["email"]))
			{
				echo "<script type='text/javascript'>alert('username o email gia presenti nel database!!');</script>";
				break;
			}
			else
			{
				$password=$_POST["password"]; 
				$str="INSERT INTO utenti (username, password, email, Nome,Cognome, admin) VALUES ( \"".$_POST['username']."\",\"".$password."\",\"". $_POST['email']."\",\"".$_POST['nome']."\",\"".$_POST['cognome']."\",'0' )";
				mysqli_query($conn, $str) or die("errore nella registrazione");
				$_SESSION["username"]=$_POST["usename"];
				header("Location: home.php");
			}
		}
		
	}
?>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Signup</title>
  
  <link rel="stylesheet" type="text/css" href="style.css">
  
</head>

<body>
 <h2>Registrati</h2>
  <div class="form">
    <form class="register-form" action="<?php $_SERVER['PHP_SELF'] ?>" method="post">
     <input type="text" placeholder="username" name="username" required>
      <input type="text" placeholder="name" name="nome" required>
      <input type="text" placeholder="surname" name="cognome" required>
      <input type="password" placeholder="password" name="password" required pattern="(?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{8,}" title="Password must contain at least one number,one uppercase and lowercase letter, and at least 8 total characters">
      <input type="email" placeholder="email address" name="email" required>
      <input type="submit" class="button" value="CREATE">
      <p class="message">Already registered? <a href="index.php">Sign In</a></p>
    </form>
  </div>

</body>
</html>
<?php
	
} else {
	header("Location: home.php");
}
?>