<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html" charset="utf-8">
	<meta name="viewport" content="width=device-width" >
	
	<link rel="stylesheet" href="css/bootstrap.css">
	<link rel="stylesheet" href="css/dropdowns.css">
	<link rel="stylesheet" href="css/cover.css">
	<link rel="stylesheet" href="css/sidebars.css">
	<link rel="stylesheet" href="css/dark.css">
	<link rel="stylesheet" href="css/style.css">

	<script src="js/jquery-3.6.4.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	
	<script src="js/jquery.bootstrap-growl.min.js"></script>
	
	<script src="js/sweetalert2.js"></script>	
	
	<script src="js/my.js"></script>

<meta charset="UTF-8">
<title>로그아웃 페이지</title>
</head>
<body class="h-75 text-center text-bg-dark">

	<%
		session.invalidate();
	%>
	<script>
		LogOutMsg( '로그아웃 성공' );
	</script>
</body>
</html>