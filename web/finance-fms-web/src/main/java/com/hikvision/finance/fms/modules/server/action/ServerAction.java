/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月11日 下午3:58:25
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.server.action;

import java.util.List;

import com.hikvision.finance.fms.common.util.Constants.SysDictionary.ServerType;
import com.hikvision.finance.fms.common.util.Constants.SysDictionary.ServerType.DictType;
import com.hikvision.finance.fms.model.Server;
import com.hikvision.finance.fms.model.Sysdictionary;
import com.hikvision.finance.fms.modules.server.dto.ServerInfo;
import com.hikvision.finance.fms.modules.server.service.IServerService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.www.action.BaseAction;

/**
 * <p>服务管理Action</p>
 * @author jinxindong 2016年3月11日 下午3:58:25
 * @version V1.0
 */
public class ServerAction extends BaseAction<Server> {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 1964869213156976501L;
	private IServerService serverService;
	private ServerInfo serverInfo;
	private List<Sysdictionary> serverTypeList;
	private List<ServerInfo> serverList;
	
	/**
	 * 获取服务器列表
	 * @author jinxindong 2016年3月16日 上午11:08:43
	 * @return
	 */
	public String getServerPage() {
		String keyWord = "";
		if (serverInfo != null && serverInfo.getKeyWord() != null) {
			keyWord = serverInfo.getKeyWord();
		}
		serverList = serverService.getServerPage(keyWord);
		operPage = "/modules/server/serverInner.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳到服务类型选择页面
	 * @author jinxindong 2016年3月15日 下午3:08:29
	 * @return
	 */
	public String getDicByNtype() {
		serverTypeList = serverService.getDicByNtype(DictType.DICT_S_SERVER);// 从数据字典中获取可用的服务类型列表
		operPage = "/modules/server/dialogs/serverSelect-win.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳转到上传服务添加页面
	 * @author jinxindong 2016年3月14日 下午4:38:17
	 * @return
	 */
	public String toUploadServerPage() {
		operPage = "/modules/server/dialogs/config-upload.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳转到校时服务添加页面
	 * @author jinxindong 2016年3月14日 下午4:38:17
	 * @return
	 */
	public String toTimeServerPage() {
		operPage = "/modules/server/dialogs/config-timing.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳转到录像守卫服务添加页面
	 * @author jinxindong 2016年3月14日 下午4:38:17
	 * @return
	 */
	public String toRecordServerPage() {
		operPage = "/modules/server/dialogs/config-recGuard.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳转到录像守卫服务添加页面
	 * @author jinxindong 2016年3月14日 下午4:38:17
	 * @return
	 */
	public String toBaseServerPage() {
		operPage = "/modules/server/dialogs/server-basic.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 添加服务
	 * @author jinxindong 2016年3月16日 下午3:11:28
	 * @return
	 */
	public String addServer() {
		if (serverInfo == null) {
			throw new ExpectedException("", "未获取到该服务信息");
		}
		ajaxData = serverService.addServer(serverInfo);
		return AJAX;
	}
	
	/**
	 * 跳转到编辑页面
	 * @author jinxindong 2016年3月14日 下午4:39:33
	 * @return
	 */
	public String toEditServerPage() {
		if (serverInfo == null || serverInfo.getId() == null) {
			throw new ExpectedException("", "未获取到该服务信息id");
		}
		serverInfo = serverService.getServerById(serverInfo.getId());
		if (serverInfo.getNtype() == ServerType.MT_WEB) {
			operPage = "/modules/server/dialogs/server-basicEdit.jsp";
		} else if (serverInfo.getNtype() == ServerType.MT_SERVICE_UPLOAD) {
			operPage = "/modules/server/dialogs/config-uploadEdit.jsp";
		} else if (serverInfo.getNtype() == ServerType.MT_SERVICE_GUARD) {
			operPage = "/modules/server/dialogs/config-recGuardEdit.jsp";
		} else if (serverInfo.getNtype() == ServerType.MT_SERVICE_TIMING) {
			operPage = "/modules/server/dialogs/config-timingEdit.jsp";
		}
		return DISPATCHER;
	}
	
	/**
	 * 保存编辑服务
	 * @author jinxindong 2016年3月16日 下午3:54:19
	 * @return
	 */
	public String updateServer() {
		if (serverInfo == null || serverInfo.getId() == null) {
			throw new ExpectedException("", "未获取到该服务信息id");
		}
		ajaxData = serverService.updateServer(serverInfo);
		return AJAX;
	}
	
	/**
	 * 删除服务
	 * @author jinxindong 2016年3月16日 下午3:54:11
	 * @return
	 */
	public String deleteServer() {
		if (serverInfo == null || serverInfo.getId() == null) {
			throw new ExpectedException("", "未获取到该服务信息id");
		}
		ajaxData = serverService.deleteServer(serverInfo.getId());
		return AJAX;
	}
	
	/**
	 * 重启服务
	 * @author jinxindong 2016年3月24日 下午2:10:41
	 * @return
	 */
	public String restartServer() {
		if (serverInfo == null || serverInfo.getId() == null) {
			throw new ExpectedException("", "未获取到该服务信息id");
		}
		ajaxData = serverService.restartServer(serverInfo.getId());
		return AJAX;
	}

	// ==================================getter/setter==============================================
	
	public IServerService getServerService() {
		return serverService;
	}
	
	public void setServerService(IServerService serverService) {
		this.serverService = serverService;
	}
	
	public ServerInfo getServerInfo() {
		return serverInfo;
	}
	
	public void setServerInfo(ServerInfo serverInfo) {
		this.serverInfo = serverInfo;
	}
	
	public List<Sysdictionary> getServerTypeList() {
		return serverTypeList;
	}
	
	public void setServerTypeList(List<Sysdictionary> serverTypeList) {
		this.serverTypeList = serverTypeList;
	}
	
	public List<ServerInfo> getServerList() {
		return serverList;
	}
	
	public void setServerList(List<ServerInfo> serverList) {
		this.serverList = serverList;
	}
}
