#ifndef __NPC_CONTOROLL_PREDICATE_H__
#define __NPC_CONTOROLL_PREDICATE_H__

class Field_Node;

/**
*@note CÇ™ControllObjä÷òAÅ@FÇ™Field_Informationä÷òA
*/
namespace npc_predicate
{
	bool C_Test(ControllObject* character);

/***************************************************************/

	bool F_VendingMachine(const Field_Node* node);

	bool F_Tactival_Point(const Field_Node* node);

	bool F_Flug(const Field_Node* node);

	//namespace memory
	//{
	//	bool	Pickup_Mapobj_ObjectReport(const Attribute<ObjectReport>& memory);
	//	bool	Pickup_Contorllobj_ObjectReport(const Attribute<ObjectReport>& memory);
	//}

}
#endif
