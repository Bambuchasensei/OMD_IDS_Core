// Client hint for the item action cursor. Do not call super: the vanilla body throws when GetInput() is null.
// 1.30 SetActionWidget, the crashing line is action.GetInput().GetInputType().
// The call that throws is BuildCursor's m_Continuous slot: empty hands, HasTarget() == false.
#ifndef SERVER
modded class ItemActionsWidget
{
	override void SetActionWidget(ActionBase action, string descText, string actionWidget, string descWidget)
	{
		Widget widget;
		TextWidget actionName;
		ActionInput input;
		bool showName;
		bool holdText;

		widget = m_Root.FindAnyWidget(actionWidget);
		if (!widget)
		{
			return;
		}

		showName = false;
		if (action)
		{
			if (!action.HasTarget())
			{
				showName = true;
			}
			else if (m_Player)
			{
				if (m_Player.GetCommand_Vehicle())
				{
					showName = true;
				}
			}
		}

		if (!showName)
		{
			widget.Show(false);
			return;
		}

		actionName = null;
		Class.CastTo(actionName, widget.FindAnyWidget(descWidget));

		holdText = false;
		input = null;
		if (action)
		{
			input = action.GetInput();
		}

		if (input)
		{
			if (input.GetInputType() == ActionInputType.AIT_CONTINUOUS)
			{
				holdText = true;
			}
		}
		else if (action)
		{
			if (action.IsInherited(ActionContinuousBase))
			{
				holdText = true;
			}
		}

		if (holdText)
		{
			descText = descText + " " + "#action_target_cursor_hold";
		}

		if (actionName)
		{
			actionName.SetText(descText);
		}

		widget.Show(true);
	}
};
#endif
