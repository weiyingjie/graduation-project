<?php
require_once('./functions.php');
$link = dbInit(); // 连接数据库

if (!empty($_POST)) {
    // 从前端访问的数据，将数据查询出来返回给前端显示
    $ret = array('code'=>0, 'email'=>0, 'name'=>0);

    // 判断是否存在并存入
    $sql = 'select * from user where email = "' . $_POST['email'] . '";';
    if (fetchRow($link, $sql)) {
        $ret['code'] = 1;
        $ret['email'] = 1;
    }

    $sql = 'select * from user where name = "' . $_POST['name'] . '";';
    if (fetchRow($link, $sql)) {
        $ret['code'] = 1;
        $ret['name'] = 1;
    }

    if ($ret['code'] == 0) {
        $sql = 'insert into user(u_pwd, email, name) values("' . $_POST['passwd'] . '", "' . $_POST['email'] . '", "' . $_POST['name'] . '");';
        query($link, $sql);
    }

    $ret = json_encode($ret, JSON_UNESCAPED_UNICODE);
    die($ret);
}


