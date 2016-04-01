#include	"..\All.h"

//********************************************************
//		ContorllObjectŠÖ˜A
//********************************************************
bool npc_predicate::C_Test(ControllObject* character)
{
	if (!character) return false;
	return true;
}

//********************************************************
//		FieldInfromationŠÖ˜A
//********************************************************
//bool npc_predicate::F_VendingMachine(const Field_Node* node)
//{
//	return (node->attribute == nodeattribute::vending_machine) ? true : false;
//}
//
//bool npc_predicate::F_Tactival_Point(const Field_Node* node)
//{
//	return (node->attribute == nodeattribute::tactical_point) ? true : false;
//}
//
//bool npc_predicate::F_Flug(const Field_Node* node)
//{
//	return (node->attribute == nodeattribute::flug_point) ? true : false;
//}

//bool npc_predicate::memory::Pickup_Mapobj_ObjectReport(const Attribute<ObjectReport>& memory)
//{
//	return (memory.value.object_Type == object_type::mapobj) ? true : false;
//}
//
//bool npc_predicate::memory::Pickup_Contorllobj_ObjectReport(const Attribute<ObjectReport>& memory)
//{
//	return (memory.value.object_Type == object_type::controllobj) ? true : false;
//}