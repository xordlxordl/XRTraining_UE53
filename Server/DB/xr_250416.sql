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


-- arts 데이터베이스 구조 내보내기
CREATE DATABASE IF NOT EXISTS `arts` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */;
USE `arts`;

-- 프로시저 arts.log_badfire 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_badfire`(
	IN `$TID` INT,
	IN `$UID` INT
)
BEGIN

	INSERT INTO arts.tl_badfire
	( TID, UID, iCount, nDateTime )
	VALUES
	( `$TID`, `$UID`, 1, now() );
	
END//
DELIMITER ;

-- 프로시저 arts.log_bind 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_bind`(
	IN `$TID` INT,
	IN `$UID` INT
)
BEGIN

	INSERT INTO arts.tl_bind
	( TID, UID, iCount, nDateTime )
	VALUES
	( `$TID`, `$UID`, 1, now() );
	
END//
DELIMITER ;

-- 프로시저 arts.log_breifsituation 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_breifsituation`(
	IN `$TID` INT,
	IN `$UID` INT
)
BEGIN

	INSERT INTO arts.tl_breifsituation
	( TID, UID, iCount, nDateTime )
	VALUES
	( `$TID`, `$UID`, 1, now() );
	
END//
DELIMITER ;

-- 프로시저 arts.log_call 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_call`(
	IN `$TID` INT,
	IN `$UID` INT
)
BEGIN

	INSERT INTO arts.tl_call
	( TID, UID, iCount, nDateTime )
	VALUES
	( `$TID`, `$UID`, 1, now() );
	
END//
DELIMITER ;

-- 프로시저 arts.log_challenge 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_challenge`(
	IN `$TID` INT,
	IN `$UID` INT
)
BEGIN

INSERT INTO arts.tl_challenge
( TID, UID, iCount, nDateTime)
VALUES
( $TID, $UID, 1, NOW() );

END//
DELIMITER ;

-- 프로시저 arts.log_commandmove 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_commandmove`(
	IN `$TID` INT,
	IN `$UID` INT
)
BEGIN

	INSERT INTO arts.tl_commandmove
	( TID, UID, iCount, nDateTime )
	VALUES
	( `$TID`, `$UID`, 1, now() );
	
END//
DELIMITER ;

-- 프로시저 arts.log_distance 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_distance`(
	IN `$TID` INT,
	IN `$UID` INT,
	IN `$dist` FLOAT
)
BEGIN

INSERT INTO arts.tl_distance
( TID, UID, dist, nDateTime)
VALUES
( $TID, $UID, $dist, NOW() );

END//
DELIMITER ;

-- 프로시저 arts.log_eod 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_eod`(
	IN `$TID` INT,
	IN `$UID` INT
)
BEGIN

	INSERT INTO arts.tl_eod
	( TID, UID, iCount, nDateTime )
	VALUES
	( `$TID`, `$UID`, 1, now() );
	
END//
DELIMITER ;

-- 프로시저 arts.log_fire 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_fire`(
	IN `$TID` INT,
	IN `$UID` INT
)
    COMMENT '사격 로그 프로시저'
BEGIN

INSERT INTO arts.tl_fire
	( TID, UID, nDateTime )
VALUES
	( `$TID`, `$UID`, now() );
	
END//
DELIMITER ;

-- 프로시저 arts.log_hit 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_hit`(
	IN `$TID` INT,
	IN `$UID` INT
)
    COMMENT '사격 적중 로그'
BEGIN

INSERT INTO arts.tl_hit
( TID, UID, nDateTime )
VALUES
( `$TID`, `$UID`, now() );
	
END//
DELIMITER ;

-- 프로시저 arts.log_motion 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_motion`(
	IN `$TID` INT,
	IN `$str` MEDIUMTEXT
)
BEGIN
	INSERT INTO arts.tl_motion
	(TID, str, nDateTime)
	VALUES
	( $TID, $str, now());
END//
DELIMITER ;

-- 프로시저 arts.log_object 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_object`(
	IN `$TID` INT,
	IN `$str` VARCHAR(8192)
)
BEGIN
	
INSERT INTO arts.tl_object
( TID, str, nDateTime)
VALUES
( `$TID`, `$str`, now() );

END//
DELIMITER ;

-- 프로시저 arts.log_surrender 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_surrender`(
	IN `$TID` INT,
	IN `$UID` INT
)
BEGIN
	
INSERT INTO arts.tl_surrender
( TID, UID, iCount, nDateTime )
VALUES
( `$TID`, `$UID`, 1, now() );

END//
DELIMITER ;

-- 프로시저 arts.log_tr_gun 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_tr_gun`(
	IN `$TID` INT,
	IN `$UID` INT,
	IN `$x` FLOAT,
	IN `$y` FLOAT,
	IN `$z` FLOAT,
	IN `$p` FLOAT,
	IN `$r` FLOAT,
	IN `$w` FLOAT
)
BEGIN

INSERT INTO arts.tl_tr_gun
( TID, UID, x, y, z, p, r, w, nDateTime )
VALUES
( `$TID`, `$UID`, `$x`, `$y`, `$z`, `$p`, `$r`, `$w`, now() );
	
END//
DELIMITER ;

-- 프로시저 arts.log_tr_hmd 구조 내보내기
DELIMITER //
CREATE PROCEDURE `log_tr_hmd`(
	IN `$TID` INT,
	IN `$UID` INT,
	IN `$x` FLOAT,
	IN `$y` FLOAT,
	IN `$z` FLOAT,
	IN `$p` FLOAT,
	IN `$r` FLOAT,
	IN `$w` FLOAT
)
BEGIN
	
INSERT INTO arts.tl_tr_hmd
( TID, UID, x, y, z, p, r, w, nDateTime )
VALUES
( `$TID`, `$UID`, `$x`, `$y`, `$z`, `$p`, `$r`, `$w`, now() );

END//
DELIMITER ;

-- 프로시저 arts.spAddAdmin 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddAdmin`(
	IN `$account` VARCHAR(100),
	IN `$pass` VARCHAR(100)
)
PROC_BODY : BEGIN

if EXISTS( SELECT * FROM tb_admin WHERE ACCOUNT = $ACCOUNT )
THEN

 	SELECT -1 AS Result;
	LEAVE PROC_BODY;

END if;

	INSERT INTO arts.tb_admin
	( Account, Pass, nGrade, nDateTime )
	VALUES
	( $account, MD5( $pass ), 1, now() );

	SELECT 1 AS RESULT;

END//
DELIMITER ;

-- 프로시저 arts.spAddEffect 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddEffect`(
	IN `$filename` VARCHAR(250),
	IN `$fullpath` VARCHAR(250)
)
BEGIN

