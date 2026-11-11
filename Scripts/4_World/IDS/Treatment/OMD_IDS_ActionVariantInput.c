// ItemActionsWidget.SetActionWidget calls action.GetInput().GetInputType() with no null check.
// 1.30 file, verified against the client log lines: BuildCursor 182, Update 216, SetActionWidget 638.
// A variant is a new instance. GetActionVariant copies the input once, and only when the count changes.
// P:\scripts\4_world\classes\useractionscomponent\actionvariantsmanager.c:24
// P:\scripts\4_world\classes\useractionscomponent\actionmanagerbase.c:207
// While two hands stay offered, those same instances are kept. If their input is still null, the hint throws every frame.
// This writes the registered action's input onto any variant that does not have one, on every refresh.
modded class ActionVariantManager
{
	override void SetActionVariantCount(int count)
	{
		ActionBase registered;
		ActionInput input;
		array<ref ActionBase> variants;
		int i;
		ActionBase variant;

		super.SetActionVariantCount(count);

		registered = ActionManagerBase.GetAction(m_type);
		if (!registered)
		{
			return;
		}

		input = registered.GetInput();
		if (!input)
		{
			return;
		}

		GetActions(variants);
		if (!variants)
		{
			return;
		}

		for (i = 0; i < variants.Count(); i++)
		{
			variant = variants.Get(i);
			if (!variant)
			{
				continue;
			}

			if (variant.GetInput())
			{
				continue;
			}

			variant.SetInput(input);
		}
	}
};
