<?php
header('Content-Type: application/json; charset=utf8');
require_once('./functions.php');
$link = dbInit(); // 连接数据库

// 设置时区
date_default_timezone_set('PRC');
// 获取时间
$time = date("Y-m-d H:i:s");

// 从ESP8266请求的数据，将请求头分割后将数据存入数据库
//$content = file_get_contents('php://input'); // 取到传感器上传的字符串{"data":12}
//    {"data":15.24}
//    POST /iot/php/esp_upload.php?s_id=1
//    [0] => {"data":15.24}
//    [1] => POST /iot/php/esp_upload.php
//    [2] => s_id
//    [3] => 1
//print_r($content);
//$arr = preg_split('[\r\n|\?|&|=]', $content); // 分割字符串
// GET /iot/php/esp_upload.php?s_id=19&s_data=36.708629
if (!empty($_GET)) {
    if (isset($_GET['s_id']) && isset($_GET['s_data'])) {
        $s_id = $_GET['s_id'];
        $s_data = $_GET['s_data'];
        $sql = 'update sensors set s_data = "' . $s_data . '" where s_id = "' . $s_id . '";';
        if (query($link, $sql)) {
            $ret = array('status' => 200);
            $ret = json_encode($ret, JSON_UNESCAPED_UNICODE);
            echo $ret;
        }

        // 存入history
        $sql = 'select * from sensors where s_id = "' . $s_id . '";';
        if ($res = fetchRow($link, $sql)) {
            if ($res['s_type'] == 'data') {
                $d_id = $res['d_id'];
                $s_name = $res['s_name'];
                $sql = 'insert into history(d_id, s_id, s_name, s_data, time) values("' . $d_id . '", "' . $s_id . '", "' . $s_name . '", "' . $s_data . '", "' . $time . '");';
                query($link, $sql);
            }
        }
    }
}





