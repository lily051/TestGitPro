/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:30:43
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.dao;

import java.util.Date;
import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Configlog;
import com.hikvision.finance.fms.modules.log.dto.ConfiglogInfo;
import com.hikvision.finance.fms.modules.log.qo.ConfiglogQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:30:43
 * @version V1.0 
 */
public interface IConfigLogDao extends IBaseDao<Configlog,ConfiglogQo>{
	/**
	 * 配置日志
	 * @author jinxindong 2016年4月9日 上午10:17:16
	 * @param limit
	 * @param start
	 * @param configlogInfo
	 * @return
	 */
	public Page getConfiglogDataPage(Integer limit, Integer start, ConfiglogInfo configlogInfo);
	
	/**
	 * 配置日志导出表格
	 * @author jinxindong 2016年4月9日 下午3:49:12
	 * @param configlogInfo
	 * @return
	 */
	public List<ConfiglogInfo> exportConfiglogExcel(ConfiglogInfo configlogInfo);
	
	/**
	 * 设置操作日志的内容
	 * @author jinxindong 2016年4月11日 上午11:31:39
	 * @param operType 操作类型
	 * @param strCode 操作模块
	 * @param conetnt 操作内容
	 */
	 public void setLogContent(Integer operType,String strCode,String conetnt);
	 
	 /**
	  * 未登录设置操作日志的内容
	  * @author jinxindong 2016年6月16日 上午9:20:10
	  * @param operType
	  * @param strCode
	  * @param conetnt
	  * @param name
	  */
	 public void setLogContent(Integer operType,String strCode,String conetnt,String name);
	 
	 /**
	  * 删除日志
	  * @author jinchenwu 2016年4月14日 下午3:22:38
	  * @param thresholddate
	  */
	 public void deleteLog(Date thresholddate);
}
