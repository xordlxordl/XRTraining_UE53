-- --------------------------------------------------------
-- 호스트:                          192.168.0.20
-- 서버 버전:                        11.2.2-MariaDB - mariadb.org binary distribution
-- 서버 OS:                        Win64
-- HeidiSQL 버전:                  12.8.0.6908
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- 테이블 데이터 arts.tb_admin:~10 rows (대략적) 내보내기
INSERT INTO `tb_admin` (`UID`, `Account`, `Pass`, `num`, `nGrade`, `name`, `nGroup`, `nDateTime`) VALUES
	(1, 'admin', '33104197ce1f8bc6537e6748d844e17a', '9676037089', 1, '슈퍼맨', '크립토나이트', '2024-01-12 15:28:31'),
	(2, 'interact', '628631f07321b22d8c176c200c855e1b', '9876543210', 2, '잍랙트', '관제계정수정테스트', '2024-01-23 14:27:43');

-- 테이블 데이터 arts.tb_error:~12 rows (대략적) 내보내기
INSERT INTO `tb_error` (`error`, `cont`) VALUES
	(-43, '장비ID가 없음'),
	(-42, '훈련자ID가 없음'),
	(-41, '훈련 결과가 이미 있음'),
	(-40, '훈련 시작 번호 없음'),
	(-7, '보직ID 중복'),
	(-6, '훈련자 중복'),
	(-5, '군번 중복'),
	(-4, '비밀번호 오류'),
	(-2, '계정 없음'),
	(-1, '계정 중복'),
	(0, '실패'),
	(1, '성공');

-- 테이블 데이터 arts.tb_machine:~20 rows (대략적) 내보내기
INSERT INTO `tb_machine` (`nID`, `name`, `BPC`, `HMD`, `UTT1`, `UTT2`, `UTT3`, `UTT4`, `nDateTime`) VALUES
	(1, 'VR-01', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(2, 'VR-02', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(3, 'VR-03', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(4, 'VR-04', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(5, 'VR-05', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(6, 'VR-06', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(7, 'VR-07', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(8, 'VR-08', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(9, 'VR-09', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(10, 'VR-10', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(11, 'VR-11', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(12, 'VR-12', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(13, 'VR-13', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(14, 'VR-14', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(15, 'VR-15', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(16, 'VR-16', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(17, 'VR-17', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(18, 'VR-18', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(19, 'VR-19', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34'),
	(20, 'VR-20', NULL, NULL, NULL, NULL, NULL, NULL, '2024-12-11 17:28:34');

-- 테이블 데이터 arts.tb_month:~12 rows (대략적) 내보내기
INSERT INTO `tb_month` (`Month`) VALUES
	(1),
	(2),
	(3),
	(4),
	(5),
	(6),
	(7),
	(8),
	(9),
	(10),
	(11),
	(12);

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
