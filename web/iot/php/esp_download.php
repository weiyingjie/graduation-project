<?php
header('Content-Type: application/json; charset=utf8');
require_once('./functions.php');
$link = dbInit(); // 连接数据库

// 从ESP8266请求的数据，将请求头分割后将数据存入数据库
$content = file_get_contents('php://input'); // 取到传感器上传的字符串{"data":12}
//    {"data":15.24}
//    POST /iot/php/esp_download.php?s_id=1
//    [0] => POST /iot/php/esp_download.php
//    [1] => s_id
//    [2] => 1
$arr = preg_split('[\r\n|\?|&|=]', $content); // 分割字符串
$s_id = $arr[2];
// 将ESP8266请求的数据在数据库查询
$sql = 'select s_data from sensors where s_id = "' . $s_id . '";';
$res = fetchRow($link, $sql); // array
$sensor_data = $res['s_data'];
echo $sensor_data;


