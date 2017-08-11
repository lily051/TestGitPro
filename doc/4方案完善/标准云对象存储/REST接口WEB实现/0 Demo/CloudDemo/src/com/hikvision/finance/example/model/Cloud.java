package com.hikvision.finance.example.model;

public class Cloud {
	private String ip;
	private String port;
	private String accesskey;
	private String secretkey;
	
	public Cloud() {
	}
	
	public Cloud(String ip, String port, String accesskey, String secretkey) {
		this.ip = ip;
		this.port = port;
		this.accesskey = accesskey;
		this.secretkey = secretkey;
	}
	
    public String getIp() {
    	return ip;
    }
	
    public void setIp(String ip) {
    	this.ip = ip;
    }
	
    public String getPort() {
    	return port;
    }
	
    public void setPort(String port) {
    	this.port = port;
    }
	
    public String getAccesskey() {
    	return accesskey;
    }
	
    public void setAccesskey(String accesskey) {
    	this.accesskey = accesskey;
    }
	
    public String getSecretkey() {
    	return secretkey;
    }
	
    public void setSecretkey(String secretkey) {
    	this.secretkey = secretkey;
    }
}