'''
Created on 2016-7-23

@author: caofuchun
'''
# -*- coding: utf-8 -*-

def get_all_region():
    import mysql.connector
    cnx = mysql.connector.connect(user='ddl_mgr', password='ABC@12345',
                                  host='172.10.33.18',
                                  database='ivms8000fms')
    cursor = cnx.cursor()
    cursor.execute("select nParentId, strName from region")
    tl = []
    for nParentId, strName in cursor:
        print nParentId, strName.decode('utf8')
        tl.append((nParentId, strName))
    
    cursor.close()
    cnx.close()
    return tl

def insert_regin():
    import mysql.connector
    cnx = mysql.connector.connect(user='ddl_mgr', password='Abc@12345',
                                  host='172.10.33.18',
                                  database='ivms8000fms')
    cursor = cnx.cursor()
    try:
        sql_select = "select count(*) from region"
        cursor.execute(sql_select)
        ret = cursor.fetchone()[0]

        for i in range(0, 20):
            if (i >= ret-1):
                sql_insert1 ="insert into region (nParentId, strName, strCode, strInterAreaCode, strPath, nSortId, strTxt) values (1, '%s', '%s', '%s', '%s', %s, '%s')" % (i, i, i, i, i, i)
                print sql_insert1
                cursor.execute(sql_insert1)          
        cnx.commit()
        
        sql_select = "select id from region where nParentId != 0"
        cursor.execute(sql_select)
        ret = cursor.fetchall()
        for id in ret:
            # if this node's childnodes > 50, donot insert
            sql_select = "select count(*) from region where nParentId = %s" % id[0]
            cursor.execute(sql_select)
            ret = cursor.fetchone()[0]
            if (ret<50):
                for j in range(1, 51):
                    sql_insert1 ="insert into region (nParentId, strName, strCode, strInterAreaCode, strPath, nSortId, strTxt) values (%s, '%s-%s', '%s-%s', '%s-%s', '%s-%s', %s, '%s-%s')" % (id[0], id[0], j, id[0], j, id[0], j, id[0], j, id[0]*50+j, id[0], j)
                    print sql_insert1
                    cursor.execute(sql_insert1)      
                cnx.commit()
    except mysql.connector.Error as e:
        print('insert datas error!{}'.format(e))
    cursor.close()
    cnx.close()
        
def insert_producttype():
    import mysql.connector
    cnx = mysql.connector.connect(user='ddl_mgr', password='Abc@12345',
                                  host='172.10.33.18',
                                  database='ivms8000fms')
    cursor = cnx.cursor()
    try:
        #insert 10 first Type
        sql_select = "select count(*) from producttype"
        cursor.execute(sql_select)
        ret = cursor.fetchone()[0]
        for i in range(0, 10):
            if (i >= ret-1):
                sql_insert1 ="insert into producttype (pid, strName, strInterAreaCode, strPath, nSortId) values (1, '%s', '%s', '%s', '%s')" % (i, i, i, i)
                print sql_insert1
                cursor.execute(sql_insert1)
        cnx.commit()
        
        #insert 20 second Type
        sql_select = "select id from producttype where pid != 0"
        cursor.execute(sql_select)
        ret = cursor.fetchall()
        for id in ret:
            # if this node's childnodes > 20, donot insert
            sql_select = "select count(*) from producttype where pid = %s" % id[0]
            cursor.execute(sql_select)
            ret = cursor.fetchone()[0]
            if (ret<20):
                for j in range(1, 20):
                    sql_insert1 ="insert into producttype (pid, strName, strInterAreaCode, strPath, nSortId) values (%s, 'type%s-%s', '%s-%s', '%s-%s', %s)" % (id[0], id[0], j, id[0], j, id[0], j, id[0]*20+j)
                    print sql_insert1
                    cursor.execute(sql_insert1)      
                cnx.commit()    
    except mysql.connector.Error as e:
        print('insert datas error!{}'.format(e))
    cursor.close()
    cnx.close()
        
