//Language.h  
#include "cocos2d.h"
USING_NS_CC;
//return type:const char*  
#define LocalizedCStringByKey(key) Language::getStringByKey(key)  

class Language{

public:

	static const std::string getStringByKey(const char * key){

		//获取当前系统环境语言类型  
		LanguageType languageType = Application::getInstance()->getCurrentLanguage();

		//字典  
		ValueMap dict;

		if (LanguageType::CHINESE == languageType)
			//根据语言选择不同的属性表  
			dict = FileUtils::getInstance()->getValueMapFromFile("language/zh.plist");
		else
			dict = FileUtils::getInstance()->getValueMapFromFile("language/en.plist");

		std::string keyStr = key;
		auto str = dict.find(keyStr);

		//返回key对应的value  
		return (str->second).asString();
	}

};