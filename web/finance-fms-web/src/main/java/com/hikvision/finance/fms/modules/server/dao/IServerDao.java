/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月11日 下午5:17:50
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.server.dao;

import java.util.List;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Server;
import com.hikvision.finance.fms.modules.server.qo.ServerQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月11日 下午5:17:50
 * @version V1.0 
 */
public interface IServerDao extends IBaseDao<Server, ServerQo>{
	
	/**
	 * 模糊查询
	 * @author jinxindong 2016年3月16日 下午5:41:36
	 * @param keyWord
	 * @return
	 */
	public List<Server> getServerByKeyword(String keyWord);

	/**获取服务信息--根据服务的ip和服务类型--DC接口使用
	 * @author xujiangfei 2016-5-5 下午3:25:25
	 * @param ip
	 * @param moudleType
	 * @return
	 */
	public Server findServerByip(String ip, Integer moudleType,String Modelusb);

	/**DC系统巡检服务接口--更新服务心跳时间
	 * @author xujiangfei 2016-5-5 下午8:50:06
	 * @param ip
	 * @param ntype
	 */
	public void setHeartbeatsTime(String ip, int ntype);
}
