namespace ConstFunction1 {

class Group
{
public:
	Group(int data3, int data4) :
		Data3(data3),
		Data4(data4)
	{

	}

	class Team
	{
	public:
		Team(const int data1, const int* pdata2) :
			Data1(data1),
			Data2(*pdata2)
		{

		}
	private:
		int Data1;
		int Data2;
	};

	Team buildTeam() const;


	class Squad
	{
	public:
		Squad(int a, int* b) :
			Data5(a),
			Data6(*b)
		{

		}
	private:
		int Data5;
		int Data6;
	};

	Squad BuildSquad() const;




private:
	int Data3;
	int Data4;
};





Group::Team Group::buildTeam() const
{
	return Team { Data3, &Data4 };
}


Group::Squad Group::BuildSquad() const
{
	//return Squad { Data3, &Data4 };
	int a = 70;
	return Squad { 60, &a };
}



void Test_ConstFunction1()
{
	Group group1 { 10, 20 };
	Group::Team team1 = group1.buildTeam();



	Group group2 { 30, 40 };
	const Group& conGrpRef = group2;

	//Group::Squad squad1 = conGrpRef.BuildSquad();
	// this still won't work (if line 66 is activated) even if conGrpRef promises not to change what it refers to
	// The reason is that the attribute of conGrpRef doesn't matter,
	// Squad's constructor must promise not to change its input arguments, only then BuildSquad can call it
	// because BuildSquad promises (to its caller) that it won't change Data3 and Data4



}

}