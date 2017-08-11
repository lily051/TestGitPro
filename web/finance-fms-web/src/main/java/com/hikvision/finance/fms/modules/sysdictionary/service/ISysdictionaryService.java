/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 下午4:29:30
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysdictionary.service;

import java.util.List;

import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Sysdictionary;
import com.hikvision.finance.fms.modules.sysdictionary.qo.SysdictionaryQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月9日 下午4:29:30
 * @version V1.0 
 */
public interface ISysdictionaryService extends IBaseService<Sysdictionary, SysdictionaryQo>{
	/**
	 * 通过小类型获取字典列表
	 * @author jinxindong 2016年3月16日 上午10:18:35
	 * @param ntype
	 * @return
	 */
	 public List<Sysdictionary> getDicByNtype(Integer ntype);
}
