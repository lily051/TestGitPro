/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:30:13
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.dao;

import java.util.Date;
import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Clientdeletelog;
import com.hikvision.finance.fms.modules.log.dto.ClientdeletelogInfo;
import com.hikvision.finance.fms.modules.log.qo.ClientdeletelogQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:30:13
 * @version V1.0 
 */
public interface IClientDeleteLogDao extends IBaseDao<Clientdeletelog,ClientdeletelogQo>{
	/**
	 * 删除日志
	 * @author jinxindong 2016年4月9日 上午10:16:53
	 * @param limit
	 * @param start
	 * @param clientdeletelogInfo
	 * @return
	 */
	public Page getClientDelLogDataPage(Integer limit, Integer start, ClientdeletelogInfo clientdeletelogInfo);
	
	/**
	 * 删除日志导出表格
	 * @author jinxindong 2016年4月9日 下午3:49:33
	 * @param clientdeletelogInfo
	 * @return
	 */
	public List<ClientdeletelogInfo> exportDelLogExcel(ClientdeletelogInfo clientdeletelogInfo);
	
	 /**
	  * 删除日志
	  * @author jinchenwu 2016年4月14日 下午3:22:38
	  * @param thresholddate
	  */
	 public void deleteLog(Date thresholddate);
}
