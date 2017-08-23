package com.userEntity;

import javax.persistence.*;

/**
 * Sysconfigtbl entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "sysconfigtbl", schema = "public")
public class Sysconfigtbl implements java.io.Serializable {

	// Fields
	private static final long serialVersionUID = 1L;
	private Integer sysid;
	private Boolean enableLogoBk;
	private String logopath;
	private String productpath;
	private String backgroundpath;
	private Boolean enableTitle;
	private String maintitle;
	private String subtitle;
	private Boolean enableCopyright;
	private String copyright;
	private Integer longitude;
	private Integer latitude;
	private Integer mapLevel;
	private Integer gisalarmspan;
	private Integer overSpeed;
	private Integer lowSpeed;
	private Integer gpsDate;
	private Integer logDate;
	private Integer alarmDate;
	private Integer captureDate;
	private Integer treeOrder;
	private String pushIp;
	private Integer pushHppPort;
	private Integer pushHttpsPort;
	private Boolean enableIpLocked;
	private Integer ipLockedTime;
	private Integer pwdMinRank;
	private Integer pwdExpireDate;

	// Constructors

	/** default constructor */
	public Sysconfigtbl() {
	}

	/** minimal constructor */
	public Sysconfigtbl(Integer sysid, Boolean enableLogoBk, String logopath, String productpath, String backgroundpath, Boolean enableTitle,
                        Boolean enableCopyright, Integer longitude, Integer latitude, Integer gisalarmspan, Integer gpsDate, Integer logDate, Integer alarmDate,
                        Integer captureDate, Integer treeOrder, String pushIp, Integer pushHppPort, Integer pushHttpsPort) {
		this.sysid = sysid;
		this.enableLogoBk = enableLogoBk;
		this.logopath = logopath;
		this.productpath = productpath;
		this.backgroundpath = backgroundpath;
		this.enableTitle = enableTitle;
		this.enableCopyright = enableCopyright;
		this.longitude = longitude;
		this.latitude = latitude;
		this.gisalarmspan = gisalarmspan;
		this.gpsDate = gpsDate;
		this.logDate = logDate;
		this.alarmDate = alarmDate;
		this.captureDate = captureDate;
		this.treeOrder = treeOrder;
		this.pushIp = pushIp;
		this.pushHppPort = pushHppPort;
		this.pushHttpsPort = pushHttpsPort;
	}

	/** full constructor */
	public Sysconfigtbl(Integer sysid, Boolean enableLogoBk, String logopath, String productpath, String backgroundpath, Boolean enableTitle,
                        String maintitle, String subtitle, Boolean enableCopyright, String copyright, Integer longitude, Integer latitude, Integer gisalarmspan,
                        Integer overSpeed, Integer lowSpeed, Integer gpsDate, Integer logDate, Integer alarmDate, Integer captureDate, Integer treeOrder, String pushIp,
                        Integer pushHppPort, Integer pushHttpsPort) {
		this.sysid = sysid;
		this.enableLogoBk = enableLogoBk;
		this.logopath = logopath;
		this.productpath = productpath;
		this.backgroundpath = backgroundpath;
		this.enableTitle = enableTitle;
		this.maintitle = maintitle;
		this.subtitle = subtitle;
		this.enableCopyright = enableCopyright;
		this.copyright = copyright;
		this.longitude = longitude;
		this.latitude = latitude;
		this.gisalarmspan = gisalarmspan;
		this.overSpeed = overSpeed;
		this.lowSpeed = lowSpeed;
		this.gpsDate = gpsDate;
		this.logDate = logDate;
		this.alarmDate = alarmDate;
		this.captureDate = captureDate;
		this.treeOrder = treeOrder;
		this.pushIp = pushIp;
		this.pushHppPort = pushHppPort;
		this.pushHttpsPort = pushHttpsPort;
	}

	// Property accessors
	@Id
	@GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "s_sysconfigtbl")
	@SequenceGenerator(name = "s_sysconfigtbl", sequenceName = "s_sysconfigtbl", allocationSize = 1)
	@Column(name = "sysid", unique = true, nullable = false)
	public Integer getSysid() {
		return this.sysid;
	}

	public void setSysid(Integer sysid) {
		this.sysid = sysid;
	}

	@Column(name = "enable_logo_bk", nullable = false)
	public Boolean getEnableLogoBk() {
		return this.enableLogoBk;
	}

	public void setEnableLogoBk(Boolean enableLogoBk) {
		this.enableLogoBk = enableLogoBk;
	}

	@Column(name = "logopath", nullable = false, length = 60)
	public String getLogopath() {
		return this.logopath;
	}

	public void setLogopath(String logopath) {
		this.logopath = logopath;
	}

	@Column(name = "backgroundpath", nullable = false, length = 60)
	public String getBackgroundpath() {
		return this.backgroundpath;
	}

	public void setBackgroundpath(String backgroundpath) {
		this.backgroundpath = backgroundpath;
	}

	@Column(name = "enable_title", nullable = false)
	public Boolean getEnableTitle() {
		return this.enableTitle;
	}

	public void setEnableTitle(Boolean enableTitle) {
		this.enableTitle = enableTitle;
	}

	@Column(name = "maintitle", length = 80)
	public String getMaintitle() {
		return this.maintitle;
	}

	public void setMaintitle(String maintitle) {
		this.maintitle = maintitle;
	}

	@Column(name = "subtitle", length = 80)
	public String getSubtitle() {
		return this.subtitle;
	}

	public void setSubtitle(String subtitle) {
		this.subtitle = subtitle;
	}

	@Column(name = "enable_copyright", nullable = false)
	public Boolean getEnableCopyright() {
		return this.enableCopyright;
	}

	public void setEnableCopyright(Boolean enableCopyright) {
		this.enableCopyright = enableCopyright;
	}

	@Column(name = "copyright", length = 80)
	public String getCopyright() {
		return this.copyright;
	}

	public void setCopyright(String copyright) {
		this.copyright = copyright;
	}

	@Column(name = "longitude", nullable = false)
	public Integer getLongitude() {
		return this.longitude;
	}

	public void setLongitude(Integer longitude) {
		this.longitude = longitude;
	}

	@Column(name = "latitude", nullable = false)
	public Integer getLatitude() {
		return this.latitude;
	}

	public void setLatitude(Integer latitude) {
		this.latitude = latitude;
	}

	@Column(name = "map_level", nullable = false)
	public Integer getMapLevel() {
		return mapLevel;
	}

	public void setMapLevel(Integer mapLevel) {
		this.mapLevel = mapLevel;
	}

	@Column(name = "gisalarmspan", nullable = false)
	public Integer getGisalarmspan() {
		return this.gisalarmspan;
	}

	public void setGisalarmspan(Integer gisalarmspan) {
		this.gisalarmspan = gisalarmspan;
	}

	@Column(name = "over_speed")
	public Integer getOverSpeed() {
		return this.overSpeed;
	}

	public void setOverSpeed(Integer overSpeed) {
		this.overSpeed = overSpeed;
	}

	@Column(name = "low_speed")
	public Integer getLowSpeed() {
		return this.lowSpeed;
	}

	public void setLowSpeed(Integer lowSpeed) {
		this.lowSpeed = lowSpeed;
	}

	@Column(name = "gps_date", nullable = false)
	public Integer getGpsDate() {
		return this.gpsDate;
	}

	public void setGpsDate(Integer gpsDate) {
		this.gpsDate = gpsDate;
	}

	@Column(name = "log_date", nullable = false)
	public Integer getLogDate() {
		return this.logDate;
	}

	public void setLogDate(Integer logDate) {
		this.logDate = logDate;
	}

	@Column(name = "alarm_date", nullable = false)
	public Integer getAlarmDate() {
		return this.alarmDate;
	}

	public void setAlarmDate(Integer alarmDate) {
		this.alarmDate = alarmDate;
	}

	@Column(name = "capture_date", nullable = false)
	public Integer getCaptureDate() {
		return this.captureDate;
	}

	public void setCaptureDate(Integer captureDate) {
		this.captureDate = captureDate;
	}

	@Column(name = "tree_order", nullable = false)
	public Integer getTreeOrder() {
		return this.treeOrder;
	}

	public void setTreeOrder(Integer treeOrder) {
		this.treeOrder = treeOrder;
	}

	@Column(name = "push_ip", nullable = false, length = 64)
	public String getPushIp() {
		return this.pushIp;
	}

	public void setPushIp(String pushIp) {
		this.pushIp = pushIp;
	}

	@Column(name = "push_hpp_port", nullable = false)
	public Integer getPushHppPort() {
		return this.pushHppPort;
	}

	public void setPushHppPort(Integer pushHppPort) {
		this.pushHppPort = pushHppPort;
	}

	@Column(name = "push_https_port", nullable = false)
	public Integer getPushHttpsPort() {
		return this.pushHttpsPort;
	}

	public void setPushHttpsPort(Integer pushHttpsPort) {
		this.pushHttpsPort = pushHttpsPort;
	}

	@Column(name = "productpath", nullable = false, length = 60)
	public String getProductpath() {
		return this.productpath;
	}

	public void setProductpath(String productpath) {
		this.productpath = productpath;
	}

	@Column(name = "enable_ip_locked", nullable = false)
	public Boolean getEnableIpLocked() {
		return enableIpLocked;
	}

	public void setEnableIpLocked(Boolean enableIpLocked) {
		this.enableIpLocked = enableIpLocked;
	}

	@Column(name = "ip_locked_time", nullable = false)
	public Integer getIpLockedTime() {
		return ipLockedTime;
	}

	public void setIpLockedTime(Integer ipLockedTime) {
		this.ipLockedTime = ipLockedTime;
	}

	@Column(name = "pwd_min_rank", nullable = false)
	public Integer getPwdMinRank() {
		return pwdMinRank;
	}

	public void setPwdMinRank(Integer pwdMinRank) {
		this.pwdMinRank = pwdMinRank;
	}

	@Column(name = "pwd_expire_date", nullable = false)
	public Integer getPwdExpireDate() {
		return pwdExpireDate;
	}

	public void setPwdExpireDate(Integer pwdExpireDate) {
		this.pwdExpireDate = pwdExpireDate;
	}

	
}