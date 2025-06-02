<%@ page language="java" contentType="text/html; charset=EUC-KR"
    pageEncoding="EUC-KR"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="EUC-KR">
<title>DR Chatting</title>
</head>
<body>
  <form>
    <!-- 유저 명을 입력하는 텍스트 박스(기본 값은 anonymous(익명)) -->
    <input id="user" type="text" value="anonymous">
    <!-- 송신 메시지를 작성하는 텍스트 박스 -->
    <input id="textMessage" type="text">
    <!-- 메세지를 송신하는 버튼 -->
    <input onclick="sendMessage()" value="Send" type="button"> 
    <!-- WebSocket 접속 종료하는 버튼 -->
    <input onclick="disconnect()" value="Disconnect" type="button">
  </form>
    <br />
    <!-- 콘솔 메시지의 역할을 하는 로그 텍스트 에리어.(수신 메시지도 표시한다.) -->
    <textarea id="messageTextArea" rows="10" cols="50"></textarea>
<script src="js/DRServer.js"></script>
</body>
</html>