DECLARE $i int;

SELECT i INTO $i FROM arts.tb_effect WHERE filename = $filename;

IF $i IS NULL
THEN

	INSERT INTO arts.tb_effect
	( filename, savename, fullpath, ver, nDateTime )
	VALUES
	( $filename, $filename, $fullpath, 1, now() );

ELSE

	UPDATE arts.tb_effect
	SET ver = ver + 1, nDateTime = now()
	WHERE i = $i;

END IF;

CALL spAddFile( $filename, $fullpath, 9 ); 

	
	
END//
DELIMITER ;

-- 프로시저 arts.spAddFBX 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddFBX`(
	IN `$filename` VARCHAR(250),
	IN `$fullpath` VARCHAR(250)
)
BEGIN
	
DECLARE $i int;

SELECT i INTO $i FROM arts.tb_fbx WHERE filename = $filename;

IF $i IS NULL
THEN

	INSERT INTO arts.tb_fbx
	( filename, savename, fullpath, ver, nDateTime )
	VALUES
	( $filename, $filename, $fullpath, 1, now() );

ELSE

	UPDATE arts.tb_fbx
	SET ver = ver + 1, nDateTime = now()
	WHERE i = $i;

END IF;

CALL spAddFile( $filename, $fullpath, 5 ); 

	
END//
DELIMITER ;

-- 프로시저 arts.spAddFBXAnim 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddFBXAnim`(
	IN `$filename` VARCHAR(250),
	IN `$fullpath` VARCHAR(250)
)
BEGIN

			
DECLARE $i int;

SELECT i INTO $i FROM arts.tb_fbxanim WHERE filename = $filename;

IF $i IS NULL
THEN

	INSERT INTO arts.tb_fbxanim
	( filename, savename, fullpath, ver, nDateTime )
	VALUES
	( $filename, $filename, $fullpath, 1, now() );

ELSE

	UPDATE arts.tb_fbxanim
	SET ver = ver + 1, nDateTime = now()
	WHERE i = $i;

END IF;

CALL spAddFile( $filename, $fullpath, 8 ); 

END//
DELIMITER ;

-- 프로시저 arts.spAddFile 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddFile`(
	IN `$filename` VARCHAR(250),
	IN `$fullpath` VARCHAR(250),
	IN `$filetype` INT
)
BEGIN

DECLARE $i int;

SELECT i INTO $i FROM arts.tb_file WHERE filename = $filename;

IF $i IS NULL
THEN

	INSERT INTO arts.tb_file
	( filename, savename, fullpath, ver, nDateTime, filetype )
	VALUES
	( $filename, $filename, $fullpath, 1, NOW(), $filetype );

ELSE

	UPDATE arts.tb_file
	SET ver = ver + 1, nDateTime = now()
	WHERE i = $i;

END IF;
	
	
END//
DELIMITER ;

-- 프로시저 arts.spAddMCT 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddMCT`(
	IN `$filename` VARCHAR(250),
	IN `$fullpath` VARCHAR(250)
)
BEGIN
	
DECLARE $i int;

SELECT i INTO $i FROM arts.tb_mct WHERE filename = $filename;

IF $i IS NULL
THEN

	INSERT INTO arts.tb_mct
	( filename, savename, fullpath, ver, nDateTime )
	VALUES
	( $filename, $filename, $fullpath, 1, now() );

ELSE

	UPDATE arts.tb_mct
	SET ver = ver + 1, nDateTime = now()
	WHERE i = $i;

END IF;

CALL spAddFile( $filename, $fullpath, 2 ); 

END//
DELIMITER ;

-- 프로시저 arts.spAddPNG 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddPNG`(
	IN `$filename` VARCHAR(250),
	IN `$fullpath` VARCHAR(250)
)
BEGIN
	
DECLARE $i int;

SELECT i INTO $i FROM arts.tb_png WHERE filename = $filename;

IF $i IS NULL
THEN

	INSERT INTO arts.tb_png
	( filename, savename, fullpath, ver, nDateTime )
	VALUES
	( $filename, $filename, $fullpath, 1, now() );

ELSE

	UPDATE arts.tb_png
	SET ver = ver + 1, nDateTime = now()
	WHERE i = $i;

END IF;

CALL spAddFile( $filename, $fullpath, 6 ); 

END//
DELIMITER ;

-- 프로시저 arts.spAddRePlay 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddRePlay`(
	IN `$filename` VARCHAR(200),
	IN `$fullpath` VARCHAR(400),
	IN `$TID` INT,
	IN `$UID` INT
)
BEGIN

	DECLARE $nID bigint;
	

INSERT INTO arts.tb_replay
( TID, UID, filename, FullPath, nDateTime )
VALUES
( `$TID`, `$UID`, `$filename`, `$fullpath`, now() );

	
SET $nID = LAST_INSERT_ID();

SELECT nID, filename FROM arts.tb_replay WHERE nID = $nID;
	
END//
DELIMITER ;

-- 프로시저 arts.spAddRole 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddRole`(
	IN `$RID` INT,
	IN `$RName` VARCHAR(50)
)
BODY_PROC : BEGIN
	
	IF exists( SELECT * FROM arts.tb_role WHERE RID = $RID )
	THEN 
		SELECT -7 AS RESULT;
		LEAVE BODY_PROC;
	END IF;

INSERT INTO arts.tb_role
( RID, RName )
VALUES
( `$RID`, `$RName` );

SELECT 1 AS RESULT;

END//
DELIMITER ;

-- 프로시저 arts.spAddSCT 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddSCT`(
	IN `$filename` VARCHAR(250),
	IN `$fullpath` VARCHAR(250)
)
BEGIN
	
DECLARE $i int;

SELECT i INTO $i FROM arts.tb_sct WHERE filename = $filename;

IF $i IS NULL
THEN

	INSERT INTO arts.tb_sct
	( filename, savename, fullpath, ver, nDateTime )
	VALUES
	( $filename, $filename, $fullpath, 1, now() );

ELSE

	UPDATE arts.tb_sct
	SET ver = ver + 1, nDateTime = now()
	WHERE i = $i;

END IF;

CALL spAddFile( $filename, $fullpath, 1 ); 

END//
DELIMITER ;

