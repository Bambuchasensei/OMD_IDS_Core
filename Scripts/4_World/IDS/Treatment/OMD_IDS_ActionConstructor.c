// ItemBase.AddAction only accepts an action already in the action map.
// P:\scripts\4_world\entities\itembase.c:257 ActionManagerBase.GetAction
modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		actions.Insert(OMD_IDS_ActionSutureSelf);
		actions.Insert(OMD_IDS_ActionSutureTarget);
		actions.Insert(OMD_IDS_ActionExtractBulletSelf);
		actions.Insert(OMD_IDS_ActionExtractBulletTarget);
		actions.Insert(OMD_IDS_ActionRemoveDressingSelf);
		actions.Insert(OMD_IDS_ActionRemoveDressingTarget);
	}
};
