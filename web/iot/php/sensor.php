<?php
header('Content-Type: application/json; charset=utf8');
require_once('./functions.php');
$link = dbInit(); // 连接数据库

if (isset($_POST['code'])) {
    // 前端请求
    if ($_POST['code'] == 'updateSensor') {
        // 更新传感器
        $ret = array('status' => 'updateSensorError');
        $sql = 'update sensors set s_data = "' . $_POST['s_data'] . '" where s_id = "' . $_POST['s_id'] . '";';
        if ($res = query($link, $sql)) {
            $ret['status'] = 'updateSensorOk';
        }
    }

    $ret = json_encode($ret, JSON_UNESCAPED_UNICODE);
    echo $ret;
}



