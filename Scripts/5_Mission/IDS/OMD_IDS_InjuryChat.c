#ifndef SERVER

class OMD_IDS_InjuryChat
{
	static bool TryHandle(string text)
	{
		if (text == "")
		{
			return false;
		}

		string trimmed = text.Trim();
		if (trimmed.Length() < 4)
		{
			return false;
		}

		string head = trimmed.Substring(0, 4);
		head.ToLower();
		if (head != "ids ")
		{
			return false;
		}

		Send(trimmed);
		return true;
	}

	private static void Send(string line)
	{
		if (!GetGame())
		{
			Print("[OMD_IDS] player is not ready");
			return;
		}

		PlayerBase player;
		if (!Class.CastTo(player, GetGame().GetPlayer()))
		{
			Print("[OMD_IDS] player is not ready");
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(line);
		rpc.Send(player, OMD_IDS_Rpc.DEBUG_LINE, true, null);
	}
};

#endif
