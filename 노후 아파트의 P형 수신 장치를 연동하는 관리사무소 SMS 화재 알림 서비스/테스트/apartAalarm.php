<?php
$conn = mysqli_connect("localhost", "rodemeng", "computer21#","rodemeng")or die("db connect error");

//A조 첫번째 근무자
$apart_name = "양지아파트";
$mananger_name = "홍길동";
$phone_num = "010-1234-5567";
$position = "전기과장";
// 두번째 근무자
$apart_name2 = "양지아파트";
$mananger_name2 = "이중희";
$phone_num2 = "010-9899-0435";
$position2 = "시설과장";
$boolean = 1;

//sql 문 -> 데이터베이스에 내용 집어넣기 위한 구문
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
