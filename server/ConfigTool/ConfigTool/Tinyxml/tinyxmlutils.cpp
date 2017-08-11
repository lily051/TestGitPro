
#include "stdafx.h"
#include "tinyxmlutils.h"
#include <algorithm>

namespace TiXmlUtils
{
	void AddChild(TiXmlNode* node, const std::string& name)
	{
		if (node != NULL && !name.empty())
		{
			node->InsertEndChild(TiXmlElement(name));
		}
	}

	std::string GetChildText(const TiXmlNode* node, const std::string& name) 
	{
		if (node != NULL)
		{
			const TiXmlElement* element = (TiXmlElement*)node->FirstChildElement(name);
			if (element != NULL)
			{
				const char* text = element->GetText();
				if (text != NULL)
				{
					return text;
				}
			}
		}
		return "";
	}

    TiXmlNode* FindXmlNode(TiXmlNode* rootNode, const std::string& path)
    {
        TiXmlNode* node = NULL;
        if (rootNode == NULL)
            return node;

        std::string root;
        std::string tail;
        std::string::size_type pos = path.find_first_of('.');
        if (pos != std::string::npos)
        {
            root = path.substr(0, pos);
            tail = path.substr(pos + 1);
        }
        else
        {
            root = path;
            tail = "";
        }

        node = rootNode->FirstChild(root.c_str());
        if (!tail.empty())
        {
            // Recursive call.
            node = FindXmlNode(node, tail);
        }

        return node;
    }

    bool is_true(const char* val)
    {
        if (val != NULL && 0 == memcmp(val, "true", 4))
        {
            return true;
        }
        return false;
    }

    void to_bool(const char* val, bool& b)
    {
        if (val != NULL)
        {
            if (0 == memcmp(val, "true", 4))
            {
                b = true;
            }
            else
            {
                b = false;
            }
        }
    }

    void to_number(const char* val, int& num)
    {
        if (val != NULL)
        {
            num = atoi(val);
        }
    }

    void to_number(const char* val, int& num, int min)
    {
        if (val != NULL)
        {
            num  = atoi(val);
            if (num < min)
            {
                num = min;
            }
        }
    }

#ifdef _WIN32
    void to_number(const char* val, __int64& num, __int64 min)
    {
        if (val != NULL)
        {
            num  = _atoi64(val);
            if (num < min)
            {
                num = min;
            }
        }
    }
#else
    void to_number(const char* val, int64_t& num, int64_t min)
    {
        if (val != NULL)
        {
            num = strtoll(val, 0, 10);
            if (num < min)
            {
                num = min;
            }
        }
    }
#endif

    void to_number(const char* val, int& num, int min, int max)
    {
        if (val != NULL)
        {
            num = atoi(val);
            if (num < min)
            {
                num = min;
            }
            if (num > max)
            {
                num = max;
            }
        }
    }

    void to_string(const char* val, std::string& str) 
    {
        if (val != NULL)
        {
            str = val;
        }
    } 

}
