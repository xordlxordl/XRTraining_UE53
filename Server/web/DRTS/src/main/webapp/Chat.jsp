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
    <!-- ���� ���� �Է��ϴ� �ؽ�Ʈ �ڽ�(�⺻ ���� anonymous(�͸�)) -->
    <input id="user" type="text" value="anonymous">
    <!-- �۽� �޽����� �ۼ��ϴ� �ؽ�Ʈ �ڽ� -->
    <input id="textMessage" type="text">
    <!-- �޼����� �۽��ϴ� ��ư -->
    <input onclick="sendMessage()" value="Send" type="button"> 
    <!-- WebSocket ���� �����ϴ� ��ư -->
    <input onclick="disconnect()" value="Disconnect" type="button">
  </form>
    <br />
    <!-- �ܼ� �޽����� ������ �ϴ� �α� �ؽ�Ʈ ������.(���� �޽����� ǥ���Ѵ�.) -->
    <textarea id="messageTextArea" rows="10" cols="50"></textarea>
<script src="js/DRServer.js"></script>
</body>
</html>