-- 프로시저 arts.spAddTCT 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddTCT`(
	IN `$filename` VARCHAR(250),
	IN `$fullpath` VARCHAR(250)
)
BEGIN
	
DECLARE $i int;

SELECT i INTO $i FROM arts.tb_tct WHERE filename = $filename;

IF $i IS NULL
THEN

	INSERT INTO arts.tb_tct
	( filename, savename, fullpath, ver, nDateTime )
	VALUES
	( $filename, $filename, $fullpath, 1, now() );

ELSE

	UPDATE arts.tb_tct
	SET ver = ver + 1, nDateTime = now()
	WHERE i = $i;

END IF;

CALL spAddFile( $filename, $fullpath, 4 ); 

END//
DELIMITER ;

-- 프로시저 arts.spAddTrainer 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddTrainer`(
	IN `$account` VARCHAR(100),
	IN `$pass` VARCHAR(100),
	IN `$num` VARCHAR(100),
	IN `$name` VARCHAR(50),
	IN `$nGroup` VARCHAR(100)
)
PROC_BODY : BEGIN

	if EXISTS( SELECT * FROM tb_admin WHERE ACCOUNT = $ACCOUNT )
	THEN

	 	SELECT -1 AS Result;
		LEAVE PROC_BODY;

	END if;
	

	if EXISTS( SELECT * FROM tb_admin WHERE num = $num )
	THEN
		SELECT -5 AS Result;
		LEAVE PROC_BODY;
	END IF;


	INSERT INTO arts.tb_admin
	( Account, Pass, num, nGrade, name, nGroup, nDateTime )
	VALUES
	( $account, MD5( $pass ), $num, 2, $name, $nGroup, now() );


	SELECT 1 AS RESULT;
END//
DELIMITER ;

-- 프로시저 arts.spAddUser 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddUser`(
	IN `$name` VARCHAR(100),
	IN `$num` VARCHAR(100),
	IN `$group` VARCHAR(100),
	IN `$height` INT
)
PROC_BODY : BEGIN

	IF exists( SELECT * FROM arts.tb_user WHERE num = $num )
	THEN 
		SELECT -6 AS RESULT;
		LEAVE PROC_BODY;
	END IF;
	
	INSERT INTO arts.tb_user ( name, num, nGroup, height, nDateTime) VALUES( `$name`, `$num`, `$group`, `$height`,  NOW() );

	

	SELECT 1 AS RESULT;

END//
DELIMITER ;

-- 프로시저 arts.spAddVoice 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddVoice`(
	IN `$filename` VARCHAR(400),
	IN `$fullpath` VARCHAR(400),
	IN `$tid` INT,
	IN `$uid` INT
)
PROC_BODY : BEGIN

DECLARE $i bigint;
	
IF exists( SELECT * FROM arts.TB_VOICE WHERE TID = $tid AND UID = $uid )
THEN 

	SELECT -41 AS RESULT;
	LEAVE PROC_BODY;

END IF;


	
INSERT INTO arts.tb_voice
( TID, UID, filename, fullpath, nDateTime)
VALUES
( $tid, $uid, $filename, $fullpath, now() );


SET $i = LAST_INSERT_ID();

SELECT i, filename FROM arts.tb_voice WHERE i = $i;

END//
DELIMITER ;

-- 프로시저 arts.spAddWAV 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddWAV`(
	IN `$filename` VARCHAR(250),
	IN `$fullpath` VARCHAR(250)
)
BEGIN

		
DECLARE $i int;

SELECT i INTO $i FROM arts.tb_wav WHERE filename = $filename;

IF $i IS NULL
THEN

	INSERT INTO arts.tb_wav
	( filename, savename, fullpath, ver, nDateTime )
	VALUES
	( $filename, $filename, $fullpath, 1, now() );

ELSE

	UPDATE arts.tb_wav
	SET ver = ver + 1, nDateTime = now()
	WHERE i = $i;

END IF;

CALL spAddFile( $filename, $fullpath, 7 ); 
	
END//
DELIMITER ;

-- 프로시저 arts.spAddWCT 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spAddWCT`(
	IN `$filename` VARCHAR(250),
	IN `$fullpath` VARCHAR(250)
)
BEGIN

			
DECLARE $i int;

SELECT i INTO $i FROM arts.tb_wct WHERE filename = $filename;

IF $i IS NULL
THEN

	INSERT INTO arts.tb_wct
	( filename, savename, fullpath, ver, nDateTime )
	VALUES
	( $filename, $filename, $fullpath, 1, now() );

ELSE

	UPDATE arts.tb_wct
	SET ver = ver + 1, nDateTime = now()
	WHERE i = $i;

END IF;

CALL spAddFile( $filename, $fullpath, 3 ); 

END//
DELIMITER ;

-- 프로시저 arts.spCheckFile 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spCheckFile`(
	IN `$filename` VARCHAR(250),
	IN `$filetype` INT
)
PROC_BODY : BEGIN

IF NOT EXISTS( SELECT * FROM arts.tb_file WHERE filename = $filename AND filetype = $filetype )
THEN

	SELECT 0 AS RESULT;

	LEAVE PROC_BODY;

END IF;

SELECT 1 AS RESULT, fullpath FROM arts.tb_file WHERE filename = $filename and filetype = $filetype;
	
END//
DELIMITER ;

-- 프로시저 arts.spEnterUser 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spEnterUser`(
	IN `$TID` INT,
	IN `$UID` INT,
	IN `$MID` INT,
	IN `$Job` VARCHAR(200)
)
PROC_BODY : BEGIN

IF NOT EXISTS( SELECT * FROM arts.tl_start WHERE TID =`$TID` )
THEN 
	SELECT -40 AS RESULT;
	LEAVE PROC_BODY;
END IF;

IF NOT EXISTS( SELECT * FROM arts.tb_user WHERE UID =`$UID` )
THEN 
	SELECT -42 AS RESULT;
	LEAVE PROC_BODY;
END IF;


IF NOT EXISTS( SELECT * FROM arts.tb_machine WHERE nID =`$MID` )
THEN 
	SELECT -43 AS RESULT;
	LEAVE PROC_BODY;
END IF;

	
INSERT INTO arts.tl_userinfo
( TID, UID, MID, Job, nDateTime )
VALUES
( `$TID`, `$UID`, `$MID`, `$Job`, now() );

SELECT 1 AS RESULT;

END//
DELIMITER ;

