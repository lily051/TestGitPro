package com.hikvision.finance.example.model;

public class File {
	private String fileName;
	private String fileDisplayName;
	private String bucket;
	private Cloud cloud;
	
	public File() {
	}
	
	public File(Cloud cloud, String fileName, String fileDisplayName, String bucket) {
		this.fileName = fileName;
		this.fileDisplayName = fileDisplayName;
		this.bucket = bucket;
		this.cloud = cloud;
	}
	
    public String getFileName() {
    	return fileName;
    }
	
    public void setFileName(String fileName) {
    	this.fileName = fileName;
    }
	
    public String getFileDisplayName() {
    	return fileDisplayName;
    }
	
    public void setFileDisplayName(String fileDisplayName) {
    	this.fileDisplayName = fileDisplayName;
    }
    
    public String getBucket() {
    	return bucket;
    }
	
    public void setBucket(String bucket) {
    	this.bucket = bucket;
    }
	
    public Cloud getCloud() {
    	return cloud;
    }

    public void setCloud(Cloud cloud) {
    	this.cloud = cloud;
    }
}