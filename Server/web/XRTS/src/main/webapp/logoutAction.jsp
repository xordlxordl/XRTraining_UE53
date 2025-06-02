<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>


<meta charset="UTF-8">
<title>로그아웃 페이지</title>
</head>
<body class="h-75 text-center text-bg-dark">

	<%
		session.invalidate();
	%>
	<script>
		location.href = 'index.jsp?myresult=logout';
	</script>
</body>
</html>