-- 프로시저 arts.spFinish 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spFinish`(
	IN `$TID` INT
)
PROC_BODY : BEGIN

IF NOT EXISTS( SELECT * FROM arts.tl_start WHERE TID =`$TID` )
THEN 
	SELECT -40 AS RESULT;
	LEAVE PROC_BODY;
END IF;
	
IF exists( SELECT * FROM arts.TL_RESULT WHERE TID = `$TID` )
THEN 
	SELECT -41 AS RESULT;
	LEAVE PROC_BODY;
END IF;

INSERT INTO arts.tl_result
	( TID, nStartTime, nEndTime, PlaySec, name, nDateTime )
SELECT 
	TID, 
	nStartTime, 
	NOW(), 
	TIMESTAMPDIFF( SECOND, nStartTime, NOW() ) AS PlayTime, 
	TName, 
	NOW() 
FROM arts.tl_start WHERE TID = `$TID`;

SELECT 1 AS RESULT;
	

END//
DELIMITER ;

-- 프로시저 arts.spGetAllFileList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetAllFileList`()
BEGIN

SELECT i, filename, savename, fullpath, ver, nDateTime, filetype
FROM arts.tb_file;

END//
DELIMITER ;

-- 프로시저 arts.spGetEffList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetEffList`()
BEGIN


SELECT i, filename, savename, fullpath, ver, nDateTime
FROM arts.tb_effect;
END//
DELIMITER ;

-- 프로시저 arts.spGetFBXAnimList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetFBXAnimList`()
BEGIN

SELECT i, filename, savename, fullpath, ver, nDateTime
FROM arts.tb_fbxanim;

END//
DELIMITER ;

-- 프로시저 arts.spGetFBXList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetFBXList`()
BEGIN
SELECT i, filename, savename, fullpath, ver, nDateTime
FROM arts.tb_fbx;
END//
DELIMITER ;

-- 프로시저 arts.spGetFullPath 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetFullPath`(
	IN `$filename` VARCHAR(250)
)
PROC_BODY : BEGIN
	
IF NOT EXISTS( SELECT * FROM arts.tb_file WHERE filename = $filename )
THEN

	SELECT 0 AS RESULT;

	LEAVE PROC_BODY;
END IF;

SELECT 1 AS RESULT, fullpath FROM arts.tb_file WHERE filename = $filename;

END//
DELIMITER ;

-- 프로시저 arts.spGetLogMotion 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetLogMotion`(
	IN `$TID` INT
)
BEGIN

SELECT i, str, nDateTime
FROM arts.tl_motion WHERE TID = $TID;

END//
DELIMITER ;

-- 프로시저 arts.spGetMachineData 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetMachineData`(
	IN `$nID` INT
)
BEGIN

	SELECT * FROM arts.tb_machine WHERE nID = $nID;
	
END//
DELIMITER ;

-- 프로시저 arts.spGetMachineList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetMachineList`()
BEGIN

	SELECT * FROM arts.tb_machine;
	
END//
DELIMITER ;

-- 프로시저 arts.spGetMCTList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetMCTList`()
BEGIN
SELECT i, filename, savename, fullpath, ver, nDateTime
FROM arts.tb_mct;
END//
DELIMITER ;

-- 프로시저 arts.spGetObjectLog 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetObjectLog`(
	IN `$TID` INT
)
BEGIN

	SELECT i,  str, nDateTime FROM arts.tl_object WHERE TID = $TID;
	
END//
DELIMITER ;

-- 프로시저 arts.spGetPNGList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetPNGList`()
BEGIN
SELECT i, filename, savename, fullpath, ver, nDateTime
FROM arts.tb_png;
END//
DELIMITER ;

-- 프로시저 arts.spGetReplayFile 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetReplayFile`(
IN `$nID` INT
)
BEGIN

	SELECT FullPath FROM arts.tb_replay WHERE nID = $nID;
	
END//
DELIMITER ;

-- 프로시저 arts.spGetReplayFileTIDandUID 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetReplayFileTIDandUID`(
	IN `$TID` INT,
	IN `$UID` INT
)
BEGIN

	SELECT  FullPath FROM arts.tb_replay WHERE TID = `$TID` and UID = `$UID`;
	
END//
DELIMITER ;

-- 프로시저 arts.spGetReplayList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetReplayList`()
BEGIN
	
SELECT nID, filename FROM arts.tb_replay;

END//
DELIMITER ;

-- 프로시저 arts.spGetRoleList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetRoleList`()
BEGIN

	SELECT RID, RName FROM arts.tb_role;
	
END//
DELIMITER ;

-- 프로시저 arts.spGetSCTList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetSCTList`()
BEGIN
SELECT i, filename, savename, fullpath, ver, nDateTime
FROM arts.tb_sct;
END//
DELIMITER ;

-- 프로시저 arts.spGetTCTList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetTCTList`()
BEGIN
SELECT i, filename, savename, fullpath, ver, nDateTime
FROM arts.tb_tct;
END//
DELIMITER ;

-- 프로시저 arts.spGetTrainerData 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetTrainerData`(
	IN `$UID` INT
)
BEGIN

	SELECT UID, Account, num, name, nGroup FROM arts.tb_admin WHERE UID = $UID;

	
END//
DELIMITER ;

-- 프로시저 arts.spGetTrainerList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetTrainerList`()
BEGIN

	SELECT UID, Account, Pass, num, name, nGroup FROM arts.tb_admin WHERE nGrade = 2;

END//
DELIMITER ;

-- 프로시저 arts.spGetTrDataList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetTrDataList`()
BEGIN

SELECT 
TID, 
DATE_FORMAT( arts.tl_result.nStartTime, '%Y-%m-%d %H:%i:%S' ) AS `nStartTime`,
DATE_FORMAT( arts.tl_result.nEndTime, '%Y-%m-%d %H:%i:%S' ) AS `nEndTime`,
DATE_FORMAT( SEC_TO_TIME( PlaySec ), '%H:%i:%S' ) AS PlayTime, 
name
FROM arts.tl_result;

END//
DELIMITER ;

-- 프로시저 arts.spGetTrTitle 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetTrTitle`(
	IN `$TID` INT
)
BEGIN
	
SELECT 
TID, 
DATE_FORMAT( arts.tl_start.nStartTime, '%Y-%m-%d %H:%i:%S' ) AS `nStartTime`, 
TName FROM arts.tl_start WHERE TID = `$TID`;

END//
DELIMITER ;

