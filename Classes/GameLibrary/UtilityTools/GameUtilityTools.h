#ifndef GameUtilityTools_h__
#define GameUtilityTools_h__

class GameUilityTools
{
public:
	static GameUilityTools* getInstance()
	{
		if (s_instance == nullptr)
		{
			s_instance = new GameUilityTools();
		}
		return s_instance;
	}

	static const char* CreateGuidString();
private:
	static GameUilityTools* s_instance;
};

#endif // GameUtilityTools_h__