def insert_product():
    import mysql.connector
    cnx = mysql.connector.connect(user='ddl_mgr', password='Abc@12345',
                                  host='172.10.33.18',
                                  database='ivms8000fms')
    cursor = cnx.cursor()
    try:
        # morn than 10000 return
        sql_select = "select count(*) from product"
        cursor.execute(sql_select)
        ret = cursor.fetchone()[0]
        if (ret >= 10000):
            return None
        
        #insert 50 producr
        sql_select = "select id from producttype where pid != 0"
        cursor.execute(sql_select)
        ret = cursor.fetchall()
        for id in ret:
            # if this node's childnodes > 50, donot insert
            sql_select = "select count(*) from product where pid = %s" % id[0]
            cursor.execute(sql_select)
            ret = cursor.fetchone()[0]
            if (ret<50):
                for j in range(0, 50):
                    sql_insert1 ="insert into product (pid, strName, strCode, strEndDate, saveYears, nEnabled, strGrade, strRisklevel, importUser, importTime ) values (%s, 'product%s-%s', '%s-%s', '2017-07-31', 10, 1, 'bank', 1, '1', '2016-07-08 14:15:58')" % (id[0], id[0], j, id[0], j)
                    print sql_insert1
                    cursor.execute(sql_insert1)      
                    cnx.commit()
    except mysql.connector.Error as e:
        print('insert datas error!{}'.format(e))    
    cursor.close()
    cnx.close()
    
def insert_clientinput():
    import mysql.connector
    cnx = mysql.connector.connect(user='ddl_mgr', password='Abc@12345',
                                  host='172.10.33.18',
                                  database='ivms8000fms')
    cursor = cnx.cursor()
    try:
        # call
        sql_callproc = "select insert_clientinput()"
        cursor.execute(sql_callproc)   
        ret = cursor.fetchone()[0]
        cnx.commit()
    except mysql.connector.Error as e:
        print('insert datas error!{}'.format(e))
    cursor.close()
    cnx.close()
    
def insert_prosaverecord():
    import mysql.connector
    cnx = mysql.connector.connect(user='ddl_mgr', password='Abc@12345',
                                  host='172.10.33.18',
                                  database='ivms8000fms')
    cursor = cnx.cursor()
    try:
        # call
        sql_callproc = "select insert_prosaverecord()"
        cursor.execute(sql_callproc)   
        ret = cursor.fetchone()[0]
        cnx.commit()
    except mysql.connector.Error as e:
        print('insert datas error!{}'.format(e))
    cursor.close()
    cnx.close()
    
def insert_alarmlog():
    import mysql.connector
    cnx = mysql.connector.connect(user='ddl_mgr', password='Abc@12345',
                                  host='172.10.33.18',
                                  database='ivms8000fms')
    cursor = cnx.cursor()
    try:
        # call
        sql_callproc = "select insert_alarmlog()"
        cursor.execute(sql_callproc)   
        ret = cursor.fetchone()[0]
        cnx.commit()
    except mysql.connector.Error as e:
        print('insert datas error!{}'.format(e))
    cursor.close()
    cnx.close()
    
