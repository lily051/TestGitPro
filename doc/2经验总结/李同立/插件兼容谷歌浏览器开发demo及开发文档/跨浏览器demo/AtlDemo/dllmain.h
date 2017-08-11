// dllmain.h : Declaration of module class.

class CAtlDemoModule : public CAtlDllModuleT< CAtlDemoModule >
{
public :
	DECLARE_LIBID(LIBID_AtlDemoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLDEMO, "{9C9D48C9-0384-4F78-8574-C8B9BA651051}")
};

extern class CAtlDemoModule _AtlModule;
