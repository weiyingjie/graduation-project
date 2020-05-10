<?php
header('Content-Type: application/json; charset=utf8');
require_once('./functions.php');
$link = dbInit(); // 连接数据库


// 从ESP8266请求的数据，将请求头分割后将数据存入数据库
$content = file_get_contents('php://input'); // 取到传感器上传的字符串{"data":12}
//    {"data":15.24}
//    POST /iot/php/esp_upload.php?s_id=1
//    [0] => {"data":15.24}
//    [1] => POST /iot/php/esp_upload.php
//    [2] => s_id
//    [3] => 1
$arr = preg_split('[\r\n|\?|&|=]', $content); // 分割字符串
$sensor_data = $arr[0];
$s_id = $arr[3];
$sensor_data = json_decode($sensor_data, true); // array
$sensor_data = $sensor_data['data']; // 数据
// 将ESP8266上传的数据存入数据库
$sql = 'update sensors set s_data = "' . $sensor_data . '" where s_id = "' . $s_id . '";';
query($link, $sql);
echo $sensor_data;
echo $sql;




