<?php
header('Content-Type: application/json; charset=utf8');
require_once('./functions.php');
$link = dbInit(); // 连接数据库

//// 从ESP8266请求的数据，将请求头分割后将数据存入数据库
//$content = file_get_contents('php://input'); // 取到传感器上传的字符串{"data":12}
////    {"data":15.24}
////    POST /iot/php/esp_download.php?s_id=1
////    [0] => POST /iot/php/esp_download.php
////    [1] => s_id
////    [2] => 1
//$arr = preg_split('[\r\n|\?|&|=]', $content); // 分割字符串

if (!empty($_GET)) {
    if (isset($_GET['s_id'])) {
        $s_id = $_GET['s_id'];
        $sql = 'select s_cmd from sensors where s_id = "' . $s_id . '";';
        if($res = fetchRow($link, $sql)) {
            $s_cmd = $res['s_cmd'];
            $ret = array('s_id' => $s_id, 'cmd' => $s_cmd == 1);
            $ret = json_encode($ret, JSON_UNESCAPED_UNICODE);
            echo $ret; // {"s_id":"16 ","cmd":true}
        } // array
    }
}

