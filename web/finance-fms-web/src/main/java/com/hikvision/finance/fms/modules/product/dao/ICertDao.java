/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月10日 上午9:23:27
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.dao;

import java.util.List;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Crtificate;
import com.hikvision.finance.fms.modules.product.qo.CrtificateQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月10日 上午9:23:27
 * @version V1.0 
 */
public interface ICertDao extends IBaseDao<Crtificate,CrtificateQo> {
	/**
	 * 根据证书ids获取该用户的证书名字
	 * @author jinxindong 2016年3月15日 下午6:02:26
	 * @param ids
	 * @return
	 */
	public List<Crtificate> getCertNamesByIds(List<Integer> ids);
	
	/**
	 * 搜索证书
	 * @author jinxindong 2016年6月14日 下午2:43:58
	 * @param name
	 * @return
	 */
	public List<Crtificate> getCertByName(String name);
}
