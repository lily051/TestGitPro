/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 下午4:39:02
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.qo;

import java.util.List;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月9日 下午4:39:02
 * @version V1.0 
 */
public class SysconfigQo extends BaseQo{
	 /**
     * 序列化ID
     */
    private static final long serialVersionUID = 5411183513633074995L;
	private List<Integer> nkeys;
	 private Integer nkey;
	
    public List<Integer> getNkeys() {
    	return nkeys;
    }
	
    public void setNkeys(List<Integer> nkeys) {
    	this.nkeys = nkeys;
    }
	
    public Integer getNkey() {
    	return nkey;
    }
	
    public void setNkey(Integer nkey) {
    	this.nkey = nkey;
    }
	 
	 
}
