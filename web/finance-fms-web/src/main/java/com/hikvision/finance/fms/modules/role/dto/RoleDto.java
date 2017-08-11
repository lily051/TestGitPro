/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午2:30:39
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.role.dto;

import java.util.List;

import com.hikvision.finance.fms.model.Role;
import com.hikvision.finance.fms.model.Roleprivilegemapping;


/**
 * <p></p>
 * @author jinxindong 2016年2月29日 下午2:30:39
 * @version V1.0 
 */
public class RoleDto {
	private Role role;
	private List<Roleprivilegemapping> powers;
	
    public Role getRole() {
    	return role;
    }
	
    public void setRole(Role role) {
    	this.role = role;
    }
	
    public List<Roleprivilegemapping> getPowers() {
    	return powers;
    }
	
    public void setPowers(List<Roleprivilegemapping> powers) {
    	this.powers = powers;
    }
	
}
