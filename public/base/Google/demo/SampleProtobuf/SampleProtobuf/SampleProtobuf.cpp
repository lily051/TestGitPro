// SampleProtobuf.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "../message/tutorial.Device.pb.h"
#include <string>
#include "../message/tutorial.AddressBook.pb.h"
#include "../message/alarmclient.UnhandleAlarmInfo.pb.h"

#if _DEBUG
#pragma comment(lib,"../protobuf-2.4.1/lib_d/libprotobuf.lib")
#else
#pragma comment(lib,"../protobuf-2.4.1/lib_r/libprotobuf.lib")
#endif

void ShwoAddressBookInfo(const tutorial::AddressBook& address_book);
void SaveUnhandleAlarmInfo();
void ReadUnhandleAlarmInfo();

int _tmain(int argc, _TCHAR* argv[])
{
	//writer 
	do 
	{
		tutorial::Device dev1; 
		dev1.set_id(8100);
		dev1.set_name("HIK-Dev1");
		std::fstream output("./devinfo", std::ios::out | std::ios::trunc | std::ios::binary); 
		if (!dev1.SerializeToOstream(&output)) 
		{ 
			std::cerr << "Failed to write msg." << std::endl; 
			return -1; 
		}  
	} while (0);

	//reader
	do 
	{
		tutorial::Device dev1; 
		std::fstream input("./devinfo", std::ios::in | std::ios::binary); 
		if (!dev1.ParseFromIstream(&input)) 
		{ 
			std::cerr << "Failed to parse address book." << std::endl; 
			return -1; 
		} 
		std::cout << dev1.id() << std::endl; 
		std::cout << dev1.name() << std::endl; 
	} while (0);

	//字符串
	std::string sDevInfo = "";
	do 
	{
		tutorial::Device dev1; 
		int nid = dev1.id();
		dev1.set_id(8100);
		dev1.set_name("HIK-Dev1");
		if (!dev1.SerializeToString(&sDevInfo)) 
		{ 
			return -1; 
		}  
	} while (0);

	do 
	{
		tutorial::Device dev1; 
		if (!dev1.ParseFromString(sDevInfo)) 
		{ 
			return -1; 
		}  
		std::cout << dev1.id() << std::endl; 
		std::cout << dev1.name() << std::endl; 
	} while (0);

	//write address book
	do 
	{
		tutorial::AddressBook addressBook;
		//第一个联系人的数据定义与初始化   
		tutorial::Person    *person1  = addressBook.add_person();  
		person1->set_id(1);  
		person1->set_name("liangjungao");      
		person1->set_email("liangjungao@hikvision.com.cn");  
		//手机号码数据定义与初始化   
		tutorial::Person_PhoneNumber *phoneNumberMobile = person1->add_phone();  
		phoneNumberMobile->set_number("15869128801");  
		phoneNumberMobile->set_type(tutorial::Person_PhoneType_MOBILE);  
		//座机号码数据定义与初始化   
		tutorial::Person_PhoneNumber *phoneNumberWork   = person1->add_phone();  
		phoneNumberWork->set_number("0571-88075998-8280");  
		phoneNumberWork->set_type(tutorial::Person_PhoneType_WORK);  

		tutorial::Person  *person2 = addressBook.add_person();  
		person2->set_id(2);  
		person2->set_name("xuezhihu");  
		person2->set_email("xuezhihu@hikvision.com.cn");  
		tutorial::Person_PhoneNumber *phoneNumber2      = person2->add_phone();  
		phoneNumber2->set_number("15988122701");      
		phoneNumber2->set_type(tutorial::Person_PhoneType_MOBILE);  

		std::fstream output("./addressbook.data", std::ios::out | std::ios::trunc | std::ios::binary); 
		if (!addressBook.SerializeToOstream(&output)) 
		{ 
			std::cerr << "Failed to write msg." << std::endl; 
			return -1; 
		} 
	} while (0);

	//read address book
	do 
	{
		tutorial::AddressBook addressBook;
		std::fstream input("./addressbook.data", std::ios::in | std::ios::binary); 
		if (!addressBook.ParseFromIstream(&input)) 
		{ 
			std::cerr << "Failed to parse address book." << std::endl; 
			return -1; 
		} 
		ShwoAddressBookInfo(addressBook);
	} while (0);

	//SaveUnhandleAlarmInfo();
	//ReadUnhandleAlarmInfo();

	system("Pause");

	return 0;
}

