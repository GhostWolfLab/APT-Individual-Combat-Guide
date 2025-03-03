<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    //检查文件是否已上传
    if (isset($_FILES['file']) && $_FILES['file']['error'] === UPLOAD_ERR_OK) {
        //获取已上传文件的信息
        $fileTmpPath = $_FILES['file']['tmp_name'];
        $fileName = $_FILES['file']['name'];
        $fileSize = $_FILES['file']['size'];
        $fileType = $_FILES['file']['type'];
        $fileNameCmps = explode(".", $fileName);
        $fileExtension = strtolower(end($fileNameCmps));

        //指定上传目录
        $uploadFileDir = './uploads/';
        $dest_path = $uploadFileDir . $fileName;

        //检查上传目录是否存在，如果不存在，则创建它
        if (!is_dir($uploadFileDir)) {
            mkdir($uploadFileDir, 0777, true);
        }

        //将上传的文件移动到目标目录
        if (move_uploaded_file($fileTmpPath, $dest_path)) {
            echo "File is successfully uploaded to $dest_path";
        } else {
            echo "There was an error moving the uploaded file.";
        }
    } else {
        echo "No file uploaded or there was an upload error.";
    }
}
?>
