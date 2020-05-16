<?php
// 连接数据库
function dbInit()
{
    $link = mysqli_connect('localhost', 'root', '123456');
    //判断数据库连接是否成功，如果不成功则显示错误信息并终止脚本继续执行
    if (!$link) {
        die('连接数据库失败！' . mysqli_error());
    }
    //设置字符集，选择数据库
    mysqli_query($link, 'set names utf8');
    mysqli_query($link, 'use iot');
    return $link; // 返回连接情况true/false
}

// 执行SQL语句,正确返回结果，错误返回错误并结束
function query($link, $sql)
{
    if ($result = mysqli_query($link, $sql)) {
        return $result;
    } else {
        echo 'SQL语句错误<br/>';
        echo '错误的SQL：' . $sql . '<br/>';
        echo '错误代码：' . mysqli_errno($link) . '<br/>';
        echo '错误信息：' . mysqli_error($link) . '<br/>';
        die; // 错误结束程序
    }
}

function fetchAll($link, $sql)
{

    //执行query()函数
    if ($result = query($link, $sql)) {
        //执行成功
        //遍历结果集
        $rows = array();
        while ($row = mysqli_fetch_array($result, MYSQLI_ASSOC)) {
            $rows[] = $row;
        }
        //释放结果集资源
        mysqli_free_result($result);
        return $rows;

    } else {
        //执行失败
        return false;
    }
}

function fetchRow($link, $sql)
{
    //执行query()函数
    if ($result = query($link, $sql)) {
        //从结果集取得一次数据即可
        $row = mysqli_fetch_array($result, MYSQLI_ASSOC);
        return $row;
    } else {
        return false;
    }
}