void ShwoAddressBookInfo(const tutorial::AddressBook& address_book) 
{
	for (int i = 0; i < address_book.person_size(); i++) 
	{
		const tutorial::Person& person = address_book.person(i);
		std::cout << "Person ID: " << person.id() << std::endl;
		std::cout << "  Name: " << person.name() << std::endl;
		if (person.has_email()) 
		{
			std::cout << "  E-mail address:" << person.email() << std::endl;
		}

		for (int j = 0; j < person.phone_size(); j++) 
		{
			const tutorial::Person::PhoneNumber& phone_number = person.phone(j);
			switch (phone_number.type()) 
			{
			case tutorial::Person_PhoneType_MOBILE:
				{
					std::cout << "  Mobile phone #: ";
				}
				break;
			case tutorial::Person_PhoneType_HOME:
				{
					std::cout << "  Home phone #: ";
				}

				break;
			case tutorial::Person_PhoneType_WORK:
				std::cout << "  Work phone #: ";
				break;
			}
			std::cout << phone_number.number() << std::endl;
		}
	}
}

void SaveUnhandleAlarmInfo()
{
	alarmclient::UnhandleAlarmInfo unhandleAlarmInfo; 

	//添加一条未处理报警信息
	alarmclient::UnhandleAlarmInfo_Item    *item  = unhandleAlarmInfo.add_uaitem();  
	item->set_name("201_通道_01_移动侦测报警");  
	item->set_level(5);
	item->set_typedesc("移动侦测报警");
	item->set_plan("xxyyy");
	item->set_msg("xx地方报警，请联系yyy");
	item->set_addr("杭州海康威视");
	item->set_contact("联系yyy");
	item->set_index("1101000000000038");
	item->set_time("1324351125");
	alarmclient::UnhandleAlarmInfo_Item_Camera * itemCamera1 = item->add_camera();
	itemCamera1->set_id(1234);
	alarmclient::UnhandleAlarmInfo_Item_Camera * itemCamera2 = item->add_camera();
	itemCamera2->set_id(1235);
	alarmclient::UnhandleAlarmInfo_Item_Camera * itemCamera3 = item->add_camera();
	itemCamera3->set_id(1236);
	
	//序列化保存1条未处理报警信息
	std::fstream output("./UnhandleAlarmInfo.data", std::ios::out | std::ios::trunc | std::ios::binary); 
	if (!unhandleAlarmInfo.SerializeToOstream(&output)) 
	{ 
		std::cerr << "Failed to write msg." << std::endl; 
		return ; 
	} 
}
void ReadUnhandleAlarmInfo()
{
	alarmclient::UnhandleAlarmInfo unhandleAlarmInfo; 
	std::fstream input("./UnhandleAlarmInfo.data", std::ios::in | std::ios::binary); 
	if (!unhandleAlarmInfo.ParseFromIstream(&input)) 
	{ 
		std::cerr << "Failed to parse address book." << std::endl; 
		return; 
	} 

	for (int i = 0; i < unhandleAlarmInfo.uaitem_size(); ++i) 
	{
		std::cout <<"************************************"<< std::endl;
		const alarmclient::UnhandleAlarmInfo_Item& item = unhandleAlarmInfo.uaitem(i);
		std::cout <<"name:"<< item.name() << std::endl;
		std::cout <<"level:"<< item.level() << std::endl;
		std::cout <<"typedesc:"<< item.typedesc() << std::endl;
		std::cout <<"plan:"<< item.plan() << std::endl;
		std::cout <<"msg:"<< item.msg() << std::endl;
		std::cout <<"addr:"<< item.addr() << std::endl;
		std::cout <<"contact:"<< item.contact() << std::endl;
		std::cout <<"index:"<< item.index() << std::endl;
		std::cout <<"time:"<< item.time() << std::endl;
		for(int iCam=0;iCam<item.camera_size();++iCam)
		{
			const alarmclient::UnhandleAlarmInfo_Item_Camera& camera = item.camera(iCam);
			std::cout <<"camera:"<< camera.id() << std::endl;
		}
	}
	std::cout <<"************************************"<< std::endl;
}