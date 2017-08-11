---------------------------------------------------------------------------------------------
delimiter //
CREATE DEFINER=`root`@`%` PROCEDURE `sp_MarkDelete`()
BEGIN
	DECLARE done INT DEFAULT FALSE;
  DECLARE productType, saveMonth INT DEFAULT 0;
  DECLARE cur1 CURSOR FOR SELECT ID, saveYears FROM ivms8120v2.bussinesstype;

  DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

  OPEN cur1;

  read_loop: LOOP
    FETCH cur1 INTO productType, saveMonth;

    IF done THEN
      LEAVE read_loop;
    END IF;
		
		CALL sp_MarkDelete_step2(productType, saveMonth);
		
  END LOOP;

  CLOSE cur1;

END
//
---------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------
CREATE DEFINER=`root`@`%` PROCEDURE `sp_MarkDelete_step2`(IN `inProductType` int,IN `inSaveMonth` int)
BEGIN

	UPDATE clientinput 
	SET tipflag = tipflag + 1, markDate = CURDATE()
	WHERE productType = inProductType AND saveflag = 0 AND clientinput.status != 0
		AND DATEDIFF(CURDATE(),DATE_ADD(DATE_ADD(businessTime,INTERVAL inSaveMonth MONTH), INTERVAL delaydays DAY)) > 0 
		AND DATEDIFF(CURDATE(),DATE_ADD(DATE_ADD(businessTime,INTERVAL inSaveMonth MONTH), INTERVAL delaydays DAY)) != tipflag
		AND (markDate != CURDATE() or ISNULL(markDate));

END
//
delimiter ;
----------------------------------------------------------------------------------------------------------