-- 프로시저 arts.spGetTrUserList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetTrUserList`(
	IN `$TID` INT
)
BEGIN

 SELECT A.i,A.TID , A.UID , A.JOb, C.name as Mname, B.name, B.num FROM `arts`.`tl_userinfo` A
 join `arts`.`tb_user` B ON B.UID = A.UID
 JOIN `arts`.`tb_machine` C ON C.nID = A.MID  WHERE TID = `$TID`;
 
END//
DELIMITER ;

-- 프로시저 arts.spGetUIDfromTrList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetUIDfromTrList`(
	IN `$UID` INT
)
BEGIN

SELECT 
B.TID, 
DATE_FORMAT( B.nStartTime, '%Y-%m-%d %H:%i:%S' ) AS `nStartTime`,
DATE_FORMAT( B.nEndTime, '%Y-%m-%d %H:%i:%S' ) AS `nEndTime`,
DATE_FORMAT( SEC_TO_TIME( B.PlaySec ), '%H:%i:%S' ) AS PlayTime, 
B.name
FROM arts.tl_userinfo A 
JOIN arts.tl_result B ON B.TID = A.TID
WHERE UID = $UID;

END//
DELIMITER ;

-- 프로시저 arts.spGetUserData 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetUserData`(
	IN `$UID` INT
)
BEGIN


SELECT UID, REPLACE( name, ' ', '' ) AS name, num, nGroup, height, nDateTime FROM arts.tb_user WHERE UID = $UID;
	
END//
DELIMITER ;

-- 프로시저 arts.spGetUserFindList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetUserFindList`(
	IN `$name` VARCHAR(50)
)
BEGIN

SELECT UID, REPLACE( name, ' ', '' ) AS name, num, nGroup FROM arts.tb_user WHERE name regexp $name;

END//
DELIMITER ;

-- 프로시저 arts.spGetUserList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetUserList`()
BEGIN

	
	SELECT UID, REPLACE( name, ' ', '' ) AS name, num, nGroup, height, nDateTime FROM arts.tb_user;
	
END//
DELIMITER ;

-- 프로시저 arts.spGetUserLogDetail 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetUserLogDetail`(
	IN `$TID` INT,
	IN `$UID` INT
)
BEGIN


DECLARE $FireCount INT;
DECLARE $HitCount INT;
DECLARE $ChallengeCount INT;
DECLARE $SurrenderCount INT;
DECLARE $BindCount INT;
DECLARE $CommandMoveCount INT;
DECLARE $BreifCount INT;
DECLARE $CallCount INT;
DECLARE $EodCount INT;
DECLARE $BadFireCount INT;


SELECT COUNT(*) INTO $FireCount FROM arts.tl_fire WHERE UID = $UID AND TID = $TID;
SELECT COUNT(*) INTO $HitCount FROM arts.tl_hit WHERE UID = $UID AND TID = $TID;
SELECT COUNT(*) INTO $ChallengeCount FROM arts.tl_challenge WHERE UID = $UID AND TID = $TID;
SELECT COUNT(*) INTO $SurrenderCount FROM arts.tl_surrender WHERE UID = $UID AND TID = $TID;
SELECT COUNT(*) INTO $BindCount FROM arts.tl_bind WHERE UID = $UID AND TID = $TID;
SELECT COUNT(*) INTO $CommandMoveCount FROM arts.tl_commandmove WHERE UID = $UID AND TID = $TID;
SELECT COUNT(*) INTO $BreifCount FROM arts.tl_breifsituation WHERE UID = $UID AND TID = $TID;
SELECT COUNT(*) INTO $CallCount FROM arts.tl_call WHERE UID = $UID AND TID = $TID;
SELECT COUNT(*) INTO $EodCount FROM arts.tl_eod WHERE UID = $UID AND TID = $TID;
SELECT COUNT(*) INTO $BadFireCount FROM arts.tl_badfire WHERE UID = $UID AND TID = $TID;

SELECT $FireCount AS FireCount,
$HitCount AS HitCount,
$ChallengeCount AS ChallengeCount,
$SurrenderCount AS SurrenderCount,
$BindCount AS BindCount,
$CommandMoveCount AS CommandMoveCount,
$BreifCount AS BreifCount,
$CallCount AS CallCount,
$EodCount AS EodCount,
$BadFireCount AS BadFireCount,
DATE_FORMAT( SEC_TO_TIME( A.PlaySec ), '%H:%i:%S' ) AS PlayTime,
DATE_FORMAT( A.nStartTime, '%Y-%m-%d %H:%i:%S' ) AS `nStartTime`, 
DATE_FORMAT( A.nEndTime, '%Y-%m-%d %H:%i:%S' ) AS `nEndTime`, 

A.name AS TName,
B.Job,
C.name AS MName,
D.name AS UName,
D.num AS num
FROM tl_result A 
JOIN arts.tl_userinfo B ON B.TID = $TID AND B.UID = $UID
JOIN arts.tb_machine C ON C.nID = B.MID
JOIN arts.tb_user D ON D.UID = $UID
WHERE B.UID = $UID AND B.TID = $TID AND A.TID = $TID;
 
	
END//
DELIMITER ;

-- 프로시저 arts.spGetVoiceFile 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetVoiceFile`(
	IN `$i` BIGINT
)
BEGIN

	SELECT filename, fullpath FROM arts.tb_voice WHERE i = $i;
	
END//
DELIMITER ;

-- 프로시저 arts.spGetVoiceList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetVoiceList`(
	IN `$TID` INT
)
BEGIN

	SELECT i, filename FROM arts.tb_voice WHERE TID = $TID;
	
END//
DELIMITER ;

-- 프로시저 arts.spGetWAVList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetWAVList`()
BEGIN
SELECT i, filename, savename, fullpath, ver, nDateTime
FROM arts.tb_wav;
END//
DELIMITER ;

-- 프로시저 arts.spGetWCTList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spGetWCTList`()
BEGIN
SELECT i, filename, savename, fullpath, ver, nDateTime
FROM arts.tb_wct;
END//
DELIMITER ;

-- 프로시저 arts.spLogin 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spLogin`(
	IN `$account` VARCHAR(100),
	IN `$pass` VARCHAR(100)
)
PROC_BODY : BEGIN


DECLARE $DBPass varchar(100);
DECLARE $InputPass varchar(100);
DECLARE $bSamePass int;

IF NOT exists( SELECT * FROM arts.tb_admin WHERE Account = $account )
THEN 
	SELECT -2 RESULT;

	LEAVE PROC_BODY;

END IF;



SELECT md5( $pass ) INTO $InputPass;
SELECT pass INTO $DBPass FROM arts.tb_admin WHERE Account = $account;

select STRCMP( $InputPass, $DBPass ) INTO $bSamePass;

IF $bSamePass <> 0
THEN

	SELECT -4 AS RESULT;
	LEAVE PROC_BODY;

END IF;

SELECT 1 AS RESULT, UID, Account, nGrade FROM arts.tb_admin WHERE Account = $account;


END//
DELIMITER ;

-- 프로시저 arts.spResult 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spResult`(
	IN `$TID` INT
)
PROC_BODY : BEGIN

