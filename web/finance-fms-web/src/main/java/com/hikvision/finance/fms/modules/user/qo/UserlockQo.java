/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年11月3日 上午11:02:37
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.qo;

import java.util.Date;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;

/**
 * 
 * <p>
 * </p>
 * 
 * @author zhoujiajun 2016年11月3日 上午11:02:37
 * @version V1.0
 */
public class UserlockQo extends BaseQo {
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 8475312279513990801L;
	private Integer id;
	private String strName;
	private Integer nfailCount;
	private Date dtLoginTime;
	private Integer userId;

	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public String getStrName() {
		return strName;
	}

	public void setStrName(String strName) {
		this.strName = strName;
	}

	public Integer getNfailCount() {
		return nfailCount;
	}

	public void setNfailCount(Integer nfailCount) {
		this.nfailCount = nfailCount;
	}

	public Date getDtLoginTime() {
		return dtLoginTime;
	}

	public void setDtLoginTime(Date dtLoginTime) {
		this.dtLoginTime = dtLoginTime;
	}

	public Integer getUserId() {
		return userId;
	}

	public void setUserId(Integer userId) {
		this.userId = userId;
	}

}
