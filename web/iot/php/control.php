<?php
require_once('./functions.php');
$link = dbInit(); // 连接数据库
// 开启session
session_start();

if (isset($_POST['code'])) {
    if ($_POST['code'] == 'session') {
        // 获取session
        $ret = array('status' => 'nologin', 'u_id' => '', 'email' => '', 'name' => '');
        if (isset($_SESSION['userinfo'])) {
            $ret['status'] = 'login';
            $ret['u_id'] = $_SESSION['userinfo']['u_id'];
            $ret['email'] = $_SESSION['userinfo']['email'];
            $ret['name'] = $_SESSION['userinfo']['name'];
        }
    } elseif ($_POST['code'] == 'logout') {
        // 注销账户
        unset($_SESSION['userinfo']);
        if (empty($_SESSION)) {
            session_destroy();
        }
        $ret = array('status' => 'logoutOk');
    } elseif ($_POST['code'] == 'deviceAndSensor') {
        // 获取设备数量
        $device = $sensor = 0;
        $ret = array('status' => 'deviceAndSensor', 'devices' => 0, 'sensors' => 0);
        $sql = 'select * from devices where u_id = "' . $_POST['u_id'] . '";';
        if ($res_d = fetchAll($link, $sql)) {
            foreach ($res_d as $d) {
                $sql = 'select * from sensors where d_id = "' . $d['d_id'] . '";';
                if ($res_s = fetchAll($link, $sql)) {
                    $sensor += sizeof($res_s);
                }
            }
            $device += sizeof($res_d);
            $ret['devices'] = $device;
            $ret['sensors'] = $sensor;
        }
    } elseif ($_POST['code'] == 'devices') {
        // 获取设备信息
        $ret = array('status' => 'devices');
        $count = 0;
        $sql = 'select * from devices where u_id = "' . $_POST['u_id'] . '";';
        if ($res = fetchAll($link, $sql)) {
            foreach ($res as $v) {
                $tmp = array('des' => $v['des'], 'd_name' => $v['d_name'], 'd_id' => $v['d_id']);
                array_push($ret, $tmp);
                $count++;
            }
        }
        $ret['count'] = $count;
    } elseif ($_POST['code'] == 'addDevice') {
        // 添加设备
        $ret = array('status' => 'addDeviceError');
        $sql = 'insert into devices(u_id, d_name, des) values("' . $_POST['u_id'] . '", "' . $_POST['d_name'] . '", "' . $_POST['des'] . '");';
        if ($res = query($link, $sql)) {
            $ret['status'] = 'addDeviceOk';
        }
    } elseif ($_POST['code'] == 'delDevice') {
        // 删除设备
        $ret = array('status' => 'delDeviceError');
        $sql = 'delete from devices where d_id = "' . $_POST['d_id'] . '";';
        if ($res = query($link, $sql)) {
            $ret['status'] = 'delDeviceOk';
        }
    } elseif ($_POST['code'] == 'getSensors') {
        // 获取传感器信息
        $ret = array('status' => 'getSensors');
        $count = 0;
        $sql = 'select * from sensors where d_id = "' . $_POST['d_id'] . '";';
        if ($res = fetchAll($link, $sql)) {
            foreach ($res as $v) {
                $tmp = array('s_id' => $v['s_id'], 's_name' => $v['s_name'], 'isData' => $v['s_type'] == 'data', 's_data' => $v['s_data'], 's_mst' => $v['s_mst']);
                array_push($ret, $tmp);
                $count++;
            }
        }
        $ret['count'] = $count;
    } elseif ($_POST['code'] == 'getDev') {
        // 获取传感器信息
        $ret = array('status' => 'getDev');
        $count = 0;
        $sql = 'select * from devices where d_id = "' . $_POST['d_id'] . '";';
        if ($res = fetchRow($link, $sql)) {
            array_push($ret, $res);
        }
        $count = 0;
        $sql = 'select * from sensors where d_id = "' . $_POST['d_id'] . '";';
        if ($res = fetchAll($link, $sql)) {
            foreach ($res as $v) {
                $count++;
            }
        }
        $ret['0']['count'] = $count;
    } elseif ($_POST['code'] == 'addSensor') {
        // 添加传感器
        $ret = array('status' => 'addSensorError');
        $sql = 'insert into sensors(d_id, s_name, s_type, s_data, s_mst) values("' . $_POST['d_id'] . '", "' . $_POST['s_name'] . '", "' . $_POST['s_type'] . '", "0", "' . $_POST['s_mst'] . '");';
        if ($res = query($link, $sql)) {
            $ret['status'] = 'addSensorOk';
        }
    } elseif ($_POST['code'] == 'delSensor') {
        // 删除传感器
        $ret = array('status' => 'delSensorError');
        $sql = 'delete from sensors where s_id = "' . $_POST['s_id'] . '";';
        if ($res = query($link, $sql)) {
            $ret['status'] = 'delSensorOk';
        }
    } elseif ($_POST['code'] == 'getHistory') {
        // 查看历史
        $ret = array('status' => 'getHistoryError');
        $count = 0;
        $sql = 'select * from history where d_id = "' . $_POST['d_id'] . '" order by h_id desc limit 300;';
        if ($res = fetchAll($link, $sql)) {
            foreach ($res as $v) {
                $tmp = array('d_id' => $v['d_id'], 's_id' => $v['s_id'], 's_name' => $v['s_name'], 's_data' => $v['s_data'], 'time' => $v['time']);
                array_push($ret, $tmp);
                $count++;
            }
            $ret['count'] = $count;
            $ret['status'] = 'getHistoryOk';
        }
    } elseif ($_POST['code'] == 'getD_id') {
        // 查看历史
        $ret = array('status' => 'getD_idError');
        $count = 0;
        $sql = 'select d_id from devices where u_id = "' . $_POST['u_id'] . '";';
        if ($res = fetchAll($link, $sql)) {
            foreach ($res as $v) {
                array_push($ret, $v['d_id']);
                $count++;
            }
            $ret['count'] = $count;
            $ret['status'] = 'getD_idOk';
        }
    }


    $ret = json_encode($ret, JSON_UNESCAPED_UNICODE);
    echo $ret;
}