IF NOT EXISTS( SELECT * FROM arts.tl_start WHERE TID =`$TID` )
THEN 
	SELECT -40 AS RESULT;
	LEAVE PROC_BODY;
END IF;
	
IF exists( SELECT * FROM arts.TL_RESULT WHERE TID = `$TID` )
THEN 
	SELECT -41 AS RESULT;
	LEAVE PROC_BODY;
END IF;

INSERT INTO arts.tl_result
	( TID, nStartTime, nEndTime, PlaySec, name, nDateTime )
SELECT 
	TID, 
	nStartTime, 
	NOW(), 
	TIMESTAMPDIFF( SECOND, nStartTime, NOW() ) AS PlayTime, 
	TName, 
	NOW() 
FROM arts.tl_start WHERE TID = `$TID`;

SELECT 1 AS RESULT;
	
END//
DELIMITER ;

-- 프로시저 arts.spStart 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spStart`(
	IN `$tname` VARCHAR(200)
)
BEGIN

	INSERT INTO arts.tl_start
	( nStartTime, TName )
	VALUES
	( now(), $tname );

	SELECT LAST_INSERT_ID() as TID;

END//
DELIMITER ;

-- 프로시저 arts.spUpdateMachine 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spUpdateMachine`(
	IN `$nID` INT,
	IN `$BPC` VARCHAR(50),
	IN `$HMD` VARCHAR(50),
	IN `$UTT1` VARCHAR(50),
	IN `$UTT2` VARCHAR(50),
	IN `$UTT3` VARCHAR(50),
	IN `$UTT4` VARCHAR(50)
)
BEGIN

	UPDATE arts.tb_machine
	SET 
	BPC = $BPC, 
	HMD = $HMD, 
	UTT1 = $UTT1, 
 UTT2 = $UTT2,
 UTT3 = $UTT3,
 UTT4 = $UTT4,
 nDateTime = NOW()
	WHERE nID = $nID;



SELECT 1 AS RESULT;
	
END//
DELIMITER ;

