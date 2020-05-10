<?php
require_once('./functions.php');
$link = dbInit(); // 连接数据库

if (!empty($_POST)) {
// 从前端访问的数据，将数据查询出来返回给前端显示
    $ret = array('code' => 0, 'email' => 0, 'passwd' => 0);

    // 判断是否存在
    $sql = 'select * from user where email = "' . $_POST['email'] . '";';
    if (fetchRow($link, $sql)) {
        // 账号存在 判断密码
        $sql = 'select * from user where email = "' . $_POST['email'] . '" and u_pwd = "' . $_POST['passwd'] . '";';
        if (!($res = fetchRow($link, $sql))) {
            // 密码错误 1 0 1
            $ret['code'] = 1;
            $ret['passwd'] = 1;
        } else {
            session_start();
            $_SESSION['userinfo']=array(
                'u_id'=>$res['u_id'],
                'email'=>$res['email'],
                'name'=>$res['name']
            );
        }
    } else {
        // 账号不存在 1 1 0
        $ret['code'] = 1;
        $ret['email'] = 1;
    }

    $ret = json_encode($ret, JSON_UNESCAPED_UNICODE);
    echo($ret);
}