def insert_device():
    import mysql.connector
    cnx = mysql.connector.connect(user='ddl_mgr', password='Abc@12345',
                                  host='172.10.33.18',
                                  database='ivms8000fms')
    cursor = cnx.cursor()
    try:
        sql_insert1 ="INSERT INTO `device` (`nRegionId`,`nType`, `strName`, `strIP`, `nPort`, `strUser`, `strPassword`, `strPasswordLevel`, `online`, `strIndexCode`, `strManufacture`,\
        `uiTotalSpace`, `uiFreeSpace`, `nChanNum`, `telChanNum`, `nStreamType`, `nDevType`, `nProductType`, `strDeviceId`, `dvrType`, `nConfigPort`, `strConfigUser`, `strConfigPassword`) \
        SELECT region.`id`, device.`nType`, region.`id`, device.`strIP`, device.`nPort`, device.`strUser`, device.`strPassword`, device.`strPasswordLevel`, device.`online`, device.`strIndexCode`,\
        device.`strManufacture`, device.`uiTotalSpace`, device.`uiFreeSpace`, device.`nChanNum`, device.`telChanNum`, device.`nStreamType`, device.`nDevType`, device.`nProductType`,\
        device.`strDeviceId`, device.`dvrType`, device.`nConfigPort`, device.`strConfigUser`, device.`strConfigPassword`\
        FROM region, device WHERE device.id = 3 LIMIT 1000;"
        print sql_insert1
        cursor.execute(sql_insert1)      
        cnx.commit()
    except mysql.connector.Error as e:
        print('insert datas error!{}'.format(e))
    cursor.close()
    cnx.close()
    
def insert_devicechannel():
    import mysql.connector
    cnx = mysql.connector.connect(user='ddl_mgr', password='Abc@12345',
                                  host='172.10.33.18',
                                  database='ivms8000fms')
    cursor = cnx.cursor()
    try:
        # call
        sql_select = "select id, nRegionId, strIP FROM device;"
        cursor.execute(sql_select)
        ret = cursor.fetchall()
        
        for id, nRegionId, strIP in ret:
            for i in range(0, 10):
                sql_insert1 ="INSERT INTO devicechannel (nRegionId, nDeviceId, nChannelNo, nChannelName, recordPlane, nFlag) VALUES (%s, %s, %s, '%s-%s', '0', '0')" % (nRegionId, id, i, strIP, i);
                print sql_insert1
                cursor.execute(sql_insert1)      
            cnx.commit()
    except mysql.connector.Error as e:
        print('insert datas error!{}'.format(e))
    cursor.close()
    cnx.close()
    
def insert_users():
    import mysql.connector
    cnx = mysql.connector.connect(user='ddl_mgr', password='Abc@12345',
                                  host='172.10.33.18',
                                  database='ivms8000fms')
    cursor = cnx.cursor()
    try:
        # call
        sql_select = "SELECT * FROM users WHERE sysRole = 1 LIMIT 1;"
        cursor.execute(sql_select)
        ret = cursor.fetchall()
        
        for net in ret:
            for i in range(0, 10000):
                sql_insert1 ="INSERT INTO users (nOwner, netId, sysRole, strName, strCode, nSex, userNo, net, nState, strDate, strPassword, nPasswordLevel, strCredentialNo, strDuty, strEmail, strTel, strComment, strUpdatePwdTime, nonline, branch, userSessionId, lastTimeOnline) \
                VALUES ('1', '1', '1', '%s', '%s', '1', '%s', '%s', 0, '2016-07-26 10:27:30', 'f8aa14da2301e201e817f5b8667a36bb40c8ca49da69b3470a74d0f4ec194961', NULL, '', '', '', '', '', '2016-07-26 10:27:30', '3', NULL, '%s', '2016-07-26 10:27:30');" %(i, i, i, i, i);
                print sql_insert1
                cursor.execute(sql_insert1)      
            cnx.commit()
    except mysql.connector.Error as e:
        print('insert datas error!{}'.format(e))
    cursor.close()
    cnx.close()

if __name__ == '__main__':
    #print 'Insert regin table, Enter!'
    #raw_input()
    #insert_regin()
    #insert_producttype()
    #insert_product()
    #insert_clientinput()
    #insert_prosaverecord()
    #insert_alarmlog()
    #insert_device()
    #insert_devicechannel()
    insert_users()

