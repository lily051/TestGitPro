/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月11日 下午5:17:18
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.server.service;

import java.util.List;

import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Server;
import com.hikvision.finance.fms.model.Sysdictionary;
import com.hikvision.finance.fms.modules.server.dto.ServerInfo;
import com.hikvision.finance.fms.modules.server.qo.ServerQo;
import com.hikvision.finance.fwork.util.AjaxData;


/**
 * <p></p>
 * @author jinxindong 2016年3月11日 下午5:17:18
 * @version V1.0 
 */
public interface IServerService extends IBaseService<Server,ServerQo>{
	
	/**
	 * 通过小类型查询数据字典列表
	 * @author jinxindong 2016年3月16日 上午10:09:11
	 * @param ntype
	 * @return
	 */
	public List<Sysdictionary> getDicByNtype(Integer ntype);
	
	/**
	 * 获取服务列表
	 * @author jinxindong 2016年3月16日 下午2:20:14
	 * @param keyWord
	 * @return
	 */
	public List<ServerInfo> getServerPage(String keyWord);
	
	/**
	 * 增加服务
	 * @author jinxindong 2016年3月16日 下午3:13:29
	 * @param serverInfo
	 * @return
	 */
	public AjaxData addServer(ServerInfo serverInfo);
	
	/**
	 * 修改服务
	 * @author jinxindong 2016年3月16日 下午3:53:17
	 * @param serverInfo
	 * @return
	 */
	public AjaxData updateServer(ServerInfo serverInfo);
	
	/**
	 * 删除服务
	 * @author jinxindong 2016年3月16日 下午3:53:29
	 * @param serverId
	 * @return
	 */
	public AjaxData deleteServer(Integer serverId);
	
	/**
	 * 根据服务id获取服务信息
	 * @author jinxindong 2016年3月16日 下午3:57:17
	 * @return
	 */
	public ServerInfo getServerById(Integer serverId);
	
	/**
	 * 重启服务
	 * @author jinxindong 2016年3月24日 下午2:12:04
	 * @param serverId
	 * @return
	 */
	public AjaxData restartServer(Integer serverId);
}
