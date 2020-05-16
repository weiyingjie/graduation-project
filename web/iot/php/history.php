<?php
header('Content-Type: application/json; charset=utf8');
require_once('./functions.php');
$link = dbInit(); // 连接数据库


// 设置时区
date_default_timezone_set('PRC');
// 获取时间
$time = date("Y-m-d H:i");

echo $time;

