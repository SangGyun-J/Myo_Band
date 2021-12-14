<?php
$conn = mysqli_connect("localhost", "rodemeng", "computer21#","rodemeng")or die("db connect error");

//A조 첫번째 근무자
$apart_name = "양지아파트";
$mananger_name = "성경주";
$phone_num = "010-4599-0538";
$position = "경리";
// 두번째 근무자
$apart_name2 = "양지아파트";
$mananger_name2 = "이광복";
$phone_num2 = "010-9899-0445";
$position2 = "경비";
$boolean = 1;

$sql = "INSERT INTO `apart_alarm`(`apart_name`, `manager_name`, `phone_num`, `position`,`alarm`) VALUES('$apart_name','$mananger_name','$phone_num','$position','$boolean')";
$sql2 = "INSERT INTO `apart_alarm`(`apart_name`, `manager_name`, `phone_num`, `position`,`alarm`) VALUES('$apart_name2','$mananger_name2','$phone_num2','$position2','$boolean')";

if(!mysqli_query($conn, $sql)) {    //만약 변수 sql에 담긴 쿼리를 실행하였는데 에러가 나면
    echo mysqli_error($conn);       //에러 시, 에러 내용을 화면에 출력합니다.
    die("query error");             //괄호 안의 문장을 출력한 후 스크립트를 종료합니다.
}
if(!mysqli_query($conn, $sql2)) {    //만약 변수 sql에 담긴 쿼리를 실행하였는데 에러가 나면
    echo mysqli_error($conn);       //에러 시, 에러 내용을 화면에 출력합니다.
    die("query error");             //괄호 안의 문장을 출력한 후 스크립트를 종료합니다.
}
mysqli_close($conn);

?>
