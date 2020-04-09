<?php
session_start();
require("database.php");
if((!isset($_POST["usename"])) || (!isset($_POST["password"])))
{
	if(isset($_POST["username"]))
	{
		$str1="SELECT username FROM utenti";
		$risultato=mysqli_query($conn, $str1) or die("errore nella connessione");
		while($riga=mysqli_fetch_array($risultato))
		{
			if($_POST["username"]==$riga["username"])
			{
				$str2="SELECT password,admin,id FROM utenti where username=\"".$_POST["username"]."\" ;";
				$risultato=mysqli_query($conn, $str2) or die("errore nella connessione");
				$password=mysqli_fetch_array($risultato);
				if($_POST["password"]==$password["password"])
				{
					$_SESSION["username"]=$_POST["username"];
					$_SESSION["admin"]=$password["admin"];
					$_SESSION["id"]=$password["id"];
					header("Location: home.php");
				}
					
				else
					{
						echo "<script type='text/javascript'>alert('password errata');</script>";
						break;
					}
				
			}
			else
				echo "<script type='text/javascript'>alert('utente errato');</script>";
				
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
<h2>Accedi</h2>
<div class="login-page">
  <div class="form">
    <form class="login-form" action="<?php $_SERVER['PHP_SELF'] ?>" method="post">
      <input type="text" placeholder="username" name="username" required>
      <input type="password" placeholder="password" name="password" required>
      <input type="submit" class="button" value="LOGIN">
	  </form>
      <p class="message">Not registered? <a href="signup.php">Create an account</a></p>
  </div>
</div>
</body>
</html>
<?php
	

} else {
	$_SESSION["username"]=$_POST["username"];
	header("Location: home.php");
}	
?>