#insert_clientinput()
'''
BEGIN
    #Routine body goes here...
    DECLARE i INT DEFAULT 300;
    DECLARE j INT DEFAULT 10000;
    WHILE i > 0 DO
        WHILE j > 0 DO
            INSERT INTO clientinput(inputRegionId, inputUser, productTypeId, productId, creditCode, nProId, inputTime, businessTime, inputRegionCode, inputRegionName, status, nisRead, nisPay, delaydays, strEndDate, name, creditId, proName, proUserNo, productTypeName, productName, productCode, strVideoEndDate, guaranteeYears, productSaveMonth, strGrade, strRisklevel, strBankNo, strBuyMoney, strCreditPhone, strRemarks, tipflag, markDate, nisUpload, productType, saveflag) SELECT inputRegionId, inputUser, productTypeId, productId, creditCode, 1000*i+j, DATE_SUB(inputTime,INTERVAL i DAY), DATE_SUB(businessTime,INTERVAL i DAY), inputRegionCode, inputRegionName, 0, 0, 0, 0, DATE_ADD(strEndDate,INTERVAL i DAY), name, creditId, proName, proUserNo, productTypeName, productName, productCode, DATE_ADD(strVideoEndDate,INTERVAL i DAY), guaranteeYears, productSaveMonth, strGrade, strRisklevel, strBankNo, strBuyMoney, strCreditPhone, strRemarks, tipflag, markDate, nisUpload, productType, saveflag FROM clientinput LIMIT 1;
            SET j = j - 1;
        END WHILE;
        SET j = 10000;
        SET i = i - 1;
    END WHILE;
    RETURN 0;
END
'''
    
# insert_prosaverecord
'''
BEGIN
    #Routine body goes here...
    DECLARE i INT DEFAULT 300;
    DECLARE j INT DEFAULT 10000;
    WHILE i > 0 DO
        WHILE j > 0 DO
            INSERT INTO prosaverecord(`recordId`, `clientId`, `regionId`, `userId`, `cvr`, `streamNo`, `stateType`, `fileName`, `startTime`, `endTime`, `upload`, `recordCheck`, `checkRemark`, `strUrl`, `devId`, `devChannel`, `screenshotSrc`, `rtspPath`, `nFileSize`, `nType`, `mediaType`, `calltype`, `callNo`, `recordUserNo`, `locked`, `devIp`) SELECT `recordId`, `clientId`, `regionId`, `userId`, `cvr`, `streamNo`, `stateType`, `fileName`, `startTime`, `endTime`, `upload`, `recordCheck`, `checkRemark`, `strUrl`, `devId`, `devChannel`, `screenshotSrc`, `rtspPath`, `nFileSize`, `nType`, `mediaType`, `calltype`, `callNo`, `recordUserNo`, `locked`, `devIp` FROM prosaverecord WHERE prosaverecord.id = 1;
            SET j = j - 1;
        END WHILE;
        SET j = 10000;
        SET i = i - 1;
    END WHILE;
    RETURN 0;
END
'''

# insert_alarmlog
'''
BEGIN
    #Routine body goes here...
    DECLARE i INT DEFAULT 500;
    DECLARE j INT DEFAULT 10000;
    WHILE i > 0 DO
        WHILE j > 0 DO
            INSERT INTO alarmlog (`checkType`, `checkObjId`, `checkChan`, `contactObjId`, `alarmType`, `alarmTime`, `alarmbeginTime`, `alarmRegionId`, `comments`, `strHost`, `status`, `checker`, `userNo`, `dealTime`, `dealcomment`, `alarmlevel`, `strGuid`) SELECT `checkType`, `checkObjId`, `checkChan`, `contactObjId`, `alarmType`, DATE_SUB(alarmTime,INTERVAL i DAY), DATE_SUB(alarmbeginTime,INTERVAL i DAY), `alarmRegionId`, `comments`, `strHost`, `status`, `checker`, `userNo`, `dealTime`, `dealcomment`, `alarmlevel`, `strGuid` FROM alarmlog LIMIT 1;
            SET j = j - 1;
        END WHILE;
        SET j = 10000;
        SET i = i - 1;
    END WHILE;
    RETURN 0;
END
'''