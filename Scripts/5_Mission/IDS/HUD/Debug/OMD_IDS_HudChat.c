#ifndef SERVER

modded class ChatInputMenu
{
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (!finished)
		{
			return super.OnChange(w, x, y, finished);
		}

		string text = ReadBoxText();
		if (OMD_IDS_HudDebug.TryHandle(text))
		{
			ClearBox();
			Close();
			GetUApi().GetInputByID(UAPersonView).Supress();
			return true;
		}

		if (OMD_IDS_InjuryChat.TryHandle(text))
		{
			ClearBox();
			Close();
			GetUApi().GetInputByID(UAPersonView).Supress();
			return true;
		}

		return super.OnChange(w, x, y, finished);
	}

	private string ReadBoxText()
	{
		Widget root = GetLayoutRoot();
		if (!root)
		{
			return "";
		}

		Widget boxWidget = root.FindAnyWidget("InputEditBoxWidget");
		if (!boxWidget)
		{
			return "";
		}

		EditBoxWidget editBox;
		if (!Class.CastTo(editBox, boxWidget))
		{
			return "";
		}

		return editBox.GetText();
	}

	private void ClearBox()
	{
		Widget root = GetLayoutRoot();
		if (!root)
		{
			return;
		}

		Widget boxWidget = root.FindAnyWidget("InputEditBoxWidget");
		if (!boxWidget)
		{
			return;
		}

		EditBoxWidget editBox;
		if (!Class.CastTo(editBox, boxWidget))
		{
			return;
		}

		editBox.SetText("");
	}
};

#endif
