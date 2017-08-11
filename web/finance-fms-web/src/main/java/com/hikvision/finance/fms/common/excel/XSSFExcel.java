package com.hikvision.finance.fms.common.excel;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.List;
import java.util.Map;

import org.apache.poi.ss.usermodel.DataValidationHelper;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.xssf.usermodel.XSSFDataValidationHelper;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import com.hikvision.finance.fms.common.excel.model.ExcelHead;


public class XSSFExcel implements IPoiExcelOperation {
	
	private Workbook wb;
	private DataValidationHelper dataValidationHelper;
	
	public Workbook readExcel(InputStream s) throws Exception {
		XSSFWorkbook wb = new XSSFWorkbook(s);
		return wb;
	}
	
	public Workbook readExcel(String filePath) throws Exception {
		FileInputStream fis = new FileInputStream(new File(filePath));
		return readExcel(fis);
	}
	
	public Workbook createExcel(String excelName, List<Map<Object, Object>> dataList, List<ExcelHead> headList) {
		XSSFWorkbook wb = new XSSFWorkbook();
		return wb;
	}
	
	private DataValidationHelper createdataValidationHelper(Sheet sheet) {
		DataValidationHelper dataValidationHelper = new XSSFDataValidationHelper((XSSFSheet)sheet);
		this.dataValidationHelper = dataValidationHelper;
		return dataValidationHelper;
	}
	
	public DataValidationHelper getdataValidationHelper(Sheet sheet) {
		if (dataValidationHelper == null) {
			return createdataValidationHelper(sheet);
		} else {
			return dataValidationHelper;
		}
	}
	
	private Workbook createWorkbook() {
		return new XSSFWorkbook();
	}
	
	public Workbook getWorkbook() {
		if (wb == null) {
			wb = createWorkbook();
		}
		return wb;
	}
}
