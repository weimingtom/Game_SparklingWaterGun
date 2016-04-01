#include	"..\All.h"


void	DeFuzzify_Param::Load(char* filename)
{
	textLoader loader(filename);
	//MyAssert(loader.Be_Init(), "OK");
	loader.In();

	m_sight_target = loader.LoadFloat();
}

void	NPC_FuzzyTool::Initialize()
{
	/*視界の攻撃スイッチファジー*/
	module_Map.insert(std::pair<std::string, FuzzyModule*>("Sight_Target", new FuzzyModule()));
	FuzzyVariable& dist_to_Target = module_Map["Sight_Target"]->Create_EnptyFLV("Dist_to_Target");
	FuzzyVariable& ammo_State = module_Map["Sight_Target"]->Create_EnptyFLV("Charge_State");
	FuzzyVariable& Desiraliblity = module_Map["Sight_Target"]->Create_EnptyFLV("Desiraliblity");

	//ターゲットとの距離
	FuzzySet_Proxy& dist_very_Desirable = dist_to_Target.Add_LeftShoulderSet("distVeryDesirable", 0, 10, 20);
	FuzzySet_Proxy& dist_Desirable = dist_to_Target.Add_TriangleSet("distDesirable", 10, 20, 40);
	FuzzySet_Proxy& dist_not_Desirable = dist_to_Target.Add_RightShoulderSet("distNotDesirable", 20, 40, 60);

	//弾の状態
	FuzzySet_Proxy& ammo_not_Desirable = ammo_State.Add_LeftShoulderSet("ammoNotDesirable", 0, 30, 70);
	FuzzySet_Proxy& ammo_Desirable = ammo_State.Add_TriangleSet("ammoDesirable", 30, 70, 100);
	FuzzySet_Proxy& ammo_very_Desirable = ammo_State.Add_RightShoulderSet("ammoveryDesirable", 70, 100, 120);

	//望ましさのFLV
	FuzzySet_Proxy& notDesirable = Desiraliblity.Add_LeftShoulderSet("notDesirable", 0, 20, 50);
	FuzzySet_Proxy& Desirable = Desiraliblity.Add_TriangleSet("Desirable", 20, 50, 80);
	FuzzySet_Proxy& VeryDesirable = Desiraliblity.Add_RightShoulderSet("VeryDesirable", 50, 80, 100);

	//ルールの設定
	module_Map["Sight_Target"]->Add_Rule(Fuzzy_AND(dist_very_Desirable, ammo_very_Desirable), VeryDesirable);
	module_Map["Sight_Target"]->Add_Rule(Fuzzy_AND(dist_very_Desirable, ammo_Desirable), VeryDesirable);
	module_Map["Sight_Target"]->Add_Rule(Fuzzy_AND(dist_very_Desirable, ammo_not_Desirable), Desirable);

	module_Map["Sight_Target"]->Add_Rule(Fuzzy_AND(dist_Desirable, ammo_very_Desirable), VeryDesirable);
	module_Map["Sight_Target"]->Add_Rule(Fuzzy_AND(dist_Desirable, ammo_Desirable), Desirable);
	module_Map["Sight_Target"]->Add_Rule(Fuzzy_AND(dist_Desirable, ammo_not_Desirable), notDesirable);

	module_Map["Sight_Target"]->Add_Rule(Fuzzy_AND(dist_not_Desirable, ammo_very_Desirable), Desirable);
	module_Map["Sight_Target"]->Add_Rule(Fuzzy_AND(dist_not_Desirable, ammo_Desirable), notDesirable);
	module_Map["Sight_Target"]->Add_Rule(Fuzzy_AND(dist_not_Desirable, ammo_not_Desirable), notDesirable);

}

NPC_FuzzyTool::~NPC_FuzzyTool()
{
	for (std::map<std::string, FuzzyModule*>::iterator it = module_Map.begin();
		it != module_Map.end();
		++it)
	{
		SAFE_DELETE(it->second);
	}
}

float	NPC_FuzzyTool::Calculate_Sight_Target_Character(const float& dist, const float& charge)
{
	module_Map["Sight_Target"]->Fuzzify("Dist_to_Target", dist);
	module_Map["Sight_Target"]->Fuzzify("Charge_State", charge);

	return module_Map["Sight_Target"]->Defuzzify("Desiraliblity", defuzzify_alg::centroid);
}