-- 프로시저 arts.spUpdateTrainer 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spUpdateTrainer`(
	IN `$UID` INT,
	IN `$Group` VARCHAR(100),
	IN `$Pass` VARCHAR(100)
)
PROC_BODY : BEGIN
	

IF NOT exists( SELECT * FROM arts.tb_admin WHERE UID = $UID )
THEN 

	SELECT -2 AS RESULT;

	LEAVE PROC_BODY;

END IF;

IF $Pass IS NULL
THEN

	UPDATE arts.tb_admin
	SET `nGroup` = $Group
	WHERE UID = $UID;

	SELECT 1 AS RESULT;

	LEAVE PROC_BODY;
	

END IF;


UPDATE arts.tb_admin
SET `Pass` = md5( $Pass ), `nGroup`=$Group
WHERE UID = $UID ;


SELECT 1 AS RESULT;

END//
DELIMITER ;

-- 프로시저 arts.spUpdateUser 구조 내보내기
DELIMITER //
CREATE PROCEDURE `spUpdateUser`(
	IN `$UID` INT,
	IN `$name` VARCHAR(100),
	IN `$num` VARCHAR(100),
	IN `$group` VARCHAR(100),
	IN `$height` INT
)
PROC_BODY : BEGIN

	IF NOT exists( SELECT * FROM arts.tb_user WHERE UID = $UID )
	THEN 
		SELECT -2 AS RESULT;
		LEAVE PROC_BODY;
	END IF;
	
	UPDATE arts.tb_user
	SET name = `$name`, num = `$num`, nGroup = `$group`, height=`$height` 
	WHERE UID = $UID;


	SELECT 1 AS RESULT;

END//
DELIMITER ;

-- 테이블 arts.tb_admin 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_admin` (
  `UID` int(11) NOT NULL AUTO_INCREMENT,
  `Account` varchar(100) DEFAULT NULL,
  `Pass` varchar(100) DEFAULT NULL,
  `num` varchar(100) NOT NULL,
  `nGrade` int(11) DEFAULT NULL,
  `name` varchar(50) DEFAULT NULL,
  `nGroup` varchar(100) DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  PRIMARY KEY (`UID`,`num`) USING BTREE,
  KEY `Account` (`Account`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_effect 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_effect` (
  `i` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(250) DEFAULT NULL,
  `savename` varchar(250) DEFAULT NULL,
  `fullpath` varchar(250) DEFAULT NULL,
  `ver` int(11) DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  `filetype` int(11) DEFAULT 9,
  PRIMARY KEY (`i`),
  KEY `filename` (`filename`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_error 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_error` (
  `error` int(11) NOT NULL,
  `cont` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`error`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_fbx 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_fbx` (
  `i` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(250) NOT NULL,
  `savename` varchar(250) NOT NULL,
  `fullpath` varchar(250) NOT NULL,
  `ver` int(11) DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  `filetype` int(11) DEFAULT 5,
  PRIMARY KEY (`i`),
  KEY `filename` (`filename`)
) ENGINE=InnoDB AUTO_INCREMENT=39 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_fbxanim 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_fbxanim` (
  `i` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(250) NOT NULL,
  `savename` varchar(250) NOT NULL,
  `fullpath` varchar(250) NOT NULL,
  `ver` int(11) NOT NULL DEFAULT 0,
  `nDateTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `filetype` int(11) NOT NULL DEFAULT 8,
  PRIMARY KEY (`i`),
  KEY `filename` (`filename`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_file 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_file` (
  `i` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(250) NOT NULL,
  `savename` varchar(250) NOT NULL,
  `fullpath` varchar(250) NOT NULL,
  `ver` int(11) DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  `filetype` int(11) DEFAULT NULL,
  PRIMARY KEY (`i`) USING BTREE,
  KEY `filename` (`filename`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=110 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_machine 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_machine` (
  `nID` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `BPC` varchar(50) DEFAULT NULL,
  `HMD` varchar(50) DEFAULT NULL,
  `UTT1` varchar(50) DEFAULT NULL,
  `UTT2` varchar(50) DEFAULT NULL,
  `UTT3` varchar(50) DEFAULT NULL,
  `UTT4` varchar(50) DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  PRIMARY KEY (`nID`),
  UNIQUE KEY `name` (`name`),
  KEY `BPC` (`BPC`),
  KEY `HMD` (`HMD`),
  KEY `UTT1` (`UTT1`),
  KEY `UTT2` (`UTT2`),
  KEY `UTT3` (`UTT3`),
  KEY `UTT4` (`UTT4`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_mct 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_mct` (
  `i` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(250) NOT NULL,
  `savename` varchar(250) NOT NULL,
  `fullpath` varchar(250) NOT NULL,
  `ver` int(11) DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  `filetype` int(11) DEFAULT 2,
  PRIMARY KEY (`i`) USING BTREE,
  KEY `filename` (`filename`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_month 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_month` (
  `Month` int(11) NOT NULL,
  PRIMARY KEY (`Month`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_png 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_png` (
  `i` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(250) NOT NULL,
  `savename` varchar(250) NOT NULL,
  `fullpath` varchar(250) NOT NULL,
  `ver` int(11) DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  `filetype` int(11) DEFAULT 6,
  PRIMARY KEY (`i`) USING BTREE,
  KEY `filename` (`filename`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_replay 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_replay` (
  `nID` int(11) NOT NULL AUTO_INCREMENT,
  `TID` int(11) DEFAULT NULL,
  `UID` int(11) DEFAULT NULL,
  `filename` varchar(200) DEFAULT NULL,
  `FullPath` varchar(400) DEFAULT NULL,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`nID`),
  UNIQUE KEY `nID` (`nID`,`TID`,`UID`),
  KEY `filename` (`filename`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_role 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_role` (
  `RID` int(11) NOT NULL,
  `RName` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`RID`),
  UNIQUE KEY `RID` (`RID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_sct 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_sct` (
  `i` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(250) NOT NULL,
  `savename` varchar(250) NOT NULL,
  `fullpath` varchar(250) NOT NULL,
  `ver` int(11) DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  `filetype` int(11) DEFAULT 1,
  PRIMARY KEY (`i`) USING BTREE,
  KEY `filename` (`filename`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_tct 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_tct` (
  `i` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(250) NOT NULL,
  `savename` varchar(250) NOT NULL,
  `fullpath` varchar(250) NOT NULL,
  `ver` int(11) DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  `filetype` int(11) DEFAULT 4,
  PRIMARY KEY (`i`) USING BTREE,
  KEY `filename` (`filename`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=47 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_testtraining 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_testtraining` (
  `UID` int(11) DEFAULT NULL,
  KEY `UID` (`UID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_user 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_user` (
  `UID` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) DEFAULT NULL,
  `num` varchar(100) NOT NULL,
  `nGroup` varchar(100) DEFAULT NULL,
  `height` int(11) DEFAULT 0,
  `nDateTime` datetime DEFAULT NULL,
  PRIMARY KEY (`UID`),
  UNIQUE KEY `num` (`num`),
  KEY `name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_voice 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_voice` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `filename` varchar(400) NOT NULL,
  `fullpath` varchar(400) NOT NULL,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`),
  KEY `filename` (`filename`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_wav 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_wav` (
  `i` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(250) NOT NULL,
  `savename` varchar(250) NOT NULL,
  `fullpath` varchar(250) NOT NULL,
  `ver` int(11) DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  `filetype` int(11) DEFAULT 7,
  PRIMARY KEY (`i`) USING BTREE,
  KEY `filename` (`filename`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tb_wct 구조 내보내기
CREATE TABLE IF NOT EXISTS `tb_wct` (
  `i` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(250) NOT NULL,
  `savename` varchar(250) NOT NULL,
  `fullpath` varchar(250) NOT NULL,
  `ver` int(11) DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  `filetype` int(11) DEFAULT 3,
  PRIMARY KEY (`i`) USING BTREE,
  KEY `filename` (`filename`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_badfire 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_badfire` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `iCount` int(11) NOT NULL DEFAULT 1,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB AUTO_INCREMENT=115 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='오인 사격 로그';

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_bind 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_bind` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `iCount` int(11) NOT NULL DEFAULT 1,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='포박 로그';

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_breifsituation 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_breifsituation` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `iCount` int(11) NOT NULL DEFAULT 1,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB AUTO_INCREMENT=30 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_call 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_call` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `iCount` int(11) NOT NULL DEFAULT 1,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB AUTO_INCREMENT=34 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_challenge 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_challenge` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `iCount` int(11) NOT NULL DEFAULT 1,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB AUTO_INCREMENT=61 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='수하 로그';

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_commandmove 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_commandmove` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `iCount` int(11) NOT NULL DEFAULT 1,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_distance 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_distance` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) DEFAULT NULL,
  `UID` int(11) DEFAULT NULL,
  `dist` float DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_eod 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_eod` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `iCount` int(11) NOT NULL DEFAULT 1,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_fire 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_fire` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `iCount` int(11) DEFAULT 1,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB AUTO_INCREMENT=2194 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='사격 로그';

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_hit 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_hit` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `iCount` int(11) DEFAULT 1,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB AUTO_INCREMENT=112 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='사격 적중 로그';

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_motion 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_motion` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `str` mediumtext DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=1628083 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_object 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_object` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `str` varchar(8192) NOT NULL DEFAULT '',
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_result 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_result` (
  `TID` int(11) NOT NULL,
  `nStartTime` datetime NOT NULL,
  `nEndTime` datetime NOT NULL,
  `PlaySec` int(11) DEFAULT NULL,
  `name` varchar(200) DEFAULT NULL,
  `nDateTime` datetime DEFAULT NULL,
  PRIMARY KEY (`TID`) USING BTREE,
  KEY `StartTime` (`nStartTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_start 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_start` (
  `TID` int(11) NOT NULL AUTO_INCREMENT,
  `nStartTime` datetime DEFAULT NULL,
  `TName` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`TID`)
) ENGINE=InnoDB AUTO_INCREMENT=3031 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_surrender 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_surrender` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `iCount` int(11) NOT NULL DEFAULT 1,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB AUTO_INCREMENT=62 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='투하명령 로그';

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_tr_gun 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_tr_gun` (
  `i` bigint(20) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `x` float DEFAULT NULL,
  `y` float DEFAULT NULL,
  `z` float DEFAULT NULL,
  `p` float DEFAULT NULL,
  `r` float DEFAULT NULL,
  `w` float DEFAULT NULL,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='무기 트랜스폼 로그';

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_tr_hmd 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_tr_hmd` (
  `i` int(11) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL,
  `UID` int(11) NOT NULL,
  `x` float DEFAULT NULL,
  `y` float DEFAULT NULL,
  `z` float DEFAULT NULL,
  `p` float DEFAULT NULL,
  `r` float DEFAULT NULL,
  `w` float DEFAULT NULL,
  `nDateTime` time NOT NULL,
  PRIMARY KEY (`i`),
  UNIQUE KEY `i` (`i`,`TID`,`UID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='HMD 트랜스폼 로그';

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 테이블 arts.tl_userinfo 구조 내보내기
CREATE TABLE IF NOT EXISTS `tl_userinfo` (
  `i` int(11) NOT NULL AUTO_INCREMENT,
  `TID` int(11) NOT NULL COMMENT 'tl_result의 TID',
  `UID` int(11) NOT NULL COMMENT 'tb_user의 UID',
  `MID` int(11) NOT NULL COMMENT 'tb_machine의 nID',
  `Job` varchar(200) DEFAULT NULL,
  `nDateTime` datetime NOT NULL,
  PRIMARY KEY (`i`,`TID`,`UID`,`MID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2770 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- 내보낼 데이터가 선택되어 있지 않습니다.

-- 프로시저 arts.webGetMCList 구조 내보내기
DELIMITER //
CREATE PROCEDURE `webGetMCList`()
BEGIN

SELECT * FROM arts.tb_machine ORDER BY ndateTime DESC, nID LIMIT 5;

END//
DELIMITER ;

-- 프로시저 arts.webGetMonthUserData 구조 내보내기
DELIMITER //
CREATE PROCEDURE `webGetMonthUserData`()
BEGIN

DECLARE $YEAR INT;
DECLARE $Month INT;
set $YEAR = DATE_FORMAT( NOW(), '%Y' ), $MONTH = DATE_FORMAT( NOW(), '%c' );
#set $YEAR = DATE_FORMAT( NOW(), '%Y' ), $MONTH = 7;

SELECT A.Month, IFNULL( B.nCount, 0 ) AS nCount FROM arts.tb_Month A LEFT JOIN 
( SELECT DATE_FORMAT( nDateTime, '%c' ) AS 'Month', COUNT(UID) AS 'nCount' from arts.tb_user 
WHERE DATE_FORMAT( nDateTime,'%Y' ) = $YEAR AND DATE_FORMAT( nDateTime, '%c' ) <= $MONTH
GROUP BY DATE_FORMAT( nDateTime, '%c' ) ) B
ON A.Month = B.Month 
WHERE A.Month <= $MONTH
ORDER BY A.Month;
 

END//
DELIMITER ;

-- 프로시저 arts.webGetRandomData 구조 내보내기
DELIMITER //
CREATE PROCEDURE `webGetRandomData`()
BEGIN
SELECT FLOOR( RAND() * 100 ) AS '1',
FLOOR( RAND() * 100 ) AS '2',
FLOOR( RAND() * 100 ) AS '3',
FLOOR( RAND() * 100 ) AS '4',
FLOOR( RAND() * 100 ) AS '5',
FLOOR( RAND() * 100 ) AS '6',
FLOOR( RAND() * 100 ) AS '7',
FLOOR( RAND() * 100 ) AS '8',
FLOOR( RAND() * 100 ) AS '9',
FLOOR( RAND() * 100 ) AS '10',
FLOOR( RAND() * 100 ) AS '11',
FLOOR( RAND() * 100 ) AS '12';
END//
DELIMITER ;

-- 프로시저 arts.webGetTrainingCount 구조 내보내기
DELIMITER //
CREATE PROCEDURE `webGetTrainingCount`()
BEGIN

SELECT TName, COUNT(*) AS nCount FROM arts.tl_start GROUP BY TName ORDER BY nCount Desc LIMIT 10;

END//
DELIMITER ;

-- 프로시저 arts.webGetTrainingPer 구조 내보내기
DELIMITER //
CREATE PROCEDURE `webGetTrainingPer`()
BEGIN

DECLARE $UserCount FLOAT;
DECLARE $TrainingCount FLOAT;
DECLARE $NotTraining FLOAT;

DECLARE $TrainingPer FLOAT;
DECLARE $NottrainingPer FLOAT;

SELECT COUNT(*) into $UserCount FROM tb_user;

SELECT COUNT(*) into $TrainingCount FROM arts.tb_user A
#JOIN ( SELECT UID FROM arts.tl_userinfo GROUP BY UID ) B
JOIN ( SELECT UID FROM arts.tb_testtraining GROUP BY UID ) B
ON A.UID = B.UID;

SET $NotTraining = $UserCount - $TrainingCount;


SET $TrainingPer = $TrainingCount / $UserCount * 100; 
SET $NottrainingPer = $NotTraining / $UserCount * 100; 

#SELECT  $NottrainingPer  as 'false',  $TrainingPer as 'true';
SELECT  $NotTraining  as 'false',  $TrainingCount as 'true';
#SELECT  CONCAT( CAST( $NottrainingPer AS CHAR ),'%' ) as 'false', CONCAT( CAST( $TrainingPer AS CHAR ),'%' ) as 'true';
 
END//
DELIMITER ;

-- 프로시저 arts.webGetTrainingTime 구조 내보내기
DELIMITER //
CREATE PROCEDURE `webGetTrainingTime`()
BEGIN

SELECT NAME,
FLOOR( AVG(playsec)/60) AS Min
FROM arts.tl_result GROUP BY NAME ORDER BY MIN Desc;

END//
DELIMITER ;

-- 프로시저 arts.webGetTrDataListDesc 구조 내보내기
DELIMITER //
CREATE PROCEDURE `webGetTrDataListDesc`()
BEGIN

SELECT 
TID, 
DATE_FORMAT( arts.tl_result.nStartTime, '%Y-%m-%d %H:%i:%S' ) AS `nStartTime`,
DATE_FORMAT( arts.tl_result.nEndTime, '%Y-%m-%d %H:%i:%S' ) AS `nEndTime`,
DATE_FORMAT( SEC_TO_TIME( PlaySec ), '%H:%i:%S' ) AS PlayTime, 
name
FROM arts.tl_result ORDER BY nEndTime DESC LIMIT 5 ;

END//
DELIMITER ;

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
